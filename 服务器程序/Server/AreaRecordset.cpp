#include "StdAfx.h"
#include "AreaRecordset.h"

CAreaRecordset::CAreaRecordset(void)
{
}

CAreaRecordset::~CAreaRecordset(void)
{
}

//
//���������ID������������ƺ������µ�·
//
BOOL CAreaRecordset::SetAreaByAreaID(CString AreaID, CString AreaName, CString Terminals)
{
	CString SQL;
	SQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(SQL))
	{
		SetAsString("AreaName",AreaName);
		SetAsString("Roads",Terminals);
		return TRUE;
	}
	return FALSE;
}

//
//������������Ʋ鿴�����µ�· 
//
CString CAreaRecordset::GetTerminalsByAreaName(CString AreaName)
{
	CString SQL;
	CString Terminals;
	CString TerminalsName;
	CString TerminalsID;

	SQL.Format("Select * From Areas Where AreaName = '%s'",AreaName);
	if (Open(SQL))
	{
		Terminals=GetAsString("TerminalsName");
		return Terminals; 
	}
	return NULL;
}
