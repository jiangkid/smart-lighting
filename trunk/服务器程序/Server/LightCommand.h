#pragma once
#include "dbcommand.h"

class CLightCommand :
	public CDBCommand
{
public:
	CLightCommand(void);
	~CLightCommand(void);

	BOOL AddLight(CString LightID,CString LightName);
	BOOL AddLightID(CString LightID);
private:
	int GetMaxID();
};
