#include "process_msg.h"
#include "display_msg.h"
#include <iostream>
Processer * Processer::m_process = NULL;
extern int write_file;
Processer *Processer::getInstance()
{
#if 0
	if(NULL == Processer::m_process)
	{
		Processer::m_process = new Processer();
	}
#endif
	static Processer  g_processer;
	return &g_processer;
}
//static 
void Processer::processMsg(Processer * thiz,sniffer_notify_message_t * pmsg )
{
	thiz->processMsg(pmsg);
}
// non static
void Processer::processMsg(sniffer_notify_message_t * pmsg)
{
	/*只处理setup 和respone*/
	if(pmsg ->m_type != SNIFFER_NOTIFY_MESSAGE_RTSP_SETUP 
		&& pmsg ->m_type != SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE  )
	{
		return;
	}
	if(pmsg->m_type == SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONE)
	{
		MSG_LIST_T::iterator it;
		for(it = this->m_msg_list.begin(); it != this->m_msg_list.end(); ++it)
		{
			sniffer_notify_message_t * request = *it;

			if(compare_ip(&request->m_ip_saddr, &pmsg->m_ip_daddr)
				&& compare_ip(&request->m_ip_daddr, &pmsg->m_ip_saddr)
				&&compare_port(&request->m_ip_sport , &pmsg->m_ip_dport)
				&& compare_port(&request->m_ip_dport , &pmsg->m_ip_sport)
				&& request->th_ack == pmsg->th_seq)
			{
				//std::cout<<"find"<<endl;
				display(request, NULL);
				KeyValueParser_t parser_url;
				KeyValueParser_t parser_request_tranport;
				if(write_file)
				{
					//处理 request URL
					std::string url;
					url.append(strstr(request->body.m_rtsp_msg.m_target, "?") + 1);
					url.append("\r\n");
					key_value_parser_init(&parser_url);
					key_value_parser_execute(&parser_url, url.c_str(), url.length(), 0);

					std::string request_tranport = snifferGetOptionContent(request, "TRANSPORT");
					request_tranport.append("\r\n");
					key_value_parser_init(&parser_request_tranport);
					key_value_parser_execute(&parser_request_tranport, request_tranport.c_str(), request_tranport.length(), 0);

					std::cout<<std::endl;
					std::cout<<"url_startTime    "<<parser_url.url_startTime    <<std::endl ;
					std::cout<<"url_endTime      "<<parser_url.url_endTime      <<std::endl ;
					std::cout<<"url_areaCode     "<<parser_url.url_areaCode     <<std::endl ;
					std::cout<<"url_resgroupId   "<<parser_url.url_resgroupId   <<std::endl ;
					std::cout<<"url_userId       "<<parser_url.url_userId       <<std::endl ;
					std::cout<<"url_sessionId    "<<parser_url.url_sessionId    <<std::endl ;
					std::cout<<"url_sessionType  "<<parser_url.url_sessionType  <<std::endl ;
					std::cout<<"url_payType      "<<parser_url.url_payType      <<std::endl ;
					std::cout<<"url_productId    "<<parser_url.url_productId    <<std::endl ;
					std::cout<<"url_displayName  "<<parser_url.url_displayName  <<std::endl ;
					std::cout<<"url_provider     "<<parser_url.url_provider     <<std::endl ;
					std::cout<<"url_ssId         "<<parser_url.url_ssId         <<std::endl ;
					std::cout<<"url_vcrInfo      "<<parser_url.url_vcrInfo      <<std::endl ;
					std::cout<<"url_token		 "<<parser_url.url_token		<<std::endl;

					std::cout<<"lower_transport		 "<<parser_request_tranport.lower_transport			<<std::endl;
					std::cout<<"lower_transport_type "<<parser_request_tranport.lower_transport_type      <<std::endl;
					std::cout<<"frequency_info       "<<parser_request_tranport.frequency_info            <<std::endl;
					std::cout<<"annex_info           "<<parser_request_tranport.annex_info                <<std::endl;
					std::cout<<"modulation_info      "<<parser_request_tranport.modulation_info           <<std::endl;
					std::cout<<"symbolRate_info      "<<parser_request_tranport.symbolRate_info           <<std::endl;
					std::cout<<"channelSpace_info    "<<parser_request_tranport.channelSpace_info         <<std::endl;
					std::cout<<"areacode_info        "<<parser_request_tranport.areacode_info             <<std::endl;
					std::cout<<"programnum_info      "<<parser_request_tranport.programnum_info           <<std::endl;
					std::cout<<std::endl;

				}				
				display(pmsg, NULL);
				KeyValueParser_t parser_reqspone_transport;
				if(write_file)
				{
					//处理TRANSPORT
					std::string reqspone_transport = snifferGetOptionContent(pmsg, "TRANSPORT");
					reqspone_transport.append("\r\n");
					key_value_parser_init(&parser_reqspone_transport);
					key_value_parser_execute(&parser_reqspone_transport, reqspone_transport.c_str(), reqspone_transport.length(), 0);
					std::cout<<"lower_transport		 "<<parser_reqspone_transport.lower_transport			<<std::endl;
					std::cout<<"lower_transport_type "<<parser_reqspone_transport.lower_transport_type      <<std::endl;
					std::cout<<"frequency_info       "<<parser_reqspone_transport.frequency_info            <<std::endl;
					std::cout<<"annex_info           "<<parser_reqspone_transport.annex_info                <<std::endl;
					std::cout<<"modulation_info      "<<parser_reqspone_transport.modulation_info           <<std::endl;
					std::cout<<"symbolRate_info      "<<parser_reqspone_transport.symbolRate_info           <<std::endl;
					std::cout<<"channelSpace_info    "<<parser_reqspone_transport.channelSpace_info         <<std::endl;
					std::cout<<"areacode_info        "<<parser_reqspone_transport.areacode_info             <<std::endl;
					std::cout<<"programnum_info      "<<parser_reqspone_transport.programnum_info           <<std::endl;
					std::cout<<std::endl;
				}
				char src_ip_str[64];
				char dst_ip_str[64];
				char src_port_str[64];
				char dst_port_str[64];
				char src_mac_str[64];
				std::string program_url = request->body.m_rtsp_msg.m_target;
				if(write_file)
				{
					program_url = program_url.substr(0,program_url.find('?'));
				}
				sprintf(src_ip_str, "%03d.%03d.%03d.%03d", request->m_ip_saddr.byte1, request->m_ip_saddr.byte2,request->m_ip_saddr.byte3,request->m_ip_saddr.byte4);
				sprintf(dst_ip_str, "%03d.%03d.%03d.%03d", request->m_ip_daddr.byte1, request->m_ip_daddr.byte2,request->m_ip_daddr.byte3,request->m_ip_daddr.byte4);
				sprintf(src_port_str, "%d", request->m_ip_sport.byte1 << 8 | request->m_ip_sport.byte2);
				sprintf(dst_port_str, "%d", request->m_ip_dport.byte1 << 8 | request->m_ip_dport.byte2);
						sprintf(src_mac_str,"%02x:%02x:%02x:%02x:%02x:%02x",
								request->m_eth_addr.ether_shost[0],
								request->m_eth_addr.ether_shost[1],
								request->m_eth_addr.ether_shost[2],
								request->m_eth_addr.ether_shost[3],
								request->m_eth_addr.ether_shost[4],
								request->m_eth_addr.ether_shost[5]);
				if(write_file)
				{
					this->m_out<<"<elementInfo>" <<std::endl;

					this->m_out<<"<userInfo>" <<std::endl;

						this->m_out<<"<baseInfo>"<<std::endl;

							this->m_out<<"<userId>"<<std::endl;

								this->m_out<<parser_url.url_userId<<std::endl;

							this->m_out<<"</userId>"<<std::endl;

							this->m_out<<"<SN>"<<std::endl;
								
								this->m_out<<parser_url.url_token<<std::endl;

							this->m_out<<"</SN>"<<std::endl;

							this->m_out<<"<MAC>"<<std::endl;

								this->m_out<<src_mac_str<<std::endl;

							this->m_out<<"</MAC>"<<std::endl;
					
						this->m_out<<"</baseInfo>";

							this->m_out<<"<areaInfo>"<<std::endl;
														
							this->m_out<<"<areaCode>"<<std::endl;
								
								this->m_out<<parser_request_tranport.areacode_info<<std::endl;
							
							this->m_out<<"</areaCode>"<<std::endl;

							this->m_out<<"<IP>"<<std::endl;
								
								this->m_out<<src_ip_str<<std::endl;

							this->m_out<<"</IP>"<<std::endl;

							this->m_out<<"<Port>"<<std::endl;
								
								this->m_out<<src_port_str<<std::endl;

							this->m_out<<"</Port>"<<std::endl;

							this->m_out<<"<Frequency>"<<std::endl;

								this->m_out<<parser_request_tranport.frequency_info<<std::endl;

							this->m_out<<"</Frequency>"<<std::endl;


							this->m_out<<"<Annex>"<<std::endl;

								this->m_out<<parser_request_tranport.annex_info<<std::endl;

							this->m_out<<"</Annex>"<<std::endl;

							
							this->m_out<<"<SymbolRate>"<<std::endl;

								this->m_out<<parser_request_tranport.symbolRate_info<<std::endl;

							this->m_out<<"</SymbolRate>"<<std::endl;

							
							this->m_out<<"<Modulation>"<<std::endl;

								this->m_out<<parser_request_tranport.modulation_info<<std::endl;

							this->m_out<<"</Modulation>"<<std::endl;
							
							this->m_out<<"<ChannelSpace>"<<std::endl;

								this->m_out<<parser_request_tranport.channelSpace_info<<std::endl;

							this->m_out<<"</ChannelSpace>"<<std::endl;

						this->m_out<<"</areaInfo>"<<std::endl;
						
					this->m_out<<"</userInfo>"<<std::endl;

					this->m_out<<"<serverInfo>"<<std::endl;
						
						this->m_out<<"<IP>"<<std::endl;

							this->m_out<<dst_ip_str<<std::endl;

						this->m_out<<"</IP>"<<std::endl;

						this->m_out<<"<Port>"<<std::endl;

							this->m_out<<dst_port_str<<std::endl;

						this->m_out<<"</Port>"<<std::endl;
						
					this->m_out<<"</serverInfo>"<<std::endl;

					this->m_out<<"<programInfo>"<<std::endl;
						
						this->m_out<<"<ssId>"<<std::endl;

							this->m_out<<parser_url.url_ssId<<std::endl;

						this->m_out<<"</ssId>"<<std::endl;

						this->m_out<<"<ProgramNo>"<<std::endl;

							this->m_out<<parser_reqspone_transport.programnum_info<<std::endl;

						this->m_out<<"</ProgramNo>"<<std::endl;

						this->m_out<<"<ProgramUrl>"<<std::endl;

							this->m_out<<program_url<<std::endl;

						this->m_out<<"</ProgramUrl>"<<std::endl;

						this->m_out<<"<ProgramType>"<<std::endl;

							this->m_out<<parser_reqspone_transport.lower_transport<<std::endl;

						this->m_out<<"</ProgramType>"<<std::endl;

						this->m_out<<"<Frequency>"<<std::endl;

							this->m_out<<parser_reqspone_transport.frequency_info<<std::endl;

						this->m_out<<"</Frequency>"<<std::endl;


						this->m_out<<"<Annex>"<<std::endl;
							
							this->m_out<<parser_reqspone_transport.annex_info<<std::endl;

						this->m_out<<"</Annex>"<<std::endl;


						this->m_out<<"<SymbolRate>"<<std::endl;

							this->m_out<<parser_reqspone_transport.symbolRate_info<<std::endl;

						this->m_out<<"</SymbolRate>"<<std::endl;


						this->m_out<<"<ChannelSpace>"<<std::endl;
							
							this->m_out<<parser_reqspone_transport.channelSpace_info<<std::endl;

						this->m_out<<"</ChannelSpace>"<<std::endl;


						this->m_out<<"<Modulation>"<<std::endl;

							this->m_out<<parser_reqspone_transport.modulation_info<<std::endl;

						this->m_out<<"</Modulation>"<<std::endl;

						
						this->m_out<<"<startTime>"<<std::endl;
							
							this->m_out<<parser_url.url_startTime<<std::endl;

						this->m_out<<"</startTime>"<<std::endl;

						this->m_out<<"<endTime>"<<std::endl;

							this->m_out<<parser_url.url_endTime<<std::endl;

						this->m_out<<"</endTime>"<<std::endl;
					
					this->m_out<<"</programInfo>"<<std::endl;

					this->m_out<<"</elementInfo>" <<std::endl;
				}
				this->m_msg_list.remove(request);
				snifferFreeNotifyMessage(pmsg);
				snifferFreeNotifyMessage(request);
				break;
			}
		}

	}
	else if(pmsg->m_type == SNIFFER_NOTIFY_MESSAGE_RTSP_SETUP)
	{
		this->m_msg_list.push_back(pmsg);
	}


}

bool Processer::compare_ip(ip_address_t *ip1, ip_address_t *ip2)
{
	return (ip1->byte1 == ip2->byte1 
		&& ip1->byte2 == ip2->byte2
		&& ip1->byte3 == ip2->byte3
		&& ip1->byte4 == ip2->byte4);
}
bool Processer::compare_port(ip_port_t *p1, ip_port_t *p2)
{
	return ( (p1->byte1 == p2->byte1) 
			&& (p1->byte2 == p2->byte2));
}
Processer::~Processer()
{
	if(write_file)
	{
		this->m_out<<"</document>"<<std::endl;
		this->m_out.close();
	}
}
Processer::Processer()
{
	if(write_file)
	{
		this->m_out.open("info.xml");
		this->m_out<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>"<<std::endl;
		this->m_out<<"<document>"<<std::endl;
	}
	
}
