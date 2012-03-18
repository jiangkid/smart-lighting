#pragma once
#include "dbrecordset.h"

#define U8  unsigned char
class CLightRecordset :
	public CDBRecordset
{
public:
	CLightRecordset(void);
	~CLightRecordset(void);

	//���ݵƵ�ID�������õƵ�����
	 BOOL SetLightName(CString LightID,CString LightName);

	 BOOL UpdateLightInfo(CString lightID,U8 cmdObj,U8 actionType[4]);

	 BOOL UpdateoneLightInfo(CString lightID,U8 cmdObj,U8 actionType[4]);
	 //���ݵƵ�ID�����鿴�Ƶ�����
	 CString GetLightName(CString LightID);

	//���ݵƵ�ID���õƵ�״̬
	BOOL SetLightStatus(CString LightID,BOOL LightStatus);

	//�õ�ָ���Ƶ�״̬
	CString GetLightStatus(CString LightID);

	//����ID���鿴�Ƶ����
	int GetGroupNum(CString LightID);

	//����ID�����õƵ����
	BOOL SetGoupNum(CString LightID,int GroupNum);

	////�õ����еƵ�ID,���Ƽ�������·��ID��ȫ���Ƶ�����
	CString GetAllLightsAndCount();
};
