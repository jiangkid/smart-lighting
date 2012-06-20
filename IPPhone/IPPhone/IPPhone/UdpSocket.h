#pragma once
#include <afxsock.h>

class CUdpSocket :
	public CAsyncSocket
{
public:
	CUdpSocket(void);
	~CUdpSocket(void);

public:
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

	void SetIp(CString ip);
	BOOL CloseSocket();
	BOOL m_bIni;
	BOOL Ini();

protected:
	struct Frame *m_pFrame;
	CString m_ip;
	char m_cBuffer[1024];
};
