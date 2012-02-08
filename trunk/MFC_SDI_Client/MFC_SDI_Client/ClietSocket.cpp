
#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ClietSocket.h"
#include "LightView.h"
#include "MainFrm.h"
#include "GTRLView.h"
#include "MFC_SDI_ClientDoc.h"
HDR hdr;
USERINFO userInfo[9];
IintInfo m_InitInfo;
LInfo    m_InitLInfo;
bool BGTrue = false;
bool BTTrue = false;
bool BRTrue = false;
bool BLTrue = false;
extern CMFC_SDI_ClientApp theApp;

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
		AfxMessageBox(_T("新建Socket失败！"));
		return FALSE;
	}
	if ( pChatRoom->m_port <= 0 || pChatRoom->m_port  > 65535 ) {
		AfxMessageBox(_T("请输入合适的端口：1 - 65535"));
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
		AfxMessageBox(_T("连接失败，请重试！"));
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
					case 0x2F:
						CheckBack((unsigned char*)szBuf,iRet);
						break;
					default:
						break;
					}
				}
			else
				{
				HWND m_wnd = theApp.m_WaitDlg.GetSafeHwnd();
				SendMessage(m_wnd,WM_CLOSE,0,0);
				AfxMessageBox(_T("服务器已经关闭！"));
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
/************************************************************************************
功能:判断用户登录是否成功
返回的格式:I 0/1#
*************************************************************************************/
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
			AfxMessageBox(_T("用户名或者密码错误，请重新输入！"));
			theApp.m_return=false;
			TerminateThread(theApp.h1,-1);
		}
}
/************************************************************************************
功能:判断创建用户是否成功
返回的格式:C 0/1#
*************************************************************************************/
void ChenkSet(char* buff,int nRecvLength)
{
	char checkBuf[3];
	for (int i=0;i<nRecvLength;i++)
	{
		checkBuf[i]=buff[i];
	}
	if (buff[0]=='C'&&buff[1]=='1')
	{
		AfxMessageBox("设置成功");
	}
	else
		if (buff[0]=='C'&&buff[1]=='0')
		{
			AfxMessageBox(_T("此次操作不成功"));
		}
}
/************************************************************************************
功能:判断修改用户密码是否成功
返回的格式:M 0/1#
*************************************************************************************/
void ChenkModify(char* buff,int nRecvLength)
{
	char checkBuf[3];
	for (int i=0;i<nRecvLength;i++)
	{
		checkBuf[i]=buff[i];
	}
	if (checkBuf[0]=='M' && checkBuf[1]=='0')
	{
		AfxMessageBox(_T("修改密码成功！"));
	}
	else if (checkBuf[0]=='M' && checkBuf[1]=='1')
	{
		AfxMessageBox(_T("修改密码失败，请重新操作！"));
	}
}
/************************************************************************************
功能:判断BGTRLID与信息绑定是否成功
返回的格式:S Ox30-0x39 0/1 0x00-0xFF(表示个数)+ID#
*************************************************************************************/
void ChenkGetGID(char* buff,int nRecvLength)
{
	if (buff[0]=='S'&&buff[1]==0x30 && buff[2]=='1')
	{
		AfxMessageBox(_T("获取G地址失败，请重新获取！"));
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

		AfxMessageBox(_T("获取T的地址失败，请重新获取！"));

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
		AfxMessageBox(_T("获取路地址失败，请重新获取！"));
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
		AfxMessageBox(_T("获取灯地址失败，请重新获取！"));
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
		AfxMessageBox(_T("获取用户名，请关闭窗口重新获取！"));
	}
	else if (buff[0]=='S'&&buff[1]==0x29 && buff[2]=='0')
	{
		theApp.m_NumInfo.UserNum=buff[3];
		int n(0);
		for (int i=4;buff[i]!='#';i++)
		{				
			if (buff[i]=='<')
			{
				int m(0);
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
		AfxMessageBox(_T("GPRS设置不成功，请重试！"));
	}
	else if (buff[0]=='S'&&buff[1]==0x39 && buff[2]=='0')
	{
		AfxMessageBox(_T("GPRS设置成功！"));
	}
	if(buff[0]=='S'&&buff[1]==0x38 && buff[2]=='1')
	{
		AfxMessageBox(_T("终端设置不成功，请重试！"));
	}
	else if (buff[0]=='S'&&buff[1]==0x38 && buff[2]=='0')
	{
		AfxMessageBox(_T("终端设置成功！"));
	}
	if(buff[0]=='S'&&buff[1]==0x37 && buff[2]=='1')
	{
		AfxMessageBox(_T("支路设置不成功，请重试！"));
	}
	else if (buff[0]=='S'&&buff[1]==0x37 && buff[2]=='0')
	{
		AfxMessageBox(_T("支路设置成功！"));
	}
	if(buff[0]=='S'&&buff[1]==0x36 && buff[2]=='1')
	{
		AfxMessageBox(_T("单灯设置不成功，请重试！"));
	}
	else if (buff[0]=='S'&&buff[1]==0x36 && buff[2]=='0')
	{
		AfxMessageBox(_T("单灯设置成功！"));
	}

}
/************************************************************************************
功能:发送用户登录信息
返回的格式：2个结构体（头结构+用户信息结构）
*************************************************************************************/
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
		AfxMessageBox(_T("SendUserInfo数据失败。"));
	}
}
/************************************************************************************
功能:判断BGTRL 获取ID失败
返回的格式:BG/T/L/R 1+ID#
*************************************************************************************/
void ChenkBGTRL(char* buff,int nRecvLength)
{
	if (buff[0]=='B'&&buff[1]=='G'&&buff[2]=='1')
	{
		BGTrue = true;
		AfxMessageBox(_T("添加GID失败，请重新操作！"));
	}
	if (buff[0]=='B'&&buff[1]=='T'&&buff[2]=='1')
	{
		BTTrue = true;
		AfxMessageBox(_T("添加TID失败，请重新操作！"));
	}
	if (buff[0]=='B'&&buff[1]=='R'&&buff[2]=='1')
	{
		BRTrue = true;
		AfxMessageBox(_T("添加RID失败，请重新操作！"));
	}
	if (buff[0]=='B'&&buff[1]=='L'&&buff[2]=='1')
	{
		BLTrue = true;
		AfxMessageBox(_T("添加LID失败，请重新操作！"));
	}
}
void ChenkInitInfo(char* buff,int nRecvLength)
{
	
	HWND m_wnd = theApp.m_WaitDlg.GetSafeHwnd();
	if (buff[0]=='L'&&buff[1]=='0'&&buff[2]=='L')
	{
		SendMessage(m_wnd,WM_CLOSE,0,0);
	}
	if (buff[0]=='L'&&buff[1]=='1')
	{
		theApp.m_InitTrue=true;
		//SendMessage(m_wnd,WM_CLOSE,0,0);
		SendMessage(m_wnd,WM_DESTROY,0,0);
	}
	if (buff[0]=='L'&&buff[1]=='0'&&buff[2]=='G')
	{
		ZeroMemory(&m_InitInfo.GNum,sizeof(int));
		m_InitInfo.GNum=buff[3];
		int nGCount(0);
		for (int i=4;buff[i]!='#';i++)
		{
			if(buff[i]=='<')
			{
				int m(0);
				for (;buff[i+1]!='>';i++)
				{
					m_InitInfo.m_InitGInfo[nGCount].GName[m]=buff[i+1];
					m++;
				}
				i+=1;
			}
			if (buff[i]=='{')
			{
				int n(0);
				for (;buff[i+1]!='}';i++)
				{
					m_InitInfo.m_InitGInfo[nGCount].GID[n]=buff[i+1];
					n++;
				}
				i+=1;
				nGCount++;
			}
		}
		char buffer[3]={'L','T','#'};
		send(theApp.m_ConnectSock,buffer,3,0);
	}
	if (buff[0]=='L'&&buff[1]=='0'&&buff[2]=='T')
	{
		ZeroMemory(&m_InitInfo.TNum,sizeof(int));
		m_InitInfo.TNum=buff[3];
		int nTCount(0);
		for (int i=4;buff[i]!='#';i++)
		{
			if (buff[i]=='<')
			{
				int m(0);
				for (;buff[i+1]!='>';i++)
				{
					m_InitInfo.m_InitTInfo[nTCount].TName[m]=buff[i+1];
					m++;
				}
				i+=1;
			}
			if (buff[i]=='{')
			{
				int n(0);
				for (;buff[i+1]!='}';i++)
				{
					m_InitInfo.m_InitTInfo[nTCount].TID[n]=buff[i+1];
					n++;
				}
				i+=1;
			}
			if (buff[i]=='(')
			{
				int k(0);
				for (;buff[i+1]!=')';i++)
				{
					m_InitInfo.m_InitTInfo[nTCount].GID[k]=buff[i+1];
					k++;
				}
				i+=1;
				nTCount++;
			}
		}
		char buffer[3]={'L','R','#'};
		send(theApp.m_ConnectSock,buffer,3,0);
	}
	if (buff[0]=='L'&&buff[1]=='0'&&buff[2]=='R')
	{
		ZeroMemory(&m_InitInfo.RNum,sizeof(int));
		m_InitInfo.RNum=buff[3];
		int nRCount(0);
		for (int i=4;buff[i]!='#';i++)
		{
			if (buff[i]=='<')
			{
				int m(0);
				for (;buff[i+1]!='>';i++)
				{
					m_InitInfo.m_InitRInfo[nRCount].RName[m]=buff[i+1];
					m++;
				}
				i+=1;
			}
			if (buff[i]=='{')
			{
				int n(0);
				for (;buff[i+1]!='}';i++)
				{
					m_InitInfo.m_InitRInfo[nRCount].RID[n]=buff[i+1];
					n++;
				}
				i+=1;
			}
			if (buff[i]=='(')
			{
				int k(0);
				for (;buff[i+1]!=')';i++)
				{
					m_InitInfo.m_InitRInfo[nRCount].TID[k]=buff[i+1];
					k++;
				}
				i+=1;
				nRCount++;
			}
		}
		SendMessage(m_wnd,WM_CLOSE,0,0);
	}
}

