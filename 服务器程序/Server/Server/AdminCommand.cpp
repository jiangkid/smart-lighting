#include "StdAfx.h"
#include "AdminCommand.h"

CAdminCommand::CAdminCommand(void)
{
}

CAdminCommand::~CAdminCommand(void)
{
}

int CAdminCommand::GetMaxID()
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

BOOL CAdminCommand::NewAdmin(CString AdminName,CString AdminPsw)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;

	SQL.Format("Insert Into Admin([ID],[Name],[Password])Values(%d,\""+AdminName+"\",\""+AdminPsw+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CAdminCommand::ChangeAdminPsw(CString AdminName,CString NewPsw)
{
	CString SQL;
	SQL.Format("Update Admin Set [Password]=\""+NewPsw+"\" Where Name = '%s'",AdminName);
	if(!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CAdminCommand::DeleteAdmin(CString AdminName)
{
	CString SQL;
	SQL.Format("Delete From Admin Where Name = '%s'",AdminName);
	if(ExcuteSQL(SQL))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CAdminCommand::IsExistedAdmin(CString AdminName)
{
	int Count=0;
	_RecordsetPtr pRs;
	CString sql;
	sql.Format("SELECT * FROM Admin Where Name='%s'",AdminName);
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
		Count++;
	}
	if (Count>1)
	{
		return TRUE;
	}
	return FALSE;
	pRs->Close();

}

