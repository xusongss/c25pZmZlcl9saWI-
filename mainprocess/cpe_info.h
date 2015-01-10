#pragma once
#include <string>
class SessionInfo
{
private��

	//��������ַ
	string m_serverIp;
	string m_serverPort;
	//�ͻ���IP
	string m_clientIp;
	string m_clientPort;
	//rtsp �ỰID
	string m_rtspSessionId;
	//��Ŀ���ƺ�ID
	string m_programName;
	string m_programSessionId;
	//��ĿƵ����Ϣ
	string m_programFrequency;
	string m_programAnnex;
	string m_programModulation;
	string m_programSymbolRate;
	string m_programChannelSpace;
	//��Ŀʱ��
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

	//֧�ֵ�Ƶ�ʷ�Χ
	string m_frequency;
	//֧�ֵĵ��Ʒ�ʽ
	string m_annex;

	string m_modulation;
	//֧�ֵķ����ʷ�Χ
	string m_symbolRate;
	//����Ƶ�����
	string m_channelSpace;
};
class Cpe
{

}