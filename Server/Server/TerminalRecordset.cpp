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

/************************************************************************/
/* ����:�õ�һ���ն������е�·����Ϣ
���ظ�ʽ:·����Ŀ �ն����� ·���� ·ID ·��״̬ ����λ ����ʱ�� 
/************************************************************************/
char *CTerminalRecordset::GetRoadsOfTerminal(CString termianlID)
{
	roadInfo *pRoadInfo= (roadInfo *)malloc(ROADINFOLEN);
	CString terminalSQL;
	CString roadSQL;
	CString strTerminalName;
	CString strID;
	int intID;
	char count = 0;  
	_RecordsetPtr pRoadRs;
	_variant_t vRoadName;
	CString strRoadName;
	_variant_t vRoadID;
	CString strRoadID;
	_variant_t vStatus;
	bool bStatus;
	_variant_t vUpdateStatus;
	_variant_t vUpdateI1;
	_variant_t vUpdateI2;
	_variant_t vUpdateI3;

// 	_variant_t vUpdateTime;
// 	CString strUpdateTime;
	_variant_t vCurrentValue1;
	_variant_t vCurrentValue2;
	_variant_t vCurrentValue3;

	ZeroMemory(pRoadInfo,ROADINFOLEN);
	//������·�ļ�¼
	terminalSQL.Format("Select * From Terminals Where TerminalID = '%s'",termianlID);
	if (Open(terminalSQL))
	{
		strID = GetAsString("ID");
		intID = atoi(strID);              //�����ƽ���һ�Զ��ϵ��IDֵ

		strTerminalName = GetAsString("TerminalName");       //�ն˵�����	
	}

	//��Terminals����TerminalIDΪָ��ֵ(terminalID)�ļ�¼
	roadSQL.Format("Select * From Roads Where IDTerminal =%d",intID);
	try
	{
		pRoadRs.CreateInstance("ADODB.RecordSet");
		pRoadRs->Open((_variant_t)roadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	while (!pRoadRs->adoEOF)
	{
		//�ն˵�����
		memcpy(pRoadInfo->ternimalName,(U8 *)strTerminalName.GetBuffer(),strTerminalName.GetLength());

		vRoadName=pRoadRs->GetCollect("RoadName");   //RoadName�ֶε�ֵ
		if ((vRoadName.vt != VT_NULL)&&(vRoadName.vt != VT_EMPTY))
		{			
			strRoadName=(LPCTSTR)(_bstr_t)vRoadName;
			memcpy(pRoadInfo->roadName,(U8 *)strRoadName.GetBuffer(),strRoadName.GetLength());
		}

		vRoadID=pRoadRs->GetCollect("RoadID");     //RoadID�ֶε�ֵ
		if ((vRoadID.vt != VT_NULL)&&(vRoadID.vt != VT_EMPTY))
		{
			strRoadID=(LPCTSTR)(_bstr_t)vRoadID;
			memcpy(pRoadInfo ->roadID,(U8 *)strRoadID.GetBuffer(),strRoadID.GetLength());
		}

		vStatus = pRoadRs->GetCollect("Status");  //Status�ֶε�ֵ
		if ((vStatus.vt != VT_NULL) && (vStatus.vt != VT_EMPTY))
		{
			if (vStatus.boolVal == VARIANT_TRUE)
			{
				bStatus = true;
			}
			else
			{
				bStatus = false;
			}

			pRoadInfo ->roadStatus = bStatus;
		}

		vUpdateStatus = pRoadRs->GetCollect("UpdateStatus");  //Update�ֶε�ֵ
		pRoadRs->PutCollect("UpdateStatus",_variant_t("0"));
		if ((vUpdateStatus.vt != VT_NULL) && (vUpdateStatus.vt != VT_EMPTY))
		{
			pRoadInfo->update |=0x80 ;
		}

		vUpdateI1 = pRoadRs->GetCollect("UpdateI1");  //Update�ֶε�ֵ
		pRoadRs->PutCollect("UpdateI1",_variant_t("0"));
		if ((vUpdateI1.vt != VT_NULL) && (vUpdateI1.vt != VT_EMPTY))
		{
			pRoadInfo->update |=0x40 ;
		}

		vUpdateI2 = pRoadRs->GetCollect("UpdateI2");  //Update�ֶε�ֵ
		pRoadRs->PutCollect("UpdateI2",_variant_t("0"));
		if ((vUpdateI2.vt != VT_NULL) && (vUpdateI2.vt != VT_EMPTY))
		{
			pRoadInfo->update |=0x20 ;
		}
		
		vUpdateI3 = pRoadRs->GetCollect("UpdateI3");  //Update�ֶε�ֵPutCollect
		pRoadRs->PutCollect("UpdateI3",_variant_t("0"));
		if ((vUpdateI3.vt != VT_NULL) && (vUpdateI3.vt != VT_EMPTY))
		{
			pRoadInfo->update |=0x10 ;
		}
		
		// 		vUpdateTime=pRoadRs->GetCollect("UpdateTime");   //Update�ֶε�ֵ
		// 		if ((vUpdateTime.vt != VT_NULL)&&(vUpdateTime.vt != VT_EMPTY))
		// 		{			
		// 			strUpdateTime = (LPCTSTR)(_bstr_t)vUpdateTime;
		// 			memcpy(pRoadInfo->updateTime,(U8 *)strUpdateTime.GetBuffer(),strUpdateTime.GetLength());
		// 		}

		vCurrentValue1=pRoadRs->GetCollect("I1");     //I1��ֵ
		if ((vCurrentValue1.vt != VT_NULL)&&(vCurrentValue1.vt != VT_EMPTY))
		{	
			pRoadInfo->currentValue1=vCurrentValue1.fltVal;
		}

		vCurrentValue2=pRoadRs->GetCollect("I2");     //I2��ֵ
		if ((vCurrentValue2.vt != VT_NULL)&&(vCurrentValue2.vt != VT_EMPTY))
		{			
			pRoadInfo->currentValue2=vCurrentValue2.fltVal;
		}

		vCurrentValue3=pRoadRs->GetCollect("I3");     //I3��ֵ
		if ((vCurrentValue3.vt != VT_NULL)&&(vCurrentValue3.vt != VT_EMPTY))
		{			
			pRoadInfo->currentValue3=vCurrentValue3.fltVal;
		}

		memcpy(m_allRoads+(count * ROADINFOLEN)+1,pRoadInfo,ROADINFOLEN);
		ZeroMemory(pRoadInfo,ROADINFOLEN);
		count++;

		pRoadRs->MoveNext();
	}

	memcpy(m_allRoads,&count,1);
	memcpy(m_allRoads+(count * ROADINFOLEN)+1,"#",1);
	pRoadRs->Close();
	return m_allRoads;
}

