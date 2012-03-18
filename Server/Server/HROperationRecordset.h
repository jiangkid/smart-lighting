#pragma once
#include "dbrecordset.h"

class CHROperationRecordset :
	public CDBRecordset
{
public:
	CHROperationRecordset(void);
	~CHROperationRecordset(void);

public:
	//得到指定时间段的操作记录信息
	CString GetOpertionRecord(CString StartTime,CString EndTime);
};
