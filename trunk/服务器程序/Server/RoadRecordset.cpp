#include "StdAfx.h"
#include "RoadRecordset.h"

CRoadRecordset::CRoadRecordset(void)
{
}

CRoadRecordset::~CRoadRecordset(void)
{
}

//
//加载路这张表中的所有数据
//
BOOL CRoadRecordset::LoadRoadData()
{
	return Open("Select * From Roads");
}


//
//设置路的名称
//
BOOL CRoadRecordset::SetRoadName(int RoadID,CString RoadName)
{
	CString SQL;
	SQL.Format("Select * From  Roads Where [ID] = %d",RoadID);
	if (Open(SQL))
	{
		SetAsString("Name",RoadName);
		return TRUE;
	}
	return FALSE;
}

//
//根据ID来查看路的名称
//
CString CRoadRecordset::GetRoadName(int RoadID)
{
	CString SQL;
	CString RoadName;
	SQL.Format("Select * From  Roads Where [ID] = %d",RoadID);
	if (Open(SQL))
	{
		RoadName=GetAsString("Name");
		return RoadName;
	}
	return NULL;
}