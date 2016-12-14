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

static int send_notification(struct http_endpoint_t *endpoint, const char *api_key,
                        const unsigned char *message, const unsigned int message_len,
                        const char *dh, const char *salt, struct libfirebase_response_t *out)
{
    int ret;

    char authorization_header[1024];
    char content_len_header[128];
    char crypto_key[128];
    char encryption[128];

    sprintf(content_len_header, "Content-Length: %u", message_len);
    sprintf(crypto_key, "Crypto-Key: keyid=p256dh;dh=%s", dh);
    sprintf(encryption, "Encryption: keyid=p256dh;salt=%s", salt);

    if(api_key)
    {
        sprintf(authorization_header, "Authorization: key=%s", api_key);

        ret = http_do_post(endpoint, message, message_len, out, 7,
                                                    authorization_header,
                                                    "TTL: 0",
                                                    "Content-Type: application/octet-stream",
                                                    "Content-Encoding: aesgcm",
                                                    crypto_key,
                                                    encryption,
                                                    content_len_header);
    }
    else
    {
        ret = http_do_post(endpoint, message, message_len, out, 6,
                                                    "TTL: 0",
                                                    "Content-Type: application/octet-stream",
                                                    "Content-Encoding: aesgcm",
                                                    crypto_key,
                                                    encryption,
                                                    content_len_header);
    }

    return ret;
}

int fcm_send_no_payload(const char *endpoint, const char *api_key, struct libfirebase_response_t *out)
{
    struct http_endpoint_t *_endpoint;

    int ret;

    char authorization_header[1024];

    _endpoint = http_parse_endpoint(endpoint);

    if(_endpoint == NULL)
    {
        return -1;
    }

    if(api_key)
    {
        sprintf(authorization_header, "Authorization: key=%s", api_key);
        ret = http_do_post(_endpoint, (unsigned char *)"", 0, out, 3,
                                        authorization_header,
                                        "TTL: 0",
                                        "Content-Length: 0");
    }
    else
    {
        ret = http_do_post(_endpoint, (unsigned char *)"", 0, out, 2,
                                        "TTL: 0",
                                        "Content-Length: 0");
    }

    free_http_endpoint(_endpoint);

    return ret;
}

int fcm_send(const char *endpoint, const unsigned char *message, const unsigned int message_len,
                const char *p256, const char *auth, const char *api_key, struct libfirebase_response_t *out)
{
    char *_p256;
    char *_auth;
    char *dh;
    char *salt;

    unsigned char *p256_dec;
    unsigned char *auth_dec;
    unsigned char *message_enc;

    unsigned int p256_len;
    unsigned int auth_len;
    unsigned int message_out_len;

    int ret;

    struct http_endpoint_t *_endpoint;

    _endpoint = http_parse_endpoint(endpoint);

    if(_endpoint == NULL)
    {
        return -1;
    }

    p256_len = strlen(p256);
    auth_len = strlen(auth);

    _p256 = env_malloc(p256_len + 1);
    memcpy(_p256, p256, p256_len + 1);

    _auth = env_malloc(auth_len + 1);
    memcpy(_auth, auth, auth_len + 1);

    p256_dec = env_base64WebDecode(_p256, &p256_len);
    auth_dec = env_base64WebDecode(_auth, &auth_len);

    free(_p256);
    free(_auth);

    env_logWrite(LOG_TYPE_INFO, "Decoded p256 has len %u and starts with %hhu and ends with %hhu",
                                                        3, p256_len, p256_dec[0], p256_dec[p256_len - 1]);
    env_logWrite(LOG_TYPE_INFO, "Decoded auth has len %u and starts with %hhu and ends with %hhu",
                                                        3, auth_len, auth_dec[0], auth_dec[auth_len - 1]);

    if(env_prime256v1_aesgcm_encrypt_message(p256_dec, p256_len, auth_dec, auth_len,
                                        message, message_len, &message_enc, &message_out_len, &dh, &salt))
    {
        env_logWrite(LOG_TYPE_ERROR, "Unable to encrypt message", 0);

        free(p256_dec);
        free(auth_dec);

        free_http_endpoint(_endpoint);

        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "Encrypted %u bytes in %u bytes", 2, message_len, message_out_len);

    free(p256_dec);
    free(auth_dec);

    ret = send_notification(_endpoint, api_key, message_enc, message_out_len, dh, salt, out);

    free_http_endpoint(_endpoint);
    free(message_enc);
    free(dh);
    free(salt);

    return ret;
}