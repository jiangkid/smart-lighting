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

	//����ָ������Ա������
	BOOL ChangeAdminPsw(CString AdminName,CString NewPsw);

	//ɾ��ָ���Ĺ���Ա
	BOOL DeleteAdmin(CString AdminName);

	//�жϸù���Ա�Ƿ��������ݿ��д���
	BOOL IsExistedAdmin(CString AdminName);

private:
	int GetMaxID();
};
