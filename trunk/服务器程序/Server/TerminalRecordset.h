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
	BOOL SetTerminalName(int TID,CString TerminalName);

	//根据ID查看终端的名称
	CString GetTerminalName(int TID);

}; 
