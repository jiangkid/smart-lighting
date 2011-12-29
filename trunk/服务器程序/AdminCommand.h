#pragma once
#include "dbcommand.h"

class CAdminCommand :
	public CDBCommand
{
public:
	CAdminCommand(void);
	~CAdminCommand(void);

	//新建一个管理员
	BOOL NewAdmin(CString AdminName,CString AdminPsw);

	//修改管理员的密码
	BOOL ModifyAdminPsw(CString AdminName,CString NewPsw);

	//删除指定的管理员
	BOOL DeleteAdmin(CString AdminName);
	
private:
	int GetMaxAdminID();
};
