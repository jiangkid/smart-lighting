#include "StdAfx.h"
#include "DBConnection.h"

CDBConnection::CDBConnection(void)
{
	//��ʼ��ADO��Connection����
	m_pConn.CreateInstance("ADODB.Connection");
	m_Actived = FALSE;
}

CDBConnection::~CDBConnection(void)
{
	//�ͷ�ADO��Connection����
	m_pConn.Release();
}

BOOL CDBConnection::Open(CString CnnStr)
{
	try
	{
		m_pConn->Open(_bstr_t(CnnStr),"","",adConnectUnspecified);
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
