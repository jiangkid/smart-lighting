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
		if (vAreaID.vt!=NULL)
		{
			strAreaID+="<";
			strAreaID+=(LPCTSTR)(_bstr_t)vAreaID;
			strAreaID+=">";
		}
		count++;
		pAreaRs->MoveNext();
	}
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
			strTerminalIDs+="<";
			strTerminalIDs+=(LPCTSTR)(_bstr_t)vTerminalID;
			strTerminalIDs+=">";
		}
		count++;
		pRs->MoveNext();
	}
	//strCount.Format("%d",count);
	strTerminalIDs=count+strTerminalIDs;
	return strTerminalIDs;
	pRs->Close();

}

