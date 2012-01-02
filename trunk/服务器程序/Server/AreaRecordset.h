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

	//根据区域的名称来查看区域下的远程终端
	CString GetTerminalsByAreaName(CString AreaName);
};
