
// MFC_SDI_Client.h : MFC_SDI_Client Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "ClietSocket.h"
#include "logindlg.h"
#include "ClietSocket.h"
#include "SetNewUser.h"
#include "MainSetDlg.h"
#include "WaitDlg.h"
#include "FileView.h"
#include "LightListView.h"
#include "CVListView.h"
#include "TabListView.h"
#include "LightView.h"
#include "RoudInfoView.h"
// CMFC_SDI_ClientApp:
// �йش����ʵ�֣������ MFC_SDI_Client.cpp
class CFileView;
class CMFC_SDI_ClientApp : public CWinAppEx
{
public:
	CMFC_SDI_ClientApp();
	HANDLE m_hConnectThred;	
	SOCKET m_ConnectSock;
	int	   m_port;//�˿ں�
	char   m_ip[20];//IP��ַ
	CString strNamePass;
	bool m_return;
	bool m_InitTrue;
	bool m_connected;
	HANDLE h1;
	char TID[6];
	char nStatus[4];//����״̬4��
	GInfo m_GInfo[8];//GPRS����8 GName GID
	TInfo m_TInfo[32];//�ն˸���32 TName TID
	RInfo m_RInfo[196];//·�ĸ���196 RName RID
	LInfo m_LInfo[1024];//���Ƹ���1024 LName LID
	ZigbeeInfo* m_ZigbeeInfo[512];//һ���ڵ����Ϣ���512����ʾ����Ϣ
	RoadListViewInfo*   m_RoadListInfo[8];//1���ն������8·����Ϣ
	U8  m_lightPack[LENTH*1000];//���Ļ����С�����30K
	NumInfo m_NumInfo;
	CWaitDlg			m_WaitDlg;
	CFileView*			m_pFileView;
	CLightView*			m_pLightListView;
	CCVListView*		m_pCVListView;
	CTabListView*		m_pTabListView;
	CRoudInfoView*		m_pRoadView;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSetuser();
	afx_msg void OnSetg();
	afx_msg void OnModify();
	afx_msg void OnCagain();
	private:
public:
	afx_msg void On32783();
	virtual int ExitInstance();
	afx_msg void OnUpdateCagain(CCmdUI *pCmdUI);
	bool m_DlgMainONStatus;
	bool m_DlgMainOFFStatus;
	bool m_DlgAssistONStatus;
	bool m_DlgAssistOFFStatus;
	bool m_DlgDoubleONStatus;
	bool m_DlgDoubleOFFStatus;
};

extern CMFC_SDI_ClientApp theApp;
