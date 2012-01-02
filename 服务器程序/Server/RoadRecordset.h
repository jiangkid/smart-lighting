#pragma once
#include "dbrecordset.h"

class CRoadRecordset :
	public CDBRecordset
{
public:
	CRoadRecordset(void);
	~CRoadRecordset(void);

	//加载Road表中的所有数据
	BOOL LoadRoadData();

	//根据ID来设置路的名称
	BOOL SetRoadName(CString RoadID,CString RoadName);

	//根据ID来查看路的名称
	CString GetRoadName(CString RoadID);
};
