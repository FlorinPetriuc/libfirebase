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

struct http2_header *create_header_struct(  enum header_type_t type,
                                            const unsigned char *header_name,
                                            const unsigned char header_name_len,
                                            const unsigned char *header_value,
                                            const unsigned char header_value_len)
{
    struct http2_header *ret;

    ret = env_malloc(sizeof(struct http2_header));

    ret->type = type;

    ret->header_name_len = header_name_len;
    if(header_name_len)
    {
        ret->header_name = env_malloc(header_name_len);
        memcpy(ret->header_name, header_name, header_name_len);
    }
    else
    {
        ret->header_name = NULL;
    }

    ret->header_value_len = header_value_len;

    if(header_value_len)
    {
        ret->header_value = env_malloc(header_value_len);
        memcpy(ret->header_value, header_value, header_value_len);
    }
    else
    {
        ret->header_value = NULL;
    }

    return ret;
}

unsigned char *generate_byte_header(struct http2_header *header, unsigned int *header_len)
{
    unsigned char *ret;

    unsigned char *v;
    unsigned char *n;

    unsigned int v_len = 0;
    unsigned int n_len = 0;

    switch(header->type)
    {
        case INDEXED_HEADER_FIELD:
        {
            ret = env_malloc(1);

            ret[0] = 0x80 | (header->header_name[0] & 0x7F);

            *header_len = 1;
        }
        return ret;

        case LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_INDEXED_NAME:
        {
            v = huffman_encode(header->header_value, header->header_value_len, &v_len);
            v_len &= 0x7F;

            ret = env_malloc(2 + v_len);

            ret[0] = 0x40 | (header->header_name[0] & 0x3F);

            ret[1] = 0x80 | v_len;
            memcpy(ret + 2, v, v_len);

            free(v);

            *header_len = 2 + v_len;
        }
        return ret;

        case LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_NEW_NAME:
        {
            n = huffman_encode(header->header_name, header->header_name_len, &n_len);
            v = huffman_encode(header->header_value, header->header_value_len, &v_len);
            v_len &= 0x7F;
            n_len &= 0x7F;

            ret = env_malloc(3 + n_len + v_len);

            ret[0] = 0x40;

            ret[1] = 0x80 | n_len;
            memcpy(ret + 2, n, n_len);

            ret[2 + n_len] = 0x80 | v_len;
            memcpy(ret + 2 + n_len + 1, v, v_len);

            free(n);
            free(v);

            *header_len = 3 + v_len + n_len;
        }
        return ret;

        case LITERAL_HEADER_FIELD_NEVER_INDEXED_INDEXED_NAME:
        {
            v = huffman_encode(header->header_value, header->header_value_len, &v_len);
            v_len &= 0x7F;

            ret = env_malloc(2 + v_len);

            ret[0] = 0x10 | (header->header_name[0] & 0x0F);

            ret[1] = 0x80 | v_len;
            memcpy(ret + 2, v, v_len);

            free(v);

            *header_len = 2 + v_len;
        }
        return ret;

        case LITERAL_HEADER_FIELD_NEVER_INDEXED_NEW_NAME:
        {
            n = huffman_encode(header->header_name, header->header_name_len, &n_len);
            v = huffman_encode(header->header_value, header->header_value_len, &v_len);
            v_len &= 0x7F;
            n_len &= 0x7F;

            ret = env_malloc(3 + n_len + v_len);

            ret[0] = 0x10;

            ret[1] = 0x80 | n_len;
            memcpy(ret + 2, n, n_len);

            ret[2 + n_len] = 0x80 | v_len;
            memcpy(ret + 2 + n_len + 1, v, v_len);

            free(n);
            free(v);

            *header_len = 3 + v_len + n_len;
        }
        return ret;

        case LITERAL_HEADER_FIELD_WITHOUT_INDEXING_INDEXED_NAME:
        {
            v = huffman_encode(header->header_value, header->header_value_len, &v_len);
            v_len &= 0x7F;

            ret = env_malloc(2 + v_len);

            ret[0] = 0x00 | (header->header_name[0] & 0x0F);

            ret[1] = 0x80 | v_len;
            memcpy(ret + 2, v, v_len);

            free(v);

            *header_len = 2 + v_len;
        }
        return ret;

        case LITERAL_HEADER_FIELD_WITHOUT_INDEXING_NEW_NAME:
        {
            n = huffman_encode(header->header_name, header->header_name_len, &n_len);
            v = huffman_encode(header->header_value, header->header_value_len, &v_len);
            v_len &= 0x7F;
            n_len &= 0x7F;

            ret = env_malloc(3 + n_len + v_len);

            ret[0] = 0x00;

            ret[1] = 0x80 | n_len;
            memcpy(ret + 2, n, n_len);

            ret[2 + n_len] = 0x80 | v_len;
            memcpy(ret + 2 + n_len + 1, v, v_len);

            free(n);
            free(v);

            *header_len = 3 + v_len + n_len;
        }
        return ret;
    }

    return NULL;
}

