#pragma once
#include "dbrecordset.h"

#define   U8                       unsigned char    
#define   AREAINFOLEN    sizeof(areaInfo)   

typedef struct _areaInfo 
{
	U8 areaID[2];
	U8 areaName[20];
	U8 dutyArea[20];
	U8 telephone[11];
	U8 installTime[17];
	U8 installLocation[20];
}areaInfo,*pAreaInfo;


class CAreaRecordset :
	public CDBRecordset
{
public:
	CAreaRecordset(void);
	~CAreaRecordset(void);
	
	//根据区域的ID设置区域的名称和区域下的路
	BOOL SetAreaByAreaID(CString AreaID,CString AreaName,CString Terminals);

	//根据区域的ID来获取该区域下的终端ID
	CString GetTerminalIDsByAreaID(CString AreaID);

	//根据区域名获得区域的ID
	CString GetAreaIDByAreaName(CString AreaName);

	//根据区域的ID来获取该区域下的终端名称
	CString GetTerminalNamesByAreaID(CString AreaID);
	/********函数功能：获得所有的区域ID*********/
	CString GetAllAreaIDAndCount();
	//根据区域的ID来获取该区域下的终端ID和终端数目
	CString GetTerminalIDsAndCountByAreaID(CString AreaID);
	//根据区域的ID和用户名来设置区域的信息以及和相关用户关联
	BOOL SetAreaInfoByAreaID(CString areaID,CString areaName,CString dutyArea,CString telephone,CString installTime,CString installLocation,CString userName);
	//获得所有的区域的ID及其名称和总数目
	CString GetAllAreaAndCount();
	//根据区域的ID(areaID)获得与之关联的用户名(userName)
	CString GetUserNamebyAreaID(CString areaID);

	//根据区域的ID来获得区域的信息
	char *GetAreaInfoByAreaID(CString areaID);

private:
	char mAreaInfo[90];
};
