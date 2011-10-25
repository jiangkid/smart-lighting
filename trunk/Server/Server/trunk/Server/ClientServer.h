#pragma once
#include "genericserver.h"


class CClientServer :
	public CGenericServer
{
public:
	CClientServer(void);
	~CClientServer(void);
	/*****��ʼ����״̬������ʼ����socket������󶨡���ɶ˿ڴ��������*****/
	BOOL Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts,int nInitialAccepts,  int nInitialReads);

	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);


};

extern CClientServer *ClientServer;