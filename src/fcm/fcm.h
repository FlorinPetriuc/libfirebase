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

int fcm_send(const char *endpoint, const unsigned char *message, const unsigned int message_len,
                const char *p256, const char *auth, const char *api_key, struct libfirebase_response_t *out);
int fcm_send_no_payload(const char *endpoint, const char *api_key, struct libfirebase_response_t *out);

#endif