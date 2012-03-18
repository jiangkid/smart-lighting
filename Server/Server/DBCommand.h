#pragma once
#include "DBConnection.h"

class CDBCommand
{
public:
	CDBConnection *m_cnn;
	//Ö´ÐÐSQLÓï¾ä
	BOOL ExcuteSQL(CString SQL);

public:
	CDBCommand(void);
	~CDBCommand(void);
};
