
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
// CMFC_SDI_ClientApp:
// �йش����ʵ�֣������ MFC_SDI_Client.cpp

class CMFC_SDI_ClientApp : public CWinAppEx
{
public:
	CMFC_SDI_ClientApp();
	HANDLE m_hConnectThred;	
	SOCKET m_ConnectSock;
	int m_port;
	char m_ip[20];
	bool logintrue;
	CString strNamePass;
	bool m_return;
	bool m_InitTrue;
	HANDLE h1;
	bool settrue;
	GInfo m_GInfo[8];
	TInfo m_TInfo[32];
	RInfo m_RInfo[196];
	LInfo m_LInfo[4096];
	NumInfo m_NumInfo;
	CWaitDlg m_WaitDlg;
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
	afx_msg void OnMainSet();
	afx_msg void OnCagain();
	private:
public:
	afx_msg void On32783();
};

extern CMFC_SDI_ClientApp theApp;