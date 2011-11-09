#include "StdAfx.h"
#include "DBCommand.h"

CDBCommand::CDBCommand(void)
{
}

CDBCommand::~CDBCommand(void)
{
}

void CDBCommand::ExecuteSQL(CString SQL)
{
	ASSERT(m_cnn);
	ASSERT(m_cnn->m_Actived);
	m_cnn->m_pConnection->Execute(_bstr_t(SQL),NULL,adCmdText);
}
