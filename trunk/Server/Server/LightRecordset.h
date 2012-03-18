#pragma once
#include "dbrecordset.h"

#define U8  unsigned char
class CLightRecordset :
	public CDBRecordset
{
public:
	CLightRecordset(void);
	~CLightRecordset(void);

	//根据灯的ID号来设置灯的名称
	 BOOL SetLightName(CString LightID,CString LightName);

	 BOOL UpdateLightInfo(CString lightID,U8 cmdObj,U8 actionType[4]);

	 BOOL UpdateoneLightInfo(CString lightID,U8 cmdObj,U8 actionType[4]);
	 //根据灯的ID号来查看灯的名称
	 CString GetLightName(CString LightID);

	//根据灯的ID设置灯的状态
	BOOL SetLightStatus(CString LightID,BOOL LightStatus);

	//得到指定灯的状态
	CString GetLightStatus(CString LightID);

	//根据ID来查看灯的组号
	int GetGroupNum(CString LightID);

	//根据ID来设置灯的组号
	BOOL SetGoupNum(CString LightID,int GroupNum);

	////得到所有灯的ID,名称及其所属路的ID和全部灯的数量
	CString GetAllLightsAndCount();
};
