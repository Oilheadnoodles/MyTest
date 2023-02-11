//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CUI.h"
#include <iostream>
#include "CConfig.h"
#include "windows.h"

void CUI::ConsolePrint(bool bOnline, string strDisplay)
{
	if (!bOnline)
	{
		// offline
		cout << CConfig::Instance()->GetUser() << ">";
	}
	else
	{
		// online
		cout << CConfig::Instance()->GetServer() << "|" << CConfig::Instance()->GetUser() << ">";
	}
}

void CUI::ConsoleEcho(string strDisplay, bool bRecv)
{
	// 2021/06/04 15:35:40 tom: hi alice, I'm to
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	char buf[64] = { 0 };
	sprintf_s(buf, "%04d/%02d/%02d %02d:%02d:%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	if (bRecv)
	{		
		cout << "\n" << buf << "  " << strDisplay << endl;
	}
	else
	{
		cout << "\r" << buf << "  " << strDisplay << endl;
	}
}