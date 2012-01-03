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
	CString UserSQL;
	CString AreaSQL;
	CString strID;
	CString strAreas;
	int intID;
	_variant_t vAreaName;
	_RecordsetPtr pRs;

	UserSQL.Format("Select * From Users Where [Name]='%s'",UserName);
	if (Open(UserSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	AreaSQL.Format("Select * From Areas Where IDUser=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)AreaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vAreaName=pRs->GetCollect("AreaName");
		if (vAreaName.vt!=VT_NULL)
		{
			//strAreas+="+";
			strAreas+=(LPCTSTR)(_bstr_t)vAreaName;
		}
		pRs->MoveNext();
	}
	return strAreas;

	pRs->Close();
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