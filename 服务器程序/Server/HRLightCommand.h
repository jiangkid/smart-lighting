#pragma once
#include "dbcommand.h"

class CHRLightCommand :
	public CDBCommand
{
public:
	CHRLightCommand(void);
	~CHRLightCommand(void);

	//���һ���µĵƵ���ʷ��¼
	BOOL AddHistoryRecordOfLight(CString Name,CString Time,BOOL LightStatus);

private:
	int GetMaxID();
};
