//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#pragma once
#include <cstddef>
#include <string>

using namespace std;

class CPacket
{
public:
	CPacket();
	CPacket(const CPacket& packet);
	~CPacket();

	void OutputDebug();
	bool FromBinary(char* pBuf, int nlen);
	bool ToBinary(char* pBuf, int* len);
	
	CPacket& operator=(const CPacket& right);

public:
	string m_strSrc;
	string m_strDest;
	string m_strDetail;
};
