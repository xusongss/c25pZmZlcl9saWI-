#include "sniffer_lib.h" 
#include "pcap.h"
#include "string.h"


int snifferGetDevHandles(devHandle_t * p_handle, int size)
{
	
	pcap_if_t *alldevs;
    pcap_if_t *d;
    int i = 0;
    char errbuf[PCAP_ERRBUF_SIZE];

    /* get local devices */
    if(pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		alldevs = NULL;
		return 0;
    }

    /* print devices list */
    for(d = alldevs; d != NULL; d = d->next)
    {
		i ++;
#if 0
        fprintf(stderr, "%d. %s\n", i, d->name);
#endif

		p_handle->m_handle = i;

		strcpy(p_handle->m_name,d->name);

		p_handle->m_file = NULL;

		p_handle++;

#if 0
        if (d->description)
		{
           fprintf(stderr, "(%s)\n", d->description);
		}
        else
		{
           fprintf(stderr, "(No description available)\n");
		}
#endif
    }

    if(i == 0)
    {
        fprintf(stderr, "\nNo interfaces found! Make sure Winpcap is installed.\n");
    }
	return i;
}