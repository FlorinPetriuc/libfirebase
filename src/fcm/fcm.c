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

static int send_notification(struct http_endpoint_t *endpoint, const char *api_key,
                        const unsigned char *message, const unsigned int message_len,
                        const char *dh, const char *salt, struct libfirebase_response_t *out)
{
    int ret;

    char authorization_header[1024];
    char content_len_header[128];
    char crypto_key[128];
    char encryption[128];

    sprintf(content_len_header, "Content-Length: %u", message_len);
    sprintf(crypto_key, "Crypto-Key: keyid=p256dh;dh=%s", dh);
    sprintf(encryption, "Encryption: keyid=p256dh;salt=%s", salt);

    if(api_key)
    {
        sprintf(authorization_header, "Authorization: key=%s", api_key);

        ret = http_do_post(endpoint, message, message_len, out, 7,
                                                    authorization_header,
                                                    "TTL: 0",
                                                    "Content-Type: application/octet-stream",
                                                    "Content-Encoding: aesgcm",
                                                    crypto_key,
                                                    encryption,
                                                    content_len_header);
    }
    else
    {
        ret = http_do_post(endpoint, message, message_len, out, 6,
                                                    "TTL: 0",
                                                    "Content-Type: application/octet-stream",
                                                    "Content-Encoding: aesgcm",
                                                    crypto_key,
                                                    encryption,
                                                    content_len_header);
    }

    return ret;
}

int fcm_send_no_payload(const char *endpoint, const char *api_key, struct libfirebase_response_t *out)
{
    struct http_endpoint_t *_endpoint;

    int ret;

    char authorization_header[1024];

    _endpoint = http_parse_endpoint(endpoint);

    if(_endpoint == NULL)
    {
        return -1;
    }

    if(api_key)
    {
        sprintf(authorization_header, "Authorization: key=%s", api_key);
        ret = http_do_post(_endpoint, (unsigned char *)"", 0, out, 3,
                                        authorization_header,
                                        "TTL: 0",
                                        "Content-Length: 0");
    }
    else
    {
        ret = http_do_post(_endpoint, (unsigned char *)"", 0, out, 2,
                                        "TTL: 0",
                                        "Content-Length: 0");
    }

    free_http_endpoint(_endpoint);

    return ret;
}

int fcm_send(const char *endpoint, const unsigned char *message, const unsigned int message_len,
                const char *p256, const char *auth, const char *api_key, struct libfirebase_response_t *out)
{
    char *_p256;
    char *_auth;
    char *dh;
    char *salt;

    unsigned char *p256_dec;
    unsigned char *auth_dec;
    unsigned char *message_enc;

    unsigned int p256_len;
    unsigned int auth_len;
    unsigned int message_out_len;

    int ret;

    struct http_endpoint_t *_endpoint;

    _endpoint = http_parse_endpoint(endpoint);

    if(_endpoint == NULL)
    {
        return -1;
    }

    p256_len = strlen(p256);
    auth_len = strlen(auth);

    _p256 = env_malloc(p256_len + 1);
    memcpy(_p256, p256, p256_len + 1);

    _auth = env_malloc(auth_len + 1);
    memcpy(_auth, auth, auth_len + 1);

    p256_dec = env_base64WebDecode(_p256, &p256_len);
    auth_dec = env_base64WebDecode(_auth, &auth_len);

    free(_p256);
    free(_auth);

    env_logWrite(LOG_TYPE_INFO, "Decoded p256 has len %u and starts with %hhu and ends with %hhu",
                                                        3, p256_len, p256_dec[0], p256_dec[p256_len - 1]);
    env_logWrite(LOG_TYPE_INFO, "Decoded auth has len %u and starts with %hhu and ends with %hhu",
                                                        3, auth_len, auth_dec[0], auth_dec[auth_len - 1]);

    if(env_prime256v1_aesgcm_encrypt_message(p256_dec, p256_len, auth_dec, auth_len,
                                        message, message_len, &message_enc, &message_out_len, &dh, &salt))
    {
        env_logWrite(LOG_TYPE_ERROR, "Unable to encrypt message", 0);

        free(p256_dec);
        free(auth_dec);

        free_http_endpoint(_endpoint);

        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "Encrypted %u bytes in %u bytes", 2, message_len, message_out_len);

    free(p256_dec);
    free(auth_dec);

    ret = send_notification(_endpoint, api_key, message_enc, message_out_len, dh, salt, out);

    free_http_endpoint(_endpoint);
    free(message_enc);
    free(dh);
    free(salt);

    return ret;
}

