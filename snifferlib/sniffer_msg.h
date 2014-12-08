#pragma once
#include "sniffer_type.h"


#pragma pack(push)
#pragma pack(1)
/*struct define*/
/* four bytes ip address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address_t;
/*2×Ö½Úport*/
typedef struct ip_port
{
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
	char m_target[256];
	//Response rtsp packet must have m_status,  m_status_string
	int m_status;
	char m_status_string[64];
	//all rtsp packet must have CSeq
	int m_cseq;
	//all rtsp packet may have some options
	p_sniffer_options_t m_options;
}rtspMsg_t;

/*genral message*/

typedef enum
{
	SNIFFER_NOTIFY_MESSAGE_RTSP = 0x01,
	SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST = SNIFFER_NOTIFY_MESSAGE_RTSP,
	SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONSE,
}sniffer_notify_message_type_e;

typedef struct
{
	sniffer_notify_message_type_e m_type;
	ip_address_t	m_ip_saddr;
	ip_port_t		m_ip_sport;
	ip_address_t	m_ip_daddr;
	ip_port_t		m_ip_dport;

	union
	{
		rtspMsg_t m_rtsp_msg;
	}body; 
}sniffer_notify_message_t;
