// GenericServer.h: interface for the GenericServer class.
//创建：吴红生  10月12日
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERICSERVER_H__9AD16B75_BA21_4561_ABE5_31541134C7F1__INCLUDED_)
#define AFX_GENERICSERVER_H__9AD16B75_BA21_4561_ABE5_31541134C7F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GenericServer  
{
public:
	GenericServer();
	virtual ~GenericServer();

	/*****开始服务：状态变量初始化、socket创建与绑定、完成端口创建与关联*****/
	BOOL Start(int nPort=5002, int nMaxConnections=2000, int nMaxFreeBuffers=200, int nMaxFreeContexts=100, int nInitialReads=4);
	/*****停止服务*****/
	void Shutdown();
	/*****关闭一个连接和关闭所有连接*****/
	void CloseAConnection(CIOCPContext *pContext);
	void CloseAllConnection();
	/*****取得当前连接的数量*****/
	ULONG GetCurrentConnection();
	
protected:
	/*************************函数*************************/
	/*****申请和释放缓冲区对象*****/
	CIOCPBuffer *AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);
	/*****申请和释放套接字上下文*****/
	CIOCPContext *AllocateContext(SOCKET s);
	void ReleaseContext(IOCPContext *pContext);
	/*****释放空闲缓冲区对象列表和空闲上下文对象列表*****/
	void FreeBuffers();
	void FreeContext();
	/*****向连接列表中添加一个连接******/
	BOOL AddAConnection(CIOCPContext *pContext);
	/*****插入和移除未决的接受请求******/
	BOOL InsertPendingAccept(CIOCPBuffer *pBuffer);
	BOOL RemovePendingAccept(CIOCPBuffer *pBuffer);
	/*****取得下一个要读取的******/
	CIOCPBuffer *GetNextReadBuffer(CIOCPContext *pContext, CIOCPBuffer *pBuffer );
	/*****投递IO、发送IO、接收IO******/
	BOOL PostAccept(CIOCPBuffer *pBuffer);
	BOOL PostSend(CIOCPContex *pContext,CIOCPBuffer *pBuffer);
	BOOL PostRecv(CIOCPContex *pContext,CIOCPBuffer *pBuffer);
	void HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError);
	/*****事件通知函数******/
	/*****建立一个新的连接******/
	virtual void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****一个连接关闭******/
	virtual void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****在一个连接上发生了错误******/
	virtual void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer,int nError);
	/*****一个连接上读操作完成******/
	virtual void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****一个连接上写操作完成******/
	virtual void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

	/*************************变量*************************/
	/*****记录空闲结构信息 ******/
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	int m_nFreeBufferCount;
	int m_nFreeContextCount;
	CRITICAL_SECTION m_FreeBufferListLock;
	CRITICAL_SECTION m_FreeContextListLock;
	/*****记录抛出的accept请求******/
	 CIOCPBuffer *m_pPendingAccepts;//抛出请求列表
	long m_nPendingAcceptCount;
	CRITICAL_SECTION m_PendingAcceptsLock;
	/*****记录连接列表******/
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	CRITICAL_SECTION m_ConnectionListLock;
	/*****用于投递的accept请求******/
	HANDLE m_hAcceptEvent;
	HANDLE m_hRepostEvent;
	LONG m_nRepostCount;
	int m_nPort;
	int m_nInitialAccepts;
	int m_nInitialReads;
	int m_nMaxAccepts;
	int m_nMaxSends;
	int m_nMaxFreeBuffers;
	int m_nMaxFreeContexts;
	int m_nMaxConnections;
	HANDLE m_hListenThread;
	HANDLE m_hCompletion;
	SOCKET m_sListen;
	LPFN_ACCEPTEX m_lpfnAcceptEx;  //AcceptEx函数地址
	BOOL m_bShutdown;//用于通知监听线程退出
	BOOL m_bServerStarted;//记录服务器是否启动

private:
	static DWORD WINAPI  _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI  _WorkThreadProc(LPVOID lpParam);
};

#endif // !defined(AFX_GENERICSERVER_H__9AD16B75_BA21_4561_ABE5_31541134C7F1__INCLUDED_)
