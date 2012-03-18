#include "StdAfx.h"
#include "HRLightRecordset.h"

CHRLightRecordset::CHRLightRecordset(void)
{
}

CHRLightRecordset::~CHRLightRecordset(void)
{
}

//
//根据灯的名字来得到灯的历史记录
//
CString CHRLightRecordset::GetLightRecord(CString LightName)
{
	_RecordsetPtr m_pRecordset;
	CString SQL;
	CString strLightRecord;
	/*CString strStatus;*/

	_variant_t vTime,vStatus;
	SQL.Format("Select Time,Status From HistoryRecordOfLight Where Name ='%s'", LightName);
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
			vTime=m_pRecordset->GetCollect("Time");
			vStatus=m_pRecordset->GetCollect("Status");

			if (vTime.vt!=VT_NULL)
			{
				strLightRecord+=" ";
				strLightRecord+=(LPCTSTR)(_bstr_t)vTime;
			}
			if (vStatus.vt!=NULL)
			{
				if (vStatus.vt==-1)
				{
					strLightRecord+="ON";
				}
				else
				{
					strLightRecord+="OFF";
				}
				strLightRecord+=" ";
			}
			strLightRecord+="#";
			m_pRecordset->MoveNext();
		}
		return strLightRecord;
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
		return NULL;
	}
	m_pRecordset->Close();
	m_pRecordset=NULL;

}