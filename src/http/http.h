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

#ifndef _HTTP_H_
#define _HTTP_H_

enum http_protocol_type_t
{
    PROTOCOL_TYPE_HTTP,
    PROTOCOL_TYPE_HTTPS
};

enum http_request_type_t
{
    PROTOCOL_HTTP_GET,
    PROTOCOL_HTTP_POST
};

struct http_endpoint_t
{
    enum http_protocol_type_t proto;

    char *domain;
    char *url_path;

    uint16_t port;
};

struct libfirebase_response_t
{
    char *header;
    char *body;

    uint16_t code;
};

void free_firebase_response(struct libfirebase_response_t *resp);

struct http_endpoint_t *http_parse_endpoint(const char *address);
void free_http_endpoint(struct http_endpoint_t *endpoint);

int http_do_post(struct http_endpoint_t *endpoint, const unsigned char *message,
                const unsigned int message_len, struct libfirebase_response_t *out,
                const unsigned int headers_no, ...);

#endif