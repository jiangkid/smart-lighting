#pragma once
#include "dbcommand.h"

class CHROperationCommand:
	public CDBCommand
{
public:
	CHROperationCommand(void);
	~CHROperationCommand(void);

	//添加一条新的操作历史记录
	BOOL AddOperationRecord(CString Time,CString Oper);

private:
	//得到WarningInfo表中最大的ID
	int GetMaxIDOfHROperation();
};
