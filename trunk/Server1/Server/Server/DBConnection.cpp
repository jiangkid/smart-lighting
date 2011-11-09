#include "StdAfx.h"
#include "DBConnection.h"

CDBConnection _DBConnection;
CDBConnection* g_pDBConnection = &_DBConnection;
CDBConnection::CDBConnection(void)
{
	//初始化ADO的Connection对象
	m_pConnection.CreateInstance("ADODB.Connection");
	m_Actived = FALSE;
}

CDBConnection::~CDBConnection(void)
{
	//释放ADO的Connection对象
	m_pConnection.Release();
}

//
//打开数据库的连接
//
BOOL CDBConnection::Open(CString CnnStr)
{
	CString DBName = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=";
	DBName +=CnnStr;
	try
	{
		m_pConnection->Open(_bstr_t(DBName),"","",adModeUnknown);
		m_Actived = TRUE;
		return TRUE;
	}
	catch(_com_error e)
	{
		CString errormessage;
		errormessage.Format("连接数据库失败!\r错误信息:%s",e.ErrorMessage());
		AfxMessageBox(errormessage);
		return FALSE;
	}
}

//
//关闭数据库的连接
//
void CDBConnection::Close()
{
	m_pConnection->Close();
	m_pConnection = NULL;
	m_Actived = FALSE;
}
