#include "StdAfx.h"
#include "LigntCommand.h"

CLigntCommand::CLigntCommand(void)
{
}

CLigntCommand::~CLigntCommand(void)
{
}

BOOL CLigntCommand:: AddNewInformation(CString  x, CString temp)
{
	CString SQL;
	SQL.Format("Insert Into Lights([ID],[Name])Values(\""+x+"\",\""+temp+"\")");
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}