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

#ifndef _FCM_H_
#define _FCM_H_

struct libfirebase_response_t;

typedef void (*libfirebase_fcm_recv_cb)(const unsigned char *data, const uint32_t len,
                                                                const unsigned char error);

struct fcm_recv_params
{
    env_secure_socket_t secure_socket;
    env_socket_t socket;

    libfirebase_fcm_recv_cb fcm_recv_cb;

    struct http2_settings cli_settings;
    struct http2_settings srv_settings;
};

typedef void (*libfirebase_fcm_recv_cb)(const unsigned char *data, const uint32_t len,
                                                                const unsigned char error);

int fcm_send(const char *endpoint, const unsigned char *message, const unsigned int message_len,
                const char *p256, const char *auth, const char *api_key, struct libfirebase_response_t *out);
int fcm_send_no_payload(const char *endpoint, const char *api_key, struct libfirebase_response_t *out);

int fcm_register_recv(const char *endpoint,
                      const uint64_t android_id, const uint64_t secret, const uint64_t sender_id,
                      const char *app, const char *app_id, const char *wp, const char *key,
                      libfirebase_fcm_recv_cb fcm_recv_cb);

#endif