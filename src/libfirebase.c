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

#include "libfirebase.h"

int libfirebase_init(const uint8_t log_enabled, const char *log_file)
{
    if(env_logInit(log_enabled, log_file))
    {
        return -1;
    }

    if(env_ssl_init())
    {
        return -1;
    }

    return 0;
}

struct libfirebase_response_t *libfirebase_fcm_send_bytes(const char *endpoint,
                                    const unsigned char *message, const unsigned int message_len,
                                    const char *p256, const char *auth, const char *api_key)
{
    struct libfirebase_response_t *ret;

    ret = env_malloc(sizeof(struct libfirebase_response_t));

    if(message)
    {
        if(fcm_send(endpoint, message, message_len, p256, auth, api_key, ret))
        {
            free(ret);

            return NULL;
        }

        return ret;
    }

    if(fcm_send_no_payload(endpoint, api_key, ret))
    {
        free(ret);

        return NULL;
    }

    return ret;
}

struct libfirebase_response_t *libfirebase_fcm_send(const char *endpoint, const char *message,
                                const char *p256, const char *auth, const char *api_key)
{
    if(message)
    {
        return libfirebase_fcm_send_bytes(endpoint, (const unsigned char *)message,
                                                strlen(message), p256, auth, api_key);
    }

    return libfirebase_fcm_send_bytes(endpoint, NULL, 0, p256, auth, api_key);
}

void libfirebase_free_response(struct libfirebase_response_t *resp)
{
    free_firebase_response(resp);
}

int libfirebase_fcm_register_recv(const char *endpoint,
                                  const uint64_t android_id, const uint64_t secret, const uint64_t sender_id,
                                  const char *app, const char *app_id, const char *wp, const char *key,
                                  libfirebase_fcm_recv_cb fcm_recv_cb)
{
    int ret;

    ret = fcm_register_recv(endpoint, android_id, secret, sender_id, app, app_id, wp, key, fcm_recv_cb);

    return ret;
}