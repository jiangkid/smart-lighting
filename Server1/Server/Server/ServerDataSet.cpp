#include "StdAfx.h"
#include "ServerDataSet.h"

CServerDataSet::CServerDataSet(void)
{
}

CServerDataSet::~CServerDataSet(void)
{
}

CString CServerDataSet::GetUserName()
{
	return GetAsString("User")
}
