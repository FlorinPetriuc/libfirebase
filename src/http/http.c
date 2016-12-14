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

char *http_build_header(enum http_request_type_t req_type, const char *domain,
                        const char *url_path, const unsigned int headers_no, va_list args)
{
    char *ret;

    int i;
    int offset;

    const char *arg_header;

    ret = env_malloc(2049);

    switch(req_type)
    {
        case PROTOCOL_HTTP_GET:
        {
            offset = sprintf(ret, "GET %s HTTP/1.1\r\n"
                                  "Host: %s", url_path, domain);
        }
        break;

        case PROTOCOL_HTTP_POST:
        {
            offset = sprintf(ret, "POST %s HTTP/1.1\r\n"
                                  "Host: %s", url_path, domain);
        }
        break;

        default:
        {
            free(ret);

            env_logWrite(LOG_TYPE_ERROR, "Unknown http request type", 0);
        }
        return NULL;
    }

    for(i = 0; i < headers_no; ++i)
    {
        arg_header = va_arg(args, const char *);

        if(offset + strlen(arg_header) < 2042)
        {
            offset += sprintf(ret + offset, "\r\n%s", arg_header);

            continue;
        }

        env_logWrite(LOG_TYPE_ERROR, "Unknown http request type", 0);

        free(ret);

        return NULL;
    }

    sprintf(ret + offset, "\r\n\r\n");

    return ret;
}

static int get_http_response(env_socket_t usocket, env_secure_socket_t ssocket, struct libfirebase_response_t *out)
{
    int len_received = 0;
    int recv_ret;

    char *body;
    char *content_length_str;
    char header[2049];

    unsigned int content_length;

receive_header:

    if(2048 - len_received <= 0)
    {
        return -1;
    }

    if(usocket != ENV_INVALID_SOCKET)
    {
        recv_ret = env_read_tcp_data(usocket, header + len_received, 2048 - len_received);
    }
    else
    {
        recv_ret = env_read_secure_data(ssocket, header + len_received, 2048 - len_received);
    }

    if(recv_ret <= 0)
    {
        env_logWrite(LOG_TYPE_ERROR, "Recv returns <= 0", 0);

        return -1;
    }

    len_received += recv_ret;

    header[len_received] = '\0';
    body = strstr(header, "\r\n\r\n");

    if(body == NULL)
    {
        goto receive_header;
    }

    body[0] = '\0';

    out->header = env_malloc(body - header + 1);
    strcpy(out->header, header);

    body += 4;
    header[len_received] = '\0';

    content_length_str = strstr(header, "Content-Length: ");

    if(content_length_str == NULL)
    {
        content_length = 0;
    }
    else if(sscanf(content_length_str, "Content-Length: %u\r\n", &content_length) != 1)
    {
        env_logWrite(LOG_TYPE_ERROR, "Content length is NAN", 0);

        return -1;
    }

    out->body = env_malloc(content_length + 1);

    if(content_length)
    {
        strcpy(out->body, body);
        len_received = strlen(out->body);
    }
    else
    {
        len_received = 0;
    }

    out->body[len_received] = '\0';

receive_body:

    if(content_length - len_received > 0)
    {
        if(usocket != ENV_INVALID_SOCKET)
        {
            recv_ret = env_read_tcp_data(usocket, out->body + len_received, content_length - len_received);
        }
        else
        {
            recv_ret = env_read_secure_data(ssocket, out->body + len_received, content_length - len_received);
        }

        if(recv_ret <= 0)
        {
            env_logWrite(LOG_TYPE_ERROR, "Can not read body", 0);

            return -1;
        }

        len_received += recv_ret;
        out->body[len_received] = '\0';

        if(len_received != content_length)
        {
            goto receive_body;
        }
    }

    if(sscanf(out->header, "HTTP/1.1 %hu ", &out->code) != 1 &&
       sscanf(out->header, "HTTP/1.0 %hu ", &out->code) != 1)
    {
        env_logWrite(LOG_TYPE_ERROR, "Corrupt http header", 0);

        return -1;
    }

    return 0;
}

