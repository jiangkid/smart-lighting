#pragma once

//���ݿ�����ģ��
//��Ҫ�Ƕ�ADO��Connection����ĳ��÷����ķ�װ

class CDBConnection
{
public:
	CDBConnection(void);
	~CDBConnection(void);

	BOOL m_Actived;                     //�����ж����ݿ�������Ƿ��Ѿ���
	void Close();                             //�ر����ݵ�����
	BOOL Open(CString CnnStr);  //�����ݿ������,

	_ConnectionPtr  m_pConnection;
};

extern CDBConnection *g_pDBConnection;
