#pragma once
#include "dbcommand.h"

class CHRLightCommand :
	public CDBCommand
{
public:
	CHRLightCommand(void);
	~CHRLightCommand(void);

	//添加一条新的灯的历史记录
	BOOL AddHistoryRecordOfLight(CString Name,CString Time,BOOL LightStatus);

private:
	int GetMaxID();
};
