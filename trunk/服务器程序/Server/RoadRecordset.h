#pragma once
#include "dbrecordset.h"

class CRoadRecordset :
	public CDBRecordset
{
public:
	CRoadRecordset(void);
	~CRoadRecordset(void);

	//����Road���е���������
	BOOL LoadRoadData();

	//����ID������·������
	BOOL SetRoadName(CString RoadID,CString RoadName);

	//����ID���鿴·������
	CString GetRoadName(CString RoadID);
};
