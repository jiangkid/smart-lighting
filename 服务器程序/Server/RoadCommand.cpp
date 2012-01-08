#include "StdAfx.h"
#include "RoadCommand.h"

CRoadCommand::CRoadCommand(void)
{
}

CRoadCommand::~CRoadCommand(void)
{
}

//
//向路的这张表中添加一个新的记录
//
BOOL CRoadCommand::AddRoad(CString RoadID, CString RoadName)//,CString Lights
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;

	SQL.Format("Insert Into Roads([ID],[RoadID],[RoadName])Values(%d,\""+RoadID+"\",\""+RoadName+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;

}

//向路的这张表中添加一个新的ID
//
BOOL CRoadCommand::AddRoadID(CString RoadID)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	int IDTerminal = 0;
	SQL.Format("Insert Into Roads([ID],[RoadID],[IDTerminal])Values(%d,\""+RoadID+"\",%d)",MaxID,IDTerminal);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;

}

//向Road表中添加一个新的ID
BOOL CRoadCommand::AddRoadID(CString terminalID,CString RoadID)
{
	CString RoadSQL;
	CString terminalSQL;
	_RecordsetPtr pTerminal;
	_variant_t vIDTerminal;
	long idRoad=0;
	int MaxID = GetMaxID();
	

	terminalSQL.Format("SELECT * FROM Terminals WHERE TerminalID='%s'",terminalID);
	try
	{
		pTerminal.CreateInstance("ADODB.RecordSet");
		pTerminal->Open((_variant_t)terminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	if(!pTerminal->adoEOF)
	{
		vIDTerminal=pTerminal->GetCollect(_variant_t((long)0)); 
		if (vIDTerminal.vt!=VT_NULL)
		{
			idRoad=vIDTerminal.lVal;        //与Terminals表建立一对多的关联字段IDArea的值
		}
	}
	RoadSQL.Format("Insert Into Roads([ID],[RoadID],[IDTerminal])Values(%d,'%s',%ld)",MaxID,RoadID,idRoad);
	if (!ExcuteSQL(RoadSQL))
	{
		return FALSE;
	}
	return TRUE;
}
int CRoadCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Roads";
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



