#pragma once

class CDBConnection
{
public:
	//���ݿ������Ƿ��Ѵ�
	BOOL m_Actived;
	//�ر����ݿ�
	void Close();
	//�����ݿ�����
	BOOL Open();

	_ConnectionPtr m_pConn;

public:
	CDBConnection(void);
	~CDBConnection(void);
};
