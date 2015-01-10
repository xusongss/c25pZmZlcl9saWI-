%%{
	machine key_value_parser_common;
	
	#Value = (alnum | "-" | "/")* >ValueStart %ValueEnd; 
	
	Key = (alpha ("/"|alnum)*) >KeyStart %KeyEnd ;
	
	Value = (alnum | "-" | "/")*;
	
	#sp=(" ");
	
	#KeyValue1 = ("&" Key "=" (Value)? );
	
	#KeyValue2 = (";" Key ("=" Value)?);
	
	KeyValue1 = (
								("&" "startTime" "=" (Value >url_startTime_begin %url_startTime_end))
								("&" "endTime" "=" (Value >url_endTime_begin %url_endTime_end))
								("&" "areaCode" "=" (Value >url_areaCode_begin %url_areaCode_end))
								("&" "resgroupId" "=" (Value >url_resgroupId_begin %url_resgroupId_end))
								("&" "userId" "=" (Value >url_userId_begin %url_userId_end))
								("&" "sessionId" "=" (Value >url_sessionId_begin %url_sessionId_end))
								("&" "sessionType" "=" (Value >url_sessionType_begin %url_sessionType_end))
								("&" "payType" "=" (Value >url_payType_begin %url_payType_end))
								("&" "productId" "=" (Value >url_productId_begin %url_productId_end))
								("&" "displayName" "=" (Value >url_displayName_begin %url_displayName_end))
								("&" "provider" "=" (Value >url_provider_begin %url_provider_end))
								("&" "ssId" "=" (Value >url_ssId_begin %url_ssId_end))
								("&" "vcrInfo" "=" (Value >url_vcrInfo_begin %url_vcrInfo_end))
								("&" "token" "=" (Value >url_token_begin %url_token_end))
							);
	KeyValue2 = (
								("MP2T/DVBC/QAM" > lower_transport_begin %lower_transport_end) 
							|(";" (("unicast"| "multicast") > lower_transport_type_begin %lower_transport_type_end))
							| (";" "Frequency" "=" ( Value > frequency_info_begin %frequency_info_end))
							| (";" "Annex"  "=" ( Value > annex_info_begin %annex_info_end))
							| (";" "Modulation" "=" ( Value > modulation_info_begin %modulation_info_end))
							| (";" "SymbolRate" "=" ( Value > symbolRate_info_begin %symbolRate_info_end))
							| (";" "ChannelSpace" "=" ( Value > channelSpace_info_begin %channelSpace_info_end))
							| (";" "areaCode" "=" ( Value > areacode_info_begin %areacode_info_end))
							| (";" "ProgramNo" "=" ( Value > programnum_info_begin %programnum_info_end))
							);
	
	KeyValue = (KeyValue1* | KeyValue2*) "\r\n"@done;
}%%