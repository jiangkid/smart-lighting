#include "StdAfx.h"
#include "RoadCommand.h"

CRoadCommand::CRoadCommand(void)
{
}

CRoadCommand::~CRoadCommand(void)
{
}

BOOL CRoadCommand:: AddNewInformation(CString x, CString temp)
{
	CString SQL;
	SQL.Format("Insert Into Roads([ID],[Name])Values(\""+x+"\",\""+temp+"\")");
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}