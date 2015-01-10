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
			devHandle_t*数组， 由调用者提供

		@size:
			p_handle 元素个数

		@return:
			获取元素个数
	*/
	DLL_EXPORT int snifferGetDevHandles(devHandle_t * p_handle, int size);


	/*
		notify_call_back_t 消息上报接口

		@usr_param:
			用户私有信息，该信息会跟随消息一起上报

		@cb:
			回调接口
	*/
	typedef struct  notify_call_back
	{
		const u_char * usr_param;

		int (* cb)(sniffer_notify_message_t * pmsg, const u_char * usr_param);

	}notify_call_back_t;

	typedef notify_call_back_t* NotifyCallback;

	/*
		snifferPacketsProcessLoop 主任务，执行抓包动作，并负责向调用者上报，注意该接口不会返回，直到网卡设备关闭

		@p_handle：
			设备句柄 参照devHandle_t
		@cb：
			参照notify_call_back_t

	*/
	DLL_EXPORT int snifferPacketsProcessLoop(devHandle_t * p_handle, NotifyCallback cb);

	/*
		snifferGetOptionContent 获取option信息

		@optionsHead：
			参照 pmsg 结构
		@option：
			用户需要查询的option

		@return:
			返回option对应的内容

		例如：
			ContentLength: 4096

		const char * option = "ContentLength";
		const char * value = NULL;
		value = snifferGetOptionContent(pmsg, option);
		结果value = "4096"
	*/
	DLL_EXPORT	const char *snifferGetOptionContent(sniffer_notify_message_t * pmsg, char *option);

	/*
		snifferFreeNotifyMessage 
			释放消息所占内存，sniffer_lib上报的网络包消息，内存由sniffer_lib申请，该内存必须由调用者释放
			这是为了尽量减少内存拷贝，一般情况下，调用者在得到消息以后，应该把消息加入其他任务队列，让其
			他任务处理消息，不应阻塞NotifyCallback，否则会造成sniffer_lib丢包
		@pmsg:
			参照 pmsg 结构
	*/
	DLL_EXPORT	void snifferFreeNotifyMessage(sniffer_notify_message_t * pmsg);





}
