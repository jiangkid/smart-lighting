#include "StdAfx.h"
#include "AreaRecordset.h"

CAreaRecordset::CAreaRecordset(void)
{
}

CAreaRecordset::~CAreaRecordset(void)
{
}

//
//���������ID������������ƺ������µ�·
//
BOOL CAreaRecordset::SetAreaByAreaID(CString AreaID, CString AreaName, CString Terminals)
{
	CString SQL;
	SQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(SQL))
	{
		SetAsString("AreaName",AreaName);
		SetAsString("Roads",Terminals);
		return TRUE;
	}
	return FALSE;
}

//
//���������ID����ȡ�������µ������ն˵�ID
//
CString CAreaRecordset::GetTerminalIDsByAreaID(CString AreaID)
{
	CString AreaSQL;
	CString TerminalSQL;
	CString strID;
	CString strTerminals;
	int intID;
	_variant_t vTerminalID;
	_RecordsetPtr pRs;

	AreaSQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(AreaSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}
	TerminalSQL.Format("Select * From Terminals Where IDArea=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)TerminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vTerminalID=pRs->GetCollect("TerminalID");
		if (vTerminalID.vt!=VT_NULL)
		{
			//strTerminals+="+";
			strTerminals+=(LPCTSTR)(_bstr_t)vTerminalID;
		}
		pRs->MoveNext();
	}
	return strTerminals;
	pRs->Close();
}

//������������ֻ�������ID
CString CAreaRecordset::GetAreaIDByAreaName(CString AreaName)
{
	CString SQL;
	CString AreaID;

	SQL.Format("Select * From Areas Where AreaName = '%s'",AreaName);
	if (Open(SQL))
	{
		AreaID=GetAsString("AreaID");
		return AreaID; 
	}
	return NULL;
}

