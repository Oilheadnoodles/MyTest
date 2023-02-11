//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include "CClient.h"
#include "CUI.h"
#include <string>
using namespace std;

//状态机，用于不同命令模式的对应处理。
class CCommand;
class CState
{
public:
	CState() {}
	~CState() {}
	virtual void Handle(CCommand* pCommand, string strCommand) = 0;
};

class CCommand
{
public:
	CCommand();
	~CCommand();
	void SetClient(CClient* pClient) { m_pClient = pClient; }
	CClient* GetClient() { return m_pClient; }
	CUI* GetUI() { return m_pUI; }
	
public:
	void Handle(string strCommand);
	void ChangeState(CState* pState);
	
private:
	CState* m_pCurrentState;
	CClient* m_pClient;
	CUI* m_pUI;
};

class COfflineState : public CState
{
public:
	COfflineState() {}
	~COfflineState() {}
	virtual void Handle(CCommand* pCommand, string strCommand);
};

class COnlineState : public CState
{
public:
	COnlineState() {}
	~COnlineState() {}
	virtual void Handle(CCommand* pCommand, string strCommand);
};
