
#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ClietSocket.h"
#include "MainFrm.h"
#include "MFC_SDI_ClientDoc.h"
HDR hdr;
USERINFO userInfo[9];
IintInfo m_InitInfo;
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
		{
			AfxMessageBox(_T("连接失败，请重试！"));
			theApp.m_connected=TRUE;
			theApp.m_return=false;
		}
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
					case 'A':
						ChenkConnectAgain(szBuf,iRet);
						break;
					case 'Z':
						UnpackLightInfo(szBuf,iRet);
						break;
					case 'N':
						if (szBuf[1]=='1')
						{
							AfxMessageBox(_T("信息发送不成功！"));
							break;
						}
						if (szBuf[1]=='0')
						{
							CheckCtrlBackInfo(szBuf,iRet);
							break;
						}					
					case 'R':
						if (szBuf[1]==0x31)
						break;
						else
						{
							TranslateRInfo((U8*)szBuf,iRet);
							break;
						}
					case 'W':
						if (szBuf[1]==0x30)
						{
							UpdataZigbeeStatusInfo(szBuf,iRet);
							break;
						}
						if (szBuf[1]==0x31)
						{
							UpdataZigbeeCurrentInfo(szBuf,iRet);
							break;
						}
						if (szBuf[1]==0x33)
						{
							UpdataRoadCurrentInfo(szBuf,iRet);//路的电流
							break;
						}
						if (szBuf[1]==0x34)//路的状态
						{
							UpdataRoadStatusInfo(szBuf,iRet);
							break;
						}
					case 'G':
						GPRSLocalInfo(szBuf,iRet);
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
					theApp.m_connected=TRUE;
					theApp.m_return=false;
					TerminateThread(theApp.h1,0);
					break;
				}
		}
		Sleep(100);
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
	if (buff[0]=='C'&&buff[1]=='0')
	{
		AfxMessageBox("设置成功");
	}
	else
		if (buff[0]=='C'&&buff[1]=='1')
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
	if (buff[0]=='M' && buff[1]=='0')
	{
		AfxMessageBox(_T("修改密码成功！"));
	}
	else if (buff[0]=='M' && buff[1]=='1')
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
		for (int i=0;i<(theApp.m_NumInfo.RNum);i++)
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
			ZeroMemory(&theApp.m_LInfo[i],sizeof(LInfo));
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
		AfxMessageBox(_T("获取用户名失败，请关闭窗口重新获取！"));
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
	hdr->dataCheck[0]='I';
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
		//AfxMessageBox(_T("添加GID失败，请重新操作！"));
		CString str=_T("添加GPRSID失败，GPRSID：");
		str+=buff[3];
		str+=buff[4];
		theApp.m_pMainSetDlg->ShowUpdatatMessage(str);
	}
	if (buff[0]=='B'&&buff[1]=='T'&&buff[2]=='1')
	{
		//AfxMessageBox(_T("添加TID失败，请重新操作！"));
		CString str=_T("添加终端ID失败，终端ID：");
		str+=buff[3];
		str+=buff[4];
		str+=buff[5];
		str+=buff[6];
		theApp.m_pMainSetDlg->ShowUpdatatMessage(str);
	}
	if (buff[0]=='B'&&buff[1]=='R'&&buff[2]=='1')
	{
		//AfxMessageBox(_T("添加RID失败，请重新操作！"));
		CString str=_T("添加路ID失败，路ID：");
		str+=buff[3];
		str+=buff[4];
		str+=buff[5];
		str+=buff[6];
		str+=buff[7];
		str+=buff[8];
		theApp.m_pMainSetDlg->ShowUpdatatMessage(str);
	}
	if (buff[0]=='B'&&buff[1]=='L'&&buff[2]=='1')
	{
		//AfxMessageBox(_T("添加LID失败，请重新操作！"));
		CString str=_T("添加单灯ID失败，灯ID：");
		for (int i(0);i<16;i++)
		{
			str+=buff[i+3];
		}
		theApp.m_pMainSetDlg->ShowUpdatatMessage(str);
	}
}
void ChenkInitInfo(char* buff,int nRecvLength)
{
	
	HWND m_wnd = theApp.m_WaitDlg.GetSafeHwnd();
	if (buff[0]=='L'&&buff[1]=='0'&&buff[2]=='L')
	{
		SendMessage(m_wnd,WM_CLOSE,0,0);
		AfxMessageBox(_T("第一次登入请设置！"));
	}
	if (buff[0]=='L'&&buff[1]=='1')
	{
		theApp.m_InitTrue=true;
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
		Sleep(500);
		theApp.m_pFileView->FillFileView();
	}
}

