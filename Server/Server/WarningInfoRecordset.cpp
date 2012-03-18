#include "StdAfx.h"
#include "WarningInfoRecordset.h"

CWarningInfoRecordset::CWarningInfoRecordset(void)
{
}

CWarningInfoRecordset::~CWarningInfoRecordset(void)
{
}

//
//查询指定时间内的报警信息
//
CString CWarningInfoRecordset::GetWaringInfo(CString StartTime, CString EndTime)
{
	_RecordsetPtr m_pRecordset;
 	CString SQL;
 	CString strWarningInfo;
	CString strTID;
	_variant_t vTID,vTime,vClassOfWarning,vInfo;
	SQL.Format("Select TID,ClassOfWarning,Info From WarningInfo Where Time Between #%s# And #%s#", StartTime,EndTime);
	try
	{
		m_pRecordset.CreateInstance("ADODB.Recordset");
		m_pRecordset->Open((_variant_t)SQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
	
	try
	{
		while (!m_pRecordset->adoEOF)
		{
			vTID=m_pRecordset->GetCollect("TID");
			vClassOfWarning=m_pRecordset->GetCollect("ClassOfWarning");
			vInfo=m_pRecordset->GetCollect("Info");

			if (vTID.vt!=VT_NULL)
			{
				strTID.Format("%d",vTID.llVal);
				strWarningInfo+=strTID;
			}
			if (vClassOfWarning.vt!=VT_NULL)
			{
				strWarningInfo+=" ";
				strWarningInfo+=(LPCTSTR)(_bstr_t)vClassOfWarning;
			}
			if (vInfo.vt!=VT_NULL)
			{
				strWarningInfo+=" ";
				strWarningInfo+=(LPCTSTR)(_bstr_t)vInfo;
			}
			strWarningInfo+="#";
			m_pRecordset->MoveNext();
		}
		return strWarningInfo;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
		return NULL;
	}
	m_pRecordset->Close();
	m_pRecordset=NULL;
}
