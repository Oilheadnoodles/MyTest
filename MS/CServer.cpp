//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CServer.h"
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

CServer* CServer::_instance = nullptr;
CServer::GC CServer::gc;

CServer::CServer()
{
	m_sokcet = NULL;
	m_nPort = 0;	
	m_bExitProcessThread = false;
	m_hListenThread = NULL;
	m_hProcessThread = NULL;
	m_hNewPacketEvent = NULL;
	m_lstPacket.clear();
}

CServer::~CServer()
{
	End();
	m_lstPacket.clear();
}

void CServer::Start(int port)
{
	m_nPort = port;
	m_bExitProcessThread = false;
	InitializeCriticalSection(&m_csListPacket);

	DWORD dwThreadID1;
	m_hListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenThread, (LPVOID)this, 0, &dwThreadID1);

	m_hNewPacketEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	DWORD dwThreadID2;
	m_hListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProcessPacketThread, (LPVOID)this, 0, &dwThreadID2);
}

DWORD CServer::ListenThread(LPVOID lp)
{
	CServer* pServer = (CServer*)lp;
	if (!pServer)
		return -1;

	DWORD dwReturn = -1;

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return -2;

	SOCKET listenSocket;	
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
		return -2;

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen);
		
	sockaddr_in listenSockaddr;
	memset(&listenSockaddr, 0, sizeof(sockaddr_in));
	listenSockaddr.sin_family = AF_INET;
	listenSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenSockaddr.sin_port = htons((u_short)pServer->m_nPort);

	if (bind(listenSocket, (sockaddr*)&listenSockaddr, sizeof(listenSockaddr)) == SOCKET_ERROR)
		return-3;

	if (listen(listenSocket, 5) == SOCKET_ERROR)
		return -4;

	for (;;)
	{
		int nAddrSize = sizeof(sockaddr_in);
		sockaddr_in newSockaddr;
		SOCKET acceptSocket;		
		acceptSocket = accept(listenSocket, (sockaddr*)&newSockaddr, &nAddrSize);
		if (acceptSocket == INVALID_SOCKET)
		{
			continue;
		}
		else
		{
			char szIp[64] = { 0 };
			inet_ntop(AF_INET, &newSockaddr.sin_addr.S_un.S_addr, &(szIp[0]), 64);

			string port;
			port = to_string(newSockaddr.sin_port);
			
			CConnection* pConnection = new CConnection(acceptSocket, string(szIp), port);
			pServer->Attach(pConnection);
		}
	}
	return 0;
}

DWORD CServer::ProcessPacketThread(LPVOID lp)
{
	CServer* pServer = (CServer*)lp;
	if (!pServer)
		return -1;

	DWORD dwReturn = -1;
	while (1)
	{
		if (pServer->m_bExitProcessThread)
			break;

		DWORD dwReturn = WaitForSingleObject(pServer->m_hNewPacketEvent, 300 * 1000);
		switch (dwReturn)
		{
		case WAIT_OBJECT_0:
		{
			while (pServer->m_lstPacket.size() > 0)
			{
				CPacket packet = *(pServer->m_lstPacket.begin());
				pServer->m_lstPacket.pop_front();
				pServer->Notify(&packet);
			}
		}
		break;
		case WAIT_TIMEOUT:
		default:
			break;
		}
	}

	return dwReturn;
}

void CServer::End()
{
	if (m_hListenThread)
	{
		closesocket(m_sokcet);
		m_sokcet = NULL;

		WSACleanup();

		TerminateThread(m_hListenThread, 1204);
		CloseHandle(m_hListenThread);
		m_hListenThread = NULL;
	}
	if (m_hProcessThread)
	{
		m_bExitProcessThread = true;
		SetEvent(m_hNewPacketEvent);
		CloseHandle(m_hNewPacketEvent);
		m_hNewPacketEvent = NULL;
		TerminateThread(m_hProcessThread, 1204);
		CloseHandle(m_hProcessThread);
		m_hProcessThread = NULL;
	}
	DeleteCriticalSection(&m_csListPacket);
}

void CServer::PushPacket(CPacket& packet)
{
	EnterCriticalSection(&m_csListPacket);
	{
		CPacket newPacket(packet);
		m_lstPacket.push_back(newPacket);
	}
	LeaveCriticalSection(&m_csListPacket);

	SetEvent(m_hNewPacketEvent);
}

CServer* CServer::Instance()
{
	if (nullptr == _instance)
	{
		_instance = new CServer();
	}
	return _instance;
}