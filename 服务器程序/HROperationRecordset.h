#pragma once
#include "dbrecordset.h"

class CHROperationRecordset :
	public CDBRecordset
{
public:
	CHROperationRecordset(void);
	~CHROperationRecordset(void);

public:
	//�õ�ָ��ʱ��εĲ�����¼��Ϣ
	CString GetOpertionRecord(CString StartTime,CString EndTime);
};
