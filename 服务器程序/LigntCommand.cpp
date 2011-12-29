#include "StdAfx.h"
#include "LightCommand.h"

CLightCommand::CLightCommand(void)
{
}

CLightCommand::~CLightCommand(void)
{
}

BOOL CLightCommand:: AddNewInformation(CString  x, CString temp)
{
	CString SQL;
	SQL.Format("Insert Into Lights([ID],[Name])Values(\""+x+"\",\""+temp+"\")");
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}