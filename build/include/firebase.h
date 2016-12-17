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

#define LIBFIREBASE_SUCCESS             0
#define LIBFIREBASE_CONNECTION_DROPPED  1

typedef void (*libfirebase_fcm_recv_cb)(const unsigned char *data, const uint32_t len,
                                                                const unsigned char error);

/*
 * Initializes the library
 *
 * Params:
 * - enable logging (0 - not enabled, 1 - enabled)
 * - log file path or NULL if log file is stdout
 *
 * Returns:
 * - 0 on success
 * - -1 on error
 */
int libfirebase_init(const uint8_t log_enabled, const char *log_file);

/*
 * Sends a string via FCM
 *
 * Params:
 * - client endpoint as received from the cloud messaging server: string
 * - message to send: string
 * - client p256 key: string
 * - client secret authentication token: string
 * - api key for sending or NULL if it is not required
 *
 * Returns:
 * - HTTP response if successful: struct libfirebase_response_t with header, body and http code
 * - NULL on error
 */
struct libfirebase_response_t *libfirebase_fcm_send(const char *endpoint, const char *message, const char *p256,
                                                    const char *auth, const char *api_key);

/*
 * Sends a binary message via FCM
 *
 * Params:
 * - client endpoint as received from the cloud messaging server: string
 * - message to send: byte array
 * - the number of bytes in the message: unsigned int
 * - client p256 key: string
 * - client secret authentication token: string
 * - api key for sending or NULL if it is not required
 *
 * Returns:
 * - HTTP response if successful: struct libfirebase_response_t with header, body and http code
 * - NULL on error
 */
struct libfirebase_response_t *libfirebase_fcm_send_bytes(const char *endpoint,
                                    const unsigned char *message, const unsigned int message_len,
                                    const char *p256, const char *auth, const char *api_key);

/*
 * Frees the message that was returned by fcm send functions
 *
 * Params:
 * - the libfirebase_response_t structure returned by libfirebase_fcm_send or libfirebase_fcm_send_bytes
 */
void libfirebase_free_response(struct libfirebase_response_t *resp);

int libfirebase_fcm_register_recv(const char *endpoint,
                                  const uint64_t android_id, const uint64_t secret, const uint64_t sender_id,
                                  const char *app, const char *app_id, const char *wp, const char *key,
                                  libfirebase_fcm_recv_cb fcm_recv_cb);

#endif