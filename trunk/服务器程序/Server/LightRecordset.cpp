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
//���õƵ�״̬
//
BOOL CLightRecordset::SetLightStatus(CString LightID,BOOL LightStatus)
{
	CString SQL;
	SQL.Format("Select * From  Lights Where [LightID] = '%s'",LightID);
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
//�鿴�Ƶ�����
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
//���õƵ����
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
//�鿴�Ƶ����
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
����:�������еƵ����ơ�ID���������Ƶ�ID�͵Ƶ���Ŀ
���ظ�ʽ:0x00-0xFF<��1������>{��1��ID}{����·��ID}......<��n������>{��n��ID}{����·��ID}#
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
	   char count=0x00;    //��¼��Ŀ

	   //��Lights��
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
		   vLightName=pLightRs->GetCollect("LightName");   //LightName�ֶε�ֵ
		   if ((vLightName.vt != VT_NULL)&&(vLightName.vt != VT_EMPTY))
		   {
			   allLights+="<";
			   allLights+=(LPCTSTR)(_bstr_t)vLightName;
			   allLights+=">";
		   }

		   vLightID=pLightRs->GetCollect("LightID");     //LightID�ֶε�ֵ
		   if ((vLightID.vt != VT_NULL)&&(vLightID.vt != VT_EMPTY))
		   {
			   allLights+="{";
			   allLights+=(LPCTSTR)(_bstr_t)vRoadID;
			   allLights+="}";
		   }

		   vIDRoad=pLightRs->GetCollect("IDRoad");         //IDRoad�ֶε�ֵ
		   if(vIDRoad.vt!=VT_NULL)
		   {
			   intIDRoad=vIDRoad.intVal;
		   } 

		   //ȡ��Roads����IDΪ��Road������IDRoad�ļ�¼��
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
			   vRoadID=pRoadRs->GetCollect("RoadID");   //TerminalID�ֶε�ֵ
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