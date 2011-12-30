#pragma once
#include "dbcommand.h"

class CAreaCommand :
	public CDBCommand
{
public:
	CAreaCommand(void);
	~CAreaCommand(void);

	//向区域表中添加一条记录
	BOOL AddArea(CString AreaID,CString AreaName,CString Roads);

private:
	int GetMaxID();

};
