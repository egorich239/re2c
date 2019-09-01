/* Generated by re2c */

#include <stddef.h> /* size_t */
#include <stdio.h>
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */

static void *read_file
    ( const char *fname
    , size_t unit
    , size_t padding
    , size_t *pfsize
    )
{
    void *buffer = NULL;
    size_t fsize = 0;

    /* open file */
    FILE *f = fopen(fname, "rb");
    if(f == NULL) {
        goto error;
    }

    /* get file size */
    fseek(f, 0, SEEK_END);
    fsize = (size_t) ftell(f) / unit;
    fseek(f, 0, SEEK_SET);

    /* allocate memory for file and padding */
    buffer = malloc(unit * (fsize + padding));
    if (buffer == NULL) {
        goto error;
    }

    /* read the whole file in memory */
    if (fread(buffer, unit, fsize, f) != fsize) {
        goto error;
    }

    fclose(f);
    *pfsize = fsize;
    return buffer;

error:
    fprintf(stderr, "error: cannot read file '%s'\n", fname);
    free(buffer);
    if (f != NULL) {
        fclose(f);
    }
    return NULL;
}

#define YYCTYPE unsigned short
#define YYKEYTYPE unsigned char
#define YYPEEK() *cursor
#define YYSKIP() ++cursor
#define YYLESSTHAN(n) (limit - cursor) < n
#define YYFILL(n) { break; }

static int action_line4
    ( unsigned *pkix
    , const YYKEYTYPE *keys
    , const YYCTYPE *start
    , const YYCTYPE *token
    , const YYCTYPE **cursor
    , YYKEYTYPE rule_act
    )
{
    const unsigned kix = *pkix;
    const long pos = token - start;
    const long len_act = *cursor - token;
    const long len_exp = (long) keys[kix + 1];
    const YYKEYTYPE rule_exp = keys[kix + 2];
    *pkix = kix + 3;
    if (rule_exp == 255) {
        fprintf
            ( stderr
            , "warning: lex_line4: control flow is undefined for input"
                " at position %ld, rerun re2c with '-W'\n"
            , pos
            );
    }
    if (len_act == len_exp && rule_act == rule_exp) {
        const YYKEYTYPE offset = keys[kix];
        *cursor = token + offset;
        return 0;
    } else {
        fprintf
            ( stderr
            , "error: lex_line4: at position %ld (key %u):\n"
                "\texpected: match length %ld, rule %u\n"
                "\tactual:   match length %ld, rule %u\n"
            , pos
            , kix
            , len_exp
            , rule_exp
            , len_act
            , rule_act
            );
        return 1;
    }
}

static int check_key_count_line4(unsigned have, unsigned used, unsigned need)
{
    if (used + need <= have) return 0;
    fprintf(stderr, "error: lex_line4: not enough keys\n");
    return 1;
}

int lex_line4()
{
    const size_t padding = 2; /* YYMAXFILL */
    int status = 0;
    size_t input_len = 0;
    size_t keys_count = 0;
    YYCTYPE *input = NULL;
    YYKEYTYPE *keys = NULL;
    const YYCTYPE *cursor = NULL;
    const YYCTYPE *limit = NULL;
    const YYCTYPE *token = NULL;
    const YYCTYPE *eof = NULL;
    unsigned int i = 0;

    input = (YYCTYPE *) read_file
        ("skeleton/unicode_any.ix--skeleton.c.line4.input"
        , sizeof (YYCTYPE)
        , padding
        , &input_len
        );
    if (input == NULL) {
        status = 1;
        goto end;
    }

    for (i = 0; i < input_len; ++i) {
        /* from little-endian to host-endian */
        unsigned char *p = (unsigned char*)&input[i];
        input[i] = p[0] + (p[1] << 8u);
    }

    keys = (YYKEYTYPE *) read_file
        ("skeleton/unicode_any.ix--skeleton.c.line4.keys"
        , sizeof (YYKEYTYPE)
        , 0
        , &keys_count
        );
    if (keys == NULL) {
        status = 1;
        goto end;
    }

    cursor = input;
    limit = input + input_len + padding;
    eof = input + input_len;

    for (i = 0; status == 0 && cursor < eof && i < keys_count;) {
        token = cursor;
        YYCTYPE yych;

        if (YYLESSTHAN (2)) YYFILL(2);
        yych = YYPEEK ();
        if (yych <= 0xD7FF) goto yy2;
        if (yych <= 0xDBFF) goto yy4;
yy2:
        YYSKIP ();
yy3:
        status = check_key_count_line4(keys_count, i, 3)
             || action_line4(&i, keys, input, token, &cursor, 0);
        continue;
yy4:
        YYSKIP ();
        yych = YYPEEK ();
        if (yych <= 0xDBFF) goto yy3;
        if (yych <= 0xDFFF) goto yy2;
        goto yy3;

    }
    if (status == 0) {
        if (cursor != eof) {
            status = 1;
            const long pos = token - input;
            fprintf(stderr, "error: lex_line4: unused input strings left at position %ld\n", pos);
        }
        if (i != keys_count) {
            status = 1;
            fprintf(stderr, "error: lex_line4: unused keys left after %u keys\n", i);
        }
    }

end:
    free(input);
    free(keys);

    return status;
}

