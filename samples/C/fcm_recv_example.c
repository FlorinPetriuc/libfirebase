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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <firebase.h>

void fcm_recv(const unsigned char *data, const uint32_t len, const unsigned char error)
{
    uint32_t i;

    if(error)
    {
        printf("Got firebase error %hhu\n", error);

        fflush(stdout);

        return;
    }

    printf("Got firebase response\n");

    for(i = 0; i < len; ++i)
    {
        printf("0x%02hhx ", data[i]);

        if(i && (i % 20 == 0))
        {
            printf("\n");
        }
    }

    fflush(stdout);
}

int main(void)
{
    uint64_t android_id = 5582742263459189702;
    uint64_t secret = 7158090763596230867;
    uint64_t sender_id = 728237201201;

    const char *registration_endpoint = "https://android.clients.google.com/c2dm/register3";
    const char *app = "com.chrome.linux";
    const char *app_id = "csaqgGJFQ0U";
    const char *wp = "https://fir-test-c4d93.firebaseapp.com";
    const char *key = "2331C75DCD-EA0B-4675-B749-577B05E50-V2";

    srand(time(NULL));

    libfirebase_init(1, NULL);

    libfirebase_fcm_register_recv(registration_endpoint, android_id, secret, sender_id,
                                  app, app_id, wp, key, fcm_recv);

    while(1)
    {
        sleep(5);
    }

    return 0;
}