#include <iostream>
#include <time.h>
#include "display_msg.h"
int display(sniffer_notify_message_t * pmsg, const u_char * usr_param)
{
	if(NULL == pmsg)
		return 0;
	if( (SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST <= pmsg->m_type)  && (pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE))
	{

		struct tm *ltime;
		char timestr[16];
		time_t local_tv_sec;
		local_tv_sec = pmsg->m_time + 8*60*60;
		ltime=localtime(&local_tv_sec);
		strftime(timestr, sizeof( timestr) -1, "%H:%M:%S", ltime);

		printf("--------------------%s %s--------------------\n", 
			timestr,
			pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_TEARDOWN ? "RTSP REQUEST" : "RTSP RESPONSE");
		printf("%02x:%02x:%02x:%02x:%02x:%02x",
			pmsg->m_eth_addr.ether_shost[0],
			pmsg->m_eth_addr.ether_shost[1],
			pmsg->m_eth_addr.ether_shost[2],
			pmsg->m_eth_addr.ether_shost[3],
			pmsg->m_eth_addr.ether_shost[4],
			pmsg->m_eth_addr.ether_shost[5]
			);
		printf("==>");
		printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
			pmsg->m_eth_addr.ether_dhost[0],
			pmsg->m_eth_addr.ether_dhost[1],
			pmsg->m_eth_addr.ether_dhost[2],
			pmsg->m_eth_addr.ether_dhost[3],
			pmsg->m_eth_addr.ether_dhost[4],
			pmsg->m_eth_addr.ether_dhost[5]
			);


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
		printf("tcp seq = %u ack_seq =%u\n", pmsg->th_seq, pmsg->th_ack);
		if(pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_TEARDOWN)
		{
			printf("method: %s \n", pmsg->body.m_rtsp_msg.m_command);
			printf("target: %s\n", pmsg->body.m_rtsp_msg.m_target);
		}
		else
		{
			printf("resualt: %d %s\n", pmsg->body.m_rtsp_msg.m_status, pmsg->body.m_rtsp_msg.m_status_string);
		}
		sniffer_options_t * option = pmsg->body.m_rtsp_msg.m_options;
		while(NULL != option)
		{
			printf("option filed: %s: %s\n", option->option, option->content);
			option = option->next;
		}
		std::cout<<std::endl;

		//snifferFreeNotifyMessage(pmsg);
	}
	
	return 0;
}