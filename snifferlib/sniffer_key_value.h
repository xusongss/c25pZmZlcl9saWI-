#pragma once
#include <map>
#include <string>
typedef struct
{
	int cs;
	int key_start;
	int key_end;
	int value_start;
	int value_end;

	int mark_start;
	int mark_end;
	std::map<std::string, std::string>m_key;

	std::string lower_transport;
	std::string lower_transport_type;
	std::string frequency_info;
	std::string annex_info;
	std::string modulation_info;
	std::string symbolRate_info;
	std::string channelSpace_info;
	std::string areacode_info;
	std::string programnum_info;


	std::string url_startTime     ;
	std::string url_endTime       ;
	std::string url_areaCode      ;
	std::string url_resgroupId    ;
	std::string url_userId        ;
	std::string url_sessionId     ;
	std::string url_sessionType   ;
	std::string url_payType       ;
	std::string url_productId     ;
	std::string url_displayName   ;
	std::string url_provider      ;
	std::string url_ssId          ;
	std::string url_vcrInfo       ;
	std::string url_token		  ;


}KeyValueParser_t;

int key_value_parser_init(KeyValueParser_t *parser) ;
int  key_value_parser_execute(KeyValueParser_t *parser, const char *buffer, size_t len, size_t off);