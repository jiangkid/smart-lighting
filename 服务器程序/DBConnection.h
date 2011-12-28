#pragma once

class CDBConnection
{
public:
	//数据库连接是否已打开
	BOOL m_Actived;
	//关闭数据库
	void Close();
	//打开数据库连接
	BOOL Open();

	_ConnectionPtr m_pConn;

public:
	CDBConnection(void);
	~CDBConnection(void);
};
