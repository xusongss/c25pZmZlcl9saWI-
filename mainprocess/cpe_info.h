#pragma once
#include <string>
class SessionInfo
{
private：

	//服务器地址
	string m_serverIp;
	string m_serverPort;
	//客户端IP
	string m_clientIp;
	string m_clientPort;
	//rtsp 会话ID
	string m_rtspSessionId;
	//节目名称和ID
	string m_programName;
	string m_programSessionId;
	//节目频点信息
	string m_programFrequency;
	string m_programAnnex;
	string m_programModulation;
	string m_programSymbolRate;
	string m_programChannelSpace;
	//节目时间
	int m_startTime;
	int m_endTime;
};
class CpeInfo
{
private:
	string m_userId;
	string m_sn;
	string m_mac;
	string m_ip;
	string m_areaCode;

	//支持的频率范围
	string m_frequency;
	//支持的调制方式
	string m_annex;

	string m_modulation;
	//支持的符号率范围
	string m_symbolRate;
	//中心频点贷款
	string m_channelSpace;
};
class Cpe
{

}