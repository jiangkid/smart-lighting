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

	//更改指定管理员的密码
	BOOL ChangeAdminPsw(CString AdminName,CString NewPsw);

	//删除指定的管理员
	BOOL DeleteAdmin(CString AdminName);

	//判断该管理员是否已在数据库中存在
	BOOL IsExistedAdmin(CString AdminName);

private:
	int GetMaxID();
};
