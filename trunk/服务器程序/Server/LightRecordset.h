#pragma once
#include "dbrecordset.h"

class CLightRecordset :
	public CDBRecordset
{
public:
	CLightRecordset(void);
	~CLightRecordset(void);

	//���ݵƵ�ID�������õƵ�����
	 BOOL SetLightName(CString LightID,CString LightName);

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
};
