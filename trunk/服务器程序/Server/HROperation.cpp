#include "StdAfx.h"
#include "HROperation.h"

CHROperationCommand::CHROperationCommand(void)
{
}

CHROperationCommand::~CHROperationCommand(void)
{
}

//
//写入一条新的操作记录
//
BOOL CHROperationCommand::AddOperationRecord(CString Time,CString Oper)
{
	CString SQL;
	int MaxID = GetMaxIDOfHROperation();
	MaxID++;
	
	SQL.Format("Insert Into HistoryRecordOfOperation([ID],[Time],[Operation])Values(%d,\""+Time+"\",\""+Oper+"\")",MaxID);
	if (ExcuteSQL(SQL))
	{
		return TRUE;
	}
	return FALSE;	

}

int CHROperationCommand::GetMaxIDOfHROperation()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM HistoryRecordOfOperation";
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