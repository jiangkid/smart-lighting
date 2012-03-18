#include "StdAfx.h"
#include "HRTerminalCommand.h"

CHRTerminalCommand::CHRTerminalCommand(void)
{
}

CHRTerminalCommand::~CHRTerminalCommand(void)
{
}

int CHRTerminalCommand::GetMaxID()
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
	pRs;
	return nResult;
}

BOOL CHRTerminalCommand::AddHistoryRecordOfTerminal(CString TerminalID,CString Va,CString Vb,CString Vc,CString P,
												   CString Ir1,CString Ir2,CString Ir3,CString Ir4, CString Ir5,CString Ir6,CString Ir7,CString Ir8)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;

	SQL.Format("Insert Into HistoryRecordOfTerminal([ID],[TerminalID],Va,Vb,Vc,P,Ir1,Ir2,Ir3,Ir4,Ir5,Ir6,Ir7,Ir8)Values(%d,\""+TerminalID+"\",\""+Va+"\",\""+Vb+"\",\""+Vc+"\",\""+P+"\",\""+Ir1+"\",\""+Ir2+"\",\""+Ir3+"\",\""+Ir4+"\",\""+Ir5+"\",\""+Ir6+"\",\""+Ir7+"\",\""+Ir8+"\")",MaxID);
	
	if (ExcuteSQL(SQL))
	{
		return TRUE;
	}
	return FALSE;
}

