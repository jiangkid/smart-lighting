#include "StdAfx.h"
#include "AreaRecordset.h"

CAreaRecordset::CAreaRecordset(void)
{
}

CAreaRecordset::~CAreaRecordset(void)
{
}

//
//根据区域的ID设置区域的名称和区域下的路
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
//根据区域的ID来获取该区域下的所有终端的ID
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

//根据区域的名字获得区域的ID
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
//根据区域的ID来获取该区域下所有终端的名称
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