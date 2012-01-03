#include "StdAfx.h"
#include "LightCommand.h"

CLightCommand::CLightCommand(void)
{
}

CLightCommand::~CLightCommand(void)
{
}

BOOL CLightCommand::AddLight(CString LightID,CString LightName/*,BOOL Status,CString GroupNum*/)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	SQL.Format("Insert Into Lights([ID],[LightID],[LightName])Values(%d,\""+LightID+"\",\""+LightName+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

int CLightCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Lights";
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
