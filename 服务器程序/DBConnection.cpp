#include "StdAfx.h"
#include "DBConnection.h"

CDBConnection::CDBConnection(void)
{
	//初始化ADO的Connection对象
	m_pConn.CreateInstance(__uuidof(ADODB::Connection));
	m_Actived = FALSE;
}

CDBConnection::~CDBConnection(void)
{
	//初始化ADO的Connection对象
	m_pConn.Release();
}

BOOL CDBConnection::Open()
{
	try
	{
		m_pConn->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Demo.mdb","","",adModeUnknown);
		m_Actived = TRUE;
		return TRUE;
	}
	catch(_com_error &e)
	{
		::AfxMessageBox(e.ErrorMessage());
		return FALSE;
	}
}

void CDBConnection::Close()
{
	m_pConn->Close();
	m_Actived = FALSE;
}
