// mainprocess.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "../snifferlib/sniffer_lib.h"
#pragma comment(lib,"../debug/snifferlib.lib")
#pragma comment(lib,"../wpcap.lib")
int display(sniffer_notify_message_t * pmsg, const u_char * usr_param)
{
	if(NULL == pmsg)
		return 0;
	if( (SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST <= pmsg->m_type)  && (pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONSE))
	{
		printf("--------------------%s--------------------\n", 
			pmsg->m_type == SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST ? "RTSP REQUEST" : "RTSP RESPONSE");
		printf("%d.%d.%d.%d:%d", 
			pmsg->m_ip_saddr.byte1, 
			pmsg->m_ip_saddr.byte2, 
			pmsg->m_ip_saddr.byte3, 
			pmsg->m_ip_saddr.byte4,
			u_short(pmsg->m_ip_sport.byte1 << 8 | pmsg->m_ip_sport.byte2));
		printf("==>");
		printf("%d.%d.%d.%d:%d \n",
			pmsg->m_ip_daddr.byte1, 
			pmsg->m_ip_daddr.byte2, 
			pmsg->m_ip_daddr.byte3,  
			pmsg->m_ip_daddr.byte4, 
			u_short(pmsg->m_ip_dport.byte1 << 8 | pmsg->m_ip_dport.byte2));
		if(pmsg->m_type == SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST)
		{
			printf("%s %s\n", pmsg->body.m_rtsp_msg.m_command, pmsg->body.m_rtsp_msg.m_target);
		}
		else
		{
			printf("%d %s\n", pmsg->body.m_rtsp_msg.m_status, pmsg->body.m_rtsp_msg.m_status_string);
		}
		sniffer_options_t * option = pmsg->body.m_rtsp_msg.m_options;
		while(NULL != option)
		{
			printf("%s: %s\n", option->option, option->content);
			option = option->next;
		}
		std::cout<<std::endl;

		snifferFreeNotifyMessage(pmsg);
	}
	
	return 0;
}

static notify_call_back_t sniffer_call = 
{
	NULL,
	display
};

int _tmain(int argc, _TCHAR* argv[])
{
	int a=10;
	int b=5;
	//std::cout<<Add(a, b);
	devHandle_t devices[10];
	int size = snifferGetDevHandles(devices, 10);
	//std::cout<<std::endl;
	for(int i = 0; i< size; i++)
	{
		std::cout<<"handle:"<<devices[i].m_handle<<std::endl;
		std::cout<<"handle name:"<<devices[i].m_name<<std::endl;
		std::cout<<"this is a test"<<std::endl;
		
	}
	snifferPacketsProcessLoop(&devices[0], &sniffer_call);

	return 0;
}

