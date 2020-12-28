#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <string>

#include "benchmarks/common.h"
#include "lib/regex.h"

#ifdef HAVE_RE2_RE2_H
#include "re2/re2.h"
#endif


static inline void show(const std::vector<Result> &results)
{
    const size_t n = results.size();
    assert(n > 0);

    const uint64_t ticks0 = results[0].ticks;

    for (size_t i = 0; i < n; ++i) {
        const Result &r = results[i];
        if (r.ticks == UNAVAIL) {
            fprintf(stderr, "%30s:%10s%10s\n", r.title, "-", "-");
        }
        else {
            fprintf(stderr, "%30s:%10.2lf%10.3lfs\n", r.title
                , double(r.ticks) / double(ticks0)
                , double(r.ticks) / CLOCKS_PER_SEC);
        }
    }
}

static Result bench_re2c(const char *regexp, const std::vector<std::string> &strings,
    size_t ntimes, int flags, int mask, int need, const char *prefix)
{
    Result x;
    x.title = prefix;

    if ((flags & mask)
        || (need != 0 && !(flags & need))) {
        x.ticks = UNAVAIL;
        return x;
    }

    regex_t re;
    int err;
    clock_t /*t1 = 0, t2 = 0,*/ t3 = 0, t4 = 0;

    //t1 = clock();
    err = regcomp(&re, regexp, flags);
    //t2 = clock();
    if (err) {
        fprintf(stderr, "*** %s compile failed\n", prefix);
        exit(1);
    }

    const size_t nmatch = re.re_nsub;
    regmatch_t *pmatch = new regmatch_t[nmatch];

    // first time is warmup
    for (size_t k = 0; k < 2; ++k) {
        t3 = clock();
        err = 0;
        for (size_t j = 0; j < strings.size(); ++j) {
            for (size_t i = 0; i < ntimes; ++i) {
                err |= regexec(&re, strings[j].c_str(), nmatch, pmatch, 0);
            }
        }
        t4 = clock();
    }
    if (err) {
        fprintf(stderr, "*** %s run failed\n", prefix);
        exit(1);
    }

    delete[] pmatch;
    regfree(&re);

    x.ticks = uint64_t(t4 - t3);
    return x;
}

#ifdef HAVE_RE2_RE2_H
static Result bench_re2(const char *regexp, const std::vector<std::string> &strings,
    size_t ntimes, int mask, const char *prefix)
{
    Result x;
    x.title = prefix;

    if (mask & XREG_RE2) {
        x.ticks = UNAVAIL;
        return x;
    }

    RE2 *re2;
    clock_t /*t1 = 0, t2 = 0,*/ t3 = 0, t4 = 0;
    bool ok = true;

    //t1 = clock();
    re2 = new RE2(regexp, RE2::POSIX);
    //t2 = clock();
    if (!re2->ok()) {
        fprintf(stderr, "*** %s compile failed\n", prefix);
        exit(1);
    }

    const int argc = re2->NumberOfCapturingGroups();
    RE2::Arg *args = new RE2::Arg[argc];
    RE2::Arg **argps = new RE2::Arg*[argc];
    std::string *results = new std::string[argc];
    for (int i = 0; i < argc; ++i) {
        args[i] = &results[i];
        argps[i] = &args[i];
    }

    // first time is warmup
    for (size_t k = 0; k < 2; ++k) {
        t3 = clock();
        for (size_t j = 0; j < strings.size(); ++j) {
            for (size_t i = 0; i < ntimes; ++i) {
                ok = ok && RE2::FullMatchN(strings[j].c_str(), *re2, argps, argc);
            }
        }
        t4 = clock();
    }
    if (!ok) {
        fprintf(stderr, "*** %s run failed\n", prefix);
        exit(1);
    }

    delete[] results;
    delete[] argps;
    delete[] args;
    delete re2;

    x.ticks = uint64_t(t4 - t3);
    return x;
}
#endif

static size_t groupcnt(const char *regexp)
{
    regex_t re;
    int err = regcomp(&re, regexp, REG_NFA);
    if (err) {
        fprintf(stderr,
            "*** cannot find the number of capturing groups for RE %s\n",
            regexp);
        exit(1);
    }
    return re.re_nsub;
}

void bench(const char *regexp, const std::vector<std::string> &strings, uint32_t times,
    int mask, int need)
{
    assert(!strings.empty());
    const char *s0 = strings[0].c_str();

    fprintf(stderr, "\nr: %.*s..., s: %.*s..., n: %u, %u chars, %u groups\n",
        30, regexp, 30, s0, times, (uint32_t)strlen(regexp), (uint32_t)groupcnt(regexp));

    std::vector<Result> rs;
    rs.reserve(nbenchmarks);

    for (size_t i = 0; i < nbenchmarks; ++i) {
        const benchmark_t &b = benchmarks[i];
        switch (b.engine) {
        case ENGINE_RE2C:
            rs.push_back(bench_re2c(regexp, strings, times, b.flags, mask, need, b.name));
            break;
#ifdef HAVE_RE2_RE2_H
        case ENGINE_RE2:
            rs.push_back(bench_re2(regexp, strings, times, mask, b.name));
            break;
#endif
        }
    }

    show(rs);
}

void load_strings(const char *fname, const char *delim,
    std::vector<std::string> &strings)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        fprintf(stderr, "cannot open file %s\n", fname);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    const size_t flen = static_cast<size_t>(ftell(f));
    fseek(f, 0, SEEK_SET);

    char *fbuf = new char[flen];
    if (flen != fread(fbuf, 1, flen, f)) {
        fprintf(stderr, "cannot read file %s\n", fname);
        exit(1);
    }

    strings.clear();
    const size_t dlen = strlen(delim);
    for (char *p1 = fbuf, *p2 = fbuf, *p3 = fbuf + flen; p2 < p3; ) {
        for (p1 = p2; p2 + dlen <= p3; ++p2) {
            if (memcmp(p2, delim, dlen) == 0) {
                p2 += dlen;
                break;
            }
        }
        strings.push_back(std::string(p1, static_cast<size_t>(p2 - p1 - 1)));
    }

    delete[] fbuf;
    fclose(f);
}
