//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include "..\\Common\CPacket.h"
#include <list>
#include <winsock2.h>
#include "windows.h"

//使用观察者模式，用于子线程对服务器收到的包监控
class Observer
{
public:
	virtual void Update(CPacket* ptmp) = 0;
};

class Subject
{
public:
	Subject() { m_ObserverList.clear(); }
	~Subject() { m_ObserverList.clear(); }

	// I need a lock here, but I'm running out of time....
	void Attach(Observer* pObserver) 
	{ 
		m_ObserverList.push_back(pObserver); 
	}
	void Detach(Observer* pObserver) 
	{ 
		m_ObserverList.remove(pObserver); 
		delete pObserver;
		pObserver = nullptr;
	}
	void Notify(CPacket* ptmp) {
		list<Observer*>::iterator it = m_ObserverList.begin();
		while (it != m_ObserverList.end())
		{
			(*it)->Update(ptmp);
			++it;
		}
	}

private:
	list<Observer*> m_ObserverList;
};

class CConnection :public Observer
{
public:
	CConnection(SOCKET socket, string ip, string port);
	~CConnection();
		
	void Send(CPacket& packet);
	static DWORD Recv(LPVOID lp);

public:
	virtual void Update(CPacket* ptmp);

private:
	SOCKET m_socket;
	string m_ip;
	string m_nPort;
	string m_strUser;

	HANDLE m_hThread;
};
