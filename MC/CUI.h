//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include <string>
using namespace std;
//����ʾ�ӿڶ�������������δ��ת�Ƶ�GUI��������ʾ�ӿ�
class CUI
{
public:	
	CUI() {}
	~CUI() {}
	
	void ConsolePrint(bool bOnline, string strDisplay);
	void ConsoleEcho(string strDisplay, bool bRecv = true);
};

