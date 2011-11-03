#pragma once

#define BUFFER_SIZE		1024*3    //I/O
#define MAX_THREAD     4
#define WAIT4THREAD_MILLISECS 3000         //��ʾ�߳�A�ڵȴ��߳�B����ĳ�¼�����ʱ�����ֻ�ȴ�3����

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
}CIOCPBuffer,*LPCIOCPBuffer;

//��ɼ��ṹ�壬��������ݣ���Ӧÿ�������׽ӿڡ�ÿ������
typedef struct _CONN_CTX
{
	SOCKET                   sockAccept;                 //�����ӵķ������˷����׽ӿ�
	SOCKADDR_IN       addrAccept;
	LPCIOCPBuffer        pPerIOData;           //ָ������ӵ�I/O������Ϣ
	struct         _CONN_CTX *pPrec;              //�����γɷ�������ǰ����������Ϣ��˫������
	struct         _CONN_CTX *pNext;              //�ֱ�ָ�������е�ǰһ���ڵ�ͺ�һ���ڵ�
	_CONN_CTX() {pPerIOData = (LPCIOCPBuffer)malloc(sizeof(CIOCPBuffer));};
	~_CONN_CTX(){free(pPerIOData);};
}CIOCPContext,*LPCIOCPContext;

class CGenericServer
{
public:
	CGenericServer(void);
	~CGenericServer(void);	


	/*****ֹͣ����*****/
	//virtual void Shutdown();
	/*****�ر�һ�����Ӻ͹ر���������*****/
	void CloseAllConnection();
	/*****ȡ�õ�ǰ���ӵ�����*****/
	ULONG GetCurrentConnection();
	/*************************����*************************/
	/*****������ͷ��׽���������*****/
	CIOCPContext *AllocateContext(SOCKET s, SOCKADDR_IN  addraccept);
	void ReleaseContext(CIOCPContext *pContext);
    void ResetAll();
	/*****�������б������һ������******/
	void  ConnListAdd(LPCIOCPContext lpConnCtx);
	void  ConnListRemove(LPCIOCPContext  lpConnCtx);
	void  ConnListClear();
	//����I/O��ɶ˿�
	HANDLE CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads);
	//����I/O��ɶ˿�
	BOOL AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey);
	BOOL Initialize_Socket(int nPort);//��ʼ�������׽���
	void InitializeBuffer(LPCIOCPBuffer pBuffer, IO_OPER  operation);
protected:	
	/*************************����*************************/
	/*****��¼���нṹ��Ϣ ******/
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	LPCIOCPContext m_ptrConnCtxHead;                                     //˫���������ڱ������������������Ϣ
	
	CString  strSend;    //���͵��ַ���
	CString strRecv;    //���յ��ַ���

	int m_nFreeBufferCount;
	int m_nFreeContextCount;

	CRITICAL_SECTION m_FreeContextListLock;
	CRITICAL_SECTION  m_ConnectionListLock;// m_ConnectionListLock;

	/*****��¼�����б�******/
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	int  m_nPort;

	
	HANDLE m_hThreadList[MAX_THREAD+2];		//�߳�����
	UINT m_nSvrThreadNum;										//���ڱ������������ʱʵ�������ķ����߳���
	HANDLE m_hCompletion;
	SOCKET m_sListen;
	sockaddr_in addrLocal;

	BOOL m_bShutDown;				                 //����֪ͨ�����߳��˳�
	BOOL m_bServerStarted;		                 //��¼�������Ƿ�����	
	ServerState m_ServerState;                     //���ڱ���������ĵ�ǰ״̬
};


