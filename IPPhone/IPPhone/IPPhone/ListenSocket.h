#pragma once
#include "afxsock.h"

#include "ClientSocket.h"
#include "Interface.h"
#include "MyWaveIn.h"
#include "UdpSocket.h"

class CListenSocket :
	public CSocket
{

public:
	CListenSocket(CInterface *pTemp,CMyWaveIn *pIn,CUdpSocket *pUdp);
	virtual ~CListenSocket();

public:
	void CloseClient();

public:
	virtual void OnAccept(int nErrorCode);

protected:
	CClientSocket *m_sopClient;
	CInterface *m_pInterface;
	CMyWaveIn *m_pIn;
	CUdpSocket *m_pUdp;
};
