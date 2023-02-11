//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <Ws2tcpip.h>
#include <string>
using namespace std;

//ʹ���˾�����ģʽ�����ڿ��ǵ�δ����ʹ����������ͨѶЭ�飬����Pipe�ȡ�
class CCommStrategy
{
public:
	CCommStrategy() { m_bConnected = false; }
	virtual ~CCommStrategy() {}

	virtual bool Connect(string addr) = 0;
	virtual void Send(string& buff) = 0;
	virtual void Recv() = 0;
	virtual void Disconnect() = 0;

	bool IsOnline() { return m_bConnected; }

protected:
	bool m_bConnected;
};

class CClient
{
public:	
	CClient(CCommStrategy* commStrategy);
	~CClient();

	bool Connect(string addr);
	void Send(string& buff);
	void Recv();
	void Disconnect();
	bool IsOnline();

private:
	CCommStrategy* m_pCommStrategy;	
};

class CTcp : public CCommStrategy
{
public:
	CTcp();
	virtual ~CTcp();
	
	virtual bool Connect(string addr);
	virtual void Send(string& buff);
	virtual void Recv();
	virtual void Disconnect();	

protected:
	static DWORD RecvThread(LPVOID lp);

private:
	HANDLE m_hThread;
	SOCKET m_sokcet;
};

class CPipe : public CCommStrategy
{
public:
	CPipe();
	virtual ~CPipe();

	virtual bool Connect(string addr);
	virtual void Send(string& buff);
	virtual void Recv();
	virtual void Disconnect();
};