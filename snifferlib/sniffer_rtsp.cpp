#include "sniffer_rtsp.h"
#include <stdio.h>
#include <string.h>
#include "rtsp_parser.h"
/*define method*/
const char *rtsp_OPTION = "OPTION";
const char *rtsp_DESCRIBE = "DESCRIBE";
const char *rtsp_SETUP = "SETUP";
const char *rtsp_PLAY = "PLAY";
const char *rtsp_TEARDOWN = "TEARDOWN";
const char *rtsp_version_1_0 = "RTSP/1.0";
const char *rtsp_version_1_1 = "RTSP/1.1";


int sniffer_rtsp_process(tcp_udp_packet_t *packet, NotifyCallback cb)
{
	//printf("%.*s\n", packet->body_length, packet->body);
	RTSP_MESSAGE *rtsp_protocol = (RTSP_MESSAGE *)malloc(sizeof (RTSP_MESSAGE));
	memset(rtsp_protocol, 0, sizeof(RTSP_MESSAGE));
	if(parseRtspMessage(rtsp_protocol, (char*)packet->body) == RTSP_ERROR_SUCCESS)
	{
		sniffer_notify_message_t *notify_msg = (sniffer_notify_message_t *)malloc(sizeof(sniffer_notify_message_t));
		memset(notify_msg, 0, sizeof(sniffer_notify_message_t));

		memcpy(&notify_msg->m_ip_saddr, &packet->ih->saddr, sizeof(ip_address_t));
		memcpy(&notify_msg->m_ip_daddr, &packet->ih->daddr, sizeof(ip_address_t));
		memcpy(&notify_msg->m_ip_sport, &packet->th->th_sport, sizeof(ip_port));
		memcpy(&notify_msg->m_ip_dport, &packet->th->th_dport, sizeof(ip_port));

		notify_msg->body.m_rtsp_msg.m_cseq = rtsp_protocol->sequenceNumber;

		if(TYPE_REQUEST == rtsp_protocol->type)
		{
			notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST;
			strncpy(
						notify_msg->body.m_rtsp_msg.m_command,
						rtsp_protocol->message.request.command,
						sizeof(notify_msg->body.m_rtsp_msg.m_command) -1
					);
			strncpy(
						notify_msg->body.m_rtsp_msg.m_target,
						rtsp_protocol->message.request.target, 
						sizeof(notify_msg->body.m_rtsp_msg.m_target) -1
					);
		}
		else if(TYPE_RESPONSE == rtsp_protocol->type)
		{
			notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONSE;
			strncpy(
						notify_msg->body.m_rtsp_msg.m_status_string ,
						rtsp_protocol->message.response.statusString, 
						sizeof(notify_msg->body.m_rtsp_msg.m_status_string) -1
					);
			notify_msg->body.m_rtsp_msg.m_status = rtsp_protocol->message.response.statusCode;
		}
		POPTION_ITEM optionsHead = rtsp_protocol->options;
		while(optionsHead != NULL)
		{
			sniffer_options_t * option = (sniffer_options_t*)malloc(sizeof(sniffer_options_t));
			memset(option, 0, sizeof(sniffer_options_t));
			strncpy(option->option, optionsHead->option, sizeof(option->option) -1);
			strncpy(option->content, optionsHead->content, sizeof(option->content) -1);
			snifferInsertOption(&notify_msg->body.m_rtsp_msg.m_options, option);

			optionsHead = optionsHead->next;
		}

		if((NULL != cb )&& (NULL != cb->cb) )
		{
			cb->cb(notify_msg, cb->usr_param);
		}
		
	}

	freeMessage(rtsp_protocol );
	
	return -1;
}