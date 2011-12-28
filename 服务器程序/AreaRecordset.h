#pragma once
#include "dbrecordset.h"

class CAreaRecordset :
	public CDBRecordset
{
public:
	CAreaRecordset(void);
	~CAreaRecordset(void);

	//加载Area表中所有的数据
	BOOL LoadAreaData();

	//设置区域的ID
	void SetAreaID(int ID);

	//设置区域的名称
	void SetAreaName(CString AreaName);

public:

};
