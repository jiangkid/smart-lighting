#pragma once

#define BUFFER_SIZE		1024*3    //I/O
#define MAX_THREAD     4
#define WAIT4THREAD_MILLISECS 3000         //表示线程A在等待线程B触发某事件对象时，最多只等待3秒钟

typedef enum
{
	SERVER_RUNNING,SERVER_STOP
}ServerState;

//枚举类型，用于指示服务器I/O操作的类型
typedef enum _IO_OPER
{
	SVR_IO_READ,
	SVR_IO_WRITE,
}IO_OPER,*LPIO_OPER;

//扩展重叠结构体
typedef struct _OverLappedEx
{
	OVERLAPPED OverLapped;
	WSABUF     wbuf;                              //I/O操作的数据对象
	char       data[BUFFER_SIZE];           //实际的数据缓冲区
	IO_OPER    oper;                                //用于标志I/O操作的类型，IO_OPER枚举型，可以是SVR_IO_READ或者是SVR_IO_WRITE     
	DWORD      flags;                               //用于设定或者返回I/O操作的标志
}CIOCPBuffer,*LPCIOCPBuffer;

//完成键结构体，单句柄数据，对应每个服务套接口—每个连接
typedef struct _CONN_CTX
{
	SOCKET                   sockAccept;                 //该连接的服务器端服务套接口
	SOCKADDR_IN       addrAccept;
	LPCIOCPBuffer        pPerIOData;           //指向该连接的I/O操作信息
	struct         _CONN_CTX *pPrec;              //用于形成服务器当前所有连接信息的双向链表，
	struct         _CONN_CTX *pNext;              //分别指向链表中的前一个节点和后一个节点
	_CONN_CTX() {pPerIOData = (LPCIOCPBuffer)malloc(sizeof(CIOCPBuffer));};
	~_CONN_CTX(){free(pPerIOData);};
}CIOCPContext,*LPCIOCPContext;

class CGenericServer
{
public:
	CGenericServer(void);
	~CGenericServer(void);	


	/*****停止服务*****/
	//virtual void Shutdown();
	/*****关闭一个连接和关闭所有连接*****/
	void CloseAllConnection();
	/*****取得当前连接的数量*****/
	ULONG GetCurrentConnection();
	/*************************函数*************************/
	/*****申请和释放套接字上下文*****/
	CIOCPContext *AllocateContext(SOCKET s, SOCKADDR_IN  addraccept);
	void ReleaseContext(CIOCPContext *pContext);
    void ResetAll();
	/*****向连接列表中添加一个连接******/
	void  ConnListAdd(LPCIOCPContext lpConnCtx);
	void  ConnListRemove(LPCIOCPContext  lpConnCtx);
	void  ConnListClear();
	//创建I/O完成端口
	HANDLE CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads);
	//关联I/O完成端口
	BOOL AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey);
	BOOL Initialize_Socket(int nPort);//初始化监听套接字
	void InitializeBuffer(LPCIOCPBuffer pBuffer, IO_OPER  operation);
protected:	
	/*************************变量*************************/
	/*****记录空闲结构信息 ******/
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	LPCIOCPContext m_ptrConnCtxHead;                                     //双向链表，用于保存服务器所有连接信息
	
	CString  strSend;    //发送的字符串
	CString strRecv;    //接收的字符串

	int m_nFreeBufferCount;
	int m_nFreeContextCount;

	CRITICAL_SECTION m_FreeContextListLock;
	CRITICAL_SECTION  m_ConnectionListLock;// m_ConnectionListLock;

	/*****记录连接列表******/
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	int  m_nPort;

	
	HANDLE m_hThreadList[MAX_THREAD+2];		//线程数组
	UINT m_nSvrThreadNum;										//用于保存服务器运行时实际启动的服务线程数
	HANDLE m_hCompletion;
	SOCKET m_sListen;
	sockaddr_in addrLocal;

	BOOL m_bShutDown;				                 //用于通知监听线程退出
	BOOL m_bServerStarted;		                 //记录服务器是否启动	
	ServerState m_ServerState;                     //用于保存服务器的当前状态
};


