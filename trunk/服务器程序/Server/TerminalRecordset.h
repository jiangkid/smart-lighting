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

	//根据终端的ID来获取终端下所有路的ID和数目
	CString GetRoadIDsAndCountByTerminalID(CString TerminalID);

	//得到所有的终端的ID和名称及其所属区域的ID和终端的数目
	CString GetAllTerminalsAndCount();

	//设置指定终端的名称并将它和指定的区域建立一对多的关系
	BOOL SetTerminalNameAndIDArea(CString terminalID,CString terminalName,CString areaID);

}; 
