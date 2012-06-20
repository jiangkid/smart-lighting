#pragma once
#ifndef _WAVEIN_H_
#define _WAVEIN_H_

#define WISA_POOR	8000
#define WISA_LOW	11025
#define WISA_NORMAL	22050
#define WISA_HIGH	44100

#define NUM_BUF 10
#define SIZE_AUDIO_FRAME 960
#define SIZE_AUDIO_PACKED 60
#include <mmsystem.h>

class CWaveIn
{
public:
	static DWORD WINAPI AudioInThreadProc(LPVOID lpParameter);
public:
	virtual void GetData(char *pBuffer,int iLen);

	BOOL StartRec();
	BOOL StopRec();

	inline DWORD GetInstance();
	inline WORD GetBit();
	inline DWORD GetSample();
	inline WORD GetChannel();

	inline void SetBit(WORD wBit);
	inline void SetSample(DWORD dwSample);
	inline void SetChannel(WORD wChannel);

	inline MMRESULT GetLastMMError();
	CString GetLastErrorString();

	CWaveIn();
	virtual ~CWaveIn();	
protected:
	BOOL OpenDev();
	BOOL CloseDev();

	BOOL StopThread();
	BOOL StartThread();

	BOOL PerPareBuffer();
	BOOL FreeBuffer();

	BOOL OpenRecord();
	BOOL CloseRecord();
protected:
	static DWORD s_dwInstance;
protected:
	WORD m_wChannel;
	DWORD m_dwSample;
	WORD m_wBit;

protected:
	HWAVEIN m_hIn;
	MMRESULT m_mmr;
	DWORD m_dwAudioInId;
	HANDLE m_hAudioIn;
	WAVEHDR* m_pHdr;

	BOOL m_bThreadStart;
	BOOL m_bDevOpen;
	BOOL m_bAllocBuffer;
	BOOL m_bRecord;
};

#endif