#include "StdAfx.h"
#include "AreaRecordset.h"

CAreaRecordset::CAreaRecordset(void)
{
}

CAreaRecordset::~CAreaRecordset(void)
{
}

//
//根据区域的ID设置区域的名称和区域下的路
//
BOOL CAreaRecordset::SetAreaByAreaID(CString AreaID, CString AreaName, CString Roads)
{
	CString SQL;
	SQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(SQL))
	{
		SetAsString("AreaName",AreaName);
		SetAsString("Roads",Roads);
		return TRUE;
	}
	return FALSE;
}

//
//根据区域的名称查看区域下的路 
//
CString CAreaRecordset::GetRoadsByAreaName(CString AreaName)
{
	CString SQL;
	CString Roads;

	SQL.Format("Select * From AreaName Where Name = '%s'",AreaName);
	if (Open(SQL))
	{
		Roads=GetAsString("Roads");
		return Roads; 
	}
	return NULL;
}
