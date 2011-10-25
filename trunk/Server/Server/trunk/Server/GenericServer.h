#pragma once


#define BUFFER_SIZE		1024*4    //I/O
#define MAX_THREAD      4
/*****CIOCPBuffer�ṹ��������per-I/O���ݣ������������ݣ������������׽����ϴ���IO�����ı�Ҫ��Ϣ*****/
struct CIOCPBuffer
{
	WSAOVERLAPPED overlapped;
	SOCKET sClient;							//AcceptEx���յĿͻ����׽���
	char *buff;								// IO����ʹ�õĻ�����
	int nLen;								//buff	�������Ĵ�С
	ULONG nSequenceNumber;					//��IO�����к�
	int nOperation;							//	��������
#define OP_ACCEPT		1
#define OP_WRITE		2
#define OP_READ			3
	CIOCPBuffer *pNext;
};

/***** �ͻ��������Ķ��󣬼�per-Handle���ݣ����������׽�����Ϣ����CIOCPContext�ṹ������ *****/
/*****��ع��ܣ�����������ÿ���յ�һ���µ����ӣ���Ϊ�����Ӵ����ͻ������Ķ����Լ�¼�ͻ���Ϣ*****/
struct CIOCPContext
{
	SOCKET s;								//�׽��־��
	sockaddr_in addrLocal;					//���ӵı��ص�ַ
	sockaddr_in addrRemote;					//���ӵ�Զ�̵�ַ
	BOOL bClosing;							//�׽����Ƿ�ر�
	int nOutstandingRecv;					//���׽������׳����ص�����������
	int nOutstandingSend;					//
	ULONG nReadSequence;					//���Ÿ����յ���һ�����к�
	ULONG nCurrentSequence;					//��ǰҪ�������к�
	CIOCPBuffer *pOutOfOrderReads;			//��¼û�а�˳����ɵĶ�IO
	CRITICAL_SECTION Lock;					//��������ṹ
	CIOCPContext *pNext;
};

class CGenericServer
{
public:
	CGenericServer(void);
	~CGenericServer(void);	


	/*****ֹͣ����*****/
	//virtual void Shutdown();
	/*****�ر�һ�����Ӻ͹ر���������*****/
	void CloseAConnection(CIOCPContext *pContext);
	void CloseAllConnection();
	/*****ȡ�õ�ǰ���ӵ�����*****/
	ULONG GetCurrentConnection();

//protected:
	/*************************����*************************/
	/*****������ͷŻ���������*****/
	CIOCPBuffer *AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);
	/*****������ͷ��׽���������*****/
	CIOCPContext *AllocateContext(SOCKET s);
	void ReleaseContext(CIOCPContext *pContext);
	/*****�ͷſ��л����������б�Ϳ��������Ķ����б�*****/
	void FreeBuffers();
	void FreeContext();
	/*****�������б������һ������******/
	BOOL AddAConnection(CIOCPContext *pContext);
	/*****������Ƴ�δ���Ľ�������******/
	BOOL InsertPendingAccept(CIOCPBuffer *pBuffer);
	BOOL RemovePendingAccept(CIOCPBuffer *pBuffer);
	/*****ȡ����һ��Ҫ��ȡ��******/
	CIOCPBuffer *GetNextReadBuffer(CIOCPContext *pContext, CIOCPBuffer *pBuffer );
	/*****Ͷ��IO������IO������IO******/
	BOOL PostAccept(CIOCPBuffer *pBuffer);
	BOOL PostSend(CIOCPContext *pContext,CIOCPBuffer *pBuffer);
	BOOL PostRecv(CIOCPContext *pContext,CIOCPBuffer *pBuffer);
	
	/*****�¼�֪ͨ����******/
	/*****����һ���µ�����******/
	virtual void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****һ�����ӹر�******/
	virtual void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****��һ�������Ϸ����˴���******/
	virtual void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer,int nError);
	/*****һ�������϶��������******/
	virtual void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****һ��������д�������******/
	virtual void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

	//����I/O��ɶ˿�
	HANDLE CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads);

	//����I/O��ɶ˿�
	BOOL AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey);

	/*************************����*************************/
	/*****��¼���нṹ��Ϣ ******/
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	int m_nFreeBufferCount;
	int m_nFreeContextCount;
	CRITICAL_SECTION m_FreeBufferListLock;
	CRITICAL_SECTION m_FreeContextListLock;
	/*****��¼�׳���accept����******/
	CIOCPBuffer *m_pPendingAccepts;//�׳������б�
	long m_nPendingAcceptCount;
	CRITICAL_SECTION m_PendingAcceptsLock;
	/*****��¼�����б�******/
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	CRITICAL_SECTION  m_ConnectionListLock;// m_ConnectionListLock;
	/*****����Ͷ�ݵ�accept����******/
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
	LPFN_ACCEPTEX m_lpfnAcceptEx;  //AcceptEx������ַ
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs;//GetAcceptExSockaddrs������ַ
	BOOL m_bShutDown;//����֪ͨ�����߳��˳�
	BOOL m_bServerStarted;//��¼�������Ƿ�����
};

