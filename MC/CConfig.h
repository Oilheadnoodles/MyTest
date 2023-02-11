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

