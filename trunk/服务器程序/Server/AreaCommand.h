#pragma once
#include "dbcommand.h"

class CAreaCommand :
	public CDBCommand
{
public:
	CAreaCommand(void);
	~CAreaCommand(void);

	//������������һ����¼
	BOOL AddArea(CString AreaID,CString AreaName,CString Roads);

private:
	int GetMaxID();

};
