#pragma once
#include "dbcommand.h"

class CAdminCommand :
	public CDBCommand
{
public:
	CAdminCommand(void);
	~CAdminCommand(void);

	//�½�һ������Ա
	BOOL NewAdmin(CString AdminName,CString AdminPsw);

	//�޸Ĺ���Ա������
	BOOL ModifyAdminPsw(CString AdminName,CString NewPsw);

	//ɾ��ָ���Ĺ���Ա
	BOOL DeleteAdmin(CString AdminName);
	
private:
	int GetMaxAdminID();
};