void *fcm_recv_worker(void *arg)
{
    struct fcm_recv_params *prm = arg;

    unsigned char in_buffer[65536];

    uint32_t in_buf_offset;
    uint32_t len;

    int ret;

    in_buf_offset = 0;

    while(1)
    {
        if(prm->secure_socket != ENV_INVALID_SECURE_SOCKET)
        {
            ret = env_read_secure_data(prm->secure_socket, (char *)in_buffer + in_buf_offset,
                                                                sizeof(in_buffer) - in_buf_offset);
        }
        else
        {
            ret = env_read_tcp_data(prm->socket, (char *)in_buffer + in_buf_offset,
                                                                sizeof(in_buffer) - in_buf_offset);
        }

        if(ret < 0)
        {
            prm->fcm_recv_cb(NULL, 0, LIBFIREBASE_CONNECTION_DROPPED);

            goto out;
        }

        if(ret == 0)
        {
            continue;
        }

        in_buf_offset += ret;

        if(in_buf_offset < 3)
        {
            continue;
        }

        len = ((in_buffer[0] << 16) | (in_buffer[1] << 8) | (in_buffer[2])) + 9;

        if(in_buf_offset < len)
        {
            continue;
        }

        env_logWrite(LOG_TYPE_INFO, "Got http2 frame of len %u from %u total bytes read",
                                                                2, len, in_buf_offset);

        switch(in_buffer[3])
        {
            case HTTP2_DATA:
            {
            }
            break;

            case HTTP2_HEADERS:
            {
            }
            break;

            case HTTP2_PRIORITY:
            {
            }
            break;

            case HTTP2_RST_STREAM:
            {
            }
            break;

            case HTTP2_SETTINGS:
            {
                prm->srv_settings = process_http2_settings_request(in_buffer, prm->srv_settings);
            }
            break;

            case HTTP2_PUSH_PROMISE:
            {
            }
            break;

            case HTTP2_PING:
            {
            }
            break;

            case HTTP2_GO_AWAY:
            {
            }
            break;

            case HTTP2_WINDOW_UPDATE:
            {
            }
            break;

            default: break;
        }

        memcpy(in_buffer, in_buffer + len, in_buf_offset - len);
        in_buf_offset -= len;
    }

out:

    if(prm->secure_socket != ENV_INVALID_SECURE_SOCKET)
    {
        env_close_secure_connection(prm->secure_socket);
    }
    else
    {
        env_close_socket(prm->socket);
    }

    free(prm);

    return NULL;
}

