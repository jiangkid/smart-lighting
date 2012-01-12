#include "StdAfx.h"
#include "RoadRecordset.h"

CRoadRecordset::CRoadRecordset(void)
{
}

CRoadRecordset::~CRoadRecordset(void)
{
}

//����·���ű��е���������
BOOL CRoadRecordset::LoadRoadData()
{
	return Open("Select * From Roads");
}

//����·������
BOOL CRoadRecordset::SetRoadName(CString RoadID,CString RoadName)
{
	CString SQL;
	SQL.Format("Select * From  Roads Where [RoadID] = '%s'",RoadID);
	if (Open(SQL))
	{
		SetAsString("RoadName",RoadName);
		return TRUE;
	}
	return FALSE;
}


//����ID���鿴·������

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

//����·���õ�����·�����еƵ�����
//
CString CRoadRecordset::GetLightNamesByRoadName(CString RoadName)
{
	CString RoadSQL;
	CString LightSQL;
	CString strID;
	CString strLights;
	int intID;
	_variant_t vLightName;
	_RecordsetPtr pRs;

	RoadSQL.Format("Select * From Roads Where [RoadName]='%s'",RoadName);
	if (Open(RoadSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	LightSQL.Format("Select * From Lights Where IDRoad=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)LightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vLightName=pRs->GetCollect("LightName");
		if (vLightName.vt!=VT_NULL)
		{
			strLights+="+";
			strLights+=(LPCTSTR)(_bstr_t)vLightName;
		}
		pRs->MoveNext();
	}
	return strLights;
	pRs->Close();
}

//����·���õ�����·�����еƵ�ID
//
CString CRoadRecordset::GetLightIDsByRoadName(CString RoadName)
{
	CString RoadSQL;
	CString LightSQL;
	CString strID;
	CString strLights;
	int intID;
	_variant_t vLightID;
	_RecordsetPtr pRs;

	RoadSQL.Format("Select * From Roads Where [RoadName]='%s'",RoadName);
	if (Open(RoadSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	LightSQL.Format("Select * From Lights Where IDRoad=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)LightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vLightID=pRs->GetCollect("LightID");
		if (vLightID.vt!=VT_NULL)
		{
			strLights+="+";
			strLights+=(LPCTSTR)(_bstr_t)vLightID;
		}
		pRs->MoveNext();
	}
	return strLights;
	pRs->Close();

}

//����·��ID��õƵ�ID������Ŀ,��ʽΪ(count<LightID1><LightID2><LightID3>....<LightIDn>#)

CString CRoadRecordset::GetLightIDsAndCountByRoadID(CString roadID)
{
	CString roadSQL;
	CString lightSQL;
	CString strID;
	CString strLightIDs;
	char count=0x00;
	//CString strCount;
	int intID;
	_variant_t vLightID;
	_RecordsetPtr pRs;

	roadSQL.Format("Select * From Roads Where [RoadID]='%s'",roadID);
	if (Open(roadSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	lightSQL.Format("Select * From Lights Where IDRoad=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)lightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vLightID=pRs->GetCollect("LightID");
		if (vLightID.vt!=VT_NULL)
		{
			//strLightIDs+="<";
			strLightIDs+=(LPCTSTR)(_bstr_t)vLightID;
			//strLightIDs+=">";
		}
		count++;
		pRs->MoveNext();
	}
	//strCount.Format("%d",count);
	strLightIDs=count+strLightIDs;
	strLightIDs+="#";
	return strLightIDs;
	pRs->Close();
}

/************************************************************************/
/* ����:��������·�����ơ�ID���������ն˵�ID��·����Ŀ
���ظ�ʽ:0x00-0xFF<·1������>{·1��ID}{�����ն˵�ID}......<·n������>{·n��ID}{�����ն˵�ID}#*/
/************************************************************************/

CString CRoadRecordset::GetAllRoadsAndCount()
{
	CString roadSQL;
	CString terminalSQL;
	CString allRoads;
	_RecordsetPtr pRoadRs;
	_RecordsetPtr pTerminalRs;
	_variant_t vRoadName;
	_variant_t vRoadID;
	_variant_t vIDTerminal;
	_variant_t vTerminalID;
	CString strAreaID;
	int intIDTerminal=0;
	char count=0x00;    //��¼��Ŀ

	//��Roads��
	roadSQL="Select * From Roads";
	try
	{
		pRoadRs.CreateInstance("ADODB.Recordset");
		pRoadRs->Open((_variant_t)roadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	while (!pRoadRs->adoEOF)
	{
		vRoadName=pRoadRs->GetCollect("RoadName");   //RoadName�ֶε�ֵ
		if ((vRoadName.vt != VT_NULL)&&(vRoadName.vt != VT_EMPTY))
		{
			allRoads+="<";
			allRoads+=(LPCTSTR)(_bstr_t)vRoadName;
			allRoads+=">";
		}

		vRoadID=pRoadRs->GetCollect("RoadID");     //RoadID�ֶε�ֵ
		if (vRoadID.vt!=VT_NULL)
		{
			allRoads+="{";
			allRoads+=(LPCTSTR)(_bstr_t)vRoadID;
			allRoads+="}";
		}

		vIDTerminal=pRoadRs->GetCollect("IDTerminal");         //IDArea�ֶε�ֵ
		if((vIDTerminal.vt != VT_NULL)&&(vIDTerminal.vt != VT_EMPTY))
		{
			intIDTerminal=vIDTerminal.intVal;
		} 

		//ȡ��Terminals����IDΪ��Road������IDTerminal�ļ�¼��
		terminalSQL.Format("Select * From Terminals Where [ID] = %d",intIDTerminal);
		try
		{
			pTerminalRs.CreateInstance("ADODB.Recordset");
			pTerminalRs->Open((_variant_t)terminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
		}
		catch(_com_error&e)
		{
			AfxMessageBox(e.Description());
		}

		if(!pTerminalRs->adoEOF)
		{
			vTerminalID=pTerminalRs->GetCollect("TerminalID");   //TerminalID�ֶε�ֵ
			if ((vTerminalID.vt != VT_NULL)&&(vTerminalID.vt != VT_EMPTY))
			{
				allRoads+="(";
				allRoads+=(LPCTSTR)(_bstr_t)vTerminalID;
				allRoads+=")";
			}
		}
		count++;
		pRoadRs->MoveNext();
		pTerminalRs->Close();
	}
	
	pRoadRs->Close();
	allRoads=count+allRoads;
	allRoads+="#";
	return allRoads;
}

/************************************************************************/
/* ����: ����ָ��·�����Ʋ�������ָ�����ն˽���һ�Զ�Ĺ�ϵ*/
/*
����  ָ��·:roadID  �趨����:roadName  ָ������:terminalID
*/
/************************************************************************/

BOOL CRoadRecordset::SetRoadNameAndIDTerminal(CString roadID, CString roadName, CString terminalID)
{
	CString roadSQL;
	CString terminalSQL;
	_RecordsetPtr pTerminalRs;
	_variant_t vID;
	int intID;

	//��Terminals����TerminalIDΪָ��ֵ(terminalID)�ļ�¼
	terminalSQL.Format("Select * From Terminals Where TerminalID='%s'",terminalID);
	try
	{
		pTerminalRs.CreateInstance("ADODB.RecordSet");
		pTerminalRs->Open((_variant_t)terminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	if (!pTerminalRs->adoEOF)
	{
		vID=pTerminalRs->GetCollect("ID");  //��Terminal����һ�Զ��ϵ��IDAreaֵ
		intID=vID.intVal;
	}
	else
		return FALSE;

	//��Roads����RoadIDΪָ��ֵ(RoadID)�ļ�¼
	roadSQL.Format("Select * From Roads Where RoadID='%s'",roadID);
	if(Open(roadSQL))
	{
		SetAsString("RoadName",roadName);  //�����ն˵�����
		SetAsInt("IDTerminal",intID);                  //������Areas������IDAreaֵ,�Դ˰�
		return TRUE;
	}
	else
		return FALSE;

}