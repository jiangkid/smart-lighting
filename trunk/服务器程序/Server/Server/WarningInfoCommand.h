#pragma once
#include "dbcommand.h"

class CWarningInfoCommand :
	public CDBCommand
{
public:
	CWarningInfoCommand(void);
	~CWarningInfoCommand(void);

	//添加一条新的警告信息
	BOOL AddWarningIfo(int TID,CString Time,CString ClassOfWarning,CString Info);

private:
	//得到WarningInfo表中最大的ID
	int GetMaxIDOfWarningInfo();
};
