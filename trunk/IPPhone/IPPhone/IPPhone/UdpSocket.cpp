#include "StdAfx.h"
#include "UdpSocket.h"
#include "head.h"

CUdpSocket::CUdpSocket(void)
{
	m_pFrame = (struct Frame *)m_cBuffer;
	m_pFrame->iIndex = 0;
	m_bIni = FALSE;
}

CUdpSocket::~CUdpSocket(void)
{

}

BOOL CUdpSocket::Ini()
{
	if (m_bIni)
		return FALSE;

	if (!this->Create (NULL,SOCK_DGRAM))
	{
		return FALSE;
	};

	m_bIni = TRUE;
	return TRUE;
}

int CUdpSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	m_pFrame->iIndex ++;
	memcpy(m_cBuffer + sizeof(struct Frame),lpBuf,nBufLen);

	return CAsyncSocket::SendTo (m_pFrame, nBufLen + sizeof(struct Frame),TALK_REC_PORT,m_ip, nFlags);
}

BOOL CUdpSocket::CloseSocket()
{
	if (!m_bIni)
		return FALSE;

	CAsyncSocket::Close ();
	m_bIni = FALSE;
	return TRUE;
}

void CUdpSocket::SetIp(CString ip)
{
	m_ip = ip;
}

