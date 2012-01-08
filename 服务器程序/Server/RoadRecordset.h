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

	//得到所有路的ID,名称及其所属终端的ID和全部路的数量
	CString GetAllRoadsAndCount();
	
	//设置指定路的名称并将它和指定的终端建立一对多的关系
	BOOL SetRoadNameAndIDTerminal(CString roadID,CString roadName,CString terminalID);
};
