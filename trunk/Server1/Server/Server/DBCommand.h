//
//数据库命令模块
//

#include "DBConnection.h"
#pragma once

class CDBCommand
{
public:
	CDBConnection *m_cnn;

	//执行SQL语句
	void ExecuteSQL(CString SQL);

	CDBCommand(void);
	~CDBCommand(void);
};