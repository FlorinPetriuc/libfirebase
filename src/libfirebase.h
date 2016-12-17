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

#ifndef _LIBFIREBASE_H_
#define _LIBFIREBASE_H_

#include "./env/env.h"
#include "./http/http.h"
#include "./http/http2.h"
#include "./hpack/hpack.h"
#include "./fcm/fcm.h"

#define LIBFIREBASE_SUCCESS             0
#define LIBFIREBASE_CONNECTION_DROPPED  1

typedef void (*libfirebase_fcm_recv_cb)(const unsigned char *data, const uint32_t len,
                                                                const unsigned char error);

int libfirebase_init(const uint8_t log_enabled, const char *log_file);
struct libfirebase_response_t *libfirebase_fcm_send(const char *endpoint, const char *message, const char *p256,
                                                    const char *auth, const char *api_key);
struct libfirebase_response_t *libfirebase_fcm_send_bytes(const char *endpoint,
                                    const unsigned char *message, const unsigned int message_len,
                                    const char *p256, const char *auth, const char *api_key);
void libfirebase_free_response(struct libfirebase_response_t *resp);

int libfirebase_fcm_register_recv(const char *endpoint,
                                  const uint64_t android_id, const uint64_t secret, const uint64_t sender_id,
                                  const char *app, const char *app_id, const char *wp, const char *key,
                                  libfirebase_fcm_recv_cb fcm_recv_cb);

#endif