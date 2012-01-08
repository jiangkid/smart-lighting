#include "StdAfx.h"
#include "HRLightCommand.h"

CHRLightCommand::CHRLightCommand(void)
{
}

CHRLightCommand::~CHRLightCommand(void)
{
}

//
//得到最大的ID
//
int CHRLightCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM HistoryRecordOfLight";
	try
	{
		/*_RecordsetPtr pRs;*/
 	   pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)sql,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		pRs->MoveNext();
		nResult++;
	}
	pRs->Close();
	return nResult;
}

//
//向HistoryRecordOfLight表中添加一条新的记录
//
BOOL CHRLightCommand::AddHistoryRecordOfLight(CString Name,CString Time,BOOL LightStatus)
{
	CString SQL;
	CString strStatus;
	int MaxID = GetMaxID();
	//在Access中,对于"是/否"类型的字段,-1表示"是"，0表示"否"
	if (LightStatus)
	{
		strStatus = "-1";
	}
	else
	{
		strStatus="0";
	}
	SQL.Format("INSERT INTO HistoryRecordOfLight([HistoryRecOfLightID],[Time],[Status],[Name]) VALUES(%d,%s,%s,%s)",MaxID,Time,strStatus,Name);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

