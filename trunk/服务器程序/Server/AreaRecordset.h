#pragma once
#include "dbrecordset.h"

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
};
