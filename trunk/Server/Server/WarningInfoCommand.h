#pragma once
#include "dbcommand.h"

class CWarningInfoCommand :
	public CDBCommand
{
public:
	CWarningInfoCommand(void);
	~CWarningInfoCommand(void);

	//���һ���µľ�����Ϣ
	BOOL AddWarningIfo(int TID,CString Time,CString ClassOfWarning,CString Info);

private:
	//�õ�WarningInfo��������ID
	int GetMaxIDOfWarningInfo();
};
