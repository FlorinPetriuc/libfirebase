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

#include "../libfirebase.h"

static int full_fcm_send_chrome_test()
{
    struct libfirebase_response_t *resp;

    resp = libfirebase_fcm_send(   "https://android.googleapis.com/gcm/send/eV_vo_WxgA0:APA91bFI3JoBi8EiNCR1DWpZJ1VvC_WEyhg8yzJKatyzY6RCmqSnYBqFV6CCmgLhu7o9oGFz0hLwPMGgUnlRPInMgnV1h8Jr88rMZhjpNOXjIEgjGIvZWZUE0foqFSH-CWgCJ-30KA2v",
                            "Hello unit test chrome people",
                            "BCqEdOFfmYCt-foAePMFIgRACxnfKW1t1XK60NTNVY_P6LziZII6GyqUZXuSH5Nx7UUipZaF3Dx0bu1vKgbqrec=",
                            "EZ7qZ226-eWu47jppN0wtw==",
                            "AAAAqY5R1zE:APA91bEPvdhSxZBUcFyFDhjD0_mu9Tn-T17-0AsDTby2-f-ZUbstsRHWfauFb-y2od3wD936MF1pxkncaJFEQU64aqN3K_TGquLXtLJOJ-plk6q2WMLZvhnBLjZqlAbFnnFQ4Pnn0YaGZLiT0a2-XuFOuT-pTFOSnw");

    if(resp == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "full fcm send chrome test failed: resp == NULL", 0);

        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "full fcm send test response:", 0);
    env_logWrite(LOG_TYPE_INFO, "Code: %hu", 1, resp->code);
    env_logWrite(LOG_TYPE_INFO, "Header: %s", 1, resp->header);
    env_logWrite(LOG_TYPE_INFO, "Body: %s", 1, resp->body);

    libfirebase_free_response(resp);

    return 0;
}

static int full_fcm_send_firefox_test()
{
    struct libfirebase_response_t *resp;

    resp = libfirebase_fcm_send(   "https://updates.push.services.mozilla.com/wpush/v1/gAAAAABYLFvh_6_3RUqaWO2t1BGBwYDjax4iH7aVtrGCUrzJabOTuFcDahBrsKXdFO7fICYLr_Z0Kev72pXaqiXExzDdVRm4_K2vEAAGnWg3FmG3OWQI4cyCZObm87HcnfkcyZl4UrJ3",
                            "Hello unit test firefox people",
                            "BCOvl49UI4cQqvAzefrac3kKuC3SktN2hucF0quGihE9QIv4Q-KfmmPUs9ieZt0BeZiAd_Hayv7MB5dcvay8-PU",
                            "1HmDP4JdWA5rOmdMHZgHOw",
                            NULL);

    if(resp == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "full fcm send firefox test failed: resp == NULL", 0);

        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "full fcm send test response:", 0);
    env_logWrite(LOG_TYPE_INFO, "Code: %hu", 1, resp->code);
    env_logWrite(LOG_TYPE_INFO, "Header: %s", 1, resp->header);
    env_logWrite(LOG_TYPE_INFO, "Body: %s", 1, resp->body);

    libfirebase_free_response(resp);

    return 0;
}

int full_fcm_test()
{
    if(full_fcm_send_chrome_test())
    {
        return 1;
    }

    if(full_fcm_send_firefox_test())
    {
        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "full fcm tests passed successfuly", 0);

    return 0;
}