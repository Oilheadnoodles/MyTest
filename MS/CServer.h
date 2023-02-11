//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include "CConnection.h"
#include "..\Common\CPacket.h"
#include <list>
using namespace std;

//ʹ����������������ģʽ�����ڶ��հ��̵߳ķ���������
class CServer : public Subject
{
public:	
	~CServer();		
	void Start(int port);
	void End();
	static CServer* Instance();
	void PushPacket(CPacket& packet);

protected:
	CServer();
	static DWORD ListenThread(LPVOID lp);
	static DWORD ProcessPacketThread(LPVOID lp);

private:
	SOCKET m_sokcet;
	int m_nPort;
	bool m_bExitProcessThread;
	HANDLE m_hListenThread;
	HANDLE m_hProcessThread;
	HANDLE m_hNewPacketEvent;
	list<CPacket> m_lstPacket;
	CRITICAL_SECTION m_csListPacket;

private:
	static CServer* _instance;
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