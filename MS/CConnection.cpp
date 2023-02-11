//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CConnection.h"
#include "CServer.h"
#include "..\Common\CLog.h"
#include "CConfig.h"

CConnection::CConnection(SOCKET socket, string ip, string port)
{
	m_socket = socket;
	m_ip = ip;
	m_nPort = port;

	DWORD dwThreadID;
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Recv, (LPVOID)this, 0, &dwThreadID);
}

CConnection::~CConnection()
{
	CloseHandle(m_hThread);
}

DWORD CConnection::Recv(LPVOID lp)
{
	CConnection* pConnection = (CConnection*)lp;
	if (!pConnection)
		return -1;

	for (;;)
	{						
		const int SIZE = 1024;
		char recvbuff[SIZE] = { 0 };
		int len = 0;
		len = recv(pConnection->m_socket, &recvbuff[0], SIZE, 0);
		if (len > 0)
		{
			// success
			// RIGISTER#Alice or BYE#
			// @bob hello or @all hello
			string strbuff(recvbuff);
			if (string::npos != strbuff.find("#"))
			{
				string strbuff(recvbuff);
				if (strbuff.find("REGISTER") == 0) // REGISTER
				{
					// RIGISTER#Alice
					size_t pos1 = strbuff.find("#", 0);
					pConnection->m_strUser = strbuff.substr(++pos1, strbuff.length() - pos1);
				}
				else //BYE
				{					
					CServer::Instance()->Detach(pConnection);
				}
			}
			else
			{
				CPacket packet;
				packet.FromBinary(recvbuff, len);
				CServer::Instance()->PushPacket(packet);
			}

			CLog log(CConfig::Instance()->GetNetworkLogFile());
			log.Write("[RECV]----%s\n", recvbuff);
		}
		else if (0 == len)
		{
			// terminate			
			return -1;
		}
		else
		{
			// fail
			return -1;
		}
	}

	return 0;
}

void CConnection::Send(CPacket& packet)
{
	const int SIZE = 1024;
	char sendbuff[SIZE] = { 0 };

	int size = 0;
	packet.ToBinary((char*)&(sendbuff[0]), &size);

	int len = 0;
	len = send(m_socket, &sendbuff[0], size, 0);
	if (len > 0)
	{
		// success
		CLog log(CConfig::Instance()->GetNetworkLogFile());
		log.Write("[SEND]----%s\n", sendbuff);
	}
	else if (0 == len)
	{
		// terminate
	}
	else
	{
		// fail
	}
}

void CConnection::Update(CPacket* ptmp)
{
	CPacket packet(*ptmp);
	if ((packet.m_strDest == m_strUser) ||
		(packet.m_strSrc != m_strUser && packet.m_strDest == "all"))
	{
		Send(packet);
	}
}
