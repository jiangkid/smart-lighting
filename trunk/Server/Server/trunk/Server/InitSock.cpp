#include "StdAfx.h"
#include "InitSock.h"

CInitSock::CInitSock()
{
		WSADATA wsaData;
		BYTE minorVer =2;
		BYTE majorVer =2;
		WORD sockVersion = MAKEWORD(minorVer,majorVer);

		if (::WSAStartup(sockVersion,&wsaData)!=0)
		{
			exit(0);
		}
}

CInitSock::~CInitSock(void)
{
	::WSACleanup();
}
