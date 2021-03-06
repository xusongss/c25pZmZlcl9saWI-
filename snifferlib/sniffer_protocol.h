#pragma once 
#include "sniffer_lib.h"

/*genral struct*/
#pragma pack(push)
#pragma pack(1)

/* ipv4 header */
	typedef struct ip_header 
	{
		u_char ver_ihl;         /* version and ip header length */
		u_char tos;             /* type of service */
		u_short tlen;           /* total length */
		u_short identification; /* identification */
		u_short flags_fo;       // flags and fragment offset
		u_char ttl;             /* time to live */
		u_char proto;           /* protocol */
		u_short crc;            /* header checksum */
		ip_address_t saddr;       /* source address */
		ip_address_t daddr;       /* destination address */
		u_int op_pad;           /* option and padding */
	}ip_header_t;
	/* tcp header */
	typedef struct tcp_header 
	{
		u_short th_sport;         /* source port */
		u_short th_dport;         /* destination port */
		u_int th_seq;             /* sequence number */
		u_int th_ack;             /* acknowledgement number */
		u_char th_len_resv_code;		 /* datagram length and reserved code */
		u_char flags;
		u_short th_window;        /* window */
		u_short th_sum;           /* checksum */
		u_short th_urp;           /* urgent pointer */
	}tcp_header_t;

	/* UDP 首部*/
	typedef struct udp_header
	{
		u_short sport;          // 源端口(Source port)
		u_short dport;          // 目的端口(Destination port)
		u_short len;            // UDP数据包长度(Datagram length)
		u_short crc;            // 校验和(Checksum)
	} udp_header_t;

#pragma pack(pop)
typedef struct tcp_udp_packet
{
		char timestr[32];
		u_longlong		tv_sec;

		ether_header_t	*eh;
		ip_header_t		*ih;
		tcp_header		*th;
		udp_header		*uh;
		bool			istcp;
		const u_char	*body;
		u_int			body_length;
}tcp_udp_packet_t;

#define ETHER_HEADER_LEN(header) ( 14 )
#define IP_HEADER_LEN(header) ( (header->ver_ihl & 0xf) * 4 )
#define TCP_HEADER_LEN(header) ( ((header->th_len_resv_code & 0xf0) >> 4) * 4 )
#define UDP_HEADER_LEN(header) ( 8 )

void snifferInsertOption(p_sniffer_options_t *optionsHead, p_sniffer_options_t opt);

void snifferFreeOptionList(p_sniffer_options_t optionsHead);

const char *_snifferGetOptionContent(p_sniffer_options_t optionsHead, char *option);

typedef int (* protocol_t)(tcp_udp_packet_t *packet, NotifyCallback cb);

typedef struct
{
	const char *	m_pro_name;
	u_short			m_defalut_port;
	bool			m_must_tcp;;
	protocol_t		process;
}protocol_model_t;