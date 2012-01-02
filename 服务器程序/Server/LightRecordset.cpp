#include "StdAfx.h"
#include "LightRecordset.h"

CLightRecordset::CLightRecordset(void)
{
}

CLightRecordset::~CLightRecordset(void)
{
}


//
//返回灯的状态
//
CString CLightRecordset::GetLightStatus(CString LightID)
{
	CString SQL;
	SQL.Format("Select * From  Lights Where [LightID] = '%s'",LightID);
	if (Open(SQL))
	{
		if(GetAsString("Status")!="0")
		{
			return "ON";
		}
		else
		{
			return "OFF";
		}
	}
	return NULL;
}

//
//设置灯的状态
//
BOOL CLightRecordset::SetLightStatus(CString LightID,BOOL LightStatus)
{
	CString SQL;
	SQL.Format("Select * From  Lights Where [LightID] = '%s'",LightID);
	//在Access中,对于"是/否"类型的字段,-1表示"是",0表示"否"
	CString strLightStatus;
	if (LightStatus)
	{
		strLightStatus="-1";
	}
	else
	{
		strLightStatus="0";
	}
	if (Open(SQL))
	{
		SetAsString("Status",strLightStatus);
		return TRUE;
	}	
	else
		return FALSE;
}

//
//设置灯的名称
//
BOOL CLightRecordset::SetLightName(CString LightID,CString LightName)
{
	CString SQL;
	SQL.Format("Select * From  Lights Where [LightID] = '%s'",LightID);

	if (Open(SQL))
	{
		SetAsString("Name",LightName);
		return TRUE;
	}
	return FALSE;
}

//
//查看灯的名称
//
CString CLightRecordset::GetLightName(CString LightID)
{
	CString SQL;
	CString LightName;
	SQL.Format("Select * From  Lights Where [LightID] = '%s'",LightID);

	if (Open(SQL))
	{
		LightName=GetAsString("Name");
		return LightName;
	}
	return NULL;
}

//
//设置灯的组号
//
BOOL CLightRecordset::SetGoupNum(CString LightID, int GroupNum)
{
	CString SQL;
	CString strGroupNum;
	strGroupNum.Format("%d",GroupNum);
	SQL.Format("Select * From  Lights Where [ID] = '%s'",LightID);

	if (Open(SQL))
	{
		SetAsString("GroupNum",strGroupNum);
		return TRUE;
	}
	return FALSE;

}

//
//查看灯的组号
//
int CLightRecordset::GetGroupNum(CString LightID)
{
	CString SQL;
	CString strGroupNum;
	SQL.Format("Select * From  Lights Where [LightID] = '%s'",LightID);

	if (Open(SQL))
	{
		strGroupNum=GetAsString("GroupNum");
		return atoi(strGroupNum);
	}
	return NULL;
}