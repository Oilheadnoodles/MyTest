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

	void SetUser(string userName) { m_strUserName = userName; }
	string GetUser() { return m_strUserName; }
	void SetServer(string strServer) { m_strServer = strServer; }
	string GetServer() { return m_strServer; }

protected:
	CConfig();

private:
	static CConfig* _instance;
	string m_strUserName;
	string m_strServer;
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

