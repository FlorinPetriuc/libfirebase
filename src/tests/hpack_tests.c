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

static int huffman_decode_test_2()
{
    char *h_decoded;

    const char *expected_res = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.100 Safari/537.36";

    const unsigned char to_decode[81] = {   0xd0, 0x7f, 0x66, 0xa2, 0x81, 0xb0, 0xda,
                                            0xe0, 0x53, 0xfa, 0xfc, 0x08, 0x7e, 0xd4,
                                            0xce, 0x6a, 0xad, 0xf2, 0xa7, 0x97, 0x9c,
                                            0x89, 0xc6, 0xbf, 0xb5, 0x21, 0xae, 0xba,
                                            0x0b, 0xc8, 0xb1, 0xe6, 0x32, 0x58, 0x6d,
                                            0x97, 0x57, 0x65, 0xc5, 0x3f, 0xac, 0xd8,
                                            0xf7, 0xe8, 0xcf, 0xf4, 0xa5, 0x06, 0xea,
                                            0x55, 0x31, 0x14, 0x9d, 0x4f, 0xfd, 0xa9,
                                            0x7a, 0x7b, 0x0f, 0x49, 0x58, 0x6d, 0xa5,
                                            0xc0, 0xb8, 0x9e, 0x68, 0x0b, 0x84, 0x00,
                                            0x53, 0x70, 0xe5, 0x1d, 0x86, 0x61, 0xb6,
                                            0x5d, 0x5d, 0x97, 0x3f };

    h_decoded = huffman_decode(to_decode, sizeof(to_decode));

    if(strlen(h_decoded) != strlen(expected_res))
    {
        env_logWrite(LOG_TYPE_ERROR, "huffman_decode_test_1 len failed", 0);

        env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, expected_res, h_decoded);

        return 1;
    }

    if(memcmp(h_decoded, expected_res, strlen(expected_res)) == 0)
    {
        free(h_decoded);

        return 0;
    }

    env_logWrite(LOG_TYPE_ERROR, "huffman_decode_test_1 memcmp failed", 0);

    env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, expected_res, h_decoded);

    return 1;
}

static int huffman_decode_test_1()
{
    char *h_decoded;

    const char *expected_res = "application/x-www-form-urlencoded";

    const unsigned char to_decode[24] = {   0x1d, 0x75, 0xd0, 0x62, 0x0d, 0x26,
                                            0x3d, 0x4c, 0x79, 0x5b, 0xc7, 0x8f,
                                            0x0b, 0x4a, 0x7b, 0x29, 0x5a, 0xdb,
                                            0x28, 0x2d, 0x44, 0x3c, 0x85, 0x93 };

    h_decoded = huffman_decode(to_decode, sizeof(to_decode));

    if(strlen(h_decoded) != strlen(expected_res))
    {
        env_logWrite(LOG_TYPE_ERROR, "huffman_decode_test_1 len failed", 0);

        env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, expected_res, h_decoded);

        return 1;
    }

    if(memcmp(h_decoded, expected_res, strlen(expected_res)) == 0)
    {
        free(h_decoded);

        return 0;
    }

    env_logWrite(LOG_TYPE_ERROR, "huffman_decode_test_1 memcmp failed", 0);

    env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, expected_res, h_decoded);

    return 1;
}

static int huffman_encode_test_2()
{
    unsigned char *h_encoded;

    const char *to_encode = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.100 Safari/537.36";

    unsigned int i;

    const unsigned char expected_res[81] = {    0xd0, 0x7f, 0x66, 0xa2, 0x81, 0xb0, 0xda,
                                                0xe0, 0x53, 0xfa, 0xfc, 0x08, 0x7e, 0xd4,
                                                0xce, 0x6a, 0xad, 0xf2, 0xa7, 0x97, 0x9c,
                                                0x89, 0xc6, 0xbf, 0xb5, 0x21, 0xae, 0xba,
                                                0x0b, 0xc8, 0xb1, 0xe6, 0x32, 0x58, 0x6d,
                                                0x97, 0x57, 0x65, 0xc5, 0x3f, 0xac, 0xd8,
                                                0xf7, 0xe8, 0xcf, 0xf4, 0xa5, 0x06, 0xea,
                                                0x55, 0x31, 0x14, 0x9d, 0x4f, 0xfd, 0xa9,
                                                0x7a, 0x7b, 0x0f, 0x49, 0x58, 0x6d, 0xa5,
                                                0xc0, 0xb8, 0x9e, 0x68, 0x0b, 0x84, 0x00,
                                                0x53, 0x70, 0xe5, 0x1d, 0x86, 0x61, 0xb6,
                                                0x5d, 0x5d, 0x97, 0x3f };

    unsigned int enc_len;

    h_encoded = huffman_encode((const unsigned char *)to_encode, strlen(to_encode), &enc_len);

    if(enc_len != sizeof(expected_res))
    {
        env_logWrite(LOG_TYPE_ERROR, "huffman_encode_test_2 len failed: %u vs %u",
                                                        2, enc_len, sizeof(expected_res));

        return 1;
    }

    if(memcmp(h_encoded, expected_res, enc_len) == 0)
    {
        free(h_encoded);

        return 0;
    }

    env_logWrite(LOG_TYPE_ERROR, "huffman_encode_test_2 memcmp failed", 0);

    for(i = 0; i < enc_len; ++i)
    {
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, expected_res[i], h_encoded[i]);
    }

    return 1;
}

