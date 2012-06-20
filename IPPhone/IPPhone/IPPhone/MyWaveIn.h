#pragma once
#include "wavein.h"
#include "UdpSocket.h"

#include <afxmt.h>

class CMyWaveIn :
	public CWaveIn
{
public:
	CMyWaveIn(CUdpSocket *pTemp);
	~CMyWaveIn(void);

	void EnableSend(BOOL bSend);
	virtual void GetData(char *pBuffer,int iLen);

protected:
	CUdpSocket *m_pUdp;
	BOOL m_bSend;
	CCriticalSection m_soLock;
	char m_cBufOut[1024];
};
