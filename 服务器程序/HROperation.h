#pragma once
#include "dbcommand.h"

class CHROperationCommand:
	public CDBCommand
{
public:
	CHROperationCommand(void);
	~CHROperationCommand(void);

	//���һ���µĲ�����ʷ��¼
	BOOL AddOperationRecord(CString Time,CString Oper);

private:
	//�õ�WarningInfo��������ID
	int GetMaxIDOfHROperation();
};
