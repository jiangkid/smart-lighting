
#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ClietSocket.h"
#include "MainFrm.h"
#include "MFC_SDI_ClientDoc.h"

HDR hdr;
USERINFO userInfo[20];
IintInfo m_InitInfo;


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
		{
			AfxMessageBox(_T("����ʧ�ܣ������ԣ�"));
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
							AfxMessageBox(_T("��Ϣ���Ͳ��ɹ���"));
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
							UpdataRoadCurrentInfo(szBuf,iRet);//·�ĵ���
							break;
						}
						if (szBuf[1]==0x34)//·��״̬
						{
							UpdataRoadStatusInfo(szBuf,iRet);
							break;
						}
						if (szBuf[1]==0x36)
						{
							SendCurrent();
							break;
						}
					case 'G':
						GPRSLocalInfo(szBuf,iRet);
						break;
					case 'X':	//����
						CheckWarningInfo((U8*)szBuf,iRet);
						break;
					case 'F':
						CheckDecisionInfo((U8*)szBuf,iRet);
						break;
					case 'D':
						ChenkDelete(szBuf,iRet);
						break;
				default:
						break;
					}
				}
			else
				{
					HWND m_wnd = theApp.m_WaitDlg.GetSafeHwnd();
					SendMessage(m_wnd,WM_CLOSE,0,0);
					AfxMessageBox(_T("�������Ѿ��رգ�"));
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
����:�ж��û���¼�Ƿ�ɹ�
���صĸ�ʽ:I 0/1#
*************************************************************************************/
void ChenkLogin(char* buff,int nRecvLength)
{
	switch (buff[1])
	{
	case '0':
		theApp.m_return=true;
		break;
	case '1':
		{
			closesocket(theApp.m_ConnectSock);
			AfxMessageBox(_T("�û�����������������������룡"));
			theApp.m_return=false;
			TerminateThread(theApp.h1,-1);
		}
		break;
	default:
		break;
	}
}
//***************************************************************/
//����:�ж��û�ɾ���Ƿ�ɹ�ChenkDelete
//***************************************************************/
void ChenkDelete(char* buff,int nRecvLength)
{
	switch (buff[1])
	{
	case '1':
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("ɾ���û��ɹ�"));
		}
		else
			AfxMessageBox(_T("ɾ���û����ɹ�"));
		
		break;
	case '2':
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("ɾ������ɹ�"));
		}
		else
			AfxMessageBox(_T("ɾ�����򲻳ɹ�"));
		
		break;
	default:
		break;
	}
}
/************************************************************************************
����:�жϴ����û��Ƿ�ɹ�
���صĸ�ʽ:C 0/1#
*************************************************************************************/
void ChenkSet(char* buff,int nRecvLength)
{
	switch (buff[1])
	{
	case '0':
		AfxMessageBox("���óɹ�");
		break;
	case '1':
		AfxMessageBox(_T("�˴β������ɹ���ԭ��û�д������ݿ�"));
		break;
	case '2':
		AfxMessageBox(_T("�˴β������ɹ���ԭ�����ݿ��Ѵ��ڸ��û�"));
		break;
	default:
		break;
	}
}
/************************************************************************************
����:�ж��޸��û������Ƿ�ɹ�
���صĸ�ʽ:M 0/1#
*************************************************************************************/
void ChenkModify(char* buff,int nRecvLength)
{
	BIND* pGetInfo = (BIND*)malloc(sizeof(BIND));
	switch(buff[1])
	{
	case 0x32:
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("�޸�����ɹ���"));
			break;
		}
		else
		{
			AfxMessageBox(_T("�޸�����ʧ�ܣ������²�����"));
			break;
		}
	case 0x33:
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("�޸��û����ɹ���"));
			theApp.m_pModifyNameDlg->ChangeName();
			break;
		}
		else
		{
			AfxMessageBox(_T("�޸��û���ʧ�ܣ������²�����"));
			break;
		}
	case 0x34:
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("�޸ĵ绰����ɹ���"));
			break;
		}
		else
		{
			AfxMessageBox(_T("�޸ĵ绰����ʧ�ܣ������²�����"));
			break;
		}
	case 0x35:
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("�޸�����ɹ���"));
			break;
		}
		else
		{
			AfxMessageBox(_T("�޸�����ʧ�ܣ������²�����"));
			break;
		}
	case 0x36:
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("�޸ĵ绰����ɹ���"));
			break;
		}
		else
		{
			AfxMessageBox(_T("�޸ĵ绰����ʧ�ܣ������²�����"));
			break;
		}
	case 0x37:
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("�޸�����ɹ���"));
			break;
		}
		else
		{
			AfxMessageBox(_T("�޸�����ʧ�ܣ������²�����"));
			break;
		}
	case 0x38:
		if (buff[2]==0x30)
		{
			AfxMessageBox(_T("�޸���Ϣ�ɹ���"));
			break;
		}
		else
		{
			AfxMessageBox(_T("�޸���Ϣʧ�ܣ������²�����"));
			break;
		}
	case 0x31:
		if (buff[2]==0x30)
		{
			int nCount(0);
			nCount=buff[3];
			for (int i(0);i<nCount;i++)
			{
				MUserInfo* pGetInfo = (MUserInfo*)malloc(sizeof(MUserInfo));
				ZeroMemory(pGetInfo,sizeof(MUserInfo));
				memcpy(pGetInfo,buff+4+i*sizeof(MUserInfo),sizeof(MUserInfo));
				theApp.m_pUserCtrl->ShowUserInfo(i,pGetInfo);
				free(pGetInfo);
			}			
		}
		else
		{
			AfxMessageBox(_T("��ȡ�û���Ϣʧ��,���ˢ��"));
			//theApp.m_pUserCtrl->OnClose();
		}
		break;
	case 0x39:
		if (buff[2]==0x31)
		{
			AfxMessageBox(_T("��ȡBIND��Ϣʧ��"));
			return;
		}
		else
			if (buff[2]==0x30)
			{
					int nCount = buff[3];
					for (int i(0);i<nCount;i++)
					{
						ZeroMemory(pGetInfo,sizeof(BIND));
						memcpy(pGetInfo,buff+4+i*sizeof(BIND),sizeof(BIND));
						theApp.m_pSetNewUserDlg->FullFillListBind(i,pGetInfo);
					}
			}
		break;
	default:
		break;
	}
}
/************************************************************************************
����:�ж�BGTRLID����Ϣ���Ƿ�ɹ�
���صĸ�ʽ:S Ox30-0x39 0/1 0x00-0xFF(��ʾ����)+ID#
*************************************************************************************/
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
		AfxMessageBox(_T("��ȡ�Ƶ�ַʧ�ܣ������»�ȡ��"));
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
		AfxMessageBox(_T("��ȡ�û���ʧ�ܣ���رմ������»�ȡ��"));
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
	if (buff[0]=='S'&&buff[1]==0x28 && buff[2]=='1')
	{
		AfxMessageBox(_T("��ȡ�û�Locationʧ�ܣ���رմ������»�ȡ��"));
	}
	else if (buff[0]=='S'&&buff[1]==0x28 && buff[2]=='0')
	{
		theApp.m_NumInfo.UserNum=buff[3];
		int n(0);
		for (int i=4;buff[i]!='#';i++)
		{				
			if (buff[i]=='<')
			{
				int m(0);
				n++;
				ZeroMemory(&userInfo[n],sizeof(USERINFO));
				for(;buff[i+1]!='>';i++)
				{
					userInfo[n].AreaName[m]=buff[i+1];
					m++;
				}
				theApp.m_pSetNewUserDlg->FullFillList(n-1,&userInfo[n]);
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
	if(buff[0]=='S'&&buff[1]==0x40 && buff[2]=='1')
	{
		AfxMessageBox(_T("�󶨲��ɹ��������ԣ�"));
	}
	else if (buff[0]=='S'&&buff[1]==0x40 && buff[2]=='0')
	{
		AfxMessageBox(_T("�󶨳ɹ���"));
	}
	if(buff[0]=='S'&&buff[1]==0x41 && buff[2]=='1')
	{
		AfxMessageBox(_T("����󶨲��ɹ��������ԣ�"));
	}
	else if (buff[0]=='S'&&buff[1]==0x41 && buff[2]=='0')
	{
		AfxMessageBox(_T("����󶨳ɹ���"));
	}
}
/************************************************************************************
����:�����û���¼��Ϣ
���صĸ�ʽ��2���ṹ�壨ͷ�ṹ+�û���Ϣ�ṹ��
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
		AfxMessageBox(_T("SendUserInfo����ʧ�ܡ�"));
	}
}
/************************************************************************************
����:�ж�BGTRL ��ȡIDʧ��
���صĸ�ʽ:BG/T/L/R 1+ID#
*************************************************************************************/
void ChenkBGTRL(char* buff,int nRecvLength)
{
	if (buff[0]=='B'&&buff[1]=='G'&&buff[2]=='1')
	{
		//AfxMessageBox(_T("���GIDʧ�ܣ������²�����"));
		CString str=_T("���GPRSIDʧ�ܣ�GPRSID��");
		str+=buff[3];
		str+=buff[4];
		theApp.m_pMainSetDlg->ShowUpdatatMessage(str);
	}
	if (buff[0]=='B'&&buff[1]=='T'&&buff[2]=='1')
	{
		//AfxMessageBox(_T("���TIDʧ�ܣ������²�����"));
		CString str=_T("����ն�IDʧ�ܣ��ն�ID��");
		str+=buff[3];
		str+=buff[4];
		str+=buff[5];
		str+=buff[6];
		theApp.m_pMainSetDlg->ShowUpdatatMessage(str);
	}
	if (buff[0]=='B'&&buff[1]=='R'&&buff[2]=='1')
	{
		//AfxMessageBox(_T("���RIDʧ�ܣ������²�����"));
		CString str=_T("���·IDʧ�ܣ�·ID��");
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
		//AfxMessageBox(_T("���LIDʧ�ܣ������²�����"));
		CString str=_T("��ӵ���IDʧ�ܣ���ID��");
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
		AfxMessageBox(_T("��һ�ε��������ã�"));
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
		for(int y(0);y<m_InitInfo.GNum;y++)
		{
			ZeroMemory(&m_InitInfo.m_InitGInfo[y],sizeof(GInfo));
		}	
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
		for (int y(0);y<m_InitInfo.TNum;y++)
		{
			ZeroMemory(&m_InitInfo.m_InitTInfo[y],sizeof(TInfo));
		}
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
		for (int y(0);y<m_InitInfo.RNum;y++)
		{
			ZeroMemory(&m_InitInfo.m_InitRInfo[y],sizeof(RInfo));
		}
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
		::SendMessage(m_wnd,WM_CLOSE,0,0);
		Sleep(500);
		theApp.m_pFileView->FillFileView();
		if(userInfo[0].Idetify==0x30)
		{
			char c[3]={'G',0x32,0x30};
			CString str = c;
			str+=userInfo[0].UserName;
			str+='#';
			send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
			str.ReleaseBuffer();
		}
		else
		{
			char c[3]={'G',0x32,0x31};
			CString str = c;
			str+=userInfo[0].UserName;
			str+='#';
			send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
			str.ReleaseBuffer();
		}
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
		AfxMessageBox(_T("SendUserInfo����ʧ�ܡ�"));
	}
}
void SendDecision(LPHDR2 hdr,LPDecision contrlInfo)
{
	hdr->dataCheck[0]='F';
	hdr->nRet[0]=0x01;
	char m_buf[MAX_BUF_SIZE];
	ZeroMemory(m_buf,MAX_BUF_SIZE);
	memcpy(m_buf, (char*)hdr, 2);	
	memcpy(m_buf + 2, (char*)contrlInfo, sizeof(Decision));
	int nRet = send(theApp.m_ConnectSock, m_buf, sizeof(Decision)+2, 0);
	if (SOCKET_ERROR == nRet )
	{
		AfxMessageBox(_T("SendUserInfo����ʧ�ܡ�"));
	}
}
//Test������*******************//
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
		AfxMessageBox(_T("SendUserInfo����ʧ�ܡ�"));
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
		if (buffer[2]!=buffer[1])
		{
			memcpy(theApp.m_lightPack+4093*(j-1),buffer+3,4096-3);
		}
		else
			memcpy(theApp.m_lightPack+4093*(j-1),buffer+3,Length-3);
	}
	else
	{
		if (buffer[2]!=buffer[1])
		{
			memcpy(theApp.m_lightPack+4093*(j-1),buffer+3,4096-3);
		}
		else
			memcpy(theApp.m_lightPack+4093*(j-1),buffer+3,Length-3);
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
	if (buffer[1]==0x30)
	{
		nLcont=buffer[2];
		theApp.nRCount=buffer[2];
		for (int i=0;i<nLcont;i++)
		{
			theApp.m_RoadListInfo[i]=(RoadListViewInfo*)malloc(RLENTH);
			ZeroMemory(theApp.m_RoadListInfo[i],RLENTH);
			memcpy(theApp.m_RoadListInfo[i],buffer+3+i*RLENTH,RLENTH);
		}
		theApp.m_pRoadView->RoadInfoToView(nLcont);
	}
	else
	{
		nLcont=buffer[1];
		theApp.nRCount=buffer[1];
		for (int i=0;i<nLcont;i++)
		{
			theApp.m_RoadListInfo[i]=(RoadListViewInfo*)malloc(RLENTH);
			ZeroMemory(theApp.m_RoadListInfo[i],RLENTH);
			memcpy(theApp.m_RoadListInfo[i],buffer+2+i*RLENTH,RLENTH);
		}
		theApp.m_pRoadView->RoadInfoToView(nLcont);
		theApp.m_pGCInfoDlg->SendRUpdata();
	}
// 	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
// 	pMain->StartTimer(1);
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
					float nCurrent=(float)pGetRInfo->m_CheckData[2]*256+pGetRInfo->m_CheckData[3];
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
					theApp.m_pLightCtrlDlg->CheckMain(true);
					break;
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
						theApp.m_pLightCtrlDlg->CheckMain(false);
						break;
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
					theApp.m_pLightCtrlDlg->CheckAssist(true);
					break;
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
						theApp.m_pLightCtrlDlg->CheckAssist(true);
						break;
					}
					
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
						//theApp.m_pDoRoadDld->UpdateRoadstatus(true);
						theApp.m_pLightCtrlDlg->CheckAssist(true);
						theApp.m_pLightCtrlDlg->CheckMain(true);
						break;
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
							theApp.m_pLightCtrlDlg->CheckAssist(false);
							theApp.m_pLightCtrlDlg->CheckMain(false);
							break;
						}
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
					theApp.m_pRoadView->UpdataOneRoad(theApp.m_RoadListInfo[l]);
					//theApp.m_pRoadView->RoadInfoToView(theApp.m_pRoadView->nCount);
					theApp.m_pDoRoadDld->UpdateRoadstatus(false);
					break;
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
					theApp.m_pRoadView->UpdataOneRoad(theApp.m_RoadListInfo[l]);
					//theApp.m_pRoadView->RoadInfoToView(theApp.nRCount);
					theApp.m_pDoRoadDld->UpdateRoadstatus(true);
					break;
				}
			}
			break;
		default:
			break;
		}
	free(pGetRInfo);
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
						theApp.m_RoadListInfo[n]->nCurrent1=(pGetRInfo->m_CheckData[2]*256+pGetRInfo
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
						theApp.m_RoadListInfo[n]->nCurrent2=(pGetRInfo->m_CheckData[2]*256+pGetRInfo
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
						theApp.m_RoadListInfo[n]->nCurrent3=(pGetRInfo->m_CheckData[2]*256+pGetRInfo
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
		p+= wsprintf(p, "%.2X", str[i]);  //�ⲿ��Ϊ�ؼ�����
	}
	strShow.Format(_T("%s"), szText);
	return strShow;
}
static int nRetMapRet(0);
void GPRSLocalInfo(char* buff,int nRecvLength)
{
	GPRSInfo* pGetInfo=(GPRSInfo*)malloc(GLENTH);
	ZeroMemory(pGetInfo,GLENTH);
	GPRSInfo* pGetInfo3=(GPRSInfo*)malloc(GLENTH);
	ZeroMemory(pGetInfo3,GLENTH);
	TerminalInfo* pGetTInfo1 = (TerminalInfo*)malloc(TLENTH);
	ZeroMemory(pGetTInfo1,TLENTH);
	switch(buff[1])
	{
	case 0x30://GPRS��Ϣ
		if (buff[2]==0x30)
		{
			memcpy(pGetInfo,buff+3,GLENTH);
			theApp.m_pLocalInfoDlg->ShowLocationInfo(pGetInfo);	
			Sleep(500);
			free(pGetInfo);
		}
		else
			AfxMessageBox(_T("��ȡGPRS������Ϣ����"));
		break;
	case 0x31://�ն���Ϣ
		if(buff[2]==0x30)
		{
			int n=buff[3];
			for (int i(0);i<n;i++)
			{
				memcpy(pGetTInfo1,buff+4+i*TLENTH,TLENTH);
				memcpy(&theApp.m_TerminalInfo[i],pGetTInfo1,TLENTH);
				ZeroMemory(pGetTInfo1,TLENTH);
			}
			theApp.m_pLocalInfoDlg->ShowTerminalInfo(n);
			free(pGetTInfo1);
		}
		else
			AfxMessageBox(_T("��ȡGPRS������Ϣ����"));
		break;
	case 0x32:
		{
			nRetMapRet++;
			if (buff[2]==0x30)
			{
				int i=buff[3];
				int j=buff[4];
				if (nRetMapRet<i)
				{		
					if (buff[3]!=buff[4])
					{
						memcpy(theApp.m_MapInfoPack+4091*(j-1),buff+5,4096-5);
					}
					else
						memcpy(theApp.m_MapInfoPack+4091*(j-1),buff+5,nRecvLength-5);
				}
				else
				{
					if (buff[3]!=buff[4])
					{
						memcpy(theApp.m_MapInfoPack+4091*(j-1),buff+5,4096-5);
					}
					else
						memcpy(theApp.m_MapInfoPack+4091*(j-1),buff+5,nRecvLength-5);
					TranslateMapInfo(theApp.m_MapInfoPack);
				}
			}
			else
				AfxMessageBox(_T("��ȡ������Ϣ����"));
		}
		break;
	case 0x33://GPRS��Ϣ
		if (buff[2]==0x30)
		{
			memcpy(pGetInfo3,buff+3,GLENTH);
			Sleep(500);
			theApp.m_pMapCtrlDlg->ShowLocalInfo(pGetInfo3);
			free(pGetInfo3);
			theApp.m_pMapCtrlDlg->SendTMessage();
		}
		else
			AfxMessageBox(_T("��ȡGPRS������Ϣ����"));
		break;
	case 0x34://�ն���Ϣ
		if(buff[2]==0x30)
		{
			int n=buff[3];
			for (int i(0);i<n;i++)
			{
				memcpy(pGetTInfo1,buff+4+i*TLENTH,TLENTH);
				memcpy(&theApp.m_TerminalInfoMap[i],pGetTInfo1,TLENTH);
				ZeroMemory(pGetTInfo1,TLENTH);
			}
			theApp.m_pMapCtrlDlg->ShowTerminalInfo(n);
			free(pGetTInfo1);
		}
		else
			AfxMessageBox(_T("��ȡGPRS������Ϣ����"));
		break;
	default:
		break;
	}
}
void CheckWarningInfo(unsigned char* buff, int nLength)
{
	WarningInfo* pWarningInfo=(WarningInfo*)malloc(WARNLENGTH);
	ZeroMemory(pWarningInfo,WARNLENGTH);
  	switch(buff[1])
	{
	case 0x30:
		{
			theApp.nWarningCount++;
			memcpy(pWarningInfo,buff+3,WARNLENGTH);
			theApp.m_pWarningInfoView->OneWarningToShow(pWarningInfo,theApp.nWarningCount);
			free(pWarningInfo);
		}
		break;
	case 0x31:
		{
			if(buff[2]==0x30)
				AfxMessageBox(_T("���λ�ȡʧ��"));
			else if (buff[2]!=0x30)
			{
				theApp.m_pWarningInfoView->warningpackCount++;
				int i(0),j(0);
				i=buff[2];
				j=buff[3];
				if (theApp.m_pWarningInfoView->warningpackCount<i)
				{		
					if (buff[2]!=buff[3])
					{
						memcpy(theApp.m_warningPack+4092*(j-1),buff+4,4096-4);
					}
					else
						memcpy(theApp.m_warningPack+4092*(j-1),buff+4,nLength-4);	
				}
				else
				{
					if (buff[2]!=buff[3])
					{
						memcpy(theApp.m_warningPack+4092*(j-1),buff+4,4096-4);
					}
					else
						memcpy(theApp.m_warningPack+4092*(j-1),buff+4,nLength-4);
					TranslateWarningInfo(theApp.m_warningPack);
				}
			}
		}
		break;
	default:
		break;
	}
}
//***************************************************************/
//�������ܣ���ͼ��Ϣ���
//***************************************************************/
void TranslateMapInfo(U8* buff)
{
	nRetMapRet=0;
	MAPInfo* pGetTInfo2 = (MAPInfo*)malloc(sizeof(MAPInfo));
	int nCount=buff[0];
	theApp.nGeshu = nCount;
	for (int i(0);i<nCount;i++)
	{
		ZeroMemory(pGetTInfo2,sizeof(MAPInfo));
		memcpy(pGetTInfo2,buff+1+MAPLENGTH*i,MAPLENGTH);
		ZeroMemory(&theApp.m_MapInfo[i],MAPLENGTH);
		memcpy(&theApp.m_MapInfo[i],pGetTInfo2,MAPLENGTH);
		theApp.m_pMapViewDlg->ShowInfomation(i,pGetTInfo2);	
	}
	free(pGetTInfo2);
}
//***************************************************************/
//�������ܣ�waringpack���
//***************************************************************/
void TranslateWarningInfo(U8* buff)
{
	int nCount(0);
	nCount=buff[0];
	for (int i(0);i<nCount;i++)
	{
		ZeroMemory(&theApp.m_WarningInfo[i],WARNLENGTH);
		memcpy(&theApp.m_WarningInfo[i],buff+1+i*WARNLENGTH,WARNLENGTH);
	}
	theApp.m_pWarningInfoView->AllWarningToShow(nCount);
}

void SendCurrent()
{
	theApp.m_pGCInfoDlg->SendRCurrent();
}

void CheckDecisionInfo(U8* buff, int nLength)
{
	switch (buff[1])
	{
	case 0x30:
		AfxMessageBox(_T("���Ա���ɹ�"));
		break;
	case 0x31:
		AfxMessageBox(_T("���Ա���ʧ��"));
		break;
	default:
		break;
	}
}