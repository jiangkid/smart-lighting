#pragma once
#define SIZE_AUDIO_FRAME 960
#define SIZE_AUDIO_PACKED 60

class CAudioCode
{
public:
	CAudioCode(void);
	virtual ~CAudioCode(void);

	BOOL EncodeAudioData(char *pin,int len,char* pout,int* lenr);
	BOOL DecodeAudioData(char *pin,int len,char* pout,int* lenr);
};

