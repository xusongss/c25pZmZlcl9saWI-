// KeyValueTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
#include "../snifferlib/sniffer_lib.h"
#include "../snifferlib/sniffer_key_value.h"
#pragma comment(lib,"../debug/snifferlib.lib")
#pragma comment(lib,"../wpcap.lib")

//char * str = "MP2T/DVBC/QAM;Frequency=500000000-800000000;ProgramNo=13;unicast;Annex=A-B;Modulation=64-256;SymbolRate=6000-8000;ChannelSpace=6000000-8000000;areaCode=1012\r\n";
char * str="&startTime=10&endTime=2381&areaCode=1012&resgroupId=&userId=6605910&sessionId=0&sessionType=null&payType=1&productId=0&displayName=&provider=coship&ssId=371057310&vcrInfo=&token=E5F7A72E60E55298027E432648F1818\r\n";
int main(int argc, const char * argv[])
{
	KeyValueParser_t parser;
	key_value_parser_init(&parser);
	key_value_parser_execute(&parser, str, strlen(str), 0);

	std::cout<<std::endl;
	std::cout<<"lower_transport		 "<<parser.lower_transport			<<std::endl;
	std::cout<<"lower_transport_type "<<parser.lower_transport_type      <<std::endl;
	std::cout<<"frequency_info       "<<parser.frequency_info            <<std::endl;
	std::cout<<"annex_info           "<<parser.annex_info                <<std::endl;
	std::cout<<"modulation_info      "<<parser.modulation_info           <<std::endl;
	std::cout<<"symbolRate_info      "<<parser.symbolRate_info           <<std::endl;
	std::cout<<"channelSpace_info    "<<parser.channelSpace_info         <<std::endl;
	std::cout<<"areacode_info        "<<parser.areacode_info             <<std::endl;
	std::cout<<"programnum_info      "<<parser.programnum_info           <<std::endl;
	std::cout<<std::endl;
	std::cout<<"url_startTime    "<<parser.url_startTime    <<std::endl ;
	std::cout<<"url_endTime      "<<parser.url_endTime      <<std::endl ;
	std::cout<<"url_areaCode     "<<parser.url_areaCode     <<std::endl ;
	std::cout<<"url_resgroupId   "<<parser.url_resgroupId   <<std::endl ;
	std::cout<<"url_userId       "<<parser.url_userId       <<std::endl ;
	std::cout<<"url_sessionId    "<<parser.url_sessionId    <<std::endl ;
	std::cout<<"url_sessionType  "<<parser.url_sessionType  <<std::endl ;
	std::cout<<"url_payType      "<<parser.url_payType      <<std::endl ;
	std::cout<<"url_productId    "<<parser.url_productId    <<std::endl ;
	std::cout<<"url_displayName  "<<parser.url_displayName  <<std::endl ;
	std::cout<<"url_provider     "<<parser.url_provider     <<std::endl ;
	std::cout<<"url_ssId         "<<parser.url_ssId         <<std::endl ;
	std::cout<<"url_vcrInfo      "<<parser.url_vcrInfo      <<std::endl ;
	std::cout<<"url_token		 "<<parser.url_token		<<std::endl;
	return 0;
}

