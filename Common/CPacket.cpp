//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
#include "CPacket.h"
#include "CLog.h"

CPacket::CPacket()
{
	m_strSrc = "";
	m_strDest = "";
	m_strDetail = "";
}

CPacket::CPacket(const CPacket& packet)
{
	m_strSrc = packet.m_strSrc;
	m_strDest = packet.m_strDest;
	m_strDetail = packet.m_strDetail;
}

CPacket::~CPacket()
{
}

bool CPacket::FromBinary(char* pBuf, int nlen)
{
	if (!pBuf)
		return false;

	// source#destination#detail
	// @bob hi,i am alice
	// @src@dst@detail	
	string strbuff(pBuf);
	size_t pos1 = strbuff.find("@", 1);
	size_t pos2 = strbuff.find("@", pos1 + 1);
	m_strSrc = strbuff.substr(1, pos1 - 1);
	m_strDest = strbuff.substr(pos1 + 1, pos2 - pos1 -1);
	m_strDetail = strbuff.substr(pos2 + 1, strbuff.length() - pos2 - 1);

	return true;
}

bool CPacket::ToBinary(char* pBuf, int* len)
{
	if (!pBuf)
		return false;

	// source#destination#detail
	// @bob hi,i am alice
	// @src@dst@detail	
	string strtmp = "@" + m_strSrc + "@" + m_strDest + "@" + m_strDetail;
	*len = strtmp.length();
	if (*len > 1024)
	{
		*len = 1024;
	}
	strcpy_s(pBuf, 1024, strtmp.c_str());

	return true;
}

CPacket& CPacket::operator=(const CPacket& right)
{
	m_strSrc = right.m_strSrc;
	m_strDest = right.m_strDetail;
	m_strDetail = right.m_strDetail;

	return *this;
}

void CPacket::OutputDebug()
{
	CLog log("d:\\packet.txt");
	log.Write("Packet----source:%s,destidation:%sdetail=%s\n", m_strSrc, m_strDest, m_strDetail);
}
