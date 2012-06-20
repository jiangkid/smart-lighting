#pragma once
#include "afxsock.h"

#include "SortData.h"

class CRecSocket :
	public CSocket
{
public:
	CRecSocket(void);
	~CRecSocket(void);

public:
	void Play(char *pBuffer, int iLen);
	BOOL CloseSocket();
	BOOL Ini();

public:
	virtual void OnReceive(int nErrorCode);

public:
	CSortData m_sort;
protected:
	BOOL m_bIni;
	struct Frame *m_pFrame;
	char m_cBuffer[1024];
	int m_iLen;
	char m_cOut[1024];
};
