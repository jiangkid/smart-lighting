#pragma once
#include "genericserver.h"


class CClientServer :
	public CGenericServer
{
public:
	CClientServer(void);
	~CClientServer(void);
	/*****开始服务：状态变量初始化、socket创建与绑定、完成端口创建与关联*****/
	BOOL Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts,int nInitialAccepts,  int nInitialReads);

	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);


};

extern CClientServer *ClientServer;