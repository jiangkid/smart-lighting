// GenericServer.h: interface for the GenericServer class.
//�����������  10��12��
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

	/*****��ʼ����״̬������ʼ����socket������󶨡���ɶ˿ڴ��������*****/
	BOOL Start(int nPort=5002, int nMaxConnections=2000, int nMaxFreeBuffers=200, int nMaxFreeContexts=100, int nInitialReads=4);
	/*****ֹͣ����*****/
	void Shutdown();
	/*****�ر�һ�����Ӻ͹ر���������*****/
	void CloseAConnection(CIOCPContext *pContext);
	void CloseAllConnection();
	/*****ȡ�õ�ǰ���ӵ�����*****/
	ULONG GetCurrentConnection();
	
protected:
	/*************************����*************************/
	/*****������ͷŻ���������*****/
	CIOCPBuffer *AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);
	/*****������ͷ��׽���������*****/
	CIOCPContext *AllocateContext(SOCKET s);
	void ReleaseContext(IOCPContext *pContext);
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
	BOOL PostSend(CIOCPContex *pContext,CIOCPBuffer *pBuffer);
	BOOL PostRecv(CIOCPContex *pContext,CIOCPBuffer *pBuffer);
	void HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError);
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
	CRITICAL_SECTION m_ConnectionListLock;
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
	BOOL m_bShutdown;//����֪ͨ�����߳��˳�
	BOOL m_bServerStarted;//��¼�������Ƿ�����

private:
	static DWORD WINAPI  _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI  _WorkThreadProc(LPVOID lpParam);
};

#endif // !defined(AFX_GENERICSERVER_H__9AD16B75_BA21_4561_ABE5_31541134C7F1__INCLUDED_)