int http_do_post(struct http_endpoint_t *endpoint, const unsigned char *message,
                    const unsigned int message_len, struct libfirebase_response_t *out,
                    const unsigned int headers_no, ...)
{
    char *header;

    env_socket_t socket;
    env_secure_socket_t secure_socket;

    va_list args;

    va_start(args, headers_no);

    header = http_build_header(PROTOCOL_HTTP_POST, endpoint->domain, endpoint->url_path, headers_no, args);

    va_end(args);

    socket = env_connect_to_url(endpoint->domain, endpoint->port, 5000);

    if(socket == ENV_INVALID_SOCKET)
    {
        free(header);

        return 1;
    }

    switch(endpoint->proto)
    {
        case PROTOCOL_TYPE_HTTPS:
        {
            secure_socket = env_secure_server_connection(socket);

            if(secure_socket == ENV_INVALID_SECURE_SOCKET)
            {
                free(header);

                return 1;
            }

            if(env_send_secure_data(secure_socket, header))
            {
                env_close_secure_connection(secure_socket);

                free(header);

                return 1;
            }

            free(header);

            if(env_send_n_secure_data(secure_socket, (char *)message, message_len))
            {
                env_close_secure_connection(secure_socket);

                return 1;
            }

            if(get_http_response(ENV_INVALID_SOCKET, secure_socket, out))
            {
                env_close_secure_connection(secure_socket);

                return 1;
            }

            env_close_secure_connection(secure_socket);
        }
        return 0;

        case PROTOCOL_TYPE_HTTP:
        {
            if(env_send_tcp_data(socket, header))
            {
                free(header);

                env_close_socket(socket);

                return 1;
            }

            free(header);

            if(env_send_n_tcp_data(socket, (char *)message, message_len))
            {
                env_close_socket(socket);

                return 1;
            }

            if(get_http_response(socket, ENV_INVALID_SECURE_SOCKET, out))
            {
                env_close_socket(socket);

                return 1;
            }

            env_close_socket(socket);
        }
        return 0;

        default:
        {
            free(header);

            env_close_socket(socket);
        }
        return 1;
    }
}

void free_firebase_response(struct libfirebase_response_t *resp)
{
    if(resp == NULL)
    {
        return;
    }

    if(resp->body)
    {
        free(resp->body);
    }

    if(resp->header)
    {
        free(resp->header);
    }

    free(resp);
}

struct http_endpoint_t *http_parse_endpoint(const char *address)
{
    struct http_endpoint_t *ret;

    const char *p_address;

    enum http_protocol_type_t proto;

    uint16_t port;

    int i;
    int j;

    if(env_starts_with(address, "http://") == 0)
    {
        proto = PROTOCOL_TYPE_HTTP;
        p_address = address + ENV_STRLEN_MACRO("http://");

        port = 80;
    }
    else if(env_starts_with(address, "https://") == 0)
    {
        proto = PROTOCOL_TYPE_HTTPS;
        p_address = address + ENV_STRLEN_MACRO("https://");

        port = 443;
    }
    else
    {
        env_logWrite(LOG_TYPE_ERROR, "Endpoint address can not be parsed: %s", 1, address);

        return NULL;
    }

    ret = env_malloc(sizeof(struct http_endpoint_t));
    ret->proto = proto;

    i = 0;

    while(p_address[i] != '/' && p_address[i] != '\0' && p_address[i] != ':')
    {
        ++i;
    }

    ret->domain = env_malloc(i + 1);
    memcpy(ret->domain, p_address, i);
    ret->domain[i] = '\0';

    if(p_address[i] == '\0')
    {
        ret->url_path = env_malloc(2);
        memcpy(ret->url_path, "/", 2);

        ret->port = port;

        return ret;
    }

    if(p_address[i] == ':')
    {
        ++i;

        port = 0;

        while(p_address[i] >= '0' && p_address[i] <= '9')
        {
            port = port * 10 + p_address[i] - '0';

            ++i;
        }
    }

    ret->port = port;

    if(p_address[i] == '\0')
    {
        ret->url_path = env_malloc(2);
        memcpy(ret->url_path, "/", 2);

        return ret;
    }

    j = i + 1;

    while(p_address[j] != '\0')
    {
        ++j;
    }

    ret->url_path = env_malloc(j - i + 1);
    memcpy(ret->url_path, p_address + i, j - i + 1);

    return ret;
}

void free_http_endpoint(struct http_endpoint_t *endpoint)
{
    free(endpoint->url_path);
    free(endpoint->domain);
    free(endpoint);
}