#include "sniffer_lib.h"
#include <stdlib.h>
#include <string.h>
static const char *_snifferGetOptionContent(p_sniffer_options_t optionsHead, char *option)
{
	p_sniffer_options_t current = optionsHead;
	while (current != NULL){
		/* Check if current node is what we're looking for */
		if (!strcmp(current->option, option)){
			return current->content; 
		}
		current = current->next;
	}
	/* Not found */
	return NULL;
}

 const char *snifferGetOptionContent(sniffer_notify_message_t * pmsg, char *option)
 {
	if( (SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST <= pmsg->m_type)  && (pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONSE))
	{
		return _snifferGetOptionContent(pmsg->body.m_rtsp_msg.m_options, option);
	}
	return NULL;
 }
/* Adds new option opt to the struct's option list */
void snifferInsertOption(p_sniffer_options_t *optionsHead, p_sniffer_options_t opt){
	p_sniffer_options_t current = *optionsHead;
	opt->next = NULL;

	/* Empty options list */
	if (*optionsHead == NULL){
		*optionsHead = opt;
		return; 
	}
	/* Traverse the list and insert the new option at the end */
	while (current != NULL){
		/* Check for duplicate option; if so, replace the option currently there */
		if (!strcmp(current->option, opt->option)){
			//current->content = opt->content;
			strncpy(current->content, opt->content, sizeof(current->content) -1);
			return;
		}
		if (current->next == NULL){
			current->next = opt;
			return;
		}
		current = current->next;
	}
}


/* Free every node in the message's option list */
void snifferFreeOptionList(p_sniffer_options_t optionsHead){
	p_sniffer_options_t current = optionsHead;
	p_sniffer_options_t temp; 
	while (current != NULL){
		temp = current; 
		current = current->next;
		free(temp);
	}
}
void snifferFreeNotifyMessage(sniffer_notify_message_t * pmsg)
{
	if( (SNIFFER_NOTIFY_MESSAGE_RTSP_REQUEST <= pmsg->m_type)  && (pmsg->m_type <= SNIFFER_NOTIFY_MESSAGE_RTSP_RESPONSE))
	{
		if(NULL != pmsg->body.m_rtsp_msg.m_options)
		{
			snifferFreeOptionList(pmsg->body.m_rtsp_msg.m_options);
		}
	}
	free(pmsg);
}