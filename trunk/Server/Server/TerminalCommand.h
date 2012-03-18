#pragma once
#include "dbcommand.h"

class CTerminalCommand :
	public CDBCommand
{
public:
	CTerminalCommand(void);
	~CTerminalCommand(void);

	BOOL AddTerminal(CString TerminalID, CString TerminalName);//,CString Roads
	BOOL AddTerminalID(CString TerminalID);
	BOOL AddTerminalID(CString areaID,CString terminalID);
private:
	int GetMaxID();
};
