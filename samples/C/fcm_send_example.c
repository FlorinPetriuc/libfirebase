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

#include <firebase.h>

int main(void)
{
    libfirebase_init(1, NULL);

    struct libfirebase_response_t *resp;

    resp = libfirebase_fcm_send(   "https://android.googleapis.com/gcm/send/"
                            "czoeXf41wbw:APA91bGlXOmAYo5mrIScW_cpMA5t"
                            "yw5Vt-Ab0lKf3_-1zLW1lukUBTDqUX8sfds-lGka"
                            "2hXFgDJWj_3I45gEyAmWFP5EAm-b8_OGjLXSLWBy"
                            "xs9S2XAYvJvARW7vFpbgNUZ-8ilKtAvh",
                            "Hello C people",
                            "BP_1fWolgrIAjT7OXdpNPwELcW2pnB3Ot0sI8kt4SmuBYndgWxqh9zKYVpw9hM4Y0w8orwWr_zAvfyFIIoHUnHY=",
                            "MsoCAf0vLbZRvUYeOIryfA==",
                            "AIzaSyDbfSSZMQNydOeARELbqlRPZ9kja86h5Cc");

    if(resp == NULL)
    {
        printf("Firebase response is NULL\n");

        return 1;
    }

    printf("Firebase response code: %hu\n", resp->code);
    printf("Firebase response header: %s\n", resp->header);
    printf("Firebase response body: %s\n", resp->body);

    libfirebase_free_response(resp);

    return 0;
}