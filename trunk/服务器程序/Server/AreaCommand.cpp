#include "StdAfx.h"
#include "AreaCommand.h"

CAreaCommand::CAreaCommand(void)
{
}

CAreaCommand::~CAreaCommand(void)
{
}

int CAreaCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Areas";
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
//向Areas这张表中添加一条新的记录
//
BOOL CAreaCommand::AddArea(CString AreaID, CString AreaName)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	//int IDUser = 0;
	SQL.Format("Insert Into Areas([ID],[AreaID],[AreaName])Values(%d,\""+AreaID+"\",\""+AreaName+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

//
//向Areas这张表中添加一个新的ID
BOOL CAreaCommand::AddAreaID(long int AreaIDH)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	int IDUser = 0;
	SQL.Format("Insert Into Areas([ID],[AreaID],[IDUser])Values(%d,%ld,%d)",MaxID,AreaIDH,IDUser);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}