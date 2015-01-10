#pragma once
#include "sniffer_type.h"



#pragma pack(push)
#pragma pack(1)
/*struct define*/
/* four bytes ip address */

typedef struct ip_address
{
	/*网络字节序*/
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address_t;
/*2字节port*/
typedef struct ip_port
{
	/*网络字节序*/
	char byte1;
	char byte2;
} ip_port_t;
#pragma pack(pop)


/*genral struct*/

typedef struct sniffer_options
{
	char option [16];
	char content[256]; 
	struct sniffer_options *next; 
} sniffer_options_t, *p_sniffer_options_t;

/*rtsp struct*/
typedef struct
{
	// Request rtsp packet must have m_command, m_target
	char m_command[16];
	char m_target[1024];
	//Response rtsp packet must have m_status,  m_status_string
	int m_status;
	char m_status_string[64];
	//all rtsp packet may have some options
	p_sniffer_options_t m_options;
}rtspMsg_t;

/*genral message*/

typedef enum
{
	SNIFFER_NOTIFY_MESSAGE_RTSP = 0x01,
	SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST = SNIFFER_NOTIFY_MESSAGE_RTSP,
	SNIFFER_NOTIFY_MESSAGE_RTSP_DESCRIBE = SNIFFER_NOTIFY_MESSAGE_RTSP,
	SNIFFER_NOTIFY_MESSAGE_RTSP_ANNOUNCE,
	SNIFFER_NOTIFY_MESSAGE_RTSP_GET_PARAMETER,
	SNIFFER_NOTIFY_MESSAGE_RTSP_OPTIONS,
	SNIFFER_NOTIFY_MESSAGE_RTSP_PAUSE,
	SNIFFER_NOTIFY_MESSAGE_RTSP_PLAY,
	SNIFFER_NOTIFY_MESSAGE_RTSP_RECORD,
	SNIFFER_NOTIFY_MESSAGE_RTSP_REDIRECT,
	SNIFFER_NOTIFY_MESSAGE_RTSP_SETUP,
	SNIFFER_NOTIFY_MESSAGE_RTSP_SET_PARAMETER,
	SNIFFER_NOTIFY_MESSAGE_RTSP_TEARDOWN,
	SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE,
	SNIFFER_NOTIFY_MESSAGE_MAX
}sniffer_notify_message_type_e;

typedef struct sniffer_notify_message
{
	sniffer_notify_message_type_e m_type;

	/*time in second from TC1970-1-1 0:0:0*/
	u_longlong m_time;
	/*
		MAC信息
	*/
	ether_header_t	m_eth_addr;
	/*
		IP信息
	*/
	ip_address_t	m_ip_saddr;
	ip_address_t	m_ip_daddr;

	/*PORT信息*/
	ip_port_t		m_ip_sport;
	ip_port_t		m_ip_dport;

	u_int th_seq;             /* sequence number */
	u_int th_ack;             /* acknowledgement number */

	bool istcp;

	union
	{
		rtspMsg_t m_rtsp_msg;
	}body; 

	void (*free)(struct sniffer_notify_message * msg);
	const char* (*getOptionContent)(struct sniffer_notify_message * pmsg, char *option);
}sniffer_notify_message_t;
