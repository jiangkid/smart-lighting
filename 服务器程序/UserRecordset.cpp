#include "StdAfx.h"
#include "UserRecordset.h"

CUserRecordset::CUserRecordset(void)
{
}

CUserRecordset::~CUserRecordset(void)
{
}

//
//�����û����õ�������
//
CString CUserRecordset::GetPswByUserName(CString UserName)
{
	CString SQL;
	CString Psw;
	SQL.Format("Select * From Users Where Name = '%s'",UserName);
	if (Open(SQL))
	{
		/*strcpy_s(Psw.GetBuffer(Psw.GetLength()),Psw.GetLength(),GetAsString("Password"));*/
		Psw=GetAsString("Password");
		if (Psw=="")
		{
			::AfxMessageBox("�Ƿ����û�!");
			return NULL;
		}
		else
			return Psw;
	}
	return NULL;
}

//
//�õ��û����������������
//
CString CUserRecordset::GetAreaNameByUserName(CString UserName)
{
	CString SQL;
	CString AreaName;

	SQL.Format("Select * From Users Where Name = '%s'",UserName);
	if (Open(SQL))
	{
		/*strcpy(AreaName.GetBuffer(1024/ *AreaName.GetLength()* /),GetAsString("Area"));*/
		AreaName=GetAsString("Area");
		return AreaName; 
	}
	return NULL;
}


//
//�����û����������������
//
BOOL CUserRecordset::SetAreaName(CString UserName,CString AreaName)
{
	CString SQL;
	SQL.Format("Select * From Users Where Name='%s'",UserName);
	if (Open(SQL))
	{
		SetAsString("Area",AreaName);
		return TRUE;
	}
	return FALSE;
}