#include "StdAfx.h"
#include "TerminalRecordset.h"

CTerminalRecordset::CTerminalRecordset(void)
{
}

CTerminalRecordset::~CTerminalRecordset(void)
{
}

//
//加载Terminal表中所有的数据
//
BOOL CTerminalRecordset::LoadTerminalData()
{
	//使用Open方法加载所有数据
	return Open("Select * From Terminals");
}

//
//设置终端的ID来设置终端的名称,成功时返回TRUE，失败时返回FALSE
//
BOOL CTerminalRecordset::SetTerminalName(int TID,CString TerminalName)
{
	CString SQL;
	SQL.Format("Select * From  Terminals Where [TID] = %d",TID);
	if (Open(SQL))
	{
		SetAsString("Name",TerminalName);
		return TRUE;
	}
	return FALSE;
}

//
//根据ID查看路的名称,返回为路的名称
//
CString CTerminalRecordset::GetTerminalName(int TID)
{
	CString SQL;
	CString TerminalName;
	SQL.Format("Select * From  Terminals Where [TID] = %d",TID);
	if (Open(SQL))
	{
		TerminalName=GetAsString("Name");
		return TerminalName;
	}
	return NULL;
	
}


