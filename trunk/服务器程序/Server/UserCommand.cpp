#include "StdAfx.h"
#include "UserCommand.h"

CUserCommand::CUserCommand(void)
{
}

CUserCommand::~CUserCommand(void)
{
}
//
//新建新的用户，返回新插入员工ID
//
BOOL CUserCommand::NewUser(CString UserName,CString Psw,CString AreaName)
{
	CString SQL;
	int MaxID = GetMaxUserID();
	MaxID++;

	SQL.Format("Insert Into Users([ID],[Name],[Password],[Area])Values(%d,\""+UserName+"\",\""+Psw+"\",\""+AreaName+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

//
//更改用户的名称,NewName为用户的新名称
//
BOOL CUserCommand::ChangeUserName(CString UserName,CString NewName)
{
	CString SQL;
	SQL.Format("Update Users Set Name=\""+NewName+"\" Where Name = '%s'",UserName);
	if(!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

//
//更改指定用户的密码
//
BOOL CUserCommand::ChangeUserPsw(CString UserName,CString NewPsw)
{
	CString SQL;
	SQL.Format("Update Users Set [Password]=\""+NewPsw+"\" Where Name = '%s'",UserName);
	if(!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

//
//更改指定用户的区域名称
//
BOOL CUserCommand::ChangeUserArea(CString UserName,CString NewArea)
{
	CString SQL;
	SQL.Format("Update Users Set Area=\""+NewArea+"\" Where Name = '%s'",UserName);
	if(!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

//
//删除指定的用户
//
BOOL CUserCommand::DeleteUser(CString UserName)
{
	CString SQL;
	SQL.Format("Delete From Users Where Name = '%s'",UserName);
	if(ExcuteSQL(SQL))
	{
		return TRUE;
	}
	return FALSE;
}

//
//判断用户是否已在Access数据库中已存在,存在返回TRUE,否则返回FALSE
//
BOOL CUserCommand::IsExistedUser(CString UserName)
{
	int Count=0;
	_RecordsetPtr pRs;
	CString sql;
	sql.Format("SELECT * FROM Users Where Name='%s'",UserName);
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

//
//获得当前的最大ID值
//
int CUserCommand::GetMaxUserID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Users";
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