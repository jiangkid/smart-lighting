#include "StdAfx.h"
#include "DBCommand.h"

CDBCommand::CDBCommand(void)
{
}

CDBCommand::~CDBCommand(void)
{
}

BOOL CDBCommand::ExcuteSQL(CString SQL)
{
	_variant_t RecordsAffected;
	ASSERT(m_cnn);
	ASSERT(m_cnn->m_Actived);
	try
	{
		m_cnn->m_pConn->Execute(_bstr_t(SQL),&RecordsAffected,adCmdText);
	}
	catch(_com_error &e)
	{
		return FALSE;
		AfxMessageBox(e.Description());
	}
	return TRUE;
}