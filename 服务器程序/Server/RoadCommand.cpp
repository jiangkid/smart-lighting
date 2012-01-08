#include "StdAfx.h"
#include "RoadCommand.h"

CRoadCommand::CRoadCommand(void)
{
}

CRoadCommand::~CRoadCommand(void)
{
}

//
//��·�����ű������һ���µļ�¼
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

//��·�����ű������һ���µ�ID
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

//��Road�������һ���µ�ID
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
			idRoad=vIDTerminal.lVal;        //��Terminals����һ�Զ�Ĺ����ֶ�IDArea��ֵ
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



