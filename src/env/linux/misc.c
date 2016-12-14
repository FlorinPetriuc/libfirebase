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

void env_sleep(const unsigned int seconds, const unsigned int mseconds)
{
	unsigned int _seconds	= seconds;
	unsigned int _useconds	= mseconds * 1000;

sleep_seconds:

	_seconds = sleep(_seconds);

	if(_seconds > 0)
	{
		goto sleep_seconds;
	}

sleep_useconds:

	_useconds = usleep(_useconds);

	if(_useconds > 0)
	{
		goto sleep_useconds;
	}
}

env_time_t env_utcTime()
{
	time_t now;

	now = time(NULL);
	now = timegm(gmtime(&now));

	return now;
}