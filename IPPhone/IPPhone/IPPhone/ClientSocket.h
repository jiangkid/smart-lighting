#pragma once
#include "UdpSocket.h"

#include "Interface.h"
#include "MyWaveIn.h"

class CClientSocket :
	public CSocket
{
public:
	CClientSocket(CInterface *pInterFace,CMyWaveIn *pIn,CUdpSocket *pUdp);
	virtual ~CClientSocket();

public:
	BOOL m_bConnect;
	char *m_pBuffer;

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

protected:
	CInterface *m_pInterface;
	CMyWaveIn *m_pIn;
	CUdpSocket *m_pUdp;
};
