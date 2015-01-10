#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sniffer_lib.h"
#include "sniffer_rtsp.h"
#include "rtsp_parser.h"
#include "pcap.h"
/*define method*/
const char *rtsp_OPTION = "OPTION";
const char *rtsp_DESCRIBE = "DESCRIBE";
const char *rtsp_SETUP = "SETUP";
const char *rtsp_PLAY = "PLAY";
const char *rtsp_TEARDOWN = "TEARDOWN";
const char *rtsp_version_1_0 = "RTSP/1.0";
const char *rtsp_version_1_1 = "RTSP/1.1";
#include <iostream>
using namespace std;
//#define SNIFFER_RTSP_DEBUG
#if 0
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
#else


static void request_method_callback(
		http_parser *parser,
		void *data,
		const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "method!!! %.*s\n", (int)length, at);
#endif
	if(length <= 0)
		return ;
	sniffer_notify_message_t *notify_msg = (sniffer_notify_message_t *)parser->userdata;
	if(SNIFFER_NOTIFY_MESSAGE_RTSP <= notify_msg->m_type  && notify_msg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE)
	{
		size_t size = sizeof(notify_msg->body.m_rtsp_msg.m_command)-1;
		size = size > length ? length : size;
		strncpy(notify_msg->body.m_rtsp_msg.m_command, at, size);
	}
}

static void request_uri_callback(
	  http_parser *parser,
        void *data,
        const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "uri!!! %.*s\n", (int)length, at);
#endif
	if(length <= 0)
		return ;
	sniffer_notify_message_t *notify_msg = (sniffer_notify_message_t *)parser->userdata;
	if(SNIFFER_NOTIFY_MESSAGE_RTSP <= notify_msg->m_type  && notify_msg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE)
	{
		size_t size = sizeof(notify_msg->body.m_rtsp_msg.m_target)-1;
		size = size > length ? length : size;
		strncpy(notify_msg->body.m_rtsp_msg.m_target, at, size);
	}
}
static void request_pathcallback(
		http_parser *parser,
        void *data,
        const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "path!!! %.*s\n", (int)length, at);
#endif
}
static void fragment_callback(
		http_parser *parser,
        void *data,
        const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "fragment!!! %.*s\n", (int)length, at);
#endif
}

static void  query_string_callback(
		http_parser *parser,
        void *data,
        const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "query!!! %.*s\n", (int)length, at);
#endif
}
static void http_version_callback(
		http_parser *parser,
        void *data,
        const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "version!!! %.*s\n", (int)length, at);
#endif
}
static void header_done_callback(
		http_parser *parser,
        void *data,
        const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
	  fprintf(stderr, "down!!! %.*s\n", (int)length, at);
#endif
}

static void header_response_status_callback(
		http_parser *parser,
        void *data,
        const char *at,
        size_t length)
{
#ifdef SNIFFER_RTSP_DEBUG
	fprintf(stderr, "status!!! %.*s\n", (int)length, at);
#endif
   sniffer_notify_message_t *notify_msg = (sniffer_notify_message_t *)parser->userdata;

	if(length <=  0)
	{
		return;
	}
	if(notify_msg->m_type == SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE)
	{
		notify_msg->body.m_rtsp_msg.m_status = atoi(at);
		
	}

}
static void field_callback(
		http_parser *parser,
        void *data,
        const char *field,
        size_t flen,
        const char *value,
        size_t vlen)
{
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "field!!! %.*s %.*s\n", (int)flen, field, (int)vlen, value);
#endif
	if(flen <=0 || vlen <=0)
		return;
	sniffer_notify_message_t *notify_msg = (sniffer_notify_message_t *)parser->userdata;
	if(SNIFFER_NOTIFY_MESSAGE_RTSP <= notify_msg->m_type  && notify_msg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE)
	{
		sniffer_options_t * option = (sniffer_options_t*)malloc(sizeof(sniffer_options_t));
		memset(option, 0, sizeof(sniffer_options_t));
		size_t optionsize = sizeof(option->option)-1;
		size_t contentsize = sizeof(option->content)-1;
		optionsize = optionsize > flen ? flen:optionsize;
		contentsize = contentsize > vlen? vlen:contentsize;

		strncpy(option->option, field, optionsize);
		strncpy(option->content, value, contentsize);
		snifferInsertOption(&notify_msg->body.m_rtsp_msg.m_options, option);


	}
}

