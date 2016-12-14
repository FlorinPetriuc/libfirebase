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

static int env_create_tcp_socket(const int timeout_ms)
{
    int ret;
    int T = 1;

    struct timeval timeout;

    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    timeout.tv_sec = timeout_ms / 1000;

    ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ret < 0)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not create socket", 0);

        return -1;
    }

    if(setsockopt(ret, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval)) < 0)
    {
        close(ret);

        env_logWrite(LOG_TYPE_ERROR, "Can not configure recv timeout socket", 0);

        return -1;
    }

    if(setsockopt(ret, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(struct timeval)) < 0)
    {
        close(ret);

        env_logWrite(LOG_TYPE_ERROR, "Can not configure send timeout socket", 0);

        return -1;
    }

    if(setsockopt(ret, SOL_SOCKET, SO_REUSEADDR, &T, sizeof(int)) < 0)
    {
        close(ret);

        env_logWrite(LOG_TYPE_ERROR, "Can not enable reuse socket", 0);

        return -1;
    }

    if(setsockopt(ret, SOL_SOCKET, SO_KEEPALIVE, &T, sizeof(int)) < 0)
    {
        close(ret);

        env_logWrite(LOG_TYPE_ERROR, "Can not enable keep alive socket", 0);

        return -1;
    }

    return ret;
}

static int env_connect_to_address(const int sockfd, struct sockaddr *addr)
{
    if(connect(sockfd, addr, sizeof(*addr)) < 0)
    {
        env_logWrite(LOG_TYPE_ERROR, "Can not connect with %d", 1, sockfd);

        close(sockfd);

        return -1;
    }

    return sockfd;
}

int env_connect_to_url(const char *url, const uint16_t port, const unsigned int timeout_ms)
{
    int ret;

    unsigned char i;
    unsigned char j;

    struct addrinfo *addrs;
    struct addrinfo *it;

    i = 0;

    while(i < 2)
    {
        addrs = env_resolve_address(url, port);

        if(addrs == NULL)
        {
            env_logWrite(LOG_TYPE_ERROR, "Addrs is null", 0);

            ++i;

            env_sleep(0, 500);

            continue;
        }

        for(it = addrs; it != NULL; it = it->ai_next)
        {
            j = 0;

            while(j < 2)
            {
                ret = env_create_tcp_socket(timeout_ms);

                if(ret < 0)
                {
                    env_logWrite(LOG_TYPE_ERROR, "Socket is < 0", 0);

                    env_sleep(0, 500);

                    ++j;

                    continue;
                }

                ret = env_connect_to_address(ret, it->ai_addr);

                if(ret < 0)
                {
                    env_logWrite(LOG_TYPE_ERROR, "Connect returns < 0", 0);

                    env_sleep(0, 500);

                    ++j;

                    continue;
                }

                freeaddrinfo(addrs);

                return ret;
            }
        }

        ++i;

        freeaddrinfo(addrs);
    }

    return -1;
}

void env_close_socket(int sockFD)
{
    close(sockFD);
}

int env_send_n_tcp_data(int sockFD, const char *data, const unsigned int data_len)
{
    unsigned int offset = 0;

    unsigned char retry;

    int sent;

    retry = 0;

    while(offset < data_len)
    {
        if(offset + 60000 <= data_len)
        {
            sent = send(sockFD, data + offset, 60000, 0);
        }
        else
        {
            sent = send(sockFD, data + offset, data_len - offset, 0);
        }

        if(sent > 0)
        {
            retry = 0;
            offset += sent;
        }
        else
        {
            switch(errno)
            {
                case EINTR:
                case EAGAIN:
                {
                    ++retry;

                    if(retry > 3)
                    {
                        env_logWrite(LOG_TYPE_ERROR, "Unable to send to %d: timeout", 1, sockFD);

                        return -1;
                    }
                }
                break;

                default:
                {
                    env_logWrite(LOG_TYPE_ERROR, "Unable to send to %d: %s", 2, sockFD, strerror(errno));
                }
                return -1;
            }
        }
    }

    return 0;
}

int env_send_tcp_data(int sockFD, const char *data)
{
	return env_send_n_tcp_data(sockFD, data, strlen(data));
}

int env_read_tcp_data(int sockFD, char *data, const int max_len)
{
	int read_ret;

	unsigned char read_cnt = 0;

	if(max_len <= 0)
	{
		return 0;
	}

RETRY_READ:

	++read_cnt;

	if(read_cnt > 3)
	{
		env_logWrite(LOG_TYPE_ERROR, "Unable to read from %d: timeout", 1, sockFD);

		return 0;
	}

	read_ret = recv(sockFD, data, max_len, 0);

	if(read_ret > 0)
	{
		return read_ret;
	}

	if(read_ret == 0)
	{
		goto RETRY_READ;
	}

	if(errno == EAGAIN || errno == EINTR)
	{
		goto RETRY_READ;
	}

	env_logWrite(LOG_TYPE_ERROR, "Unable to read from %d: %s", 2, sockFD, strerror(errno));

	return -1;
}