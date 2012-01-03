#pragma once
#include "dbrecordset.h"

class CTerminalRecordset :
	public CDBRecordset
{
public:
	CTerminalRecordset(void);
	~CTerminalRecordset(void);

	//加载Terminal表中的所有数据
	BOOL LoadTerminalData();

	//根据终端的ID来设置终端的名称
	BOOL SetTerminalName(CString TerminalID,CString TerminalName);

	//根据ID查看终端的名称
	CString GetTerminalName(CString TerminalID);

	//根据终端的ID来获取终端下所有路的ID
	CString GetRoadIDsByTerminalID(CString TerminalID);

	//根据终端的ID来获取终端下所有路的名称
	CString GetRoadNamesByTerminalID(CString TerminalID);

}; 