int http2_do_post(  struct http_endpoint_t *endpoint,
                    env_secure_socket_t secure_socket, env_socket_t socket,
                    const uint32_t stream_id, const unsigned char priority,
                    const uint32_t stream_dependency, const unsigned char exclusive,
                    const unsigned char weight, const unsigned char end_stream,
                    const unsigned char *message, const uint32_t message_len,
                    const unsigned int headers_no, ...)
{
    char header[2048];

    struct http2_header *crt_header;

    unsigned char *crt_header_bytes;
    unsigned char data_header[9];

    char content_length[10];

    uint32_t header_offset;

    unsigned int i;
    unsigned int crt_header_len;

    int ret;

    va_list args;

    //http2 frame len
    header[0] = 0;
    header[1] = 0;
    header[2] = 0;

    //http2 frame type
    header[3] = HTTP2_HEADERS;

    //flags -> end headers + priority
    if(priority)
    {
        header[4] = 0x24;
    }
    else
    {
        header[4] = 0x04;
    }

    //stream id
    header[5] = (stream_id >> 24) & 0x7F;
    header[6] = (stream_id >> 16) & 0xFF;
    header[7] = (stream_id >> 8) & 0xFF;
    header[8] = (stream_id) & 0xFF;

    if(priority)
    {
        header[9] = (stream_dependency >> 24) & 0x7F;
        header[10] = (stream_dependency >> 16) & 0xFF;
        header[11] = (stream_dependency >> 8) & 0xFF;
        header[12] = (stream_dependency) & 0xFF;

        if(exclusive)
        {
            header[9] |= (1 << 7);
            header[13] = 146;

            header_offset = 14;
        }
        else
        {
            header_offset = 13;
        }
    }
    else
    {
        header_offset = 9;
    }

    //Method: POST
    header[header_offset++] = 0x83;

    //Authority: endpoint domain
    header[header_offset++] = 0x41;

    crt_header_bytes = huffman_encode((unsigned char *)endpoint->domain,
                                        strlen(endpoint->domain), &crt_header_len);

    header[header_offset++] = 0x80 | crt_header_len;
    memcpy(header + header_offset, crt_header_bytes, crt_header_len);
    header_offset += crt_header_len;
    free(crt_header_bytes);

    //Authority: endpoint domain
    header[header_offset++] = 0x00;

    crt_header_bytes = huffman_encode((unsigned char *)":path", strlen(":path"), &crt_header_len);

    header[header_offset++] = 0x80 | crt_header_len;
    memcpy(header + header_offset, crt_header_bytes, crt_header_len);
    header_offset += crt_header_len;
    free(crt_header_bytes);

    crt_header_bytes = huffman_encode((unsigned char *)endpoint->url_path,
                                        strlen(endpoint->url_path), &crt_header_len);

    header[header_offset++] = 0x80 | crt_header_len;
    memcpy(header + header_offset, crt_header_bytes, crt_header_len);
    header_offset += crt_header_len;
    free(crt_header_bytes);

    //content-length
    header[header_offset++] = 0x5c;

    sprintf(content_length, "%d", message_len);

    crt_header_bytes = huffman_encode((unsigned char *)content_length,
                                            strlen(content_length), &crt_header_len);

    header[header_offset++] = 0x80 | crt_header_len;
    memcpy(header + header_offset, crt_header_bytes, crt_header_len);
    header_offset += crt_header_len;
    free(crt_header_bytes);

    va_start(args, headers_no);

    for(i = 0; i < headers_no; ++i)
    {
        crt_header = va_arg(args, struct http2_header *);

        crt_header_bytes = generate_byte_header(crt_header, &crt_header_len);

        memcpy(header + header_offset, crt_header_bytes, crt_header_len);
        header_offset += crt_header_len;

        free(crt_header_bytes);

        if(crt_header->header_name)
        {
            free(crt_header->header_name);
        }

        if(crt_header->header_name)
        {
            free(crt_header->header_value);
        }

        free(crt_header);
    }

    va_end(args);

    //http2 header frame len
    header[0] = (header_offset >> 16) & 0xFF;
    header[1] = (header_offset >> 8) & 0xFF;
    header[2] = (header_offset) & 0xFF;

    //http2 data frame len
    data_header[0] = (message_len >> 16) & 0xFF;
    data_header[1] = (message_len >> 8) & 0xFF;
    data_header[2] = (message_len) & 0xFF;

    //http2 frame type
    data_header[3] = HTTP2_DATA;

    //http2 data frame flags
    if(end_stream)
    {
        data_header[4] = 1;
    }
    else
    {
        data_header[4] = 0;
    }

    //stream id
    data_header[5] = (stream_id >> 24) & 0x7F;
    data_header[6] = (stream_id >> 16) & 0xFF;
    data_header[7] = (stream_id >> 8) & 0xFF;
    data_header[8] = (stream_id) & 0xFF;

    if(endpoint->proto == PROTOCOL_TYPE_HTTPS)
    {
        ret = env_send_n_secure_data(secure_socket, (const char *)header, header_offset);
    }
    else
    {
        ret = env_send_n_tcp_data(socket, (const char *)header, header_offset);
    }

    if(ret)
    {
        return 1;
    }

    if(endpoint->proto == PROTOCOL_TYPE_HTTPS)
    {
        ret = env_send_n_secure_data(secure_socket, (const char *)data_header, 9);
    }
    else
    {
        ret = env_send_n_tcp_data(socket, (const char *)data_header, 9);
    }

    if(ret)
    {
        return 1;
    }

    if(endpoint->proto == PROTOCOL_TYPE_HTTPS)
    {
        ret = env_send_n_secure_data(secure_socket, (const char *)message, message_len);
    }
    else
    {
        ret = env_send_n_tcp_data(socket, (const char *)message, message_len);
    }

    if(ret)
    {
        return 1;
    }

    return 0;
}