#undef YYCTYPE
#undef YYKEYTYPE
#undef YYPEEK
#undef YYSKIP
#undef YYLESSTHAN
#undef YYFILL

int main()
{
    if(lex_line4() != 0) {
        return 1;
    }
    return 0;
}
  � ��`8���pH	 
�
���X0���h@���xP( ���`8 � �!�"p#H$ %�%�&�'�(X)0*+�+�,�-h.@/0�0�1�2x3P4(5 6�6�7�8`98:;�;�<�=p>H? @�@�A�B�CXD0EF�F�G�HhI@JK�K�L�MxNPO(P Q�Q�R�S`T8UV�V�W�XpYHZ [�[�\�]�^X_0`a�a�b�chd@ef�f�g�hxiPj(k l�l�m�n`o8pq�q�r�sptHu v�v�w�x�yXz0{|�|�}�~h@����Ȃ��x�P�(� �؇����`�8�������p�H� ���В����X�0��������h�@���ȝ��x�P�(� �آ����`�8�������p�H� ���Э����X�0��ಸ���h�@���ȸ��x�P�(� �ؽ����`�8�����Ø�p�H� ����Ȩɀ�X�0���͸ΐ�h�@�����Ӡ�x�P�(��� �$�H�lܐܴ����� �D�h݌ݰ������@�dވެ������<�`߄ߨ������8�\���������4�X�|�������0�T�x�������,�P�t������(�L�p����� �$�H�l������� �D�h��������@�d��������<�`��������8�\��������4�X�|�������0�T�x�������,�P�t������(�L�p������ �$�H�l������� �D�h��������@�d���������<�`��������8�\��������4�X�|�������0�T�x��������,�P�t��������(�L�p������� �$�H�l��������� �D�h����������@�d����������<�`����������8�\����������4�X�|��������0�T�x��������,�P�t��������(�L�p��������� � ��������������� � �$�$�(�(�,�,�0�0�4�4�8�8�<�<�@�@�D�D�H�H�L�L�P�P�T�T�X�X�\�\�`�`�d�d�h�h�l�l�p�p�t�t�x�x�|�|܀؀܄؄܈؈܌،ܐؐܔؔܘؘܜ؜ܠؠܤؤܨبܬجܰذܴشܸظܼؼ����������������������������������������������������������������� � ��������������� � �$�$�(�(�,�,�0�0�4�4�8�8�<�<�@�@�D�D�H�H�L�L�P�P�T�T�X�X�\�\�`�`�d�d�h�h�l�l�p�p�t�t�x�x�|�|݀ـ݄ل݈و݌ٌݐِݔٔݘ٘ݜٜݠ٠ݤ٤ݨ٨ݬ٬ݰٰݴٴݸٸݼټ����������������������������������������������������������������� � ��������������� � �$�$�(�(�,�,�0�0�4�4�8�8�<�<�@�@�D�D�H�H�L�L�P�P�T�T�X�X�\�\�`�`�d�d�h�h�l�l�p�p�t�t�x�x�|�|ހڀބڄވڈތڌސڐޔڔޘژޜڜޠڠޤڤިڨެڬްڰ޴ڴ޸ڸ޼ڼ����������������������������������������������������������������� � ��������������� � �$�$�(�(�,�,�0�0�4�4�8�8�<�<�@�@�D�D�H�H�L�L�P�P�T�T�X�X�\�\�`�`�d�d�h�h�l�l�p�p�t�t�x�x�|�|߀ۀ߄ۄ߈ۈߌیߐېߔ۔ߘۘߜۜߠ۠ߤۤߨۨ߬۬߰۰ߴ۴߸۸߼ۼ��������������������������������������������������������������������� �  �� ظؔ�p�L�(� ��$ؼ(ؘ,�t	0�P
4�,8�<��@��D؜H�xL�TP�0T�X��\��`ؠd�|h�Xl�4p�t��x��|ؤ�؀��\��8���������ب �؄!��`"��<#��$���$���%�ج&�؈'��d(��@)��*���*���+�ذ,�،-��h.��D/�� 0���0���1�ش2�ؐ3��l4��H5��$6 � 7��7ٸ8ٔ9�p:�L;�(<�= ��=$ټ>(٘?,�t@0�PA4�,B8�C<��C@��DDٜEH�xFL�TGP�0HT�IX��I\��J`٠Kd�|Lh�XMl�4Np�Ot��Ox��P|٤Q�ـR��\S��8T��U���U���V�٨W�لX��`Y��<Z��[���[���\�٬]�و^��d_��@`��a���a���b�ٰc�ٌd��he��Df�� g���g���h�ٴi�ِj��lk��Hl��$m � n��nڸoڔp�pq�Lr�(s�t ��t$ڼu(ژv,�tw0�Px4�,y8�z<��z@��{Dڜ|H�x}L�T~P�0T��X��\�ā`ڠ�d�|�h�X�l�4�p��t��x�ȇ|ڤ��ڀ���\���8����������̍�ڨ��ڄ���`���<����������Г�ڬ��ڈ���d���@����������ԙ�ڰ��ڌ���h���D��� �������؟�ڴ��ڐ���l���H���$� � ��ܥ۸�۔��p��L��(��� ��$ۼ�(ۘ�,�t�0�P�4�,�8��<��@���Dۜ�H�x�L�T�P�0�T��X��\�ĸ`۠�d�|�h�X�l�4�p��t��x�Ⱦ|ۤ��ۀ���\���8�Ð��Ô��ĘۨŜۄƠ�`Ǥ�<Ȩ�ɬ��ɰ��ʴ۬˸ۈ̼�d���@�������������۰��ی���h���D��� ����������۴��ې���l���H���$����� � �� ��@��`�؀�ؠ������� � �$� �(�@�,�`�0؀�4ؠ�8���<���@� �D� �H�@�L�`�P؀�Tؠ�X���\���`� �d� �h�@�l�`�p؀�tؠ�x���|���� �� ��@��`�؀�ؠ�������� �� ��@��`�؀�ؠ��������� ��� ���@���`��؀��ؠ����������� ��� ���@���`��؀��ؠ��������� � �� ��@��`�ـ�٠������� � �$� �(�@�,�`�0ـ�4٠�8���<���@� �D� �H�@�L�`�Pـ�T٠�X���\���`� �d� �h�@�l�`�pـ�t٠�x���|���� �� ��@��`�ـ�٠�������� ��� ���@���`��ـ��٠����������� ��� ���@���`��ـ��٠����������� ��� ���@���`��ـ��٠��������� � �� ��@��`�ڀ�ڠ������� � �$� �(�@�,�`�0ڀ�4ڠ�8���<���@� �D� �H�@�L�`�Pڀ�Tڠ�X���\���`� �d� �h�@�l�`�pڀ�tڠ�x���|���� �� ��@��`��ڀ��ڠ����������� ��� ���@���`��ڀ��ڠ����������� ��� ���@���`��ڀ��ڠ����������� ��� ���@���`��ڀ��ڠ��������� � �� ��@��`�ۀ�۠������� � �$� �(�@�,�`�0ۀ�4۠�8���<���@� �D� �H�@�L�`�Pۀ�T۠�X���\���`� �d� �h�@�l�`�pۀ�t۠�x���|����� ��� ���@���`��ۀ��۠����������� ��� ���@���`��ۀ��۠����������� ��� ���@���`��ۀ��۠����������� ��� ���@���`��ۀ��۠�������������                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     