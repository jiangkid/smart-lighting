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
	//����·����ȡ����·�µ����еƵ�����
	CString GetLightNamesByRoadName(CString RoadName);
	//����·���õ�����·�����еƵ�ID
	CString GetLightIDsByRoadName(CString RoadName);
	//����·��ID����ȡ�Ƶ�ID������Ŀ
	CString GetLightIDsAndCountByRoadID(CString roadID);
};
