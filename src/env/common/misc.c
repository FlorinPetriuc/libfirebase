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

int env_starts_with(const char *haystack, const char *needle)
{
    if(memcmp(haystack, needle, strlen(needle)))
    {
        return 1;
    }

    return 0;
}

void *env_malloc(const unsigned long int size)
{
	void *ret;

	ret = malloc(size);

	if(ret == NULL)
	{
		env_logWrite(LOG_TYPE_FATAL, "Out of memory", 0);

		exit(EXIT_FAILURE);
	}

	return ret;
}