int fcm_register_recv(const char *endpoint,
                      const uint64_t android_id, const uint64_t secret, const uint64_t sender_id,
                      const char *app, const char *app_id, const char *wp, const char *key,
                      libfirebase_fcm_recv_cb fcm_recv_cb)
{
    struct http_endpoint_t *endpoint_parsed = NULL;
    struct fcm_recv_params *recv_prms = NULL;

    int ret;

    pthread_t worker;

    char body[1024];
    char authorization[512];
    const char *content_type = "application/x-www-form-urlencoded";
    const char *user_agent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36"
                            " (KHTML, like Gecko) Chrome/54.0.2840.100 Safari/537.36";
    const char *accept_encoding = "gzip, deflate, br";
    const char *accept_language = "en-US,en;q=0.8";

    const unsigned char scheme_header = 7;
    const unsigned char authorization_header = 23;
    const unsigned char content_type_header = 31;
    const unsigned char user_agent_header = 58;
    const unsigned char accept_encoding_header = 16;
    const unsigned char accept_language_header = 17;

    env_socket_t socket = ENV_INVALID_SOCKET;
    env_secure_socket_t secure_socket = ENV_INVALID_SECURE_SOCKET;

    const char HTTP2_PRISM[] = "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n";

    const unsigned char HTTP2_SETTINGS_FRAME[] = {  //length - 18
                                                    0x00, 0x00, 0x12,
                                                    //http 2 settings - identifier
                                                    0x04,
                                                    //flags - 0
                                                    0x00,
                                                    //stream id - 0
                                                    0x00, 0x00, 0x00, 0x00,
                                                    //max concurrent streams - 1000
                                                    0x00, 0x03, 0x00, 0x00, 0x03, 0xe8,
                                                    //Initial window size - 6291456
                                                    0x00, 0x04, 0x00, 0x60, 0x00, 0x00,
                                                    //Header table size - 65536
                                                    0x00, 0x01, 0x00, 0x01, 0x00, 0x00 };

    const unsigned char HTTP2_WINDOW_UPDATE_FRAME[] = { //length 4
                                                        0x00, 0x00, 0x04,
                                                        //http 2 window update
                                                        0x08,
                                                        //flags - 0
                                                        0x00,
                                                        //stream id - 0
                                                        0x00, 0x00, 0x00, 0x00,
                                                        //window size increment - 15663105
                                                        0x00, 0xef, 0x00, 0x01};

    endpoint_parsed = http_parse_endpoint(endpoint);

    socket = env_connect_to_url(endpoint_parsed->domain, endpoint_parsed->port, 5000);

    if(socket == ENV_INVALID_SOCKET)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not connect to domain %s:%hu",
                                2, endpoint_parsed->domain, endpoint_parsed->port);

        goto out_error;
    }

    if(endpoint_parsed->proto == PROTOCOL_TYPE_HTTPS)
    {
        secure_socket = env_secure_server_connection(socket);

        if(secure_socket == ENV_INVALID_SECURE_SOCKET)
        {
            env_logWrite(LOG_TYPE_ERROR, "Can not secure connection for fcm receive", 0);

            goto out_error;
        }
    }

    if(endpoint_parsed->proto == PROTOCOL_TYPE_HTTPS)
    {
        ret = env_send_secure_data(secure_socket, HTTP2_PRISM);
    }
    else
    {
        ret = env_send_tcp_data(socket, HTTP2_PRISM);
    }

    if(ret)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not send prism for fcm receive", 0);

        goto out_error;
    }

    if(endpoint_parsed->proto == PROTOCOL_TYPE_HTTPS)
    {
        ret = env_send_n_secure_data(secure_socket, (const char *)HTTP2_SETTINGS_FRAME,
                                                                sizeof(HTTP2_SETTINGS_FRAME));
    }
    else
    {
        ret = env_send_n_tcp_data(socket, (const char *)HTTP2_SETTINGS_FRAME,
                                                                sizeof(HTTP2_SETTINGS_FRAME));
    }

    if(ret)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not send settings for fcm receive", 0);

        goto out_error;
    }

    if(endpoint_parsed->proto == PROTOCOL_TYPE_HTTPS)
    {
        ret = env_send_n_secure_data(secure_socket, (const char *)HTTP2_WINDOW_UPDATE_FRAME,
                                                            sizeof(HTTP2_WINDOW_UPDATE_FRAME));
    }
    else
    {
        ret = env_send_n_tcp_data(socket, (const char *)HTTP2_WINDOW_UPDATE_FRAME,
                                                            sizeof(HTTP2_WINDOW_UPDATE_FRAME));
    }

    if(ret)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not send window update for fcm receive", 0);

        goto out_error;
    }

    sprintf(body, "app=%s&X-subtype=wp:%s/%%%s&device=%llu&"
                  "scope=GCM&X-scope=GCM&gmsv=55&appid=%s&sender=%llu", app, wp, key,
                                                                        (unsigned long long)android_id,
                                                                        app_id,
                                                                        (unsigned long long)sender_id);

    sprintf(authorization, "AidLogin %llu:%llu", (unsigned long long)android_id, (unsigned long long)secret);

    ret = http2_do_post(endpoint_parsed, secure_socket, socket,
                        1, 1, 0, 1, 146, 1, (unsigned char *)body, strlen(body), 6,
                        create_header_struct(INDEXED_HEADER_FIELD, &scheme_header, 1, NULL, 0),
                        create_header_struct(LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_INDEXED_NAME,
                            &authorization_header, 1, (unsigned char *)authorization, strlen(authorization)),
                        create_header_struct(LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_INDEXED_NAME,
                            &content_type_header, 1, (unsigned char *)content_type, strlen(content_type)),
                        create_header_struct(LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_INDEXED_NAME,
                                    &user_agent_header, 1, (unsigned char *)user_agent, strlen(user_agent)),
                        create_header_struct(LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_INDEXED_NAME,
                    &accept_encoding_header, 1, (unsigned char *)accept_encoding, strlen(accept_encoding)),
                        create_header_struct(LITERAL_HEADER_FIELD_WITH_INCREMENTAL_INDEXING_INDEXED_NAME,
                    &accept_language_header, 1, (unsigned char *)accept_language, strlen(accept_language)));
    if(ret)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not send registration post for fcm receive", 0);

        goto out_error;
    }

    recv_prms = env_malloc(sizeof(struct fcm_recv_params));

    recv_prms->secure_socket = secure_socket;
    recv_prms->socket = socket;

    recv_prms->fcm_recv_cb = fcm_recv_cb;

    recv_prms->cli_settings.HEADER_TABLE_SIZE = 65536;
    recv_prms->cli_settings.ENABLE_PUSH = 1;
    recv_prms->cli_settings.MAX_CONCURRENT_STREAMS = 1000;
    recv_prms->cli_settings.INITIAL_WINDOW_SIZE = 6291456;
    recv_prms->cli_settings.MAX_FRAME_SIZE = 8192;
    recv_prms->cli_settings.MAX_HEADER_LIST_SIZE = 8192;

    recv_prms->srv_settings.HEADER_TABLE_SIZE = 65536;
    recv_prms->srv_settings.ENABLE_PUSH = 1;
    recv_prms->srv_settings.MAX_CONCURRENT_STREAMS = 1000;
    recv_prms->srv_settings.INITIAL_WINDOW_SIZE = 6291456;
    recv_prms->srv_settings.MAX_FRAME_SIZE = 8192;
    recv_prms->srv_settings.MAX_HEADER_LIST_SIZE = 8192;

    ret = pthread_create(&worker, NULL, fcm_recv_worker, recv_prms);

    if(ret)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not create thread for fcm receive", 0);

        goto out_error;
    }

    pthread_detach(worker);

    free_http_endpoint(endpoint_parsed);

    return 0;

out_error:
    if(endpoint_parsed)
    {
        free_http_endpoint(endpoint_parsed);
    }

    if(recv_prms)
    {
        free(recv_prms);
    }

    if(secure_socket != ENV_INVALID_SECURE_SOCKET)
    {
        env_close_secure_connection(secure_socket);
    }
    else if(socket != ENV_INVALID_SOCKET)
    {
        env_close_socket(socket);
    }

    return -1;
}