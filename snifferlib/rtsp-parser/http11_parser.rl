/**
 * Copyright (c) 2005 Zed A. Shaw
 * You can redistribute it and/or modify it under the same terms as Ruby.
 */
#include "http11_parser.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
 * capitalizes all lower-case ASCII characters,
 * converts dashes to underscores.
 */
static void snake_upcase_char(char *c)
{
    if (*c >= 'a' && *c <= 'z') {
        *c &= ~0x20;
    } else if (*c == '-') {
        *c = '_';
    }
}

#define LEN(AT, FPC) (FPC - buffer - parser->AT)
#define MARK(M,FPC) (parser->M = (FPC) - buffer)
#define PTR_TO(F) (buffer + parser->F)


/** Machine **/

%%{

    machine http_parser;

    action mark {MARK(mark, fpc); }


    action start_field 
    {
		 MARK(field_start, fpc); 
		}
    action snake_upcase_field
    {
			snake_upcase_char((char *)fpc); 
		}
    action write_field 
    {
        parser->field_len = LEN(field_start, fpc);
    }

    action start_value
    {
			MARK(mark, fpc);
		}
    action write_value 
    {
        if(parser->http_field != NULL) 
        {
            parser->http_field(parser,parser->data, PTR_TO(field_start), parser->field_len, PTR_TO(mark), LEN(mark, fpc));
        }
    }
    action request_method 
    {
        if(parser->request_method != NULL)
        {
            parser->request_method(parser, parser->data, PTR_TO(mark), LEN(mark, fpc));
         }
    }
    action request_uri 
    {
        if(parser->request_uri != NULL)
        {
            parser->request_uri(parser, parser->data, PTR_TO(mark), LEN(mark, fpc));
		}            
    }
    action fragment 
    {
        if(parser->fragment != NULL)
            parser->fragment(parser, parser->data, PTR_TO(mark), LEN(mark, fpc));
    }

    action start_query
     {
		MARK(query_start, fpc);
	 }
	 
    action query_string 
    {
        if(parser->query_string != NULL)
            parser->query_string(parser, parser->data, PTR_TO(query_start), LEN(query_start, fpc));
    }

    action http_version 
    {
        if(parser->http_version != NULL)
            parser->http_version(parser, parser->data, PTR_TO(mark), LEN(mark, fpc));
    }

    action request_path 
    {
        if(parser->request_path != NULL)
            parser->request_path(parser, parser->data, PTR_TO(mark), LEN(mark,fpc));
    }

    action done 
    {
        parser->body_start = fpc - buffer + 1;
        if(parser->header_done != NULL)
            parser->header_done(parser,parser->data, fpc + 1, pe - fpc - 1);
        fbreak;
    }
    #########response_state
     action start_response_state 
    {
		 	MARK(status_start, fpc); 
		}
		action end_response_state 
    {
        parser->status_len = LEN(status_start, fpc);
    }
    action write_response_state
    {
    	if(parser->response_status_cb != NULL)
            parser->response_status_cb(parser, parser->data, PTR_TO(status_start), parser->status_len);
    }
    ######### method
		action ragle_protocal_rtsp_Method_DESCRIBE
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_DESCRIBE);
			}
		}
		action ragle_protocal_rtsp_Method_ANNOUNCE
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_ANNOUNCE);
			}
		}
		action ragle_protocal_rtsp_Method_GET_PARAMETER
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_GET_PARAMETER);
			}
		}
		action ragle_protocal_rtsp_Method_OPTIONS
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_OPTIONS);
			}
		}
		action ragle_protocal_rtsp_Method_PAUSE
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_PAUSE);
			}
		}
		action ragle_protocal_rtsp_Method_PLAY
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_PLAY);
			}
		}
		action ragle_protocal_rtsp_Method_RECORD
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_RECORD);
			}
		}
		action ragle_protocal_rtsp_Method_REDIRECT	
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_REDIRECT);
			}
		}
		action ragle_protocal_rtsp_Method_SETUP	
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_SETUP);
			}
		}
		
		action ragle_protocal_rtsp_Method_SET_PARAMETER	
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_SET_PARAMETER);
			}
		}
		action ragle_protocal_rtsp_Method_TEARDOWN	
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_TEARDOWN);
			}
		}
		action ragle_protocal_rtsp_Method_RESPONE	
		{
			if(parser->request_method_type)
			{
				parser->request_method_type(parser, http_method_type_rtsp_RESPONE);
			}
		}
    

    include http_parser_common "http11_parser_common.rl";

}%%

/** Data **/
%% write data;

int http_parser_init(http_parser *parser)    {
    int cs = 0;
    %% write init;
    parser->cs = cs;
    parser->body_start = 0;
    parser->content_len = 0;
    parser->mark = 0;
    parser->nread = 0;
    parser->field_len = 0;
    parser->field_start = 0;

    return(1);
}


/** exec **/
size_t http_parser_execute(http_parser *parser, const char *buffer, size_t len, size_t off)    {
    const char *p, *pe;
    int cs = parser->cs;

    assert(off <= len && "offset past end of buffer");

    p = buffer+off;
    pe = buffer+len;

    assert(pe - p == len - off && "pointers aren't same distance");


    %% write exec;

    parser->cs = cs;
    parser->nread += p - (buffer + off);

    assert(p <= pe && "buffer overflow after parsing execute");
    assert(parser->nread <= len && "nread longer than length");
    assert(parser->body_start <= len && "body starts after buffer end");
    assert(parser->mark < len && "mark is after buffer end");
    assert(parser->field_len <= len && "field has length longer than whole buffer");
    assert(parser->field_start < len && "field starts after buffer end");

    if(parser->body_start) {
        /* final \r\n combo encountered so stop right here */
        parser->nread++;
    }

    return(parser->nread);
}

int http_parser_finish(http_parser *parser)
{
    int cs = parser->cs;

    parser->cs = cs;

    if (http_parser_has_error(parser) ) {
        return -1;
    } else if (http_parser_is_finished(parser) ) {
        return 1;
    } else {
        return 0;
    }
}

int http_parser_has_error(http_parser *parser) {
    return parser->cs == http_parser_error;
}

int http_parser_is_finished(http_parser *parser) {
    return parser->cs == http_parser_first_final;
}
