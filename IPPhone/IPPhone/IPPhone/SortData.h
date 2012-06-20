#pragma once

#define DELAY_BUFFER 24
#define NUM_BUFFER 32

class CSortData
{
public:
	void Play(char *pBuffer,int iLen);
	void ReceiveData(char *pBuffer,int iLen);
	CSortData();
	virtual ~CSortData();
protected:
	char *m_pBuffer[NUM_BUFFER];
	unsigned __int32 m_iFill[NUM_BUFFER];
	unsigned __int32 m_iLast;

	char m_cOut[1024];
};
