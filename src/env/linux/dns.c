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

struct addrinfo *env_resolve_address(const char *url, const unsigned short int port)
{
    struct addrinfo *servinfo;
    struct addrinfo hints;

    int ret;

    char sPort[6];

    unsigned char i;

    i = 0;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    sprintf(sPort, "%hu", port);

lookup:
    ret = getaddrinfo(url, sPort, &hints, &servinfo);

    if(ret == 0)
    {
        return servinfo;
    }

    ++i;

    if(i > 3)
    {
        env_logWrite(LOG_TYPE_ERROR, "DNS lookup failed on %s", 1, url);

        return NULL;
    }

    goto lookup;
}