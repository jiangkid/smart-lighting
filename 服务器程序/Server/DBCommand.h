#pragma once
#include "DBConnection.h"

class CDBCommand
{
public:
	CDBConnection *m_cnn;
	//ִ��SQL���
	BOOL ExcuteSQL(CString SQL);

public:
	CDBCommand(void);
	~CDBCommand(void);
};
