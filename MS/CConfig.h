//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include <string>
using namespace std;

//��Ϊȫ��Ψһ�����ö���ʹ�õ���ģʽ
class CConfig
{
public:
	~CConfig();
	static CConfig* Instance();

	void SetNetworkLogFile(string strNetworkLogFile) { m_strNetworkLogFile = strNetworkLogFile; }
	string GetNetworkLogFile() { return m_strNetworkLogFile; }
	void SetPacketLogFile(string strPacketLogFile) { m_strPacketLogFile = strPacketLogFile; }
	string GetPacketLogFile() { return m_strPacketLogFile; }
	void SetListenPort(int nListenPort) { m_nListenPort = nListenPort; }
	int GetListenPort() { return m_nListenPort; }
	
protected:
	CConfig();

private:
	static CConfig* _instance;
	string m_strNetworkLogFile;
	string m_strPacketLogFile;
	int m_nListenPort;

	//�ڴ��Զ����չ���������������������
	class GC
	{
	public:
		GC() {}
		~GC()
		{
			if (_instance)
			{
				delete _instance;
				_instance = nullptr;
			}
		}
	};
	static GC gc;
};

