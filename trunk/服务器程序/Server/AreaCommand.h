#pragma once
#include "dbcommand.h"

class CAreaCommand :
	public CDBCommand
{
public:
	CAreaCommand(void);
	~CAreaCommand(void);

	//������������һ����¼
	BOOL AddArea(CString AreaID,CString AreaName);
	BOOL AddAreaID(long int AreaID);
private:
	int GetMaxID();

};
