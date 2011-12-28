#pragma once
#include "dbcommand.h"

class CRoadCommand :
	public CDBCommand
{
public:
	CRoadCommand(void);
	~CRoadCommand(void);

    BOOL	AddNewInformation(CString  x, CString temp);
};
