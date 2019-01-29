// Create by xjshi 2019/01/11
/*
这里用c实现几种常用的加密算法
1）摘要算法，sha1,md5
2) ba64 加密
*/
#include<stdio.h>
#include<stdlib.h>
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
// namespace SAEBASE
// {
/*base64*/
static const char base64char[] =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#define BAD 100
static const unsigned char b64decode[] = {
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD,  62, BAD, BAD, BAD,  63,
	52,  53,  54,  55,  56,  57,  58,  59,
	60,  61, BAD, BAD, BAD, BAD, BAD, BAD,

	BAD,   0,   1,   2,   3,   4,   5,   6,
	7,   8,   9,  10,  11,  12,  13,  14,
	15,  16,  17,  18,  19,  20,  21,  22,
	23,  24,  25, BAD, BAD, BAD, BAD, BAD,
	BAD,  26,  27,  28,  29,  30,  31,  32,
	33,  34,  35,  36,  37,  38,  39,  40,
	41,  42,  43,  44,  45,  46,  47,  48,
	49,  50,  51, BAD, BAD, BAD, BAD, BAD,

	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,

	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
	BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD
};
//将长度为len的字符串加密为base64
char* encode_base64(const void*p,int len)
{
	const unsigned char *in = (const unsigned char *)p;
	char *out;
	int save = 0, nbits = 0;
	int i, k = 0;

	if (in == 0 || len <= 0) {
		return 0;
	}
	out = (char*)malloc(4 + 4 * len / 3);

	/* every three input bytes becomes 4 base64 output characters */

	for (i = 0; i < len; i++) {
		save |= in[i] << (16 - nbits);                 /* 3 x 8 bits in */
		if ((nbits += 8) == 24) {
			out[k++] = base64char[(save >> 18) & 077]; /* 4 x 6 bits out */ //八进制077
			out[k++] = base64char[(save >> 12) & 077];
			out[k++] = base64char[(save >>  6) & 077];
			out[k++] = base64char[ save        & 077];
			nbits = 0;
			save  = 0;
		}
	}

	/* convert leftover bits */

	if (nbits > 0) {
		for (i = 18; i >= 0; i -= 6) {
			if (nbits > 0) {
				out[k++] = base64char[(save >> i) & 077];
				nbits -= 6;
			}
			else {
				out[k++] = '=';
			}
		}
	}
	out[k] = 0;
	return out;
}
//将base加密串解码成明文，p为输出字串，len为输出参数，明文长度。
char* decode_base64(IN const char* p,OUT int* len)
{
	const unsigned char *in = (const unsigned char *) p;
	unsigned char *out;
	int save = 0, nbits = 0, sixbits;
	int i, k;

	if (p == 0) {
		return 0;
	}
	out = (unsigned char *)malloc(3 + 3 * strlen(p) / 4);

	/* every four base64 input characters becomes three output bytes */

	for (i = k = 0; in[i] != 0; i++) {
		if ((sixbits = b64decode[in[i]]) == BAD) {
			continue;
		}
		save |= sixbits << (18 - nbits);  /* 4 x 6 bits in */
		if ((nbits += 6) == 24) {
			out[k++] = save >> 16;        /* 3 x 8 bits out */
			out[k++] = save >> 8;
			out[k++] = save;
			nbits = 0;
			save  = 0;
		}
	}

	/* convert leftover bits */

	for (i = 16; i >= 0 && nbits >= 8; i -= 8) {
		out[k++] = save >> i;
		nbits -= 8;
	}
	out[k] = 0;
	if (len != 0) {
		*len = k;
	}
	return (char*)out;
}
//};