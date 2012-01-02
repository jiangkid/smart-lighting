#include "StdAfx.h"
#include "RoadRecordset.h"

CRoadRecordset::CRoadRecordset(void)
{
}

CRoadRecordset::~CRoadRecordset(void)
{
}

//
//����·���ű��е���������
//
BOOL CRoadRecordset::LoadRoadData()
{
	return Open("Select * From Roads");
}


//
//����·������
//
BOOL CRoadRecordset::SetRoadName(CString RoadID,CString RoadName)
{
	CString SQL;
	SQL.Format("Select * From  Roads Where [RoadID] = '%s'",RoadID);
	if (Open(SQL))
	{
		SetAsString("Name",RoadName);
		return TRUE;
	}
	return FALSE;
}

//
//����ID���鿴·������
//
CString CRoadRecordset::GetRoadName(CString RoadID)
{
	CString SQL;
	CString RoadName;
	SQL.Format("Select * From  Roads Where [RoadID] = '%s'",RoadID);
	if (Open(SQL))
	{
		RoadName=GetAsString("Name");
		return RoadName;
	}
	return NULL;
}