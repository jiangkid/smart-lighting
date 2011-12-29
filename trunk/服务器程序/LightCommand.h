#pragma once
#include "dbcommand.h"

class CLightCommand :
	public CDBCommand
{
public:
	CLightCommand(void);
	~CLightCommand(void);

	BOOL AddNewInformation(CString  x, CString temp);
};
