//
//�����ݿ������ݽ��з��ʺ����õķ�װ
//

#pragma once
#include "dataset.h"

class CServerDataSet :
	public CDataSet
{
public:
	CServerDataSet(void);
	~CServerDataSet(void);

	//��user&password���"user"�ж�Ӧ�ķ���
	CString GetUserName();
	void SetUserName(CString name);


};

extern CServerDataSet *g_pServerDataSet;
