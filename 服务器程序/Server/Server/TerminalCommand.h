#pragma once
#include "dbcommand.h"

class CTerminalCommand :
	public CDBCommand
{
public:
	CTerminalCommand(void);
	~CTerminalCommand(void);

	BOOL AddNewInformation(CString  x, CString temp);
};
