//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include <string>
using namespace std;

//作为全局唯一的配置对象，使用单例模式
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

	//内存自动回收管理器，用于清除单体对象
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

