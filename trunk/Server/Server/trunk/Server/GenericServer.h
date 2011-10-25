#pragma once


#define BUFFER_SIZE		1024*4    //I/O
#define MAX_THREAD      4
/*****CIOCPBuffer结构用来描述per-I/O数据，即缓冲区数据，它包含了在套接字上处理IO操作的必要信息*****/
struct CIOCPBuffer
{
	WSAOVERLAPPED overlapped;
	SOCKET sClient;							//AcceptEx接收的客户方套接字
	char *buff;								// IO操作使用的缓冲区
	int nLen;								//buff	缓冲区的大小
	ULONG nSequenceNumber;					//此IO的序列号
	int nOperation;							//	操作类型
#define OP_ACCEPT		1
#define OP_WRITE		2
#define OP_READ			3
	CIOCPBuffer *pNext;
};

/***** 客户的上下文对象，即per-Handle数据，它包含了套接字信息，用CIOCPContext结构来描述 *****/
/*****相关功能：服务器程序每接收到一个新的连接，就为新连接创建客户上下文对象以记录客户信息*****/
struct CIOCPContext
{
	SOCKET s;								//套接字句柄
	sockaddr_in addrLocal;					//连接的本地地址
	sockaddr_in addrRemote;					//连接的远程地址
	BOOL bClosing;							//套接字是否关闭
	int nOutstandingRecv;					//此套接字上抛出的重叠操作的数量
	int nOutstandingSend;					//
	ULONG nReadSequence;					//安排给接收的下一个序列号
	ULONG nCurrentSequence;					//当前要读的序列号
	CIOCPBuffer *pOutOfOrderReads;			//记录没有按顺序完成的读IO
	CRITICAL_SECTION Lock;					//保护这个结构
	CIOCPContext *pNext;
};

class CGenericServer
{
public:
	CGenericServer(void);
	~CGenericServer(void);	


	/*****停止服务*****/
	//virtual void Shutdown();
	/*****关闭一个连接和关闭所有连接*****/
	void CloseAConnection(CIOCPContext *pContext);
	void CloseAllConnection();
	/*****取得当前连接的数量*****/
	ULONG GetCurrentConnection();

//protected:
	/*************************函数*************************/
	/*****申请和释放缓冲区对象*****/
	CIOCPBuffer *AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);
	/*****申请和释放套接字上下文*****/
	CIOCPContext *AllocateContext(SOCKET s);
	void ReleaseContext(CIOCPContext *pContext);
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
	BOOL PostSend(CIOCPContext *pContext,CIOCPBuffer *pBuffer);
	BOOL PostRecv(CIOCPContext *pContext,CIOCPBuffer *pBuffer);
	
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

	//创建I/O完成端口
	HANDLE CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads);

	//关联I/O完成端口
	BOOL AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey);

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
	CRITICAL_SECTION  m_ConnectionListLock;// m_ConnectionListLock;
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
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs;//GetAcceptExSockaddrs函数地址
	BOOL m_bShutDown;//用于通知监听线程退出
	BOOL m_bServerStarted;//记录服务器是否启动
};