static void method_type(struct http_parser * parser, http_method_type_e type)
{
	sniffer_notify_message_t *notify_msg = (sniffer_notify_message_t *)parser->userdata;
	notify_msg->m_type =SNIFFER_NOTIFY_MESSAGE_MAX;
	switch(type)
	{
		case 	http_method_type_rtsp_DESCRIBE:
			{
				notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_DESCRIBE;
			}
			break;
		case 	http_method_type_rtsp_ANNOUNCE:
			{
				notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_ANNOUNCE;
			}
			break;
		case 	http_method_type_rtsp_GET_PARAMETER:
			{
				notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_GET_PARAMETER;
			}
			break;
		case 	http_method_type_rtsp_OPTIONS:
			{
				notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_OPTIONS;
			}
			break;
		case 	http_method_type_rtsp_PAUSE:
			{
				notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_PAUSE;
			}
			break;
		case 	http_method_type_rtsp_PLAY:
			{
					notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_PLAY;
			}
			break;
		case 	http_method_type_rtsp_RECORD:
			{
					notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_RECORD;
			}
			break;
		case 	http_method_type_rtsp_REDIRECT:
			{
					notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_REDIRECT;
			}
			break;
		case 	http_method_type_rtsp_SETUP:
			{
					notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_SETUP;
			}
			break;
		case 	http_method_type_rtsp_SET_PARAMETER:
			{
					notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_SET_PARAMETER;
			}
			break;
		case 	http_method_type_rtsp_TEARDOWN:
			{
					notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_TEARDOWN;
			}
			break;
		case 	http_method_type_rtsp_RESPONE:
			{
					notify_msg->m_type = SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE;
			}
			break;
	}
#ifdef SNIFFER_RTSP_DEBUG
	 fprintf(stderr, "method_type!!! %d\n", type);
#endif
}
const char* _rtsp_getOptionContent(sniffer_notify_message_t * pmsg, char *option)
{
	if( (SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST <= pmsg->m_type)  && (pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE))
	{
		return _snifferGetOptionContent(pmsg->body.m_rtsp_msg.m_options, option);
	}
	return NULL;
}
static void _rtsp_free(sniffer_notify_message_t * pmsg)
{
	if( (SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST <= pmsg->m_type)  && (pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE))
	{
		if(NULL != pmsg->body.m_rtsp_msg.m_options)
		{
			snifferFreeOptionList(pmsg->body.m_rtsp_msg.m_options);
		}
	}
	free(pmsg);

}
int sniffer_rtsp_process(tcp_udp_packet_t *packet, NotifyCallback cb)
{
	
	//printf("%s\n", packet->body);
	//cout<<endl;
#ifdef SNIFFER_RTSP_DEBUG
	fprintf(stderr, "*****************************************************************************\n");
#endif
	   size_t nread;
    http_parser parser;
    parser.http_field = &field_callback;
    parser.request_method = &request_method_callback;
    parser.request_uri = &request_uri_callback;
    parser.fragment = &fragment_callback;
    parser.request_path = &request_pathcallback;
    parser.query_string = &query_string_callback;
    parser.http_version = &http_version_callback;
    parser.header_done = &header_done_callback;
	parser.request_method_type = &method_type;
	parser.response_status_cb = &header_response_status_callback;
	

	sniffer_notify_message_t *notify_msg = (sniffer_notify_message_t *)malloc(sizeof(sniffer_notify_message_t));
	memset(notify_msg, 0, sizeof(sniffer_notify_message_t));
	notify_msg->free = _rtsp_free;
	notify_msg->getOptionContent = _rtsp_getOptionContent;

	parser.userdata = (void*)notify_msg;

    http_parser_init(&parser);
    nread = http_parser_execute(
            &parser,
            (const char*)packet->body,
			packet->body_length,
            0);
#ifdef SNIFFER_RTSP_DEBUG
    fprintf(stderr, "nread: %lu\n", nread);
#endif
	if(!http_parser_is_finished(&parser))
	{
		snifferFreeNotifyMessage(notify_msg);
	}
	else
	{
		memcpy(&notify_msg->m_eth_addr, packet->eh, sizeof(ether_header_t));
		memcpy(&notify_msg->m_ip_saddr, &packet->ih->saddr, sizeof(ip_address_t));
		memcpy(&notify_msg->m_ip_daddr, &packet->ih->daddr, sizeof(ip_address_t));
		memcpy(&notify_msg->m_ip_sport, &packet->th->th_sport, sizeof(ip_port));
		memcpy(&notify_msg->m_ip_dport, &packet->th->th_dport, sizeof(ip_port));
		notify_msg->th_seq = ntohl(packet->th->th_seq);
		notify_msg->th_ack = ntohl(packet->th->th_ack);
		notify_msg->istcp = packet->istcp;
		notify_msg->m_time = packet->tv_sec;
		if((notify_msg->m_type == SNIFFER_NOTIFY_MESSAGE_RTSP_SETUP) || notify_msg->m_type == SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE)
		{
			if((NULL != cb )&& (NULL != cb->cb) )
			{
				cb->cb(notify_msg, cb->usr_param);
			}
		}
		else
		{
			snifferFreeNotifyMessage(notify_msg);
		}
	}
#ifdef SNIFFER_RTSP_DEBUG
	fprintf(stderr, "*****************************************************************************\n");
#endif
	return 0;
}
#endif