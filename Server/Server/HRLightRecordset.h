#pragma once
#include "dbrecordset.h"

class CHRLightRecordset :
	public CDBRecordset
{
public:
	CHRLightRecordset(void);
	~CHRLightRecordset(void);

	//根据灯的名称来得到灯的历史记录
	CString GetLightRecord(CString LightName);
};
