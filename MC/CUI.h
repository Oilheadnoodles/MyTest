//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include <string>
using namespace std;
//将显示接口独立出来，方便未来转移到GUI等其他显示接口
class CUI
{
public:	
	CUI() {}
	~CUI() {}
	
	void ConsolePrint(bool bOnline, string strDisplay);
	void ConsoleEcho(string strDisplay, bool bRecv = true);
};

