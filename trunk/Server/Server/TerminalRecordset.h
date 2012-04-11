#pragma once
#include "dbrecordset.h"
#define U8  unsigned char
#define ROADINFOLEN sizeof(roadInfo)

typedef struct roadInfo
{
	U8 ternimalName[20];
	U8 roadName[20];
	U8 roadID[6];
	bool roadStatus;
	char update;
	float currentValue1;
	float currentValue2;
	float currentValue3;
	/*	U8 updateTime[19];*/
}roadInfo, *pRoadInfo;

class CTerminalRecordset :
	public CDBRecordset
{
public:
	CTerminalRecordset(void);
	~CTerminalRecordset(void);

	//����Terminal���е���������
	BOOL LoadTerminalData();

	//�����ն˵�ID�������ն˵�����
	BOOL SetTerminalName(CString TerminalID,CString TerminalName);

	//����ID�鿴�ն˵�����
	CString GetTerminalName(CString TerminalID);

	//�����ն˵�ID����ȡ�ն�������·��ID
	CString GetRoadIDsByTerminalID(CString TerminalID);

	//�����ն˵�ID����ȡ�ն�������·������
	CString GetRoadNamesByTerminalID(CString TerminalID);

	//�����ն˵�ID����ȡ�ն�������·��ID����Ŀ
	CString GetRoadIDsAndCountByTerminalID(CString TerminalID);

	//�õ����е��ն˵�ID�����Ƽ������������ID���ն˵���Ŀ
	CString GetAllTerminalsAndCount();

	//����ָ���ն˵����Ʋ�������ָ����������һ�Զ�Ĺ�ϵ
	BOOL SetTerminalNameAndIDArea(CString terminalID,CString terminalName,CString areaID);

	//ȡ��ָ��·�����еƵ����ֺ�ID
	char * GetRoadsOfTerminal(CString terminaID);

private:
	char m_allRoads[ROADINFOLEN*10];

}; 