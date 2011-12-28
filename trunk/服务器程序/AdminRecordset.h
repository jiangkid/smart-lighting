#pragma once
#include "dbrecordset.h"

class CAdminRecordset :
	public CDBRecordset
{
public:
	CAdminRecordset(void);
	~CAdminRecordset(void);

	//根据用户名得到密码
	CString GetPswByAdminName(CString AdminName);
};