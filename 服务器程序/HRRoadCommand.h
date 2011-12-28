#pragma once
#include "dbcommand.h"

class CHRRoadCommand :
	public CDBCommand
{
public:
	CHRRoadCommand(void);
	~CHRRoadCommand(void);

	//添加一条新的路的历史记录
	BOOL AddHistoryRecordOfRoad(CString Time,BOOL RoadStatus);

	//得到当前表中的最大ID
	int GetMaxID();
};
