//
//���ݿ�����ģ��
//

#include "DBConnection.h"
#pragma once

class CDBCommand
{
public:
	CDBConnection *m_cnn;

	//ִ��SQL���
	void ExecuteSQL(CString SQL);

	CDBCommand(void);
	~CDBCommand(void);
};