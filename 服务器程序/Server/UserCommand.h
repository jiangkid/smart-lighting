#pragma once
#include "dbcommand.h"

class CUserCommand :
	public CDBCommand
{
public:
	CUserCommand(void);
	~CUserCommand(void);

	//新建一个用户
	BOOL NewUser(CString UserName,CString Psw);

	//更改用户的名称
	BOOL ChangeUserName(CString UserName,CString NewName);

	//更改指定用户的密码
	BOOL ChangeUserPsw(CString UserName,CString NewPsw);

	//更改指定用户的区域
	BOOL ChangeUserArea(CString UserName,CString NewArea);

	//删除指定的用户
	BOOL DeleteUser(CString UserName);

	//判断用户是否已在数据库中存在
	BOOL IsExistedUser(CString UserName);

private:
	//返回当前User表中ID最大值
	int GetMaxUserID();
};
