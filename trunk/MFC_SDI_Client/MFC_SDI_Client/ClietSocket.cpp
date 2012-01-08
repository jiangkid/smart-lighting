
#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ClietSocket.h"


HDR hdr;
USERINFO userInfo[9];
bool BGTrue = false;
bool BTTrue = false;
bool BRTrue = false;
bool BLTrue = false;
BOOL SOCKET_Select(SOCKET hSocket, int nTimeOut, BOOL bRead)
{
	fd_set fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket, &fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec  = 0;
	tv.tv_usec = nTimeOut;

	int iRet = 0;
	if ( bRead ) {
		iRet = select(0, &fdset, NULL , NULL, &tv);
	}else{
		iRet = select(0, NULL , &fdset, NULL, &tv);
	}

	if(iRet <= 0) {
		return FALSE;
	} else if (FD_ISSET(hSocket, &fdset)){
		return TRUE;
	}
	return FALSE;
}
DWORD WINAPI ConnectThreadFunc(LPVOID pParam)
{
	CMFC_SDI_ClientApp *pChatRoom = (CMFC_SDI_ClientApp *)pParam;
	ASSERT(pChatRoom != NULL);
	pChatRoom->m_ConnectSock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	if ( pChatRoom->m_ConnectSock == INVALID_SOCKET ) {
		AfxMessageBox(_T("�½�Socketʧ�ܣ�"));
		return FALSE;
	}
	if ( pChatRoom->m_port <= 0 || pChatRoom->m_port  > 65535 ) {
		AfxMessageBox(_T("��������ʵĶ˿ڣ�1 - 65535"));
		goto __Error_End;
	}
	char szIpAddr[16] = {0};
	USES_CONVERSION;
	strcpy_s(szIpAddr, 16, T2A(pChatRoom->m_ip));

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(pChatRoom->m_port);
	server.sin_addr.s_addr = inet_addr(pChatRoom->m_ip);
	if ( connect(pChatRoom->m_ConnectSock, (struct sockaddr *)&server,  sizeof(struct sockaddr)) == SOCKET_ERROR ) {
		AfxMessageBox(_T("����ʧ�ܣ������ԣ�"));
		goto __Error_End;
	}
	SendUserInfo(&hdr,&userInfo[0]);
	//send(pChatRoom->m_ConnectSock,(char*)pChatRoom->strNamePass.GetBuffer(),pChatRoom->strNamePass.GetLength(),0);
	while( TRUE) {
		if ( SOCKET_Select(pChatRoom->m_ConnectSock) ) {
			TCHAR szBuf[MAX_BUF_SIZE] = {0};
			int iRet = recv(pChatRoom->m_ConnectSock, (char *)szBuf, MAX_BUF_SIZE, 0);
			if( iRet > 0 )
			{	
				switch (szBuf[0])
					{
					case 'I':
						ChenkLogin(szBuf,iRet);
						break;
					case 'C':
						ChenkSet(szBuf,iRet);
						break;
					case 'M':
						ChenkModify(szBuf,iRet);
						break;
					case 'B':
						ChenkBGTRL(szBuf,iRet);
						break;
					case 'S':
						ChenkGetGID(szBuf,iRet);
						break;
					case 'L':
						ChenkInitInfo(szBuf,iRet);
						break;
					default:
						break;
					}
				}
			else
			{
				AfxMessageBox(_T("�������Ѿ��رգ�"));
				TerminateThread(theApp.h1,0);
				break;
			}
		}
		Sleep(500);
	}

__Error_End:
	closesocket(pChatRoom->m_ConnectSock);
	return TRUE;
}
void ChenkLogin(char* buff,int nRecvLength)
{
	char checkBuf[3];
	for (int i=0;i<nRecvLength;i++)
	{
		checkBuf[i]=buff[i];
	}
	if (buff[0]=='I'&&buff[1]=='0')
	{
		theApp.m_return=true;
	}
	else
		if (buff[0]=='I'&&buff[1]=='1')
		{
			closesocket(theApp.m_ConnectSock);
			AfxMessageBox(_T("�û�����������������������룡"));
			theApp.m_return=false;
			TerminateThread(theApp.h1,-1);
		}
}
void ChenkSet(char* buff,int nRecvLength)
{
	char checkBuf[3];
	for (int i=0;i<nRecvLength;i++)
	{
		checkBuf[i]=buff[i];
	}
	if (buff[0]=='C'&&buff[1]=='1')
	{
		AfxMessageBox("���óɹ�");
	}
	else
		if (buff[0]=='C'&&buff[1]=='0')
		{
			AfxMessageBox(_T("�˴β������ɹ�"));
		}
}
void ChenkModify(char* buff,int nRecvLength)
{
	char checkBuf[3];
	for (int i=0;i<nRecvLength;i++)
	{
		checkBuf[i]=buff[i];
	}
	if (checkBuf[0]=='M' && checkBuf[1]=='0')
	{
		AfxMessageBox(_T("�޸�����ɹ���"));
	}
	else if (checkBuf[0]=='M' && checkBuf[1]=='1')
	{
		AfxMessageBox(_T("�޸�����ʧ�ܣ������²�����"));
	}
}
void ChenkGetGID(char* buff,int nRecvLength)
{
	if (buff[0]=='S'&&buff[1]==0x30 && buff[2]=='1')
	{
		AfxMessageBox(_T("��ȡG��ַʧ�ܣ������»�ȡ��"));
	}
	else if(buff[0]=='S'&&buff[1]==0x30 && buff[2]=='0')
	{
		theApp.m_NumInfo.GNum=buff[3];
		for (int i=0;i<(theApp.m_NumInfo.GNum);i++)
		{	
			int m=0;
			for (int n=i*2+4;n<((i+1)*2+4);n++)
			{
				theApp.m_GInfo[i].GID[m]=buff[n];
				m++;
			}
		}
	}
	if (buff[0]=='S'&&buff[1]==0x31 && buff[2]=='1')
	{

		AfxMessageBox(_T("��ȡT�ĵ�ַʧ�ܣ������»�ȡ��"));

	}
	else if(buff[0]=='S'&&buff[1]==0x31 && buff[2]=='0')
	{
		theApp.m_NumInfo.TNum=buff[3];
		for (int i=0;i<(theApp.m_NumInfo.TNum);i++)
		{	
			int m=0;
			for (int n=i*4+4;n<((i+1)*4+4);n++)
			{
				theApp.m_TInfo[i].TID[m]=buff[n];
				m++;
			}
		}
	}
	if (buff[0]=='S'&&buff[1]==0x32 && buff[2]=='1')
	{
		AfxMessageBox(_T("��ȡ·��ַʧ�ܣ������»�ȡ��"));
	}
	else if (buff[0]=='S'&&buff[1]==0x32 && buff[2]=='0')
	{
		theApp.m_NumInfo.RNum=buff[3];
		for (int i=0;i<(theApp.m_NumInfo.TNum);i++)
		{	
			int m=0;
			for (int n=i*6+4;n<((i+1)*6+4);n++)
			{
				theApp.m_RInfo[i].RID[m]=buff[n];
				m++;
			}
		}

	}
	if (buff[0]=='S'&&buff[1]==0x33 && buff[2]=='1')
	{
		AfxMessageBox(_T("��ȡ�Ƶ�ַʧ�ܣ������»�ȡ��"));
	}
	else if (buff[0]=='S'&&buff[1]==0x33 && buff[2]=='0')
	{
		theApp.m_NumInfo.LNum=buff[3];
		for (int i=0;i<(theApp.m_NumInfo.LNum);i++)
		{	
			int m=0;
			for (int n=i*16+4;n<((i+1)*16+4);n++)
			{
				theApp.m_LInfo[i].LID[m]=buff[n];
				m++;
			}
		}
	}
	if (buff[0]=='S'&&buff[1]==0x29 && buff[2]=='1')
	{
		AfxMessageBox(_T("��ȡ�û�������رմ������»�ȡ��"));
	}
	else if (buff[0]=='S'&&buff[1]==0x29 && buff[2]=='0')
	{
		theApp.m_NumInfo.UserNum=buff[3];
		int n(0);
		for (int i=4;buff[i]!='#';i++)
		{	
			int m(0);
			if (buff[i]=='<')
			{
				n++;
				for(;buff[i+1]!='>';i++)
				{
					userInfo[n].UserName[m]=buff[i+1];
					m++;
				}
			}
		}
	}
	if(buff[0]=='S'&&buff[1]==0x39 && buff[2]=='1')
	{
		AfxMessageBox(_T("GPRS���ò��ɹ��������ԣ�"));
	}
	else if (buff[0]=='S'&&buff[1]==0x39 && buff[2]=='0')
	{
		AfxMessageBox(_T("GPRS���óɹ���"));
	}
	if(buff[0]=='S'&&buff[1]==0x38 && buff[2]=='1')
	{
		AfxMessageBox(_T("�ն����ò��ɹ��������ԣ�"));
	}
	else if (buff[0]=='S'&&buff[1]==0x38 && buff[2]=='0')
	{
		AfxMessageBox(_T("�ն����óɹ���"));
	}
	if(buff[0]=='S'&&buff[1]==0x37 && buff[2]=='1')
	{
		AfxMessageBox(_T("֧·���ò��ɹ��������ԣ�"));
	}
	else if (buff[0]=='S'&&buff[1]==0x37 && buff[2]=='0')
	{
		AfxMessageBox(_T("֧·���óɹ���"));
	}
	if(buff[0]=='S'&&buff[1]==0x36 && buff[2]=='1')
	{
		AfxMessageBox(_T("�������ò��ɹ��������ԣ�"));
	}
	else if (buff[0]=='S'&&buff[1]==0x36 && buff[2]=='0')
	{
		AfxMessageBox(_T("�������óɹ���"));
	}

}
void SendUserInfo(LPHDR hdr,LPUSERINFO userInfo)
{
	hdr->dataCheck='I';
	hdr->dataLen=(u_short)sizeof(USERINFO);
	char m_buf[MAX_BUF_SIZE];
	ZeroMemory(m_buf,MAX_BUF_SIZE);
	memcpy(m_buf, (char*)hdr, HEADLEN);	
	memcpy(m_buf + HEADLEN, (char*)userInfo, sizeof(USERINFO));
	int nRet = send(theApp.m_ConnectSock, m_buf, HEADLEN + hdr->dataLen, 0);
	if (SOCKET_ERROR == nRet )
	{
		AfxMessageBox(_T("SendPacket����ʧ�ܡ�"));
	}
}
void ChenkBGTRL(char* buff,int nRecvLength)
{
	if (buff[0]=='B'&&buff[1]=='G'&&buff[2]=='1')
	{
		BGTrue = true;
		AfxMessageBox(_T("���GIDʧ�ܣ������²�����"));
	}
	if (buff[0]=='B'&&buff[1]=='T'&&buff[2]=='1')
	{
		BTTrue = true;
		AfxMessageBox(_T("���TIDʧ�ܣ������²�����"));
	}
	if (buff[0]=='B'&&buff[1]=='R'&&buff[2]=='1')
	{
		BRTrue = true;
		AfxMessageBox(_T("���RIDʧ�ܣ������²�����"));
	}
	if (buff[0]=='B'&&buff[1]=='L'&&buff[2]=='1')
	{
		BLTrue = true;
		AfxMessageBox(_T("���LIDʧ�ܣ������²�����"));
	}
}
void ChenkInitInfo(char* buff,int nRecvLength)
{
	HWND m_wnd = theApp.m_WaitDlg.GetSafeHwnd();
	if (buff[0]=='L'&&buff[1]=='G'&&buff[2]=='0')
	{
		theApp.m_InitTrue=true;
		SendMessage(m_wnd,WM_CLOSE,0,0);
	}
	if (buff[0]=='L'&&buff[1]=='G'&&buff[2]=='1')
	{

	}





// 	if (buff[0]=='L')
// 	{
// 		HWND m_wnd = theApp.m_WaitDlg.GetSafeHwnd();
// 		SendMessage(m_wnd,WM_CLOSE,0,0);
// 	}
}