#pragma once
#include "dbrecordset.h"

#define U8 unsigned char
#define LIGHTINFOLEN sizeof(lightInfo)
typedef struct lightInfo
{
	U8 areaID[2];
	U8 areaName[20];
	U8 terminalName[20];
	U8 roadName[20];
	U8 lightID[16];
	U8 lightName[50];
	bool mainStatus;
	bool assitStatus;
	U8 update;
	float current;
}lightInfo, *pLightInfo;

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
	//取得指定路下所有灯的名字和ID
	char * GetAllLightsOfRoad(CString roadID);
	//更新一条路的信息(电流值)
	BOOL UpdateRoadCurrent(CString roadID,U8 roadNum,U8 currentValue[2]);
	//更新一个终端下八路的状态
	BOOL UpdateRoadsStatus(CString terminalID,U8 status);

private:
	char m_allLights[LIGHTINFOLEN*1000];
};
