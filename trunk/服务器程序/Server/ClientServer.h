#pragma once
#include "genericserver.h"
//#include "ServerDlg.h"

class CClientServer :public CGenericServer
{
public:
	CClientServer(void);
	~CClientServer(void);
	/*****开始服务：状态变量初始化、socket创建与绑定、完成端口创建与关联*****/
	BOOL Start(int nPort);
	BOOL StopServer();

	/****通信协议数据处理函数****/
	CString  FieldSet(CHAR * buffer);
	BOOL LogIn(LPCIOCPContext  pContext);   //登录检测函数
	CString DataCheck(CHAR * buffer);   //传输过来的数据判断
	BOOL CreateUser(CHAR * buffer);
	BOOL DeleteUser(CHAR * buffer);
	CString  GetLedStatus(CHAR* buffer);
	BOOL ChangePassword(CHAR* buffer);
	
	HANDLE m_evtSvrToStop;                      //用于管理线程停止服务器
	HANDLE m_evtThreadLanched;              //用于通知线程已成功启动
	HANDLE m_hListenThread;

	CDBConnection *m_conn;						 //数据库的连接
	
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);
	static DWORD WINAPI _AdminThread(LPVOID pParam);
	
	SOCKET m_sAccept;
	CIOCPContext  *pClient ;
};

//extern CClientServer *ClientServer;