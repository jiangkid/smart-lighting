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
		SetAsString("LightName",LightName);
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

/************************************************************************
功能:返回所有灯的名称、ID及其所属灯的ID和灯的数目
返回格式:0x00-0xFF<灯1的名称>{灯1的ID}{所属路的ID}......<灯n的名称>{灯n的ID}{所属路的ID}#
	 /************************************************************************/

	 CString CLightRecordset::GetAllLightsAndCount()
   {
	   CString lightSQL;
	   CString roadSQL;
	   CString allLights;
	   _RecordsetPtr pLightRs;
	   _RecordsetPtr pRoadRs;
	   _variant_t vLightName;
	   _variant_t vLightID;
	   _variant_t vIDRoad;
	   _variant_t vRoadID;
	   CString strRoadID;
	   int intIDRoad=0;
	   char count=0x00;    //记录数目

	   //打开Lights表
	   lightSQL="Select * From Lights";
	   try
	   {
		   pLightRs.CreateInstance("ADODB.Recordset");
		   pLightRs->Open((_variant_t)lightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	   }
	   catch(_com_error&e)
	   {
		   AfxMessageBox(e.Description());
	   }

	   while (!pLightRs->adoEOF)
	   {
		   vLightName=pLightRs->GetCollect("LightName");   //LightName字段的值
		   if ((vLightName.vt != VT_NULL)&&(vLightName.vt != VT_EMPTY))
		   {
			   allLights+="<";
			   allLights+=(LPCTSTR)(_bstr_t)vLightName;
			   allLights+=">";
		   }

		   vLightID=pLightRs->GetCollect("LightID");     //LightID字段的值
		   if ((vLightID.vt != VT_NULL)&&(vLightID.vt != VT_EMPTY))
		   {
			   allLights+="{";
			   allLights+=(LPCTSTR)(_bstr_t)vRoadID;
			   allLights+="}";
		   }

		   vIDRoad=pLightRs->GetCollect("IDRoad");         //IDRoad字段的值
		   if(vIDRoad.vt!=VT_NULL)
		   {
			   intIDRoad=vIDRoad.intVal;
		   } 

		   //取得Roads表中ID为与Road关联的IDRoad的记录集
		   roadSQL.Format("Select * From Roads Where [ID] = %d",intIDRoad);
		   try
		   {
			   pRoadRs.CreateInstance("ADODB.Recordset");
			   pRoadRs->Open((_variant_t)roadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
		   }
		   catch(_com_error&e)
		   {
			   AfxMessageBox(e.Description());
		   }

		   if(!pRoadRs->adoEOF)
		   {
			   vRoadID=pRoadRs->GetCollect("RoadID");   //TerminalID字段的值
			   if (vRoadID.vt!=VT_NULL)
			   {
				   allLights+="(";
				   allLights+=(LPCTSTR)(_bstr_t)vRoadID;
				   allLights+=")";
			   }
		   }
		   count++;
		   pLightRs->MoveNext();
	   }
	   pLightRs->Close();
	   pRoadRs->Close();
	   allLights=count+allLights;
	   allLights+="#";
	   return allLights;
   }