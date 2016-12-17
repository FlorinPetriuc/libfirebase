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

#ifndef _HTTP2_H_
#define _HTTP2_H_

struct http2_settings
{
    uint32_t HEADER_TABLE_SIZE;
    uint32_t ENABLE_PUSH;
    uint32_t MAX_CONCURRENT_STREAMS;
    uint32_t INITIAL_WINDOW_SIZE;
    uint32_t MAX_FRAME_SIZE;
    uint32_t MAX_HEADER_LIST_SIZE;
};

#define SETTINGS_HEADER_TABLE_SIZE      1
#define SETTINGS_ENABLE_PUSH            2
#define SETTINGS_MAX_CONCURRENT_STREAMS 3
#define SETTINGS_INITIAL_WINDOW_SIZE    4
#define SETTINGS_MAX_FRAME_SIZE         5
#define SETTINGS_MAX_HEADER_LIST_SIZE   6

#define HTTP2_DATA          0
#define HTTP2_HEADERS       1
#define HTTP2_PRIORITY      2
#define HTTP2_RST_STREAM    3
#define HTTP2_SETTINGS      4
#define HTTP2_PUSH_PROMISE  5
#define HTTP2_PING          6
#define HTTP2_GO_AWAY       7
#define HTTP2_WINDOW_UPDATE 8

enum header_type_t
{
    INDEXED_HEADER_FIELD,
    LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_INDEXED_NAME,
    LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_NEW_NAME,
    LITERAL_HEADER_FIELD_WITHOUT_INDEXING_INDEXED_NAME,
    LITERAL_HEADER_FIELD_WITHOUT_INDEXING_NEW_NAME,
    LITERAL_HEADER_FIELD_NEVER_INDEXED_INDEXED_NAME,
    LITERAL_HEADER_FIELD_NEVER_INDEXED_NEW_NAME
};

struct http2_header
{
    enum header_type_t type;

    unsigned char *header_name;
    unsigned char header_name_len;

    unsigned char *header_value;
    unsigned char header_value_len;
};

struct http2_header *create_header_struct(  enum header_type_t type,
                                            const unsigned char *header_name,
                                            const unsigned char header_name_len,
                                            const unsigned char *header_value,
                                            const unsigned char header_value_len);

int http2_do_post(  struct http_endpoint_t *endpoint,
                    env_secure_socket_t secure_socket, env_socket_t socket,
                    const uint32_t stream_id, const unsigned char priority,
                    const uint32_t stream_dependency, const unsigned char exclusive,
                    const unsigned char weight, const unsigned char end_stream,
                    const unsigned char *message, const uint32_t message_len,
                    const unsigned int headers_no, ...);

struct http2_settings process_http2_settings_request(const unsigned char *payload,
                                                    struct http2_settings old_settings);

#endif