static int huffman_encode_test_1()
{
    unsigned char *h_encoded;

    const char *to_encode = "application/x-www-form-urlencoded";

    unsigned int i;

    const unsigned char expected_res[24] = {    0x1d, 0x75, 0xd0, 0x62, 0x0d, 0x26,
                                                0x3d, 0x4c, 0x79, 0x5b, 0xc7, 0x8f,
                                                0x0b, 0x4a, 0x7b, 0x29, 0x5a, 0xdb,
                                                0x28, 0x2d, 0x44, 0x3c, 0x85, 0x93 };

    unsigned int enc_len;

    h_encoded = huffman_encode((const unsigned char *)to_encode, strlen(to_encode), &enc_len);

    if(enc_len != sizeof(expected_res))
    {
        env_logWrite(LOG_TYPE_ERROR, "huffman_encode_test_1 len failed: %u vs %u",
                                                        2, enc_len, sizeof(expected_res));

        return 1;
    }

    if(memcmp(h_encoded, expected_res, enc_len) == 0)
    {
        free(h_encoded);

        return 0;
    }

    env_logWrite(LOG_TYPE_ERROR, "huffman_encode_test_1 memcmp failed", 0);

    for(i = 0; i < enc_len; ++i)
    {
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, expected_res[i], h_encoded[i]);
    }

    return 1;
}

static int hpack_bit_copy_4_test()
{
    unsigned int src;

    unsigned char dst[10];
    unsigned char expected_result[10];

    src = 0x1ff8;

    dst[0] = 0xE1;
    dst[1] = 0xD2;
    dst[2] = 0x00;
    dst[3] = 0x00;

    expected_result[0] = 0xE1;
    expected_result[1] = 0xD2;
    expected_result[2] = 0xFF;
    expected_result[3] = 0xC0;

    bit_copy(dst, src, 16, 13);

    if(memcmp(dst, expected_result, 4))
    {
        env_logWrite(LOG_TYPE_ERROR, "bit_copy 4 test failed", 0);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[0], expected_result[0]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[1], expected_result[1]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[2], expected_result[2]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[3], expected_result[3]);

        return 1;
    }

    return 0;
}

static int hpack_bit_copy_3_test()
{
    unsigned int src;

    unsigned char dst[10];
    unsigned char expected_result[10];

    src = 0x1ff8;

    dst[0] = 0xE1;
    dst[1] = 0xD2;
    dst[2] = 0x00;
    dst[3] = 0x00;

    expected_result[0] = 0xE1;
    expected_result[1] = 0xD3;
    expected_result[2] = 0xFF;
    expected_result[3] = 0x80;

    bit_copy(dst, src, 15, 13);

    if(memcmp(dst, expected_result, 4))
    {
        env_logWrite(LOG_TYPE_ERROR, "bit_copy 3 test failed", 0);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[0], expected_result[0]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[1], expected_result[1]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[2], expected_result[2]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[3], expected_result[3]);

        return 1;
    }

    return 0;
}

static int hpack_bit_copy_2_test()
{
    unsigned int src;

    unsigned char dst[10];
    unsigned char expected_result[10];

    src = 0x1ff8;

    dst[0] = 0xE0;
    dst[1] = 0;

    expected_result[0] = 0xFF;
    expected_result[1] = 0xF8;

    bit_copy(dst, src, 3, 13);

    if(memcmp(dst, expected_result, 2))
    {
        env_logWrite(LOG_TYPE_ERROR, "bit_copy 2 test failed", 0);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[0], expected_result[0]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[1], expected_result[1]);

        return 1;
    }

    return 0;
}

static int hpack_bit_copy_1_test()
{
    uint32_t src;
    unsigned char dst[10];
    unsigned char expected_result[10];

    src = 0x87FEA9;

    dst[0] = 0xF0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;
    dst[4] = 0;

    expected_result[0] = 0xF8;
    expected_result[1] = 0x7F;
    expected_result[2] = 0xEA;
    expected_result[3] = 0x90;

    bit_copy(dst, src, 4, 24);

    if(memcmp(dst, expected_result, 4))
    {
        env_logWrite(LOG_TYPE_ERROR, "bit_copy 1 test failed", 0);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[0], expected_result[0]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[1], expected_result[1]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[2], expected_result[2]);
        env_logWrite(LOG_TYPE_ERROR, "%02hhx vs %02hhx", 2, dst[3], expected_result[3]);

        return 1;
    }

    return 0;
}

int hpack_test()
{
    if(hpack_bit_copy_1_test())
    {
        return 1;
    }

    if(hpack_bit_copy_2_test())
    {
        return 1;
    }

    if(hpack_bit_copy_3_test())
    {
        return 1;
    }

    if(hpack_bit_copy_4_test())
    {
        return 1;
    }

    if(huffman_encode_test_1())
    {
        return 1;
    }

    if(huffman_encode_test_2())
    {
        return 1;
    }

    if(huffman_decode_test_1())
    {
        return 1;
    }

    if(huffman_decode_test_2())
    {
        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "hpack tests passed successfuly", 0);

    return 0;
}