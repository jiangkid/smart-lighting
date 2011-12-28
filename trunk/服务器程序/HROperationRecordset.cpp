#include "StdAfx.h"
#include "HROperationRecordset.h"

CHROperationRecordset::CHROperationRecordset(void)
{
}

CHROperationRecordset::~CHROperationRecordset(void)
{
}

CString CHROperationRecordset::GetOpertionRecord(CString StartTime,CString EndTime)
{
	_RecordsetPtr m_pRecordset;
	CString SQL;
	CString strOpertion;

	_variant_t vOperation;
	SQL.Format("Select Operation From HistoryRecordOfOperation Where Time Between #%s# And #%s#", StartTime,EndTime);
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
			vOperation=m_pRecordset->GetCollect("Operation");

			if (vOperation.vt!=VT_NULL)
			{
				strOpertion+=" ";
				strOpertion+=(LPCTSTR)(_bstr_t)vOperation;
			}
			strOpertion+="#";
			m_pRecordset->MoveNext();
		}
		return strOpertion;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
		return NULL;
	}
	m_pRecordset->Close();
	m_pRecordset=NULL;
}
