#pragma once
#include "dbrecordset.h"

#define U8 unsigned char
#define LIGHTINFOLEN sizeof(lightInfo)
typedef struct lightInfo
{
	U8 areaID[2];
	U8 areaName[20];
	U8 terminalName[20];
	U8 roadName[20];
	U8 lightID[16];
	U8 lightName[50];
	bool mainStatus;
	bool assitStatus;
	U8 update;
	float current;
}lightInfo, *pLightInfo;

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
	//�õ�����·��ID,���Ƽ��������ն˵�ID��ȫ��·������
	CString GetAllRoadsAndCount();	
	//����ָ��·�����Ʋ�������ָ�����ն˽���һ�Զ�Ĺ�ϵ
	BOOL SetRoadNameAndIDTerminal(CString roadID,CString roadName,CString terminalID);
	//ȡ��ָ��·�����еƵ����ֺ�ID
	char * GetAllLightsOfRoad(CString roadID);
	//����һ��·����Ϣ(����ֵ)
	BOOL UpdateRoadCurrent(CString roadID,U8 roadNum,U8 currentValue[2]);
	//����һ���ն��°�·��״̬
	BOOL UpdateRoadsStatus(CString terminalID,U8 status);

private:
	char m_allLights[LIGHTINFOLEN*1000];
};
