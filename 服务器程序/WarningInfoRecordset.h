#pragma once
#include "dbrecordset.h"

class CWarningInfoRecordset :
	public CDBRecordset
{
public:
	CWarningInfoRecordset(void);
	~CWarningInfoRecordset(void);
	
	//得到指定时间段的警告消息记录
	CString GetWaringInfo(CString StartTime,CString EndTime);
};
