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

char *http_build_header(enum http_request_type_t req_type, const char *domain,
                        const char *url_path, const unsigned int headers_no, va_list args);

int http_header_test(const char *result, enum http_request_type_t req_type, const char *domain,
                        const char *url_path, const unsigned int headers_no, ...)
{
    char *header;

    va_list args;

	va_start(args, headers_no);

    header = http_build_header(req_type, domain, url_path, headers_no, args);

    va_end(args);

    if(strcmp(header, result))
    {
        env_logWrite(LOG_TYPE_ERROR, "Header generation mismatch", 0);
        env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, header, result);

        return 1;
    }

    free(header);

    return 0;
}

int http_endpoint_test(const char *endpoint, const enum http_protocol_type_t proto,
                        const char *domain, const char *url_path, const uint16_t port)
{
    struct http_endpoint_t *_endpoint;

    _endpoint = http_parse_endpoint(endpoint);

    if(_endpoint == NULL)
    {
        env_logWrite(LOG_TYPE_ERROR, "Unable to parse endpoint %s", 1, endpoint);

        return 1;
    }

    if(_endpoint->proto != proto)
    {
        env_logWrite(LOG_TYPE_ERROR, "Proto mismatch on endpoint %s", 1, endpoint);

        return 1;
    }

    if(strcmp(_endpoint->domain, domain))
    {
        env_logWrite(LOG_TYPE_ERROR, "Domain mismatch on endpoint %s", 1, endpoint);
        env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, _endpoint->domain, domain);

        return 1;
    }

    if(strcmp(_endpoint->url_path, url_path))
    {
        env_logWrite(LOG_TYPE_ERROR, "Url path mismatch on endpoint %s", 1, endpoint);
        env_logWrite(LOG_TYPE_ERROR, "%s vs %s", 2, _endpoint->url_path, url_path);

        return 1;
    }

    if(_endpoint->port != port)
    {
        env_logWrite(LOG_TYPE_ERROR, "Port mismatch on endpoint %s", 1, endpoint);
        env_logWrite(LOG_TYPE_ERROR, "%hu vs %hu", 2, _endpoint->port, port);

        return 1;
    }

    free_http_endpoint(_endpoint);

    return 0;
}

int http_test()
{
    if(http_endpoint_test("http://endpoint.com", PROTOCOL_TYPE_HTTP, "endpoint.com", "/", 80))
    {
        return 1;
    }

    if(http_endpoint_test("http://endpoint.com/", PROTOCOL_TYPE_HTTP, "endpoint.com", "/", 80))
    {
        return 1;
    }

    if(http_endpoint_test("http://endpoint.com/i/want/to/pass/this/test", PROTOCOL_TYPE_HTTP,
                                                    "endpoint.com", "/i/want/to/pass/this/test", 80))
    {
        return 1;
    }

    if(http_endpoint_test("http://maryhadalittlelamb.dk/i/want/to/pass/this/test",
                        PROTOCOL_TYPE_HTTP, "maryhadalittlelamb.dk", "/i/want/to/pass/this/test", 80))
    {
        return 1;
    }

    if(http_endpoint_test("https://endpoint.com", PROTOCOL_TYPE_HTTPS, "endpoint.com", "/", 443))
    {
        return 1;
    }

    if(http_endpoint_test("https://endpoint.com/", PROTOCOL_TYPE_HTTPS, "endpoint.com", "/", 443))
    {
        return 1;
    }

    if(http_endpoint_test("https://endpoint.com/i/want/to/pass/this/test", PROTOCOL_TYPE_HTTPS,
                                                    "endpoint.com", "/i/want/to/pass/this/test", 443))
    {
        return 1;
    }

    if(http_endpoint_test("https://maryhadalittlelamb.dk/i/want/to/pass/this/test",
                        PROTOCOL_TYPE_HTTPS, "maryhadalittlelamb.dk", "/i/want/to/pass/this/test", 443))
    {
        return 1;
    }

    if(http_endpoint_test("https://maryhadalittlelamb.dk:3434/i/want/to/pass/this/test",
                        PROTOCOL_TYPE_HTTPS, "maryhadalittlelamb.dk", "/i/want/to/pass/this/test", 3434))
    {
        return 1;
    }

    if(http_endpoint_test("http://maryhadalittlelamb.dk:3434/i/want/to/pass/this/test",
                        PROTOCOL_TYPE_HTTP, "maryhadalittlelamb.dk", "/i/want/to/pass/this/test", 3434))
    {
        return 1;
    }

    if(http_endpoint_test("http://maryhadalittlelamb.dk:8787",
                        PROTOCOL_TYPE_HTTP, "maryhadalittlelamb.dk", "/", 8787))
    {
        return 1;
    }

    if(http_endpoint_test("https://maryhadalittlelamb.dk:8787",
                        PROTOCOL_TYPE_HTTPS, "maryhadalittlelamb.dk", "/", 8787))
    {
        return 1;
    }

    if(http_endpoint_test("http://maryhadalittlelamb.dk:8787/",
                        PROTOCOL_TYPE_HTTP, "maryhadalittlelamb.dk", "/", 8787))
    {
        return 1;
    }

    if(http_endpoint_test("https://maryhadalittlelamb.dk:8787/",
                        PROTOCOL_TYPE_HTTPS, "maryhadalittlelamb.dk", "/", 8787))
    {
        return 1;
    }

    if(http_header_test("GET /location_url HTTP/1.1\r\n" \
                        "Host: test.com\r\n"
                        "\r\n", PROTOCOL_HTTP_GET, "test.com", "/location_url", 0))
    {
        return 1;
    }

    if(http_header_test("GET /location_url HTTP/1.1\r\n" \
                        "Host: test.com\r\n"
                        "My-Custom-header: yesIAmHere\r\n"
                        "\r\n", PROTOCOL_HTTP_GET, "test.com", "/location_url", 1, "My-Custom-header: yesIAmHere"))
    {
        return 1;
    }

    if(http_header_test("GET /location_url HTTP/1.1\r\n" \
                        "Host: test.com\r\n"
                        "My-Custom-header: yesIAmHere\r\n"
                        "My-Custom-header1: yesIAmHere1\r\n"
                        "\r\n", PROTOCOL_HTTP_GET, "test.com", "/location_url", 2,
                                                "My-Custom-header: yesIAmHere", "My-Custom-header1: yesIAmHere1"))
    {
        return 1;
    }

    if(http_header_test("POST /location_url HTTP/1.1\r\n" \
                        "Host: test.com\r\n"
                        "My-Custom-header: yesIAmHere\r\n"
                        "My-Custom-header1: yesIAmHere1\r\n"
                        "\r\n", PROTOCOL_HTTP_POST, "test.com", "/location_url", 2,
                                                "My-Custom-header: yesIAmHere", "My-Custom-header1: yesIAmHere1"))
    {
        return 1;
    }

    if(http_header_test("POST /location_url HTTP/1.1\r\n" \
                        "Host: test.com\r\n"
                        "\r\n", PROTOCOL_HTTP_POST, "test.com", "/location_url", 0))
    {
        return 1;
    }

    env_logWrite(LOG_TYPE_INFO, "http tests passed successfuly", 0);

    return 0;
}