// mainprocess.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "display_msg.h"
#include "process_msg.h"

#include "../snifferlib/sniffer_lib.h"
#pragma comment(lib,"../debug/snifferlib.lib")
#pragma comment(lib,"../wpcap.lib")

int write_file = 0;

static int processmsg(sniffer_notify_message_t * pmsg, const u_char * usr_param)
{
	Processer::processMsg(Processer::getInstance(), pmsg);
	return 0;
}
static notify_call_back_t sniffer_call = 
{
	NULL,
	processmsg
};

static void usage()
{
	std::cout<<"\t ? help"<<std::endl;
	std::cout<<"\t -l list all net devices"<<std::endl;
	std::cout<<"\t -i select a net device"<<std::endl;
	std::cout<<"\t -f select a file"<<std::endl;

}
int main(int argc, const char * argv[])
{
	devHandle_t devices[10];
	int handle = -1;
	const char * filename = NULL;

	int size = snifferGetDevHandles(devices, sizeof(devices)/ sizeof(devices[0]));

	for(int i = 0; i < argc; i++)
	{
		if(!strcmp(argv[i], "?"))
		{
			usage();
			return 0;
		}
		else if(!strcmp(argv[i], "-l"))
		{
			for(int i = 0; i< size; i++)
			{
				std::cout<<"device\t num:"<<i<<std::endl;
				std::cout<<"\t name:"<<devices[i].m_name<<std::endl;
			}
			return 0;
		}
		else if(!strcmp(argv[i], "-i"))
		{
			if((i + 1) >= argc)
			{
				std::cout<<"please select a device"<<std::endl;
				usage();
				return 0;
			}
			else
			{
				handle = atoi(argv[i+1]);
			}
		}
		else if(!strcmp(argv[i], "-f"))
		{
			if((i + 1) >= argc)
			{
				std::cout<<"please select a device"<<std::endl;
				usage();
				return 0;
			}
			else
			{
				filename = argv[i+1];
			}
		}
		else
		{
			//usage();
		}

	}
	if(-1 != handle && handle < size)
	{
		snifferPacketsProcessLoop(&devices[handle], &sniffer_call);
		goto end;
	}
	else if(NULL  != filename)
	{
		write_file = 1;

		devices[0].m_file = filename;

		snifferPacketsProcessLoop(&devices[0], &sniffer_call);

		goto end;
	}
	
end:

	return 0;
}

