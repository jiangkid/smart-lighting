#include "StdAfx.h"
#include "TerminalRecordset.h"

CTerminalRecordset::CTerminalRecordset(void)
{
}

CTerminalRecordset::~CTerminalRecordset(void)
{
}

//
//����Terminal�������е�����
//
BOOL CTerminalRecordset::LoadTerminalData()
{
	//ʹ��Open����������������
	return Open("Select * From Terminals");
}

//
//�����ն˵�ID�������ն˵�����,�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE
//
BOOL CTerminalRecordset::SetTerminalName(CString TerminalID,CString TerminalName)
{
	CString SQL;
	SQL.Format("Select * From  Terminals Where [TerminalID] = '%s'",TerminalID);
	if (Open(SQL))
	{
		SetAsString("TerminalName",TerminalName);
		return TRUE;
	}
	return FALSE;
}

//
//����ID�鿴·������,����Ϊ·������
//
CString CTerminalRecordset::GetTerminalName(CString TerminalID)
{
	CString SQL;
	CString TerminalName;
	SQL.Format("Select * From  Terminals Where [TerminalID] = '%s'",TerminalID);
	if (Open(SQL))
	{
		TerminalName=GetAsString("Name");
		return TerminalName;
	}
	return NULL;
	
}

//
//�����ն˵�ID�õ����ն�������·��ID
//
CString CTerminalRecordset::GetRoadIDsByTerminalID(CString TerminalID)
{
	CString TerminalSQL;
	CString RoadSQL;
	CString strID;
	CString strRoads;
	int intID;
	_variant_t vRoadID;
	_RecordsetPtr pRs;

	TerminalSQL.Format("Select * From Terminals Where [TerminalID]='%s'",TerminalID);
	if (Open(TerminalSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	RoadSQL.Format("Select * From Roads Where IDTerminal=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)RoadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vRoadID=pRs->GetCollect("RoadID");
		if (vRoadID.vt!=VT_NULL)
		{
			strRoads+="+";
			strRoads+=(LPCTSTR)(_bstr_t)vRoadID;
		}
		pRs->MoveNext();
	}
	return strRoads;
	pRs->Close();
}

//
//�����ն˵�ID�õ����ն�������·������
//
CString CTerminalRecordset::GetRoadNamesByTerminalID(CString TerminalID)
{
	CString TerminalSQL;
	CString RoadSQL;
	CString strID;
	CString strRoads;
	int intID;
	_variant_t vRoadName;
	_RecordsetPtr pRs;

	TerminalSQL.Format("Select * From Terminals Where [TerminalID]='%s'",TerminalID);
	if (Open(TerminalSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	RoadSQL.Format("Select * From Roads Where IDTerminal=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)RoadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vRoadName=pRs->GetCollect("RoadName");
		if (vRoadName.vt!=VT_NULL)
		{
			strRoads+="+";
			strRoads+=(LPCTSTR)(_bstr_t)vRoadName;
		}
		pRs->MoveNext();
	}
	return strRoads;

	pRs->Close();
}

//�����ն˵�ID�õ����ն�������·��ID����Ŀ
//
CString CTerminalRecordset::GetRoadIDsAndCountByTerminalID(CString TerminalID)
{
	CString TerminalSQL;
	CString RoadSQL;
	CString strID;
	CString strRoads;
	char count=0;
	//CString strCount;
	int intID;
	_variant_t vRoadID;
	_RecordsetPtr pRs;

	TerminalSQL.Format("Select * From Terminals Where [TerminalID]='%s'",TerminalID);
	if (Open(TerminalSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	RoadSQL.Format("Select * From Roads Where IDTerminal=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)RoadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vRoadID=pRs->GetCollect("RoadID");
		if (vRoadID.vt!=VT_NULL)
		{
			//strRoads+="<";
			strRoads+=(LPCTSTR)(_bstr_t)vRoadID;
			//strRoads+=">";
		}
		count++;
		pRs->MoveNext();
	}
	//strCount.Format("%d",count);
	strRoads=count+strRoads;
	strRoads+="#";
	return strRoads;
	pRs->Close();
}

/************************************************************************/
/* ����:�������е��ն����ơ�ID��������������ն˵���Ŀ
���ظ�ʽ:0x00-0xFF<�ն�����1>{�ն�1ID}{���������ID}......<�ն�����n>{�ն�nID}{���������ID}#*/
/************************************************************************/

CString CTerminalRecordset::GetAllTerminalsAndCount()
{
	CString terminalSQL;
	CString areaSQL;
	CString allTerminals;
	_RecordsetPtr pTerminalRs;
	_RecordsetPtr pAreaRS;
	_variant_t vTerminalName;
	_variant_t vTerminalID;
	_variant_t vIDArea;
	_variant_t vAreaID;
	CString strAreaID;
	int intIDArea=0;
	char count=0x00;    //��¼��Ŀ

	//��Terminals��
	terminalSQL="Select * From Terminals";
	try
	{
		pTerminalRs.CreateInstance("ADODB.Recordset");
		pTerminalRs->Open((_variant_t)terminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	while (!pTerminalRs->adoEOF)
	{
		vTerminalName=pTerminalRs->GetCollect("TerminalName");   //TerminalName�ֶε�ֵ
		if ((vTerminalName.vt != VT_NULL)&&(vTerminalName.vt != VT_EMPTY))
		{
			allTerminals+="<";
			allTerminals+=(LPCTSTR)(_bstr_t)vTerminalName;
			allTerminals+=">";
		}

		vTerminalID=pTerminalRs->GetCollect("TerminalID");     //TerminalID�ֶε�ֵ
		if ((vTerminalID.vt != VT_NULL)&&(vTerminalID.vt != VT_EMPTY))
		{
			allTerminals+="{";
			allTerminals+=(LPCTSTR)(_bstr_t)vTerminalID;
			allTerminals+="}";
		}

		vIDArea=pTerminalRs->GetCollect("IDArea");         //IDArea�ֶε�ֵ
		if((vIDArea.vt != VT_NULL)&&(vIDArea.vt != VT_EMPTY))
		{
			intIDArea=vIDArea.intVal;
		} 

		//���������ű�
		areaSQL.Format("Select * From Areas Where [ID] = %d",intIDArea);
		try
		{
			pAreaRS.CreateInstance("ADODB.Recordset");
			pAreaRS->Open((_variant_t)areaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
		}
		catch(_com_error&e)
		{
			AfxMessageBox(e.Description());
		}

		if(!pAreaRS->adoEOF)
		{
			vAreaID=pAreaRS->GetCollect("AreaID");   //AreaID�ֶε�ֵ
			if ((vAreaID.vt != VT_NULL)&&(vAreaID.vt != VT_EMPTY))
			{
				allTerminals+="(";
				allTerminals+=(LPCTSTR)(_bstr_t)vAreaID;
				allTerminals+=")";
			}
		}
		count++;
		pTerminalRs->MoveNext();
	}
	pTerminalRs->Close();
	allTerminals=count+allTerminals;
	allTerminals+="#";
	return allTerminals;
}
/************************************************************************/
/* ����: ����ָ���ն˵����Ʋ�������ָ����������һ�Զ�Ĺ�ϵ
����  ָ���ն�:terminalID  �趨����:terminalName  ָ������:areaID*/
/************************************************************************/
BOOL CTerminalRecordset::SetTerminalNameAndIDArea(CString terminalID,CString terminalName,CString areaID)
{
	CString areaSQL;
	CString terminalSQL;
	_RecordsetPtr pAreaRs;
	_variant_t vID;
	int intID;

	//��Areas����AreaIDΪָ��ֵ�ļ�¼
	areaSQL.Format("Select * From Areas Where AreaID='%s'",areaID);
	try
	{
		pAreaRs.CreateInstance("ADODB.RecordSet");
		pAreaRs->Open((_variant_t)areaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}


	if (!pAreaRs->adoEOF)
	{
		vID=pAreaRs->GetCollect("ID");  //��Terminal����һ�Զ��ϵ��IDAreaֵ
		intID=vID.intVal;
	}
	terminalSQL.Format("Select * From Terminals Where TerminalID='%s'",terminalID);
	if(Open(terminalSQL))
	{
		SetAsString("TerminalName",terminalName);  //�����ն˵�����
		SetAsInt("IDArea",intID);                  //������Areas������IDAreaֵ,�Դ˰�
		return TRUE;
	}
	else
		return FALSE;
}