#pragma once

//数据库连接模块
//主要是对ADO的Connection对象的常用方法的封装

class CDBConnection
{
public:
	CDBConnection(void);
	~CDBConnection(void);

	BOOL m_Actived;                     //用于判断数据库的连接是否已经打开
	void Close();                             //关闭数据的连接
	BOOL Open(CString CnnStr);  //打开数据库的连接,

	_ConnectionPtr  m_pConnection;
};

extern CDBConnection *g_pDBConnection;
