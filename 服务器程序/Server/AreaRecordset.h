#pragma once
#include "dbrecordset.h"

class CAreaRecordset :
	public CDBRecordset
{
public:
	CAreaRecordset(void);
	~CAreaRecordset(void);
	
	//���������ID������������ƺ������µ�·
	BOOL SetAreaByAreaID(CString AreaID,CString AreaName,CString Roads);

	//����������������鿴�����µ�·
	CString GetRoadsByAreaName(CString AreaName);
};
