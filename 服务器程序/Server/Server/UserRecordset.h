#pragma once
#include "dbrecordset.h"

class CUserRecordset :
	public CDBRecordset
{
public:
	CUserRecordset(void);
	~CUserRecordset(void);

	//�����û����õ�������
	CString GetPswByUserName(CString UserName);

	//�鿴�û����������������
	CString GetAreaNameByUserName(CString UserName);

	//�����û������������
	BOOL SetAreaName(CString UserName,CString AreaName);
};
