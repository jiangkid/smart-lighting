#pragma once
#include "dbrecordset.h"

class CAdminRecordset :
	public CDBRecordset
{
public:
	CAdminRecordset(void);
	~CAdminRecordset(void);

	//�����û����õ�����
	CString GetPswByAdminName(CString AdminName);
};