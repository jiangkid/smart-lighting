#pragma once
#include "dbrecordset.h"

class CWarningInfoRecordset :
	public CDBRecordset
{
public:
	CWarningInfoRecordset(void);
	~CWarningInfoRecordset(void);
	
	//�õ�ָ��ʱ��εľ�����Ϣ��¼
	CString GetWaringInfo(CString StartTime,CString EndTime);
};
