#include "StdAfx.h"
#include "TerminalCommand.h"

CTerminalCommand::CTerminalCommand(void)
{
}

CTerminalCommand::~CTerminalCommand(void)
{
}

//
//向Terminal表中添加一条记录
//

BOOL CTerminalCommand::AddTerminal(CString TerminalID, CString TerminalName)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;

	SQL.Format("Insert Into Terminals([ID],[TerminalID],[TerminalName])Values(%d,\""+TerminalID+"\",\""+TerminalName+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

int CTerminalCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Terminals";
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