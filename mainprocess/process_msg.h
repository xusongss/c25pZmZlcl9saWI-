#ifndef process_msg_h
#define process_msg_h
#include <list>
#include <fstream>
#include "../snifferlib/sniffer_lib.h"

class Processer
{
public:
	static Processer *getInstance();
	static void processMsg(Processer * thiz,sniffer_notify_message_t * pmsg );
private:
	~Processer();
	Processer();
	void processMsg(sniffer_notify_message_t * pmsg);
	std::ofstream m_out;
	static Processer * m_process;
private:
	typedef std::list<sniffer_notify_message_t *> MSG_LIST_T;
	bool compare_ip(ip_address_t *ip1, ip_address_t *ip2);
	bool compare_port(ip_port_t *p1, ip_port_t *p2);
	MSG_LIST_T m_msg_list;

};
#endif