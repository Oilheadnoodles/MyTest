//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CCommand.h"
#include "CClient.h"
#include "CConfig.h"

CCommand::CCommand()
{
	m_pCurrentState = new COfflineState();
	m_pClient = nullptr;

	m_pUI = new CUI();	
}

CCommand::~CCommand()
{
	if (m_pCurrentState)
	{
		delete m_pCurrentState;
		m_pCurrentState = nullptr;
	}
	if (m_pClient)
	{
		delete m_pClient;
		m_pClient = nullptr;
	}
	if (m_pUI)
	{
		delete m_pUI;
		m_pUI = nullptr;
	}
}

void CCommand::Handle(string strCommand)
{
	m_pCurrentState->Handle(this, strCommand);
}

void CCommand::ChangeState(CState* pState)
{
	if (nullptr == pState)
		return;

	if (m_pCurrentState)
	{
		delete m_pCurrentState;
		m_pCurrentState = nullptr;
	}
	m_pCurrentState = pState;
}

void COfflineState::Handle(CCommand* pCommand, string strCommand)
{
	// HI,QUIT
	// HI 127.0.0.1:12345 
	if (!strCommand.substr(0,2).compare("HI"))
	{
		string addr = strCommand.substr(3, strCommand.length() - 3);
		CConfig::Instance()->SetServer(addr);
		CClient* pClient = pCommand->GetClient();
		if (nullptr == pClient)
		{
			if (string::npos == strCommand.find(":\\", 0))
			{
				pClient = new CClient(new CTcp);
			}
			else
			{
				pClient = new CClient(new CPipe);
			}
			pCommand->SetClient(pClient);
		}
		if (pClient->Connect(addr))
		{
			if (pClient->IsOnline())
			{
				CConfig::Instance()->SetServer(addr);
				pCommand->ChangeState(new COnlineState);

				pClient->Recv();

				//将来此处可以扩充注册消息处理，BTW时间有限，未加心跳处理
				// REGISTER:alice
				string reg = "REGISTER#" + CConfig::Instance()->GetUser();
				pClient->Send(reg);
			}
		}

		pCommand->GetUI()->ConsolePrint(true, "");
	}
	else if (!strCommand.substr(0, 4).compare("QUIT"))
	{
		// cancel Console
		exit(0);
	}
	/*
	else if (!strCommand.substr(0, 4).compare("test"))
	{
		pCommand->ChangeState(new COnlineState);
		pCommand->GetUI()->ConsolePrint(true, "");
	}*/
	else
	{
		// print
		pCommand->GetUI()->ConsolePrint(false, "");
	}
}

void COnlineState::Handle(CCommand* pCommand, string strCommand)
{
	// @name, @all ...
	if (!strCommand.compare("BYE"))
	{
		CClient* pClient = pCommand->GetClient();
		if (nullptr != pClient)
		{
			// REGISTER:alice
			string reg = "BYE#";
			pClient->Send(reg);

			pClient->Disconnect();
		}
		pCommand->ChangeState(new COfflineState);

		// print
		pCommand->GetUI()->ConsolePrint(false, "");
	}	
	else
	{		
		if (strCommand.find("@") == 0)
		{
			CClient* pClient = pCommand->GetClient();
			if (nullptr != pClient)
			{
				// @bob hi,i am alice
				// @src@dst@detail
				// source#destination#detail
				string strbuff(strCommand),strDst,strSrc,strDetail;
				size_t pos = strbuff.find(" ", 0);				
				strDst = strbuff.substr(1, pos - 1);
				strDetail = strbuff.substr(++pos, strbuff.length());
				strSrc = CConfig::Instance()->GetUser();
				string strSend;
				strSend = "@" + strSrc + "@" + strDst + "@" + strDetail;
				pClient->Send(strSend);
			}
		}

		// print
		pCommand->GetUI()->ConsolePrint(true, strCommand);
	}
}