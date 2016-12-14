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

#ifndef _ENV_LINUX_H_
#define _ENV_LINUX_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <pthread.h>
#include <signal.h>
#include <pthread.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/md5.h"
#include "openssl/aes.h"

#define ENV_INVALID_SECURE_SOCKET   NULL
#define ENV_INVALID_SOCKET          -1

struct addrinfo *env_resolve_address(const char *url, const unsigned short int port);

typedef SSL*    env_secure_socket_t;
typedef int     env_socket_t;
typedef time_t  env_time_t;

#endif