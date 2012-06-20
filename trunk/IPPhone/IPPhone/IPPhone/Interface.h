#pragma once

class CMyWaveIn;
class CWaveOut;
class CListenSocket;
class CSendClient;
class CUdpSocket;
class CRecSocket;

class CInterface
{
public:
	virtual void TalkBeClose();
	virtual void TalkStart(CString ip);
	virtual BOOL IsConnect(CString ip);
	virtual void TalkOnConnect(BOOL bRe);

	BOOL Ini();
	BOOL End();
	BOOL Start(CString ip);

	CInterface();
	virtual ~CInterface();
protected:
	void BeClose();
	void ConnectResult(int nErrorCode);
protected:
	BOOL m_bIni;
	BOOL m_bWork;
	CString m_sIp;

	CMyWaveIn *m_pIn;
	CListenSocket *m_sopListen;
	CSendClient *m_sopSend;
	CUdpSocket *m_pUdp;
	CRecSocket *m_pRec;
	friend class CSendClient;
	friend class CClientSocket;
};
