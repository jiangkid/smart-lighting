#include "StdAfx.h"
#include "LightRecordset.h"

CLightRecordset::CLightRecordset(void)
{
}

CLightRecordset::~CLightRecordset(void)
{
}


//
//���صƵ�״̬
//
CString CLightRecordset::GetLightStatus(int ID)
{
	CString SQL;
	SQL.Format("Select * From  Lights Where [ID] = %d",ID);
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
//���õƵ�״̬
//
BOOL CLightRecordset::SetLightStatus(int ID,BOOL LightStatus)
{
	CString SQL;
	SQL.Format("Select * From  Lights Where [ID] = %d",ID);
	//��Access��,����"��/��"���͵��ֶ�,-1��ʾ"��",0��ʾ"��"
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
//���õƵ�����
//
BOOL CLightRecordset::SetLightName(int ID,CString LightName)
{
	CString SQL;
	SQL.Format("Select * From  Lights Where [ID] = %d",ID);

	if (Open(SQL))
	{
		SetAsString("Name",LightName);
		return TRUE;
	}
	return FALSE;
}

//
//�鿴�Ƶ�����
//
CString CLightRecordset::GetLightName(int ID)
{
	CString SQL;
	CString LightName;
	SQL.Format("Select * From  Lights Where [ID] = %d",ID);

	if (Open(SQL))
	{
		LightName=GetAsString("Name");
		return LightName;
	}
	return NULL;
}

//
//���õƵ����
//
BOOL CLightRecordset::SetGoupNum(int ID, int GroupNum)
{
	CString SQL;
	CString strGroupNum;
	strGroupNum.Format("%d",GroupNum);
	SQL.Format("Select * From  Lights Where [ID] = %d",ID);

	if (Open(SQL))
	{
		SetAsString("GroupNum",strGroupNum);
		return TRUE;
	}
	return FALSE;

}

//
//�鿴�Ƶ����
//
int CLightRecordset::GetGroupNum(int ID)
{
	CString SQL;
	CString strGroupNum;
	SQL.Format("Select * From  Lights Where [ID] = %d",ID);

	if (Open(SQL))
	{
		strGroupNum=GetAsString("GroupNum");
		return atoi(strGroupNum);
	}
	return NULL;
}