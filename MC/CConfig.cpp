//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CConfig.h"

CConfig* CConfig::_instance = nullptr;
CConfig::GC CConfig::gc;

CConfig::CConfig()
{
	_instance = nullptr;
}

CConfig::~CConfig()
{
}

CConfig* CConfig::Instance()
{
	if (nullptr == _instance)
	{
		_instance = new CConfig();
	}
	return _instance;
}
