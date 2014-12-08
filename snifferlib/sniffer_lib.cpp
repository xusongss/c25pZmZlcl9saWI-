
#include "sniffer_lib.h" 

#include "pcap.h"
#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib,"../wpcap.lib")

#include "sniffer_rtsp.h"
static protocol_model_t g_protocols[]=
{
	{
		"rtsp",
		PROTOCOL_RTSP_PORT,
		false,
		sniffer_rtsp_process
	}
};
int Add(int &a,int &b) 
{ 
    return a+b;  
} 
static void display_ether_header(ether_header_t *header)
{
	printf("\n");
	for(int i = 0; i < ETHER_ADDR_LEN; i++)
	{
		printf("0x%x:", header->ether_shost[i]);
	}
	printf("==>");
	for(int i = 0; i < ETHER_ADDR_LEN; i++)
	{
		printf("0x%x:", header->ether_dhost[i]);
	}
	printf("\n");

}
static void display_ip_header(ip_header_t *header)
{
	printf("\n");
	printf("%d.%d.%d.%d", header->saddr.byte1,  header->saddr.byte2,  header->saddr.byte3,  header->saddr.byte4);
	printf("==>");
	printf("%d.%d.%d.%d", header->daddr.byte1,  header->daddr.byte2,  header->daddr.byte3,  header->daddr.byte4);
	printf("\n");

}
static void display_tcp_header(tcp_header_t * header)
{
	//std::cout<<"nihao "<<std::endl;
	printf("\n");
	printf("tcp %d ==> %d", ntohs(header->th_sport), ntohs(header->th_dport));
	printf("\n");
}
/* 每次捕获到数据包时，libpcap都会自动调用这个回调函数 */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{

	u_int tcp_or_udp_header_len = 0;
	u_short s_port = 0;
	u_short d_port = 0;
	tcp_udp_packet_t packetdata;
	packetdata.eh = NULL;
	packetdata.ih = NULL;
	packetdata.th = NULL;
	packetdata.uh = NULL;
	packetdata.istcp = false;
	packetdata.body = NULL;

	packetdata.eh = (ether_header_t *)pkt_data;
	if(packetdata.eh->ether_type != ntohs(ETHERTYPE_IP))
	{
		//只处理Ethernet II类型的包
		return;
	}
	
	packetdata.ih = (ip_header *) (pkt_data + ETHER_HEADER_LEN(packetdata.eh)); //以太网头部长度

	if(packetdata.ih->proto == ntohs(TCP_PROTOCAL))
	{
		//tcp 
		packetdata.th = (tcp_header_t *) ((u_char*)packetdata.ih + IP_HEADER_LEN(packetdata.ih)) ;
		packetdata.istcp = true;
		s_port = ntohs(packetdata.th->th_sport);
		d_port = ntohs(packetdata.th->th_dport);
		tcp_or_udp_header_len = TCP_HEADER_LEN(packetdata.th);
		//display_tcp_header(packetdata.th);
	}
	else if(packetdata.ih->proto == ntohs(UDP_PROTOCAL))
	{
		//udp
		packetdata.uh = (udp_header_t *)  ((u_char*)packetdata.ih + IP_HEADER_LEN(packetdata.ih)) ;
		packetdata.istcp = false;
		s_port = ntohs(packetdata.uh->sport);
		d_port = ntohs(packetdata.uh->dport);
		tcp_or_udp_header_len = UDP_HEADER_LEN(packetdata.uh);
	}
	else
	{
		return;
	}

	/* 将时间戳转换成可识别的格式 */
	struct tm *ltime;
    char timestr[16];
    time_t local_tv_sec;
    local_tv_sec = header->ts.tv_sec;
    ltime=localtime(&local_tv_sec);
    strftime( packetdata.timestr, sizeof( packetdata.timestr), "%H:%M:%S", ltime);
	//printf("%s,%.6ld len:%d\n", timestr, header->ts.tv_usec, header->len);
	//	printf("etherii_len + ip_len + tcp_or_udp_header_len = %d\n", etherii_len + ip_len + tcp_or_udp_header_len);

	packetdata.body = pkt_data + ( ETHER_HEADER_LEN(packetdata.eh)  + IP_HEADER_LEN(packetdata.ih) + tcp_or_udp_header_len);

	packetdata.body_length =  header->len - ( ETHER_HEADER_LEN(packetdata.eh) + IP_HEADER_LEN(packetdata.ih) + tcp_or_udp_header_len);

	if(packetdata.body_length <= 0)
	{
		return;
	}
	for(int i = 0; i < sizeof(g_protocols)/ sizeof(g_protocols[0]); i++)
	{
		protocol_model_t * protocol = &g_protocols[i];

		/*
		if(protocol->m_must_tcp && istcp)
		{

		}
		else
		{

		}
		*/
		if((protocol->m_defalut_port == s_port) || (protocol->m_defalut_port == d_port))
		{
			protocol->process(&packetdata,  (NotifyCallback)param);
			break;
		}
	}

}

static const char packet_filter[] = "ip and tcp and (tcp dst port 554 or tcp src port 554)";

int snifferPacketsProcessLoop(devHandle_t * p_handle, NotifyCallback cb)
{
	pcap_if_t* alldevs; // list of all devices
    pcap_if_t* d; // device you chose
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE]; //error buffer

	 if(pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		alldevs = NULL;
		return -1;
    }
	for(d=alldevs, i=0; i < p_handle->m_handle -1; d=d->next, i++); /* jump to the selected interface */

	/**/
	pcap_t* adhandle;

	if((adhandle = pcap_open(d->name, /* the interface name */
                 65536, /* length of packet that has to be retained */
                 PCAP_OPENFLAG_PROMISCUOUS, /* promiscuous mode */
                 1000, /* read time out */
                 NULL, /* auth */
                 errbuf /* error buffer */
                 )) == NULL)
                 {
                     fprintf(stderr, "\nUnable to open the adapter. %s is not supported by Winpcap\n",
                             d->description);
                     return -1;
                 }

	/*设置filter*/
	struct bpf_program fcode;
	u_int netmask;
	if(d->addresses != NULL)
	{
        /* 获得接口一个地址的掩码 */
        netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	}
    else
	{
        /* 如果接口没有地址，那么我们假设一个C类的掩码 */
        netmask=0xffffff;
	}

	//编译过滤器
    if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) <0 )
    {
        fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }
	//设置过滤器
    if (pcap_setfilter(adhandle, &fcode)<0)
    {
        fprintf(stderr,"\nError setting the filter.\n");
        /* 释放设备列表 */
        pcap_freealldevs(alldevs);
        return -1;
    }

    printf("\nListening on %s...\n", d->description);
	pcap_freealldevs(alldevs); // release device list

	 /* 开始捕获 */
    pcap_loop(adhandle, 0, packet_handler, (u_char*)cb);

    return 0;

}