void SendContrlInfo(LPHDR hdr,LPConTrlInfo contrlInfo)
{
	hdr->dataCheck[0]='N';
	hdr->dataLen=(u_short)sizeof(ConTrlInfo);
	char m_buf[MAX_BUF_SIZE];
	ZeroMemory(m_buf,MAX_BUF_SIZE);
	memcpy(m_buf, (char*)hdr, HEADLEN);	
	memcpy(m_buf + HEADLEN, (char*)contrlInfo, sizeof(ConTrlInfo));
	int nRet = send(theApp.m_ConnectSock, m_buf, HEADLEN + hdr->dataLen, 0);
	if (SOCKET_ERROR == nRet )
	{
		AfxMessageBox(_T("SendUserInfo数据失败。"));
	}
}
//Test测试用*******************//
void SendContrlInfo1(LPHDR hdr,LPConTrlInfo contrlInfo)
{
	hdr->dataLen=(u_short)sizeof(ConTrlInfo);
	char m_buf[MAX_BUF_SIZE];
	ZeroMemory(m_buf,MAX_BUF_SIZE);
	memcpy(m_buf, (char*)hdr, HEADLEN);	
	memcpy(m_buf + HEADLEN, (char*)contrlInfo, sizeof(ConTrlInfo));
	int nRet = send(theApp.m_ConnectSock, m_buf, HEADLEN + hdr->dataLen, 0);
	if (SOCKET_ERROR == nRet )
	{
		AfxMessageBox(_T("SendUserInfo数据失败。"));
	}
}
static int nRetPack(0);
void UnpackLightInfo(char* buffer, int Length)
{
	nRetPack++;	
	int i,j;
	i=buffer[1];
	j=buffer[2];
	if (nRetPack<i)
	{		
		if (buffer[2]==buffer[1])
		{
			memcpy(theApp.m_lightPack,buffer+3,Length-3);
		}
		else
			memcpy(theApp.m_lightPack+4093*(j-1),buffer+3,4096-3);
	}
	else
	{
		if (buffer[2]==buffer[1])
		{
			memcpy(theApp.m_lightPack,buffer+3,Length-3);
		}
		else
			memcpy(theApp.m_lightPack+4093*(j-1),buffer+3,4096-3);
		TranslateLInfo(theApp.m_lightPack);
	}
}
void TranslateLInfo(U8* buffer)
{
	nRetPack=0;
	int nHigh,nLower,nLCount;
	nHigh=buffer[0];
	nLower=buffer[1];
	nLCount=nHigh*255+nLower;
	for (int i=0;i<nLCount;i++)
	{
		theApp.m_ZigbeeInfo[i]=(ZigbeeInfo*)malloc(LENTH);
		ZeroMemory(theApp.m_ZigbeeInfo[i],LENTH);
		memcpy(theApp.m_ZigbeeInfo[i],buffer+i*LENTH+2,LENTH);
	}
	theApp.m_pLightListView->LightToView(nLCount);
	ZeroMemory(theApp.m_lightPack,1000*LENTH);
}
void TranslateRInfo(U8* buffer,int Length)
{
	int nLcont(0);
	nLcont=buffer[1];
	for (int i=0;i<nLcont;i++)
	{
		theApp.m_RoadListInfo[i]=(RoadListViewInfo*)malloc(RLENTH);
		ZeroMemory(theApp.m_RoadListInfo[i],RLENTH);
		memcpy(theApp.m_RoadListInfo[i],buffer+2+i*RLENTH,RLENTH);
	}
	theApp.m_pRoadView->RoadInfoToView(nLcont);
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->StartTimer(1);
}

