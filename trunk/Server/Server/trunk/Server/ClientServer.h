#pragma once
#include "genericserver.h"
#include "ServerDlg.h"

class CClientServer :	public CGenericServer, public CServerDlg
{
public:
	CClientServer(void);
	~CClientServer(void);
	/*****��ʼ����״̬������ʼ����socket������󶨡���ɶ˿ڴ��������*****/
	BOOL Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts,int nInitialAccepts,  int nInitialReads);

	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);

	void HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError);

	/*****����һ���µ�����******/
   void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****һ�����ӹر�******/
	void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****��һ�������Ϸ����˴���******/
	void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer,int nError);
	/*****һ�������϶��������******/
	void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	/*****һ��������д�������******/
	void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

};

extern CClientServer *ClientServer;