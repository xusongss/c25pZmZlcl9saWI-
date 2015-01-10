
#include "sniffer_key_value.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define printf
#define LEN(AT, FPC) (FPC - buffer - parser->AT)
#define MARK(M,FPC) (parser->M = (FPC) - buffer)
#define PTR_TO(F) (buffer + parser->F)
%%{
	 machine key_value_parser;
	action KeyStart
	{
		MARK(key_start, fpc);
		
	}
	action KeyEnd
	{
		MARK(key_end, fpc);

		printf("key: %.*s\n", parser->key_end - parser->key_start, PTR_TO(key_start));
	}
	action ValueStart
	{
		MARK(value_start, fpc);
	}
	action ValueEnd
	{
		MARK(value_end, fpc);

		printf("value %.*s\n", parser->value_end - parser->value_start, PTR_TO(value_start));
	}
	
	action lower_transport_begin
	{
		MARK(mark_start, fpc);
	}
	action lower_transport_end
	{
		MARK(mark_end, fpc);
		parser->lower_transport.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}

	#点播类型 如"unicast" | "multicast"
	action lower_transport_type_begin
	{
		MARK(mark_start, fpc);
	}
	action lower_transport_type_end
	{
		MARK(mark_end, fpc);
		parser->lower_transport_type.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	#Frequency 信息
	action frequency_info_begin
	{
		MARK(mark_start, fpc);
	}
	action frequency_info_end
	{
		MARK(mark_end, fpc);
		parser->frequency_info.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	action annex_info_begin
	{
		MARK(mark_start, fpc);
	}
	action annex_info_end
	{
		MARK(mark_end, fpc);
		parser->annex_info.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	action modulation_info_begin
	{
		MARK(mark_start, fpc);
	}
	action modulation_info_end
	{
		MARK(mark_end, fpc);
		parser->modulation_info.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	action symbolRate_info_begin
	{
		MARK(mark_start, fpc);
	}
	action symbolRate_info_end
	{
		MARK(mark_end, fpc);
		parser->symbolRate_info.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	action channelSpace_info_begin
	{
		MARK(mark_start, fpc);
	}
	action channelSpace_info_end
	{
		MARK(mark_end, fpc);
		parser->channelSpace_info.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	action areacode_info_begin
	{
		MARK(mark_start, fpc);
	}
	action areacode_info_end
	{
		MARK(mark_end, fpc);
		parser->areacode_info.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	action programnum_info_begin
	{
		MARK(mark_start, fpc);
	}
	action programnum_info_end
	{
		MARK(mark_end, fpc);
		parser->programnum_info.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	
	#URL==================================
	action url_startTime_begin
	{
		MARK(mark_start, fpc);
	}
	action url_startTime_end
	{
		MARK(mark_end, fpc);
		parser->url_startTime.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	
	action url_endTime_begin
	{
		MARK(mark_start, fpc);
	}
	action url_endTime_end
	{
		MARK(mark_end, fpc);
		parser->url_endTime.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_areaCode_begin
	{
		MARK(mark_start, fpc);
	}
	action url_areaCode_end
	{
		MARK(mark_end, fpc);
		parser->url_areaCode.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_resgroupId_begin
	{
		MARK(mark_start, fpc);
	}
	action url_resgroupId_end
	{
		MARK(mark_end, fpc);
		parser->url_resgroupId.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_userId_begin
	{
		MARK(mark_start, fpc);
	}
	action url_userId_end
	{
		MARK(mark_end, fpc);
		parser->url_userId.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_sessionId_begin
	{
		MARK(mark_start, fpc);
	}
	action url_sessionId_end
	{
		MARK(mark_end, fpc);
		parser->url_sessionId.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_sessionType_begin
	{
		MARK(mark_start, fpc);
	}
	action url_sessionType_end
	{
		MARK(mark_end, fpc);
		parser->url_sessionType.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_payType_begin
	{
		MARK(mark_start, fpc);
	}
	action url_payType_end
	{
		MARK(mark_end, fpc);
		parser->url_payType.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_productId_begin
	{
		MARK(mark_start, fpc);
	}
	action url_productId_end
	{
		MARK(mark_end, fpc);
		parser->url_productId.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_displayName_begin
	{
		MARK(mark_start, fpc);
	}
	action url_displayName_end
	{
		MARK(mark_end, fpc);
		parser->url_displayName.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_provider_begin
	{
		MARK(mark_start, fpc);
	}
	action url_provider_end
	{
		MARK(mark_end, fpc);
		parser->url_provider.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_ssId_begin
	{
		MARK(mark_start, fpc);
	}
	action url_ssId_end
	{
		MARK(mark_end, fpc);
		parser->url_ssId.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_vcrInfo_begin
	{
		MARK(mark_start, fpc);
	}
	action url_vcrInfo_end
	{
		MARK(mark_end, fpc);
		parser->url_vcrInfo.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
		action url_token_begin
	{
		MARK(mark_start, fpc);
	}
	action url_token_end
	{
		MARK(mark_end, fpc);
		parser->url_token.append(PTR_TO(mark_start), parser->mark_end - parser->mark_start);
		printf("value %.*s\n", parser->mark_end - parser->mark_start, PTR_TO(mark_start));
	}
	action done
	{
		printf("done");
	}
	 

	include key_value_parser_common "sniffer_key_value_common.rl";
	main := KeyValue;
}%%

/** Data **/
%% write data;


int key_value_parser_init(KeyValueParser_t *parser) 
{
	int cs = 0;
	%% write init;
	parser->cs = cs;
	parser->key_start = 0;
	parser->key_end=0;
	parser->value_start=0;
	parser->value_end=0;

    return(1);
}
/** exec **/
int  key_value_parser_execute(KeyValueParser_t *parser, const char *buffer, size_t len, size_t off)
{
	   const char *p, *pe;
	int cs = parser->cs;

    assert(off <= len && "offset past end of buffer");

    p = buffer+off;
    pe = buffer+len;

    assert(pe - p == len - off && "pointers aren't same distance");


    %% write exec;

	 parser->cs = cs;

	 return parser->cs;

}