//
//���������ID����ȡ�������������ն˵�����
//
CString CAreaRecordset::GetTerminalNamesByAreaID(CString AreaID)
{
	CString AreaSQL;
	CString TerminalSQL;
	CString strID;
	CString strTerminals;
	int intID;
	_variant_t vTerminalName;
	_RecordsetPtr pRs;

	AreaSQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(AreaSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	TerminalSQL.Format("Select * From Terminals Where IDArea=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)TerminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vTerminalName=pRs->GetCollect("TerminalName");
		if (vTerminalName.vt!=VT_NULL)
		{
			strTerminals+="+";
			strTerminals+=(LPCTSTR)(_bstr_t)vTerminalName;
		}
		pRs->MoveNext();
	}
	return strTerminals;

	pRs->Close();
}
/************�������ܣ�������е�����ID**************/
CString CAreaRecordset::GetAllAreaIDAndCount()
{
	CString AreaSQL;
	_RecordsetPtr pAreaRs;
	char count=0x00;
	CString strCount;
	CString AreaIDAndCount;
	CString strAreaID;
	_variant_t vAreaID;
	AreaSQL.Format("Select * From Areas");

	try
	{
		pAreaRs.CreateInstance("ADODB.Recordset");
		pAreaRs->Open((_variant_t)AreaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pAreaRs->adoEOF)
	{
		vAreaID=pAreaRs->GetCollect("AreaID");
		if (vAreaID.vt!=VT_NULL)
		{
			//strAreaID+="<";
			strAreaID+=(LPCTSTR)(_bstr_t)vAreaID;
			//strAreaID+=">";
		}
		count++;
		pAreaRs->MoveNext();
	}
	pAreaRs->Close();
	AreaIDAndCount=count+strAreaID;
	AreaIDAndCount+="#";
	return AreaIDAndCount;
}

//���������ID����ȡ�������µ������ն˵�ID���ն˵���Ŀ
//
CString CAreaRecordset::GetTerminalIDsAndCountByAreaID(CString AreaID)
{
	CString AreaSQL;
	CString TerminalSQL;
	CString strID;
	CString strTerminalIDs;
	int intID;
	char count=0;
	//CString strCount;

	_variant_t vTerminalID;
	_RecordsetPtr pRs;

	AreaSQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(AreaSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	TerminalSQL.Format("Select * From Terminals Where IDArea=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)TerminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vTerminalID=pRs->GetCollect("TerminalID");
		if (vTerminalID.vt!=VT_NULL)
		{
			//strTerminalIDs+="<";
			strTerminalIDs+=(LPCTSTR)(_bstr_t)vTerminalID;
			//strTerminalIDs+=">";
		}
		count++;
		pRs->MoveNext();
	}
	//strCount.Format("%d",count);
	strTerminalIDs=count+strTerminalIDs;
	return strTerminalIDs;
	pRs->Close();

}


//
//���������ID(areaID)�����������Ϣ��������ָ�����û�(userName)������ϵ
//
BOOL CAreaRecordset:: SetAreaInfoByAreaID(CString areaID,CString areaName,CString dutyArea,CString telephone,CString installTime,CString installLocation,CString userName)
{
	CString userSQL;
	CString areaSQL;
	_RecordsetPtr pUserRs;
	_RecordsetPtr pAreaRs;

	_variant_t vID;
	int idUser;

	userSQL.Format("Select * From Users Where Name= '%s'",userName);
	areaSQL.Format("Select * From Areas Where AreaID='%s'",areaID);

	try
	{
		pUserRs.CreateInstance("ADODB.RecordSet");
		pUserRs->Open((_variant_t)userSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	//���Users����NameΪuserName��ID�ֶε�ֵ
	if (!pUserRs->adoEOF)
	{
		vID=pUserRs->GetCollect("ID");
		if ((vID.vt!=VT_NULL)&&(vID.vt!=VT_EMPTY))
		{
			idUser=vID.intVal;
		}
		else
			return FALSE;
	}
	else
		return FALSE;

	pUserRs->Close();
	pUserRs=NULL;

	if (Open(areaSQL))
	{
		SetAsString("AreaName",areaName);
		SetAsString("DutyArea",dutyArea);
		SetAsString("Telephone",telephone);
		SetAsString("InstallTime",installTime);
		SetAsString("InstallLocation",installLocation);
		SetAsInt("IDUser",idUser);
		return TRUE;
	}
	return FALSE;
}
/************************************************************************************
����:������е������ID�����Ƽ�������
���صĸ�ʽ:0x00-0xFF<����1>{01}<����2>{01}.......<����n>{0n}#
*************************************************************************************/
CString CAreaRecordset::GetAllAreaAndCount()
{
	CString areaSQL;
	CString allAreas;
	_variant_t vAreaName;
	_variant_t vAreaID;
	char count=0x00;
	_RecordsetPtr pAreaRs;

	areaSQL.Format("Select * From Areas");

	//��Areas��
	try
	{
		pAreaRs.CreateInstance("ADODB.Recordset");
		pAreaRs->Open((_variant_t)areaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
		allAreas = "";
		return allAreas;
	}

	while (!pAreaRs->adoEOF)
	{
		vAreaName = pAreaRs->GetCollect("AreaName");   //���AreaName�ֶε�ֵ
		if ((vAreaName.vt != VT_NULL)&&(vAreaName.vt != VT_EMPTY))
		{
			allAreas+="<";
			allAreas+=(LPCTSTR)(_bstr_t)vAreaName;
			allAreas+=">";
		}
		vAreaID=pAreaRs->GetCollect("AreaID");     //���AreaID�ֶε�ֵ
		if ((vAreaID.vt != VT_NULL)&&(vAreaID.vt != VT_EMPTY))
		{
			allAreas+="{";
			allAreas+=(LPCTSTR)(_bstr_t)vAreaID;
			allAreas+="}";
		}
		count++;
		pAreaRs->MoveNext();
	}
	pAreaRs->Close();
	allAreas=count+allAreas;
	allAreas+="#";
	return allAreas;
}

/************************************************************************
����:���������ID�õ������û�������
����:�û�������
************************************************************************/
CString CAreaRecordset::GetUserNamebyAreaID(CString areaID)
{
	CString areaSQL;
	CString userSQL;
	CString strIDUser;
	int intIDUser;
	_RecordsetPtr pUserRs;
	CString strUserName;
	_variant_t vUserName;

	areaSQL.Format("Select * From Areas Where AreaID='%s'",areaID);
	if (Open(areaSQL))
	{
		strIDUser = GetAsString("IDUser");
		intIDUser = atoi(strIDUser);
	}

	userSQL.Format("Select * From Users Where ID = %d",intIDUser);
	try
	{
		pUserRs.CreateInstance("ADODB.Recordset");
		pUserRs->Open((_variant_t)userSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	if (!pUserRs->adoEOF)
	{
		vUserName=pUserRs->GetCollect("Name");
		strUserName = (LPCTSTR)(_bstr_t)vUserName;
	}
	return strUserName;
}

/************************************************************************/
/* ����:���ָ���������Ϣ
����˵��:      areaID:ָ�������ID*/
/************************************************************************/

char* CAreaRecordset::GetAreaInfoByAreaID(CString areaID)
{
	areaInfo *pAreaInfo= (areaInfo *)malloc(AREAINFOLEN);
	CString sql;
	CString strAreaID,strAreaName,strDutyArea,strTelephone,strInstallTime,strInstallLocation;

	ZeroMemory(pAreaInfo,AREAINFOLEN);

	sql.Format("Select * From Areas Where AreaID = '%s'",areaID);
	if (Open(sql))
	{
		strAreaID = GetAsString("AreaID");
		strAreaName= GetAsString("AreaName");
		strDutyArea = GetAsString("DutyArea");
		strTelephone = GetAsString("Telephone");
		strInstallTime = GetAsString("InstallTime");
		strInstallLocation = GetAsString("InstallLocation");
		memcpy(pAreaInfo->areaID,(U8 *)strAreaID.GetBuffer(),strAreaID.GetLength());
		memcpy(pAreaInfo->areaName,(U8 *)strAreaName.GetBuffer(),strAreaName.GetLength());
		memcpy(pAreaInfo->dutyArea,(U8 *)strDutyArea.GetBuffer(),strDutyArea.GetLength());
		memcpy(pAreaInfo->telephone,(U8 *)strTelephone.GetBuffer(),strTelephone.GetLength());
		memcpy(pAreaInfo->installTime,(U8 *)strInstallTime.GetBuffer(),strInstallTime.GetLength());
		memcpy(pAreaInfo->installLocation,(U8 *)strInstallLocation.GetBuffer(),strInstallLocation.GetLength());
		memcpy(mAreaInfo,pAreaInfo,90);
		return mAreaInfo;
	}
	else
	{
		return NULL;
	}
}
