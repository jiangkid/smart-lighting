#include "StdAfx.h"
#include "TerminalRecordset.h"

CTerminalRecordset::CTerminalRecordset(void)
{
}

CTerminalRecordset::~CTerminalRecordset(void)
{
}

//
//加载Terminal表中所有的数据
//
BOOL CTerminalRecordset::LoadTerminalData()
{
	//使用Open方法加载所有数据
	return Open("Select * From Terminals");
}

//
//设置终端的ID来设置终端的名称,成功时返回TRUE，失败时返回FALSE
//
BOOL CTerminalRecordset::SetTerminalName(CString TerminalID,CString TerminalName)
{
	CString SQL;
	SQL.Format("Select * From  Terminals Where [TerminalID] = %s",TerminalID);
	if (Open(SQL))
	{
		SetAsString("Name",TerminalName);
		return TRUE;
	}
	return FALSE;
}

//
//根据ID查看路的名称,返回为路的名称
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
//根据终端的ID得到该终端下所有路的ID
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
//根据终端的ID得到该终端下所有路的名称
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



