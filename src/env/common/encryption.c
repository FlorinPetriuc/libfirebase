/*
 * Copyright (C) 2016 Florin Petriuc. All rights reserved.
 * Initial release: Florin Petriuc <petriuc.florin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include "../env.h"

static const char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static char *env_base64Encode(const unsigned char *bufin, const int len)
{
    int i;
    int j;

    char *ret;

    j = 0;

    ret = env_malloc(((len / 3) << 2) + 5);

    for(i = 0; i < len - 2; i += 3)
    {
        ret[j++] = basis_64[(bufin[i] >> 2) & 0x3F];
        ret[j++] = basis_64[((bufin[i] & 0x3) << 4) | ((int) (bufin[i + 1] & 0xF0) >> 4)];
        ret[j++] = basis_64[((bufin[i + 1] & 0xF) << 2) | ((int) (bufin[i + 2] & 0xC0) >> 6)];
        ret[j++] = basis_64[bufin[i + 2] & 0x3F];
    }

    if(i < len)
    {
        ret[j++] = basis_64[(bufin[i] >> 2) & 0x3F];

        if(i == (len - 1))
        {
            ret[j++] = basis_64[((bufin[i] & 0x3) << 4)];
            ret[j++] = '=';
        }
        else
        {
            ret[j++] = basis_64[((bufin[i] & 0x3) << 4) | ((int) (bufin[i + 1] & 0xF0) >> 4)];
            ret[j++] = basis_64[((bufin[i + 1] & 0xF) << 2)];
        }
        ret[j++] = '=';
    }

    ret[j++] = '\0';

    return ret;
}

static const unsigned char pr2six[256] =
{
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

static int _base64decode_len(const char *bufcoded)
{
    int nbytesdecoded;
    int nprbytes;

    const unsigned char *bufin;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);

    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) >> 2) * 3;

    return nbytesdecoded + 1;
}

static unsigned char *_base64decode(const char *bufcoded, unsigned int *len)
{
    int nprbytes;

    unsigned int i;

    const unsigned char *bufin;
    unsigned char *ret;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;

	*len = _base64decode_len(bufcoded);
	ret = env_malloc(*len);

	i = 0;

    bufin = (const unsigned char *) bufcoded;

    while(nprbytes > 4)
	{
		ret[i++] = (unsigned char) (pr2six[bufin[0]] << 2 | pr2six[bufin[1]] >> 4);
		ret[i++] = (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		ret[i++] = (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);

		bufin += 4;
		nprbytes -= 4;
    }

    if(nprbytes > 1)
	{
		ret[i++] = (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }

    if(nprbytes > 2)
	{
		ret[i++] = (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }

    if(nprbytes > 3)
	{
		ret[i++] = (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }

    *len = i;

    return ret;
}

unsigned char *env_base64Decode(char *bufcoded, unsigned int *len)
{
	int buf_len;
    int equals_pad;

    char *_bufcoded;
    unsigned char *b_decoded;

    buf_len = strlen(bufcoded);

    if(buf_len & 3)
    {
        equals_pad = 4 - (buf_len & 3);
    }
    else
    {
        equals_pad = 0;
    }

    if(equals_pad)
    {
        _bufcoded = env_malloc(buf_len + equals_pad + 1);
        memcpy(_bufcoded, bufcoded, buf_len);
        memcpy(_bufcoded + buf_len, "===", equals_pad);
        _bufcoded[buf_len + equals_pad] = '\0';
    }
    else
    {
        _bufcoded = bufcoded;
    }

    b_decoded = _base64decode(_bufcoded, len);

    if(_bufcoded != bufcoded)
    {
        free(_bufcoded);
    }

    return b_decoded;
}

unsigned char *env_base64WebDecode(char *bufcoded, unsigned int *len)
{
    int i = 0;

    while(bufcoded[i] != '\0')
    {
        if(bufcoded[i] == '-')
        {
            bufcoded[i] = '+';
        }
        else if(bufcoded[i] == '_')
        {
            bufcoded[i] = '/';
        }

        ++i;
    }

    return env_base64Decode(bufcoded, len);
}

char *env_base64WebEncode(unsigned char *bufin, const int len)
{
    int i = 0;

    char *ret;

    ret = env_base64Encode(bufin, len);

    while(i < strlen(ret))
    {
        if(ret[i] == '+')
        {
            ret[i] = '-';
        }
        else if(ret[i] == '/')
        {
            ret[i] = '_';
        }

        ++i;
    }

    return ret;
}

void env_create_info_chunk(const char *type, const unsigned char *rpubkey, const uint16_t rpubkey_len,
                                        const unsigned char *spubkey, const uint16_t spubkey_len,
                                        unsigned char **info, unsigned int *info_len)
{
    int i;

    const char c1[] = "Content-Encoding: ";
    const char c2[] = "\0P-256\0";

    (*info) = env_malloc(sizeof(c1) + strlen(type) + sizeof(c2) + 2 + rpubkey_len + 2 + spubkey_len);

    memcpy((*info), c1, ENV_STRLEN_MACRO(c1));
    i = ENV_STRLEN_MACRO(c1);

    memcpy((*info) + i, type, strlen(type));
    i += strlen(type);

    memcpy((*info) + i, c2, ENV_STRLEN_MACRO(c2));
    i += ENV_STRLEN_MACRO(c2);

    (*info)[i++] = (rpubkey_len >> 8) & 0xFF;
    (*info)[i++] = rpubkey_len & 0xFF;

    memcpy((*info) + i, rpubkey, rpubkey_len);
    i += rpubkey_len;

    (*info)[i++] = (spubkey_len >> 8) & 0xFF;
    (*info)[i++] = spubkey_len & 0xFF;

    memcpy((*info) + i, spubkey, spubkey_len);
    i += spubkey_len;

    *info_len = i;
}