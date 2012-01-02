#include "StdAfx.h"
#include "AdminRecordset.h"

CAdminRecordset::CAdminRecordset(void)
{
}

CAdminRecordset::~CAdminRecordset(void)
{
}

CString CAdminRecordset::GetPswByAdminName(CString AdminName)
{
	CString SQL;
	CString Psw;
	SQL.Format("Select * From Admin Where Name = '%s'",AdminName);
	if (Open(SQL))
	{
		/*strcpy_s(Psw.GetBuffer(Psw.GetLength()),Psw.GetLength(),GetAsString("Password"));*/
		Psw=GetAsString("Psw");
		if (Psw==" ")
		{
//			::AfxMessageBox("非法的管理员名称!");
			return NULL;
		}
		else
			return Psw;
	}
	return NULL;
}
