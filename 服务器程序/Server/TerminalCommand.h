#pragma once
#include "dbcommand.h"

class CTerminalCommand :
	public CDBCommand
{
public:
	CTerminalCommand(void);
	~CTerminalCommand(void);

	BOOL AddTerminal(CString TerminalID, CString TerminalName,CString Roads);

private:
	int GetMaxID();
};
