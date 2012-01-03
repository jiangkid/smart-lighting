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

	//���������ID����ȡ�������µ��ն�ID
	CString GetTerminalIDsByAreaID(CString AreaID);

	//������������������ID
	CString GetAreaIDByAreaName(CString AreaName);

	//���������ID����ȡ�������µ��ն�����
	CString GetTerminalNamesByAreaID(CString AreaID);


};
