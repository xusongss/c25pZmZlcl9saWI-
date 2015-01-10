#pragma once 
#ifndef true
	#define true 1  /* define true */
#endif

#ifndef false
	#define false 0 /* define false */
#endif

#ifndef u_char
	typedef  unsigned char u_char;
#endif

#ifndef u_short
	typedef unsigned short u_short;
#endif

#ifndef u_int
	typedef unsigned int u_int;
#endif
#ifndef u_longlong
		typedef unsigned long long  u_longlong;
#endif


#ifndef ETHER_ADDR_LEN
		#define ETHER_ADDR_LEN 6 /* ethernet address */
#endif

#ifndef ETHERTYPE_IP
	#define ETHERTYPE_IP 0x0800 /* ip protocol */
#endif

#ifndef TCP_PROTOCAL
	#define TCP_PROTOCAL 0x0600 /* tcp protocol */
#endif

	#ifndef UDP_PROTOCAL
	#define UDP_PROTOCAL 0x1100 /* tcp protocol */
#endif

#ifndef BUFFER_MAX_LENGTH
	#define BUFFER_MAX_LENGTH 65536 /* buffer max length */
#endif

#pragma pack(push)
#pragma pack(1)
/* ethernet header */
typedef struct ether_header 
{
	u_char ether_dhost[ETHER_ADDR_LEN]; /* source ethernet address, 8 bytes */
	u_char ether_shost[ETHER_ADDR_LEN]; /* destination ethernet addresss, 8 bytes */
	u_short ether_type;                 /* ethernet type, 16 bytes */
}ether_header_t ;

#pragma pack(pop)
#define INVALID_HANDLE (-1)

#define DEVICE_NAME_LENGTH (256)