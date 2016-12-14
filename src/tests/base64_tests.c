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

#include "../libfirebase.h"

static int base64_enc_dec_test(const char *enc, const char *dec)
{
    unsigned int len;
    unsigned int i;

    char *_enc;

    unsigned char *_dec;

    _enc = env_malloc(strlen(enc) + 1);
    memcpy(_enc, enc, strlen(enc) + 1);

    _dec = env_base64WebDecode(_enc, &len);

    if(len != strlen(dec))
    {
        env_logWrite(LOG_TYPE_ERROR, "Error base64 decoding %s to %s: lens don't match", 2, enc, dec);

        return 1;
    }

    for(i = 0; i < len; ++i)
    {
        if(dec[i] == _dec[i])
        {
            continue;
        }

        env_logWrite(LOG_TYPE_ERROR, "Error base64 decoding %s to %s: values don't match", 2, enc, dec);
        env_logWrite(LOG_TYPE_ERROR, "%hhu vs %hhu at %u", 2, dec[i], _dec[i], i);

        return 1;
    }

    free(_dec);
    free(_enc);

    _dec = env_malloc(strlen(dec) + 1);
    memcpy(_dec, dec, strlen(dec) + 1);

    _enc = env_base64WebEncode(_dec, strlen(dec));

    for(i = 0; i < strlen(enc); ++i)
    {
        if(enc[i] == _enc[i])
        {
            continue;
        }

        env_logWrite(LOG_TYPE_ERROR, "Error base64 encoding %s to %s: values don't match", 2, dec, enc);
        env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, _enc, enc);

        return 1;
    }

    for(; i < strlen(_enc); ++i)
    {
        if(_enc[i] == '=')
        {
            continue;
        }

        env_logWrite(LOG_TYPE_ERROR, "Error base64 encoding %s to %s: values don't match", 2, dec, enc);
        env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, _enc, enc);

        return 1;
    }

    free(_dec);
    free(_enc);

    return 0;
}

int base64_test()
{
    if(base64_enc_dec_test("YWJjZGVmZw==", "abcdefg"))
    {
        return 1;
    }

    if(base64_enc_dec_test("YWJjZGVmZw", "abcdefg"))
    {
        return 1;
    }

    if(base64_enc_dec_test("aGpr", "hjk"))
    {
        return 1;
    }

    if(base64_enc_dec_test("c3ViamVjdHM_X2Q9MQ==", "subjects?_d=1"))
    {
        return 1;
    }

    if(base64_enc_dec_test("TWFyeSBoYWQgYSBsaXR0bGUgbGFtYi4uLiBASktMTU5PUFpbXF1eX2BhamtsbW5ven1-", "Mary had a little lamb... @JKLMNOPZ[\\]^_`ajklmnoz}~"))
    {
        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "base64 tests passed successfuly", 0);

    return 0;
}