struct http2_settings process_http2_settings_request(const unsigned char *payload,
                                                    struct http2_settings old_settings)
{
    uint16_t config_idx;

    uint32_t config_value;
    uint32_t frame_len;

    int i;

    struct http2_settings ret;

    frame_len = ((payload[0] << 16) | (payload[1] << 8) | payload[2]) + 9;

    if(payload[4] & 1)
    {
        return old_settings;
    }

    if((frame_len - 9) % 6)
    {
        env_logWrite(LOG_TYPE_ERROR, "Got settings frame which is not multiple of 6: %u",
                                                                                1, frame_len);

        return old_settings;
    }

    ret = old_settings;

    if(frame_len == 9)
    {
        return ret;
    }

    for(i = 9; i < frame_len; i += 6)
    {
        config_idx = (payload[i] << 8) | payload[i + 1];
        config_value = (payload[i + 2] << 24) | (payload[i + 3] << 16) |
                       (payload[i + 4] << 8) | payload[i + 5];

        env_logWrite(LOG_TYPE_INFO, "Setting http idx %u to %u", 2, config_idx, config_value);

        switch(config_idx)
        {
            case SETTINGS_HEADER_TABLE_SIZE:
            {
                ret.HEADER_TABLE_SIZE = config_value;
            }
            break;

            case SETTINGS_ENABLE_PUSH:
            {
                ret.ENABLE_PUSH = config_value;
            }
            break;

            case SETTINGS_MAX_CONCURRENT_STREAMS:
            {
                ret.MAX_CONCURRENT_STREAMS = config_value;
            }
            break;

            case SETTINGS_INITIAL_WINDOW_SIZE:
            {
                ret.INITIAL_WINDOW_SIZE = config_value;
            }
            break;

            case SETTINGS_MAX_FRAME_SIZE:
            {
                ret.MAX_FRAME_SIZE = config_value;
            }
            break;

            case SETTINGS_MAX_HEADER_LIST_SIZE:
            {
                ret.MAX_HEADER_LIST_SIZE = config_value;
            }
            break;
        }
    }

    return ret;
}