void CheckBack(unsigned char* buff,int nRecvLength)
{
	HWND m_wnd = theApp.m_WaitDlg.GetSafeHwnd();
// 	POSITION curTemplatePos = theApp.GetFirstDocTemplatePosition();
// 	CDocTemplate *m_doc=theApp.GetNextDocTemplate(curTemplatePos); 
// 	curTemplatePos=m_doc->GetFirstDocPosition();
// 	CMFC_SDI_ClientDoc *m_pdoc=(CMFC_SDI_ClientDoc*)m_doc->GetNextDoc(curTemplatePos);
// 	curTemplatePos=m_pdoc->GetFirstViewPosition();
// 	CLightView *m_light=(CLightView*)m_pdoc->GetNextView(curTemplatePos);
	
	ZeroMemory(&m_InitLInfo,sizeof(LInfo));
	int m=0;
	for (int i=4;i<20;i++)
	{
		m_InitLInfo.LID[m]=buff[i];
		m++;
	}
	if (0x31 == buff[2])
	{
		AfxMessageBox(_T("操作失败！"));
		return;
	}
	if ((buff[20]==0xA3)&&(buff[21]==0xBD))
	{
		m_InitLInfo.LMainStatus[0]=buff[23];
		m_InitLInfo.LSecondStatus[0]=buff[25];
		SendMessage(m_wnd,WM_CLOSE,0,0);
	}
	if ((buff[20]==0xA1)&&(buff[21]==0xB1))
	{
		theApp.m_light->ChangeButtonOn();
	}
	if ((buff[20]==0xA3)&&(buff[21]==0xB1))
	{
		theApp.m_light->ChangeSecondButtonOn();
	}
	if ((buff[20]==0xA2)&&(buff[21]==0xB1))
	{
		theApp.m_light->ChangeSecondButtonOn();
	}
	if ((buff[20]==0xA1)&&(buff[21]==0xB2))
	{
		theApp.m_light->ChangeButtonOff();
	}
	if ((buff[20]==0xA3)&&(buff[21]==0xB2))
	{
		theApp.m_light->ChangeSecondButtonOff();
	}
	if ((buff[20]==0xA2)&&(buff[21]==0xB2))
	{
		theApp.m_light->ChangeSecondButtonOff();
	}
}
