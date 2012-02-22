
// MFC_SDI_Client.h : MFC_SDI_Client 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "ClietSocket.h"
#include "logindlg.h"
#include "ClietSocket.h"
#include "SetNewUser.h"
#include "MainSetDlg.h"
#include "WaitDlg.h"
#include "FileView.h"
#include "LightListView.h"
#include "CVListView.h"
// CMFC_SDI_ClientApp:
// 有关此类的实现，请参阅 MFC_SDI_Client.cpp
class CFileView;
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
	ZigbeeInfo* m_ZigbeeInfo[512];
	U8  m_lightPack[LENTH*1000];//包的缓冲大小定义成30K
	NumInfo m_NumInfo;
	CWaitDlg m_WaitDlg;
	CFileView *m_pFileView;
	CLightListView *m_pLightListView;
	CCVListView *m_pCVListView;
//	CLightView *m_light;
// 重写
public:
	virtual BOOL InitInstance();

// 实现
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
	virtual int ExitInstance();
};

extern CMFC_SDI_ClientApp theApp;
