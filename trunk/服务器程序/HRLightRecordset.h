#pragma once
#include "dbrecordset.h"

class CHRLightRecordset :
	public CDBRecordset
{
public:
	CHRLightRecordset(void);
	~CHRLightRecordset(void);

	//���ݵƵ��������õ��Ƶ���ʷ��¼
	CString GetLightRecord(CString LightName);
};
