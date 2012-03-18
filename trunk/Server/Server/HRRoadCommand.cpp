#include "StdAfx.h"
#include "HRRoadCommand.h"

CHRRoadCommand::CHRRoadCommand(void)
{
}

CHRRoadCommand::~CHRRoadCommand(void)
{
}

//
//得到HistoryRecordOfRoad表的最大ID号
//
int CHRRoadCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM HistoryRecordOfRoad";
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
//向HistoryRecordOfRoad表中添加一条新的记录
//
BOOL CHRRoadCommand::AddHistoryRecordOfRoad(CString Time,BOOL RoadStatus)
{
	CString SQL;
	CString strStatus;
	int MaxID = GetMaxID();
	MaxID++;

	//在Access中,对于"是/否"类型的字段,-1表示"是"，0表示"否"
	if (RoadStatus)
	{
		strStatus = "-1";
	}
	else
	{
		strStatus="0";
	}

	SQL.Format("Insert Into HistoryRecordOfRoad([ID],[Time],[Status])Values(%d,\""+Time+"\",\""+strStatus+"\")",MaxID);
	
	if (ExcuteSQL(SQL))
	{
		return TRUE;
	}
	return FALSE;
}
