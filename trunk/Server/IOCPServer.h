#pragma once

#include "winsock2.h"
#include <process.h>

#define   BUFFER_SIZE    1024*4                      //I/O���󻺳�����С

#define   MAX_THREAD  2                               //I/O�ķ����߳�����

#define WAIT4THREAD_MILLISECS 3000         //��ʾ�߳�A�ڵȴ��߳�B����ĳ�¼�����ʱ�����ֻ�ȴ�3����

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

//ö�����ͣ�����ָʾ������I/O����������
typedef enum _IO_OPER
{
	SVR_IO_READ,
	SVR_IO_WRITE,
}IO_OPER,*LPIO_OPER;

//��չ�ص��ṹ��
typedef struct _OverLappedEx
{
	OVERLAPPED OverLapped;
	WSABUF     wbuf;                              //I/O���������ݶ���
	char       data[BUFFER_SIZE];           //ʵ�ʵ����ݻ�����
	IO_OPER    oper;                                //���ڱ�־I/O���������ͣ�IO_OPERö���ͣ�������SVR_IO_READ������SVR_IO_WRITE     
	DWORD      flags;                               //�����趨���߷���I/O�����ı�־
}PER_IO_DATA,*LPPER_IO_DATA;

//��ɼ��ṹ�壬��������ݣ���Ӧÿ�������׽ӿڡ�ÿ������
typedef struct _CONN_CTX
{
	SOCKET                   sockAccept;                 //�����ӵķ������˷����׽ӿ�
	LPPER_IO_DATA        pPerIOData;           //ָ������ӵ�I/O������Ϣ
	struct         _CONN_CTX *pPrec;              //�����γɷ�������ǰ����������Ϣ��˫������
	struct         _CONN_CTX *pNext;              //�ֱ�ָ�������е�ǰһ���ڵ�ͺ�һ���ڵ�
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
	
	CString  strSend;    //���͵��ַ���

	CString strRecv;    //���յ��ַ���

	SOCKET m_sdListen;                                                           //�������ļ����׽ӿ�

protected:
	HANDLE   m_hIOCP;                                                            //��ɶ˿ھ��
	HANDLE m_hThreadList[MAX_THREAD+2];    //�߳�����

	HANDLE m_evtSvrToStop;                                                   //���ڹ����߳�ֹͣ������
	HANDLE m_evtThreadLanched;                                          //����֪ͨ�߳��ѳɹ�����
	CRITICAL_SECTION m_CriticalSection;                                //�����̻߳�������ٽ���Դ
	UINT m_nSvcThreadNum;                                                  //���ڱ������������ʱʵ�������ķ����߳���

	LPCONN_CTX m_ptrConnCtxHead;                                     //˫���������ڱ������������������Ϣ
	ServerState m_ServerState;                                                 //���ڱ���������ĵ�ǰ״̬
	

	//�ú�������ϵͳ�е����г�Ա����ȫ����λ�����ͷ�����̬ʱռ�õ���Դ
	void ResetAll();
	
	//��ɶ˿ڲ������� 
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


