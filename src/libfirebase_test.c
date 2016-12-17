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

#include "libfirebase.h"
#include "./tests/base64_tests.h"
#include "./tests/http_tests.h"
#include "./tests/hpack_tests.h"
#include "./tests/encryption_tests.h"
#include "./tests/full_fcm_tests.h"

int main(void)
{
    libfirebase_init(1, NULL);

    if(base64_test())
    {
        return 1;
    }

    if(http_test())
    {
        return 1;
    }

    if(encryption_test())
    {
        return 1;
    }

    if(hpack_test())
    {
        return 1;
    }

    if(full_fcm_test())
    {
        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "tests passed", 0);

    return 0;
}