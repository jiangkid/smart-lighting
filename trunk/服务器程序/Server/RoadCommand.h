#pragma once
#include "dbcommand.h"

class CRoadCommand :
	public CDBCommand
{
public:
	CRoadCommand(void);
	~CRoadCommand(void);

    BOOL AddRoad(CString RoadID, CString RoadName);//,CString Lights

private:
	int GetMaxID();
};
