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
	//根据路名获取该条路下的所有灯的名称
	CString GetLightNamesByRoadName(CString RoadName);
	//根据路名得到该条路下所有灯的ID
	CString GetLightIDsByRoadName(CString RoadName);
	//根据路的ID来获取灯的ID及其数目
	CString GetLightIDsAndCountByRoadID(CString roadID);
};
