#pragma once
#include "dbcommand.h"

class CLigntCommand :
	public CDBCommand
{
public:
	CLigntCommand(void);
	~CLigntCommand(void);

	BOOL AddNewInformation(CString  x, CString temp);
};
