////////////////////////////////////////////////////
// created at 2011/10/23 by 丁亮
////封装一个CInitSock类来管理Winsock库
////////////////////////////////////////////////////

#pragma once
#include <WinSock2.h>
#pragma comment(lib,"WS2_32")


class CInitSock
{
public:
	CInitSock();

	~CInitSock();
};
