// Code generated by re2c, DO NOT EDIT.
//line "golang/002_fill_eof_rule_f.re":1
//go:generate re2go $INPUT -o $OUTPUT -f
package main

import "fmt"
import "os"

var SIZE int = 11

type YYCTYPE byte
type Input struct {
	file   *os.File
	data   []byte
	cursor int
	marker int
	token  int
	limit  int
	eof    bool
	state  int
}

const (
	lexError = iota
	lexNeedMoreSpace
	lexNeedMoreInput
	lexResume
	lexEnd
	lexNumber1
	lexNumber2
	lexSpace
)

func fill(in *Input) int {
	// Nothing can be read, resume lexing to match EOF rule or fail.
	if in.eof {
		return lexResume
	}

	// Check if at least some space can be freed.
	if in.token == 0 {
		// In real life can reallocate a larger buffer.
		fmt.Println("fill error: lexeme too long")
		return lexNeedMoreSpace
	}

	// Discard everything up to the start of the current lexeme,
	// shift buffer contents and adjust offsets.
	copy(in.data[0:], in.data[in.token:in.limit])
	in.cursor -= in.token
	in.marker -= in.token
	in.limit -= in.token
	in.token = 0

	// Read new data (as much as possible to fill the buffer).
	n, _ := in.file.Read(in.data[in.limit:SIZE])
	in.limit += n
	in.data[in.limit] = 0
	fmt.Printf("fill: %v '%s'\n", in.data[:in.limit+1],
		string(in.data[:in.limit]))

	// If read less than expected, this is the end of input.
	in.eof = in.limit < SIZE

	return lexResume
}

func Lex(in *Input) int {
	var yych YYCTYPE

	
//line "golang/002_fill_eof_rule_f.go":73
	switch (in.state) {
	default:
		goto yy0
	case 0:
		if (in.limit-in.cursor < 1) {
			goto yy11
		}
		goto yyFillLabel0
	case 1:
		if (in.limit-in.cursor < 1) {
			goto yy4
		}
		goto yyFillLabel1
	case 2:
		if (in.limit-in.cursor < 1) {
			goto yy6
		}
		goto yyFillLabel2
	case 3:
		if (in.limit-in.cursor < 1) {
			goto yy8
		}
		goto yyFillLabel3
	case 4:
		if (in.limit-in.cursor < 1) {
			goto yy10
		}
		goto yyFillLabel4
	}
yy0:
yyFillLabel0:
	yych = YYCTYPE(in.data[in.cursor])
	switch (yych) {
	case ' ':
		goto yy3
	case '0','1','2','3','4','5','6','7','8','9':
		goto yy5
	default:
		if (in.limit-in.cursor < 1) {
			in.state = 0
			return lexNeedMoreInput
		}
		goto yy2
	}
yy2:
	in.cursor += 1
	in.state = -1
//line "golang/002_fill_eof_rule_f.re":80
	{
		fmt.Println("error")
		return lexError
	}
//line "golang/002_fill_eof_rule_f.go":126
yy3:
	in.cursor += 1
yyFillLabel1:
	yych = YYCTYPE(in.data[in.cursor])
	switch (yych) {
	case ' ':
		goto yy3
	default:
		if (in.limit-in.cursor < 1) {
			in.state = 1
			return lexNeedMoreInput
		}
		goto yy4
	}
yy4:
	in.state = -1
//line "golang/002_fill_eof_rule_f.re":100
	{
		return lexSpace
	}
//line "golang/002_fill_eof_rule_f.go":147
yy5:
	in.cursor += 1
	in.marker = in.cursor
yyFillLabel2:
	yych = YYCTYPE(in.data[in.cursor])
	switch (yych) {
	case '-':
		goto yy7
	case '0','1','2','3','4','5','6','7','8','9':
		goto yy5
	default:
		if (in.limit-in.cursor < 1) {
			in.state = 2
			return lexNeedMoreInput
		}
		goto yy6
	}
yy6:
	in.state = -1
//line "golang/002_fill_eof_rule_f.re":90
	{
		fmt.Printf("number-1: %v\n", string(in.data[in.token:in.cursor]))
		return lexNumber1
	}
//line "golang/002_fill_eof_rule_f.go":172
yy7:
	in.cursor += 1
yyFillLabel3:
	yych = YYCTYPE(in.data[in.cursor])
	switch (yych) {
	case '0','1','2','3','4','5','6','7','8','9':
		goto yy9
	default:
		if (in.limit-in.cursor < 1) {
			in.state = 3
			return lexNeedMoreInput
		}
		goto yy8
	}
yy8:
	in.cursor = in.marker
	goto yy6
yy9:
	in.cursor += 1
yyFillLabel4:
	yych = YYCTYPE(in.data[in.cursor])
	switch (yych) {
	case '0','1','2','3','4','5','6','7','8','9':
		goto yy9
	default:
		if (in.limit-in.cursor < 1) {
			in.state = 4
			return lexNeedMoreInput
		}
		goto yy10
	}
yy10:
	in.state = -1
//line "golang/002_fill_eof_rule_f.re":95
	{
		fmt.Printf("number-2: %v\n", string(in.data[in.token:in.cursor]))
		return lexNumber2
	}
//line "golang/002_fill_eof_rule_f.go":211
yy11:
	in.state = -1
//line "golang/002_fill_eof_rule_f.re":85
	{
		fmt.Println("end")
		return lexEnd
	}
//line "golang/002_fill_eof_rule_f.go":219
//line "golang/002_fill_eof_rule_f.re":103

}

func test(data string) int {
	tmpfile := "input.txt"

	f, _ := os.Create(tmpfile)
	f.WriteString(data)
	f.Seek(0, 0)

	defer func() {
		f.Close()
		os.Remove(tmpfile)
	}()

	in := &Input{
		file:   f,
		data:   make([]byte, SIZE+1),
		cursor: SIZE,
		marker: SIZE,
		token:  SIZE,
		limit:  SIZE,
		eof:    false,
	}

	result := lexNeedMoreInput
loop:
	for {
		switch result {
		case lexError:
			break loop
		case lexEnd:
			break loop
		case lexNeedMoreInput:
			result = fill(in)
			if result != lexResume {
				break loop
			}
		default:
			in.token = in.cursor
			in.state = 0
		}
		result = Lex(in)
	}

	return result
}

func main() {
	var s string

	// Succeeds, the lexer needs 1 character ahead.
	s = "     1234567890     "
	if test(s) != lexEnd {
		panic("expected 'number: 1234567890'")
	}

	// Fails, the lexer needs 1 character ahead, but there is no space.
	s = "     12345678901    "
	if test(s) != lexNeedMoreSpace {
		panic("expected 'fill error: lexeme too long'")
	}

	// Succeeds, the lexer needs 1 character ahead.
	s = "     12345-6789     "
	if test(s) != lexEnd {
		panic("expected 'number: 12345-6789'")
	}

	// Fails, the lexer needs 1 character ahead, but there is no space.
	s = "     12345-67890     "
	if test(s) != lexNeedMoreSpace {
		panic("expected 'fill error: lexeme too long'")
	}

	// Fails, invalid input.
	s = "?#!*"
	if test(s) != lexError {
		panic("expected 'error'")
	}

	fmt.Println("OK")
}
