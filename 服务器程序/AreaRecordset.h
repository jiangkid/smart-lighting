#pragma once
#include "dbrecordset.h"

class CAreaRecordset :
	public CDBRecordset
{
public:
	CAreaRecordset(void);
	~CAreaRecordset(void);

	//����Area�������е�����
	BOOL LoadAreaData();

	//���������ID
	void SetAreaID(int ID);

	//�������������
	void SetAreaName(CString AreaName);

public:

};