void ChenkConnectAgain(char* buff,int nRecvLength)
{
	//ZeroMemory(&m_InitInfo.GNum,sizeof(int));
	ZeroMemory(&m_InitInfo.TNum,sizeof(int));
	ZeroMemory(&m_InitInfo.RNum,sizeof(int));
	theApp.m_pFileView->FillFileView();
}
void UpdataZigbeeStatusInfo(char* buff,int nRecvLength)
{
	CString str1=_T("");
	if (buff[2]==0x30)
	{
		return;
	}
	else
		if (buff[2]==0x31)
		{
			ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
			memcpy(pGetRInfo,buff+3,sizeof(ConTrlInfo));
			for (int i=0;i<16;i++)
			{
				str1+=pGetRInfo->m_ID[i];
			}
			for (int n=0;n<theApp.m_pLightListView->nCount;n++)
			{
				CString str2=_T("");
				for (int m=0;m<16;m++)
				{
					str2+=theApp.m_ZigbeeInfo[n]->LID[m];
				}
				if (strcmp(str1,str2)==0)
				{
					theApp.m_ZigbeeInfo[n]->Update|=0x80;
					if (pGetRInfo->m_CheckData[1]==0x01&&pGetRInfo->m_CheckData[3]==0x01)
					{
						theApp.m_ZigbeeInfo[n]->MainStatus=true;
						theApp.m_ZigbeeInfo[n]->AssistStatus=true;
					}else
					if (pGetRInfo->m_CheckData[1]==0x00&&pGetRInfo->m_CheckData[3]==0x00)
					{
						theApp.m_ZigbeeInfo[n]->MainStatus=false;
						theApp.m_ZigbeeInfo[n]->AssistStatus=false;
					}else
					if (pGetRInfo->m_CheckData[1]==0x01&&pGetRInfo->m_CheckData[3]==0x00)
					{
						theApp.m_ZigbeeInfo[n]->MainStatus=true;
						theApp.m_ZigbeeInfo[n]->AssistStatus=false;
					}else
					if (pGetRInfo->m_CheckData[1]==0x00&&pGetRInfo->m_CheckData[3]==0x01)
					{
						theApp.m_ZigbeeInfo[n]->MainStatus=false;
						theApp.m_ZigbeeInfo[n]->AssistStatus=true;
					}
					theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
					//theApp.m_pLightListView->LightToView(theApp.m_pLightListView->nCount);
				}
			}
			free(pGetRInfo);
		}
}
void UpdataZigbeeCurrentInfo(char* buff,int nRecvLength)
{
	CString str1=_T("");
	if (buff[2]==0x30)
	{
		return;
	}
	else
		if (buff[2]==0x31)
		{
			ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
			memcpy(pGetRInfo,buff+3,sizeof(ConTrlInfo));
			for (int i=0;i<16;i++)
			{
				str1+=pGetRInfo->m_ID[i];
			}
			for (int n=0;n<theApp.m_pLightListView->nCount;n++)
			{
				CString str2=_T("");
				for (int m=0;m<16;m++)
				{
					str2+=theApp.m_ZigbeeInfo[n]->LID[m];
				}
				if (strcmp(str1,str2)==0)
				{
					theApp.m_ZigbeeInfo[n]->Update|=0x40;
					float nCurrent=pGetRInfo->m_CheckData[2]*256+pGetRInfo->m_CheckData[3];
					theApp.m_ZigbeeInfo[n]->current=nCurrent;
					theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
				}
				//theApp.m_pLightListView->LightToView(theApp.m_pLightListView->nCount);
			}
			free(pGetRInfo);
		}
}
void CheckCtrlBackInfo(char* buff,int nRecvLength)
{
	CString str1=_T("");
	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
	memcpy(pGetRInfo,buff+3,sizeof(ConTrlInfo));
	for (int l=0;l<16;l++)
	{
		str1+=pGetRInfo->m_ID[l];
	}
	switch (pGetRInfo->m_OrderType[0])
	{
	case 0xA1:
		
		if (pGetRInfo->m_ActiveType[0]==0xB1)
		{
			theApp.m_DlgMainONStatus=true;
			for (int n=0;n<theApp.m_pLightListView->nCount;n++)
			{
				CString str2=_T("");
				for (int m=0;m<16;m++)
				{
					str2+=theApp.m_ZigbeeInfo[n]->LID[m];
				}
				if (strcmp(str1,str2)==0)
				{
					theApp.m_ZigbeeInfo[n]->MainStatus=true;
				}
				theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
			}
		}
		else
			if (pGetRInfo->m_ActiveType[0]==0xB2)
			{
				theApp.m_DlgMainOFFStatus=true;
				for (int n=0;n<theApp.m_pLightListView->nCount;n++)
				{
					CString str2=_T("");
					for (int m=0;m<16;m++)
					{
						str2+=theApp.m_ZigbeeInfo[n]->LID[m];
					}
					if (strcmp(str1,str2)==0)
					{
						theApp.m_ZigbeeInfo[n]->MainStatus=false;
					}
					theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
				}
			}
		break;
	case 0xA2:
		if (pGetRInfo->m_ActiveType[0]==0xB1)
		{
			theApp.m_DlgAssistONStatus=true;
			for (int n=0;n<theApp.m_pLightListView->nCount;n++)
			{
				CString str2=_T("");
				for (int m=0;m<16;m++)
				{
					str2+=theApp.m_ZigbeeInfo[n]->LID[m];
				}
				if (strcmp(str1,str2)==0)
				{
					theApp.m_ZigbeeInfo[n]->AssistStatus=true;
				}
				theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
			}
		}
		else
			if (pGetRInfo->m_ActiveType[0]=0xB2)
			{
				theApp.m_DlgAssistOFFStatus=true;
				for (int n=0;n<theApp.m_pLightListView->nCount;n++)
				{
					CString str2=_T("");
					for (int m=0;m<16;m++)
					{
						str2+=theApp.m_ZigbeeInfo[n]->LID[m];
					}
					if (strcmp(str1,str2)==0)
					{
						theApp.m_ZigbeeInfo[n]->AssistStatus=false;
					}
					theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
			}
		break;
	case 0xA3:
		if (pGetRInfo->m_ActiveType[0]==0xB1)
		{
			theApp.m_DlgDoubleONStatus=true;
			for (int n=0;n<theApp.m_pLightListView->nCount;n++)
			{
				CString str2=_T("");
				for (int m=0;m<16;m++)
				{
					str2+=theApp.m_ZigbeeInfo[n]->LID[m];
				}
				if (strcmp(str1,str2)==0)
				{
					theApp.m_ZigbeeInfo[n]->MainStatus=true;
					theApp.m_ZigbeeInfo[n]->AssistStatus=true;
					theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
				}
			}
		}
		else
			if (pGetRInfo->m_ActiveType[0]==0xB2)
			{
				theApp.m_DlgDoubleOFFStatus=true;
				for (int n=0;n<theApp.m_pLightListView->nCount;n++)
				{
					CString str2=_T("");
					for (int m=0;m<16;m++)
					{
						str2+=theApp.m_ZigbeeInfo[n]->LID[m];
					}
					if (strcmp(str1,str2)==0)
					{
						theApp.m_ZigbeeInfo[n]->MainStatus=false;
						theApp.m_ZigbeeInfo[n]->AssistStatus=false;
						theApp.m_pLightListView->UpdataOneLight(theApp.m_ZigbeeInfo[n]);
					}
					//theApp.m_pLightListView->LightToView(theApp.m_pLightListView->nCount);
				}
			}
		break;
	case 0x32:
		if (pGetRInfo->m_ActiveType[0]==0x12)
		{
			for (int l=0;l<theApp.m_pRoadView->nCount;l++)
			{
				CString str2=_T("");
				CString str3=_T("");
				for (int k(0);k<6;k++)
				{
					str2+=theApp.m_RoadListInfo[l]->m_RoadID[k];
				}
				for (int j(0);j<4;j++)
				{
					str3+=pGetRInfo->m_ID[j];
				}
				str3+=CharToCString(&pGetRInfo->m_CheckData[3],1);
				if (strcmp(str2,str3)==0)
				{
					theApp.m_RoadListInfo[l]->m_RoadStatus=false;
				}
				theApp.m_pRoadView->RoadInfoToView(theApp.m_pRoadView->nCount);
			}
		}
		if (pGetRInfo->m_ActiveType[0]==0x13)
		{
			for (int l=0;l<theApp.m_pRoadView->nCount;l++)
			{
				CString str2=_T("");
				CString str3=_T("");
				for (int k(0);k<6;k++)
				{
					str2+=theApp.m_RoadListInfo[l]->m_RoadID[k];
				}
				for (int j(0);j<4;j++)
				{
					str3+=pGetRInfo->m_ID[j];
				}
				str3+=CharToCString(&pGetRInfo->m_CheckData[3],1);
				if (strcmp(str2,str3)==0)
				{
					theApp.m_RoadListInfo[l]->m_RoadStatus=true;
				}
				theApp.m_pRoadView->RoadInfoToView(theApp.m_pRoadView->nCount);
			}
		}
		break;
	default:
		break;
		}
	free(pGetRInfo);
	}
}

