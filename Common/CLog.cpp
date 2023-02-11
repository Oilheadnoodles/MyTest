//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CLog.h"
#include "stdarg.h"
#include "windows.h"
#include <cstdio>

void CLog::Write(const char* format, ...)
{
	FILE* logfile;
	if (!fopen_s(&logfile, m_strLogfile.c_str(), "a"))
	{
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);

		char buf[64] = { 0 };
		sprintf_s(buf, "%04d%02d%02d %02d:%02d:%02d:%03d  ",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		va_list argptr;
		va_start(argptr, buf);
		vfprintf_s(logfile, buf, argptr);
		va_end(argptr);
		
		va_start(argptr, format);
		vfprintf_s(logfile, format, argptr);
		va_end(argptr);

		fclose(logfile);
		logfile = NULL;
	}
}