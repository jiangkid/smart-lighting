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
	BOOL AddLightID(CString RoadID,CString LightID);
private:
	int GetMaxID();
};
