/**
 * Copyright (c) 2005 Zed A. Shaw
 * You can redistribute it and/or modify it under the same terms as Ruby.
 */

#ifndef http11_parser_h
#define http11_parser_h

#include <sys/types.h>

#if defined(_WIN32)
#include <stddef.h>
#endif

typedef enum
{
	http_method_type_rtsp = 0x01,
	http_method_type_rtsp_DESCRIBE = http_method_type_rtsp,
	http_method_type_rtsp_ANNOUNCE,
	http_method_type_rtsp_GET_PARAMETER,
	http_method_type_rtsp_OPTIONS,
	http_method_type_rtsp_PAUSE,
	http_method_type_rtsp_PLAY,
	http_method_type_rtsp_RECORD,
	http_method_type_rtsp_REDIRECT,
	http_method_type_rtsp_SETUP,
	http_method_type_rtsp_SET_PARAMETER,
	http_method_type_rtsp_TEARDOWN,
	http_method_type_rtsp_RESPONE,
	http_method_type_MAX
}http_method_type_e;

struct http_parser;
typedef void (*element_cb)(struct http_parser * parser, void *data, const char *at, size_t length);
typedef void (*field_cb)(struct http_parser * parser, void *data, const char *field, size_t flen, const char *value, size_t vlen);
typedef void (*method_cb)(struct http_parser * parser, http_method_type_e type);

typedef struct http_parser {
    int cs;
    size_t body_start;
    int content_len;
    size_t nread;
    size_t mark;
    size_t field_start;
    size_t field_len;
    size_t query_start;

	size_t status_start;
	size_t status_len;

    void *data;

	/*for user*/
	void * userdata;

    field_cb http_field;
    element_cb request_method;
    element_cb request_uri;
    element_cb fragment;
    element_cb request_path;
    element_cb query_string;
    element_cb http_version;
    element_cb header_done;
	element_cb protocol_type;
	method_cb  request_method_type;
	element_cb	response_status_cb;

} http_parser;

int http_parser_init(http_parser *parser);
int http_parser_finish(http_parser *parser);
size_t http_parser_execute(http_parser *parser, const char *data, size_t len, size_t off);
int http_parser_has_error(http_parser *parser);
int http_parser_is_finished(http_parser *parser);

#define http_parser_nread(parser) (parser)->nread

#endif
