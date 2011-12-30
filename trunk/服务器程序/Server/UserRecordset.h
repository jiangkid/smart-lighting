#pragma once
#include "dbrecordset.h"

class CUserRecordset :
	public CDBRecordset
{
public:
	CUserRecordset(void);
	~CUserRecordset(void);

	//根据用户名得到其密码
	CString GetPswByUserName(CString UserName);

	//查看用户所管理区域的名称
	CString GetAreaNameByUserName(CString UserName);

	//设置用户所管理的区域
	BOOL SetAreaName(CString UserName,CString AreaName);
};
