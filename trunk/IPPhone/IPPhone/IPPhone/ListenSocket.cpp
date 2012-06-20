#include "StdAfx.h"
#include "ListenSocket.h"

CListenSocket::CListenSocket(CInterface *pTemp,CMyWaveIn *pIn,CUdpSocket *pUdp):
m_sopClient (NULL),
m_pIn(NULL),
m_pUdp(NULL)
{
	m_sopClient = new CClientSocket(pTemp,pIn,pUdp);
	m_pInterface = pTemp;
	m_pIn = pIn;
	m_pUdp = pUdp;
}

CListenSocket::~CListenSocket()
{
	delete m_sopClient;
}

void CListenSocket::OnAccept(int nErrorCode) 
{
	SOCKADDR add;
	int iLen;
	iLen = sizeof(SOCKADDR);
	CSocket soTemp;
	if (m_sopClient->m_bConnect )
	{
		TRACE("Someone connect but i have talk with someon.\n");
		soTemp.Accept (*this);
		soTemp.Close ();
		return ;
	}

	if (!Accept(*m_sopClient,&add,&iLen))
	{
		TRACE("Accetp error.\n");
		return ;
	}

	m_sopClient->m_bConnect = TRUE;

	CSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClient()
{
	m_sopClient->Close ();
	m_sopClient->m_bConnect = FALSE;
}
