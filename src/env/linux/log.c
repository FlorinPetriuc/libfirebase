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

static int log_fd = STDOUT_FILENO;
static uint8_t log_enabled = 1;

int env_logInit(const uint8_t enabled, const char *file)
{
    log_enabled = enabled;

    if(file == NULL)
    {
        log_fd = STDOUT_FILENO;

        return 0;
    }

    log_fd = open(file, O_WRONLY | O_APPEND | O_CREAT);

    if(log_fd < 0)
    {
        perror("can not initialize log file");

        log_fd = STDOUT_FILENO;

        return -1;
    }

    return 0;
}

void env_logWrite(enum log_type_t type, const char *template, const unsigned int n, ...)
{
    va_list args;

    int i;

    char buffer[1024];

    time_t now;

    struct tm *now_tm;

    if(log_enabled == 0)
    {
        return;
    }

    time(&now);
    now_tm = gmtime(&now);

	i = strftime(buffer, 100,"[%d/%b/%Y:%H:%M:%S]", now_tm);
    
    switch(type)
    {
        case LOG_TYPE_INFO:
        {
            i += sprintf(buffer + i, "[INFO]");
        }
        break;

        case LOG_TYPE_ERROR:
        {
            i += sprintf(buffer + i, "[ERROR]");
        }
        break;

        case LOG_TYPE_FATAL:
        {
            i += sprintf(buffer + i, "[FATAL]");
        }
        break;
    }

    va_start(args, n);
    i += vsnprintf(buffer + i, 900, template, args);
    va_end(args);

    buffer[i] = '\n';

    write(log_fd, buffer, i + 1);
}