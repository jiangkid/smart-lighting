#pragma once

#include "winsock2.h"
#include <process.h>

#define   BUFFER_SIZE    1024*4                      //I/O请求缓冲区大小

#define   MAX_THREAD  2                               //I/O的服务线程数量

#define WAIT4THREAD_MILLISECS 3000         //表示线程A在等待线程B触发某事件对象时，最多只等待3秒钟

#define  WM_USER_CLIENT WM_USER+1

#define CLIENT_CONN_REQ 0
#define CLIENT_ACCEPT 1
#define CLIENT_REJECT 2
#define CLIENT_DISCONNECT 3
#define CLIENT_FAIL_CLOSE 4

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
}PER_IO_DATA,*LPPER_IO_DATA;

//完成键结构体，单句柄数据，对应每个服务套接口—每个连接
typedef struct _CONN_CTX
{
	SOCKET                   sockAccept;                 //该连接的服务器端服务套接口
	LPPER_IO_DATA        pPerIOData;           //指向该连接的I/O操作信息
	struct         _CONN_CTX *pPrec;              //用于形成服务器当前所有连接信息的双向链表，
	struct         _CONN_CTX *pNext;              //分别指向链表中的前一个节点和后一个节点
	_CONN_CTX() {pPerIOData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));};
	~_CONN_CTX(){free(pPerIOData);};
}CONN_CTX, *LPCONN_CTX;


class CIOCPServer
{
public:
	CIOCPServer(void);
	~CIOCPServer(void);

	BOOL SrartServer();

	BOOL StopServer();
	
	CString  strSend;    //发送的字符串

	CString strRecv;    //接收的字符串

	SOCKET m_sdListen;                                                           //服务器的监听套接口

protected:
	HANDLE   m_hIOCP;                                                            //完成端口句柄
	HANDLE m_hThreadList[MAX_THREAD+2];    //线程数组

	HANDLE m_evtSvrToStop;                                                   //用于管理线程停止服务器
	HANDLE m_evtThreadLanched;                                          //用于通知线程已成功启动
	CRITICAL_SECTION m_CriticalSection;                                //用于线程互斥访问临界资源
	UINT m_nSvcThreadNum;                                                  //用于保存服务器运行时实际启动的服务线程数

	LPCONN_CTX m_ptrConnCtxHead;                                     //双向链表，用于保存服务器所有连接信息
	ServerState m_ServerState;                                                 //用于保存服务器的当前状态
	

	//该函数负责将系统中的所有成员变量全部复位，并释放运行态时占用的资源
	void ResetAll();
	
	//完成端口操作函数 
	HANDLE CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads);
	BOOL AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey);

	LPCONN_CTX CreateConnCtx(SOCKET sockAccept,HANDLE hIOCP);
	void ConnListAdd(LPCONN_CTX lpConnCtx);
	void ConnListRemove(LPCONN_CTX lpConnCtx);
	void ConnListClear();
	
private:
	LPPER_IO_DATA lpPerIOData;
	LPOVERLAPPED pOverLapped;
	LPCONN_CTX lpConnCtx;
	DWORD dwIOSize;
	static DWORD WINAPI ListenThread(LPVOID pParam);
	static DWORD WINAPI ServiceThread(LPVOID pParam);
	static DWORD WINAPI AdminThread(LPVOID pParam);
};

extern CIOCPServer     *g_pIOCPServer;