void UpdataRoadCurrentInfo(char* buff,int nRecvLength)
{
	CString str1=_T("");
	CString str=_T("");
	if (buff[2]==0x30)
	{
		return;
	}
	else
		if (buff[2]==0x31)
		{
			ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
			memcpy(pGetRInfo,buff+3,sizeof(ConTrlInfo));
			for (int i=0;i<4;i++)
			{
				str1+=pGetRInfo->m_ID[i];
			}
			str1+=CharToCString(&pGetRInfo->m_CheckData[0],1);
			switch (pGetRInfo->m_CheckData[1])
			{
			case 0x01:
				for (int n=0;n<theApp.m_pRoadView->nCount;n++)
				{
					CString str2=_T("");
					for (int m=0;m<6;m++)
					{
						str2+=theApp.m_RoadListInfo[n]->m_RoadID[m];
					}
					if (strcmp(str1,str2)==0)
					{
						theApp.m_RoadListInfo[n]->nCurrent1=(float)(pGetRInfo->m_CheckData[2]*256+pGetRInfo
							->m_CheckData[3]);
						theApp.m_RoadListInfo[n]->m_Update|=0x40;
						theApp.m_pRoadView->UpdataOneRoad(theApp.m_RoadListInfo[n]);
					}
					else 
						return;
				}
				break;
			case 0x02:
				for (int n=0;n<theApp.m_pRoadView->nCount;n++)
				{
					CString str2=_T("");
					for (int m=0;m<6;m++)
					{
						str2+=theApp.m_RoadListInfo[n]->m_RoadID[m];
					}
					if (strcmp(str1,str2)==0)
					{
						theApp.m_RoadListInfo[n]->nCurrent2=(float)(pGetRInfo->m_CheckData[2]*256+pGetRInfo
							->m_CheckData[3]);
						theApp.m_RoadListInfo[n]->m_Update|=0x20;
						theApp.m_pRoadView->UpdataOneRoad(theApp.m_RoadListInfo[n]);
					}
					else 
						return;
				}
				break;
			case 0x03:
				for (int n=0;n<theApp.m_pRoadView->nCount;n++)
				{
					CString str2=_T("");
					for (int m=0;m<6;m++)
					{
						str2+=theApp.m_RoadListInfo[n]->m_RoadID[m];
					}
					if (strcmp(str1,str2)==0)
					{
						theApp.m_RoadListInfo[n]->nCurrent3=(float)(pGetRInfo->m_CheckData[2]*256+pGetRInfo
							->m_CheckData[3]);
						theApp.m_RoadListInfo[n]->m_Update|=0x10;
						theApp.m_pRoadView->UpdataOneRoad(theApp.m_RoadListInfo[n]);
					}
					else 
						return;
				}
				break;
			}
		free(pGetRInfo);
		}
}
void UpdataRoadStatusInfo(char* buff,int nRecvLength)
{
	CString str1=_T("");
	CString str=_T("");
	if (buff[2]==0x30)
	{
		return;
	}
	else
		if (buff[2]==0x31)
		{
			ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
			memcpy(pGetRInfo,buff+3,sizeof(ConTrlInfo));
			for (int i=0;i<4;i++)
			{
				str1+=pGetRInfo->m_ID[i];
			}
			str1+="0";
			for (int l=0;l<8;l++)
			{
				char c=pGetRInfo->m_CheckData[3]&(0x01<<l);
				str.Format("%s%d",str1,l+1);
				for (int n=0;n<theApp.m_pRoadView->nCount;n++)
				{
					CString str2=_T("");
					for (int m=0;m<6;m++)
					{
						str2+=theApp.m_RoadListInfo[n]->m_RoadID[m];
					}
					if (strcmp(str,str2)==0)
					{
						if (c==0x00)
						{
							theApp.m_RoadListInfo[n]->m_RoadStatus=true;
							theApp.m_RoadListInfo[n]->m_Update|=0x80;
							theApp.m_pRoadView->UpdataOneRoad(theApp.m_RoadListInfo[n]);
						}
						else
						{
							theApp.m_RoadListInfo[n]->m_RoadStatus=false;
							theApp.m_RoadListInfo[n]->m_Update|=0x80;
							theApp.m_pRoadView->UpdataOneRoad(theApp.m_RoadListInfo[n]);
						}
					}
				}
			}
		free(pGetRInfo);
		}
}
CString CharToCString(unsigned char* str, int nLength)
{
	CString strShow=_T("");
	LPTSTR p;
	TCHAR szText[300];
	ZeroMemory(szText, 300);
	p = szText;
	for (int i = 0; i< nLength; i++)
	{
		p+= wsprintf(p, "%.2X", str[i]);  //这部分为关键部分
	}
	strShow.Format(_T("%s"), szText);
	return strShow;
}

void GPRSLocalInfo(char* buff,int nRecvLength)
{
	GPRSInfo* pGetInfo=(GPRSInfo*)malloc(sizeof(GPRSInfo));
	ZeroMemory(pGetInfo,sizeof(GPRSInfo));
	memcpy(pGetInfo,buff+1,sizeof(GPRSInfo));
	theApp.m_pLocalInfoDlg->ShowLocationInfo(pGetInfo);
	Sleep(500);
	free(pGetInfo);
}