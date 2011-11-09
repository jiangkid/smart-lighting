#include "StdAfx.h"
#include "DBConnection.h"

CDBConnection _DBConnection;
CDBConnection* g_pDBConnection = &_DBConnection;
CDBConnection::CDBConnection(void)
{
	//��ʼ��ADO��Connection����
	m_pConnection.CreateInstance("ADODB.Connection");
	m_Actived = FALSE;
}

CDBConnection::~CDBConnection(void)
{
	//�ͷ�ADO��Connection����
	m_pConnection.Release();
}

//
//�����ݿ������
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
		errormessage.Format("�������ݿ�ʧ��!\r������Ϣ:%s",e.ErrorMessage());
		AfxMessageBox(errormessage);
		return FALSE;
	}
}

//
//�ر����ݿ������
//
void CDBConnection::Close()
{
	m_pConnection->Close();
	m_pConnection = NULL;
	m_Actived = FALSE;
}
