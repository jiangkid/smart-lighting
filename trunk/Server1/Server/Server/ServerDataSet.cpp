#include "StdAfx.h"
#include "ServerDataSet.h"

CServerDataSet _ServerDataSet;
CServerDataSet* g_pServerDataSet = &_ServerDataSet;

CServerDataSet::CServerDataSet(void)
{
}

CServerDataSet::~CServerDataSet(void)
{
}

CString CServerDataSet::GetUserName()
{
	return GetAsString("User");
}
