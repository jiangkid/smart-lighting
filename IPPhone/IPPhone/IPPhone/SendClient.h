#pragma once
#include "afxsock.h"

#include "MyWaveIn.h"
#include "Interface.h"

class CSendClient :
	public CAsyncSocket
{
public:
	CSendClient(CMyWaveIn *pIn,CInterface *pInterface);
	virtual ~CSendClient();

public:
	BOOL m_bConnect;

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);

protected:
	char *m_pBuffer;
	CMyWaveIn *m_pIn;
	CInterface *m_pInterFace;
};
