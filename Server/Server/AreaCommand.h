#pragma once
#include "dbcommand.h"

class CAreaCommand :
	public CDBCommand
{
public:
	CAreaCommand(void);
	~CAreaCommand(void);

	//������������һ����¼
	BOOL AddArea(CString AreaID,CString AreaName);
	BOOL AddAreaID(CString AreaID);
	//������������������Ϣ
	BOOL AddAreaInfo(CString areaID,CString areaName,CString dutyArea,CString telephone,CString installTime,CString installLocation,CString userName);
private:
	int GetMaxID();

};
