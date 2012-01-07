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

//向Terminal表中添加一个新的ID
BOOL CTerminalCommand::AddTerminalID(CString areaID,CString terminalID)
{
	CString terminalSQL;
	CString areaSQL;
	_RecordsetPtr pAreaRs;
	_variant_t vIDArea;
	long idArea=0;
	int MaxID = GetMaxID();
	++MaxID;

	areaSQL.Format("SELECT * FROM Areas WHERE AreaID='%s'",areaID);
	try
	{
		pAreaRs.CreateInstance("ADODB.RecordSet");
		pAreaRs->Open((_variant_t)areaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	if(!pAreaRs->adoEOF)
	{
		vIDArea=pAreaRs->GetCollect(_variant_t((long)0)); 
		if (vIDArea.vt!=VT_NULL)
		{
			idArea=vIDArea.lVal;        //与Terminals表建立一对多的关联字段IDArea的值
		}
		//pAreaRs->MoveNext();
	}
	terminalSQL.Format("Insert Into Terminals([ID],[TerminalID],[IDArea])Values(%d,'%s',%ld)",MaxID,terminalID,idArea);
	if (!ExcuteSQL(terminalSQL))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CTerminalCommand::AddTerminalID(CString TerminalID)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	int IDArea = 0;
	SQL.Format("Insert Into Terminals([ID],[TerminalID],[IDArea])Values(%d,\""+TerminalID+"\",%d)",MaxID,IDArea);
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