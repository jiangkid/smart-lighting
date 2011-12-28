#pragma once
#include "dbrecordset.h"

class CLightRecordset :
	public CDBRecordset
{
public:
	CLightRecordset(void);
	~CLightRecordset(void);

	//根据灯的ID号来设置灯的名称
	 BOOL SetLightName(int ID,CString LightName);

	 //根据灯的ID号来查看灯的名称
	 CString GetLightName(int ID);

	//根据灯的ID设置灯的状态
	BOOL SetLightStatus(int ID,BOOL LightStatus);

	//得到指定灯的状态
	CString GetLightStatus(int ID);

	//根据ID来查看灯的组号
	int GetGroupNum(int ID);

	//根据ID来设置灯的组号
	BOOL SetGoupNum(int ID,int GroupNum);
};
