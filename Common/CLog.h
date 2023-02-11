//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include "stdarg.h"
#include <string>
using namespace std;

class CLog
{
public:
	CLog(string strLogfile) { m_strLogfile = strLogfile; }
	~CLog() {}

	void Write(const char* format, ...);	

private:
	string m_strLogfile;
};