#pragma once
#include "dbrecordset.h"

class CAreaRecordset :
	public CDBRecordset
{
public:
	CAreaRecordset(void);
	~CAreaRecordset(void);
	
	//���������ID������������ƺ������µ�·
	BOOL SetAreaByAreaID(CString AreaID,CString AreaName,CString Terminals);

	//����������������鿴�����µ�Զ���ն�
	CString GetTerminalsByAreaName(CString AreaName);
};
