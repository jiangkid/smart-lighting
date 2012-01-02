#pragma once
#include "dbcommand.h"

class CHRTerminalCommand :
	public CDBCommand
{
public:
	CHRTerminalCommand(void);
	~CHRTerminalCommand(void);

public:
	BOOL AddHistoryRecordOfTerminal(CString TerminalID,CString Va,CString Vb,CString Vc,CString P,CString Ir1,CString Ir2,CString Ir3,CString Ir4,
		CString	 Ir5,CString Ir6,CString Ir7,CString Ir8);

private:
	int GetMaxID();
};
