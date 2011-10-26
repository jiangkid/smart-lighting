#pragma once
#include "genericserver.h"
#include "ServerDlg.h"

class CClientServer :	public CGenericServer, public CServerDlg
{
public:
	CClientServer(void);
	~CClientServer(void);
	/*****开始服务：状态变量初始化、socket创建与绑定、完成端口创建与关联*****/
	BOOL Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts,int nInitialAccepts,  int nInitialReads);

	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);

	void HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError);

	/*****建立一个新的连接******/
   void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****一个连接关闭******/
	void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****在一个连接上发生了错误******/
	void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer,int nError);
	/*****一个连接上读操作完成******/
	void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****一个连接上写操作完成******/
	void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

};

extern CClientServer *ClientServer;