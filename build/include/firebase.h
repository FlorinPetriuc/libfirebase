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

#ifndef _FIREBASELIB_H_
#define _FIREBASELIB_H_

#include <stdio.h>
#include <stdint.h>

#define LIBFIREBASE_VERSION 100

struct libfirebase_response_t
{
    char *header;
    char *body;

    uint16_t code;
};

int libfirebase_init(const uint8_t log_enabled, const char *log_file);
struct libfirebase_response_t *libfirebase_fcm_send(const char *endpoint, const char *message, const char *p256,
                                                    const char *auth, const char *api_key);
struct libfirebase_response_t *libfirebase_fcm_send_bytes(const char *endpoint,
                                    const unsigned char *message, const unsigned int message_len,
                                    const char *p256, const char *auth, const char *api_key);
void libfirebase_free_response(struct libfirebase_response_t *resp);

#endif