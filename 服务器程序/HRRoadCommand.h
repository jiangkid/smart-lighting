#pragma once
#include "dbcommand.h"

class CHRRoadCommand :
	public CDBCommand
{
public:
	CHRRoadCommand(void);
	~CHRRoadCommand(void);

	//���һ���µ�·����ʷ��¼
	BOOL AddHistoryRecordOfRoad(CString Time,BOOL RoadStatus);

	//�õ���ǰ���е����ID
	int GetMaxID();
};
