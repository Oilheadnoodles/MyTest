//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CUI.h"
#include "CConfig.h"
#include "..\Common\CPacket.h"
#include "CClient.h"
#pragma comment(lib,"ws2_32.lib")

CClient::CClient(CCommStrategy* commStrategy)
{
	m_pCommStrategy = commStrategy;
}

CClient::~CClient()
{
	if (m_pCommStrategy)
	{
		delete m_pCommStrategy;
	}
}

bool CClient::Connect(string addr)
{
	return m_pCommStrategy->Connect(addr);
}

void CClient::Send(string& buff)
{
	return m_pCommStrategy->Send(buff);
}

void CClient::Recv()
{
	return m_pCommStrategy->Recv();
}

void CClient::Disconnect()
{
	return m_pCommStrategy->Disconnect();
}

bool CClient::IsOnline()
{
	return m_pCommStrategy->IsOnline();
}

CTcp::CTcp()
{
	m_hThread = NULL;
	m_sokcet = NULL;
}

CTcp::~CTcp()
{
	if (m_hThread)
	{
		closesocket(m_sokcet);
		m_sokcet = NULL;

		WSACleanup();

		TerminateThread(m_hThread, 1204);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	m_bConnected = false;
}

bool CTcp::Connect(string addr)
{
	if (m_bConnected)
		return true;

	size_t pos = addr.find(":");
	if (string::npos == pos)
		return false;

	string ip, port;
	ip = addr.substr(0, pos);
	port = addr.substr(++pos, addr.length() - pos);

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return false;
		
	m_sokcet = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sokcet == INVALID_SOCKET)
		return false;

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	setsockopt(m_sokcet, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, bOptLen);

	sockaddr_in serverSockaddr;
	memset(&serverSockaddr, 0, sizeof(sockaddr_in));
	serverSockaddr.sin_family = AF_INET;
	serverSockaddr.sin_port = htons((u_short)atoi(port.c_str()));
	if (inet_pton(AF_INET, ip.c_str(), &serverSockaddr.sin_addr) <= 0)
	{
		return false;
	}

	if (connect(m_sokcet, (struct sockaddr *)&serverSockaddr, sizeof(sockaddr_in)) < 0)
	{
		return false;
	}

	m_bConnected = true;

	return true;
}

void CTcp::Send(string& buff)
{
	const int SIZE = 1024;
	char sendbuff[SIZE] = { 0 };
	strcpy_s(sendbuff, SIZE, buff.c_str());
	int size = buff.length();
	int len = 0;
	len = send(m_sokcet, &sendbuff[0], size, 0);
	if (len > 0)
	{
		// success
		// RIGISTER#Alice or BYE#
	    // @bob hello or @all hello
		if (string::npos == buff.find("#"))
		{
			size_t pos = string(sendbuff).find("@");
			if (string::npos != pos)
			{
				CPacket packet;
				packet.FromBinary(sendbuff, len);

				CUI console;
				if ("all" == packet.m_strDest)
				{
					string broadcast;
					broadcast = "broadcast from " + packet.m_strSrc + ": " + packet.m_strDetail;
					console.ConsoleEcho(broadcast, false);
				}
				else
				{
					console.ConsoleEcho(packet.m_strSrc + ": " + packet.m_strDetail, false);
				}
			}
		}
	}
	else if (0 == len)
	{
		// terminate
		m_bConnected = false;
	}
	else
	{
		// fail
		m_bConnected = false;
	}
}

void CTcp::Recv()
{
	if (!m_sokcet)
		return;

	DWORD dwThreadID;
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvThread, (LPVOID)this, 0, &dwThreadID);
}

void CTcp::Disconnect()
{
	if (m_hThread)
	{
		closesocket(m_sokcet);
		m_sokcet = NULL;

		WSACleanup();

		TerminateThread(m_hThread, 1204);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	m_bConnected = false;	
}

DWORD CTcp::RecvThread(LPVOID lp)
{
	CTcp* pTcp = (CTcp*)lp;
	if (!pTcp)
		return -1;

	for (;;)
	{
		const int SIZE = 1024;
		char recvbuff[SIZE] = { 0 };
		int len = 0;
		len = recv(pTcp->m_sokcet, &recvbuff[0], SIZE, 0);
		if (len > 0)
		{
			// success
			CPacket packet;
			packet.FromBinary(recvbuff, len);
			string strbuff(recvbuff);
			size_t pos = strbuff.find("@");
			if (string::npos != pos)
			{
				CUI console;
				if ("all" == packet.m_strDest)
				{
					string broadcast;
					broadcast = "broadcast from " + packet.m_strSrc + ": " + packet.m_strDetail;
					console.ConsoleEcho(broadcast, true);
				}
				else
				{
					console.ConsoleEcho(packet.m_strSrc + ": " + packet.m_strDetail, true);
				}
				console.ConsolePrint(true, "");
			}
		}
		else if (0 == len)
		{
			// terminate			
			pTcp->m_bConnected = false;
			return -1;
		}
		else
		{
			// fail
			pTcp->m_bConnected = false;
			return -1;
		}
	}
}

CPipe::CPipe()
{

}

CPipe::~CPipe()
{

}

bool CPipe::Connect(string addr)
{
	return false;
}

void CPipe::Send(string& buff)
{

}

void CPipe::Recv()
{

}

void CPipe::Disconnect()
{

}