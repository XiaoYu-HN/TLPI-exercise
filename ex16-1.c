#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/xattr.h>
#include "tlpi_hdr.h"

#define EX_TEXT 0
#define EX_HEX 1
#define EX_BASE64 2

int hex_to_ascii(const char *src, char *tgt, size_t size);
int hexchar_to_int(int c);
int base64_to_ascii(const char *b64, char *asc, size_t size);
signed char b64char_to_byte(signed char c);

int hexchar_to_int(int c)
{
    int upperc;

    upperc = toupper(c);
    if (upperc >= '0' && upperc <= '9')
        upperc = upperc - '0';
    else if (upperc >= 'A' && upperc <= 'F')
        upperc = upperc - 'A' + 10;
    else
        upperc = -1;

    return upperc;
}

int hex_to_ascii(const char *src, char *tgt, size_t size)
{
    size_t i, j;
    int c1, c2, v;

    if (strlen(src) % 2 != 0 )
        return -1;

    for (i = 0, j = 0; j<size && src[i] != 0; i+=2, j++) {
        c1 = hexchar_to_int(src[i]);
        c2 = hexchar_to_int(src[i+1]);

        if (c1 == -1 || c2 == -1) {
            return -1;
        }

        v = c1 << 4 | c2;
        tgt[j] = v;

    }
    tgt[j] = '\0';

    return j;
}

signed char b64char_to_byte(signed char c)
{
	signed char ret;

	if (c >= 'A' && c <= 'Z')	
		ret = c - 'A';
	else if (c >= 'a' && c <= 'z')	
		ret = c - 'a' + 26;
	else if (c >= '0' && c <= '9')	
		ret = c - '0' + 52;
	else if (c == '+')
		ret = 62;
	else if (c == '/')
		ret = 63; 
	else if (c == '=')
		ret = 0; 
	else
		ret = -1;

	return ret;
}

//
// 字符 → 6位数值
// 'A' → 0,      'B' → 1,      ..., 'Z' → 25
// 'a' → 26,     'b' → 27,     ..., 'z' → 51
// '0' → 52,     '1' → 53,     ..., '9' → 61
// '+' → 62,     '/' → 63
// '=' → 填充标记（无数值）

int base64_to_ascii(const char *src, char *tgt, size_t size)
{
	signed char c[4];
	size_t i, j;
	long v;

    if (strlen(src) % 4 != 0 )
        return -1;

    for (i = 0, j = 0; j<size && src[i] != 0; i+=4, j+=3) {
		for (int k = 0; k < 4; k++) {
			c[k] = b64char_to_byte(src[i+k]);
			if (c[k] == -1) {
				return -1;
			}
		}

		v = c[0] << 18 | c[1] <<12 | c[2] << 6 | c[3]; 
        tgt[j] = (v >> 16) & 0xFF;
        tgt[j+1] = (v >> 8) & 0xFF;
        tgt[j+2] = v & 0xFF ;

    }
    tgt[j] = '\0';

    return j;
}

int
main(int argc, char *argv[])
{
    int encoding = EX_TEXT;
    char *decode_value;
    int c;
    int len_value, len_dvalue;

    if (argc < 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file name value\n", argv[0]);

    char *name = argv[2];
    char *value = argv[3];
    decode_value = value;

    len_value = strlen(value);
    if (len_value > 2 && value[0] == '0') {
        c = toupper(value[1]);
        if (c == 'X' && len_value % 2 == 0) {
            encoding = EX_HEX;
            len_dvalue = (len_value - 2) / 2 + 1;
            decode_value = malloc(len_dvalue);
            hex_to_ascii(value + 2, decode_value, len_dvalue);
        }
        else if (c == 'S' && ((len_value - 2) % 4) == 0) {
            encoding = EX_BASE64;
            len_dvalue = 3 * (len_value - 2) / 4 + 1;
            decode_value = malloc(len_dvalue);
            base64_to_ascii(value + 2, decode_value, len_dvalue);
        }
    }

    printf("encoding is <%d>\n", encoding);
    printf("value is <%s>\n", value);
    printf("decode value is <%s>\n", decode_value);

    if (setxattr(argv[1], name, decode_value, strlen(decode_value), 0) == -1)
        errExit("setxattr");

    exit(EXIT_SUCCESS);
}
