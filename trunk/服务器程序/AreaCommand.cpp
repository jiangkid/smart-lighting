#include "StdAfx.h"
#include "AreaCommand.h"

CAreaCommand::CAreaCommand(void)
{
}

CAreaCommand::~CAreaCommand(void)
{
}

//�½�
//
BOOL CAreaCommand:: AddNewInformation(CString x, CString temp)
{
	CString SQL;
	SQL.Format("Insert Into Areas([AreaID],[AreaName])Values(\""+x+"\",\""+temp+"\")");
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}