
#include "http11_parser.h"

#include <stdio.h>

static char http[] =
"GET /index.html HTTP/1.1\r\n"
"Host: www.example.com\r\n"
"\r\n"
"Sdp goes here I guess";

static char rtsp_options[] =
"OPTIONS rtsp://example.com/media.mp4 RTSP/1.0\r\n"
"CSeq: 1\r\n"
"Require: implicit-play\r\n"
"Proxy-Require: gzipped-messages\r\n"
"\r\n";


void element_callback(
        void *data,
        const char *at,
        size_t length)
{
    printf("element!!! %.*s\n", (int)length, at);
}

void field_callback(
        void *data,
        const char *field,
        size_t flen,
        const char *value,
        size_t vlen)
{
    printf("field!!! %.*s %.*s\n", (int)flen, field, (int)vlen, value);
}

int main()
{
    size_t nread;
    http_parser parser;

    parser.http_field = &field_callback;
    parser.request_method = &element_callback;
    parser.request_uri = &element_callback;
    parser.fragment = &element_callback;
    parser.request_path = &element_callback;
    parser.query_string = &element_callback;
    parser.http_version = &element_callback;
    parser.header_done = &element_callback;


    http_parser_init(&parser);
    nread = http_parser_execute(
            &parser,
            rtsp_options,
            sizeof(rtsp_options)-1,
            0);
    printf("nread: %lu\n", nread);
}
