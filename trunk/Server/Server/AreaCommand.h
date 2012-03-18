#pragma once
#include "dbcommand.h"

class CAreaCommand :
	public CDBCommand
{
public:
	CAreaCommand(void);
	~CAreaCommand(void);

	//向区域表中添加一条记录
	BOOL AddArea(CString AreaID,CString AreaName);
	BOOL AddAreaID(CString AreaID);
	//向区域表中添加区域信息
	BOOL AddAreaInfo(CString areaID,CString areaName,CString dutyArea,CString telephone,CString installTime,CString installLocation,CString userName);
private:
	int GetMaxID();

};
