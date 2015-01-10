#pragma once 
#define HAVE_REMOTE
#define WINPACP
#include <stdio.h>
#include <stdlib.h>
#include "sniffer_type.h"
#include "sniffer_msg.h"
#include "sniffer_key_value.h"

#define DLL_EXPORT __declspec(dllexport) 
extern "C"
{

	typedef struct
	{
		int m_handle;
		char m_name[DEVICE_NAME_LENGTH];
		const char *m_file;
	}devHandle_t;

	/*
		getDevHandles

		@p_handle:
			devHandle_t*���飬 �ɵ������ṩ

		@size:
			p_handle Ԫ�ظ���

		@return:
			��ȡԪ�ظ���
	*/
	DLL_EXPORT int snifferGetDevHandles(devHandle_t * p_handle, int size);


	/*
		notify_call_back_t ��Ϣ�ϱ��ӿ�

		@usr_param:
			�û�˽����Ϣ������Ϣ�������Ϣһ���ϱ�

		@cb:
			�ص��ӿ�
	*/
	typedef struct  notify_call_back
	{
		const u_char * usr_param;

		int (* cb)(sniffer_notify_message_t * pmsg, const u_char * usr_param);

	}notify_call_back_t;

	typedef notify_call_back_t* NotifyCallback;

	/*
		snifferPacketsProcessLoop ������ִ��ץ����������������������ϱ���ע��ýӿڲ��᷵�أ�ֱ�������豸�ر�

		@p_handle��
			�豸��� ����devHandle_t
		@cb��
			����notify_call_back_t

	*/
	DLL_EXPORT int snifferPacketsProcessLoop(devHandle_t * p_handle, NotifyCallback cb);

	/*
		snifferGetOptionContent ��ȡoption��Ϣ

		@optionsHead��
			���� pmsg �ṹ
		@option��
			�û���Ҫ��ѯ��option

		@return:
			����option��Ӧ������

		���磺
			ContentLength: 4096

		const char * option = "ContentLength";
		const char * value = NULL;
		value = snifferGetOptionContent(pmsg, option);
		���value = "4096"
	*/
	DLL_EXPORT	const char *snifferGetOptionContent(sniffer_notify_message_t * pmsg, char *option);

	/*
		snifferFreeNotifyMessage 
			�ͷ���Ϣ��ռ�ڴ棬sniffer_lib�ϱ����������Ϣ���ڴ���sniffer_lib���룬���ڴ�����ɵ������ͷ�
			����Ϊ�˾��������ڴ濽����һ������£��������ڵõ���Ϣ�Ժ�Ӧ�ð���Ϣ��������������У�����
			����������Ϣ����Ӧ����NotifyCallback����������sniffer_lib����
		@pmsg:
			���� pmsg �ṹ
	*/
	DLL_EXPORT	void snifferFreeNotifyMessage(sniffer_notify_message_t * pmsg);





}
