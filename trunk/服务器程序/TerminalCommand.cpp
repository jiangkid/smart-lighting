#include "StdAfx.h"
#include "TerminalCommand.h"

CTerminalCommand::CTerminalCommand(void)
{
}

CTerminalCommand::~CTerminalCommand(void)
{
}
//�½�
//
BOOL CTerminalCommand:: AddNewInformation(CString x, CString temp)
{
	CString SQL;
	SQL.Format("Insert Into Terminals([TID],[Name])Values(\""+x+"\",\""+temp+"\")");
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE; 
}