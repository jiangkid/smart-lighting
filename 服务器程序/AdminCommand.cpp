#include "StdAfx.h"
#include "AdminCommand.h"

CAdminCommand::CAdminCommand(void)
{
}

CAdminCommand::~CAdminCommand(void)
{
}

BOOL CAdminCommand::NewAdmin(CString AdminName,CString AdminPsw)
{
	CString SQL;
	int MaxID = GetMaxAdminID();
	MaxID++;

	SQL.Format("Insert Into Admin([ID],[Name],[Psw])Values(%d,\""+AdminName+"\",\""+AdminPsw+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

int CAdminCommand::GetMaxAdminID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Admin";
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