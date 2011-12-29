#pragma once
#include "dbrecordset.h"

class CLightRecordset :
	public CDBRecordset
{
public:
	CLightRecordset(void);
	~CLightRecordset(void);

	//���ݵƵ�ID�������õƵ�����
	 BOOL SetLightName(int ID,CString LightName);

	 //���ݵƵ�ID�����鿴�Ƶ�����
	 CString GetLightName(int ID);

	//���ݵƵ�ID���õƵ�״̬
	BOOL SetLightStatus(int ID,BOOL LightStatus);

	//�õ�ָ���Ƶ�״̬
	CString GetLightStatus(int ID);

	//����ID���鿴�Ƶ����
	int GetGroupNum(int ID);

	//����ID�����õƵ����
	BOOL SetGoupNum(int ID,int GroupNum);
};
