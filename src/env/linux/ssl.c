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

#include "../env.h"

static pthread_mutex_t *ssl_mtx = NULL;
static int mtx_nr = 0;

static unsigned long thread_id_callback()
{
	return ((unsigned long) pthread_self());
}

static void lock_callback(int mode, int n, const char *file, int line)
{
	if(n >= mtx_nr)
	{
		return;
	}

	if(mode & CRYPTO_LOCK)
	{
		pthread_mutex_lock(&ssl_mtx[n]);
	}
	else
	{
		pthread_mutex_unlock(&ssl_mtx[n]);
	}
}

int env_ssl_init()
{
    int i;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    mtx_nr = CRYPTO_num_locks();

    ssl_mtx = env_malloc(mtx_nr * sizeof(pthread_mutex_t));

    for(i = 0; i < mtx_nr; ++i)
    {
        if(pthread_mutex_init(&ssl_mtx[i], NULL))
        {
            return 1;
        }
    }

    CRYPTO_set_id_callback(thread_id_callback);
    CRYPTO_set_locking_callback(lock_callback);

    srand(time(NULL));

    env_logWrite(LOG_TYPE_INFO, "Initialized OpenSSL v%x", 1, OPENSSL_VERSION_NUMBER);

    return 0;
}

SSL *env_secure_server_connection(int server_socket)
{
    SSL_CTX *client_ctx;
    SSL *ssl;

    client_ctx = SSL_CTX_new(TLSv1_2_client_method());

    if(client_ctx == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "Unable to create new client_ctx", 0);

        return NULL;
    }

    ssl = SSL_new(client_ctx);
    SSL_set_fd(ssl, server_socket);

    if(SSL_connect(ssl) < 0)
    {
        env_logWrite(LOG_TYPE_ERROR, "Unable to SSL connect", 0);

        SSL_free(ssl);
        SSL_CTX_free(client_ctx);

        return NULL;
    }

    return ssl;
}

void env_close_secure_connection(SSL *ssl)
{
    int fd = SSL_get_fd(ssl);
    SSL_CTX *ctx = SSL_get_SSL_CTX(ssl);

    SSL_free(ssl);
    SSL_CTX_free(ctx);

    close(fd);
}

int env_read_secure_data(SSL *ssl, char *data, const int max_len)
{
    int read_ret;
    int err_ret;

    unsigned char read_cnt = 0;

    if(max_len <= 0)
    {
        return 0;
    }

RETRY_READ:

    ++read_cnt;

    if(read_cnt > 3)
    {
        env_logWrite(LOG_TYPE_ERROR, "Read failed: timeout", 0);

        return 0;
    }

    ERR_clear_error();

    read_ret = SSL_read(ssl, data, max_len);

    if(read_ret > 0)
    {
        return read_ret;
    }

    err_ret = SSL_get_error(ssl, read_ret);

    if(err_ret == SSL_ERROR_WANT_READ || err_ret == SSL_ERROR_WANT_WRITE)
    {
        goto RETRY_READ;
    }

    env_logWrite(LOG_TYPE_ERROR, "Read failed: %d", 1, err_ret);

    return -1;
}

int env_send_secure_data(SSL *ssl, const char *data)
{
    return env_send_n_secure_data(ssl, data, strlen(data));
}

int env_send_n_secure_data(SSL *ssl, const char *data, const unsigned int data_len)
{
    unsigned int offset = 0;

    unsigned char retry;

    int sent;
    int err_ret;

    retry = 0;

    while(offset < data_len)
    {
        ERR_clear_error();

        if(offset + 60000 <= data_len)
        {
            sent = SSL_write(ssl, data + offset, 60000);
        }
        else
        {
            sent = SSL_write(ssl, data + offset, data_len - offset);
        }

        if(sent > 0)
        {
            retry = 0;
            offset += sent;

            continue;
        }

        err_ret = SSL_get_error(ssl, sent);

        if(err_ret == SSL_ERROR_WANT_READ || err_ret == SSL_ERROR_WANT_WRITE)
        {
            ++retry;

            if(retry > 3)
            {
                env_logWrite(LOG_TYPE_ERROR, "Send failed: timeout", 0);

                return -1;
            }

            continue;
        }

        env_logWrite(LOG_TYPE_ERROR, "Send failed: %d", 1, err_ret);

        return -1;
    }

    return 0;
}