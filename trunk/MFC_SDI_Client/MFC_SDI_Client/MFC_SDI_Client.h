
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
#include "TabListView.h"
#include "LightView.h"
#include "RoudInfoView.h"
#include "SplashWnd.h"
#include "MainSetDlg.h"
#include "GPRSLocaInfomation.h"
#include "WarningDlg.h"
#include "GprsInfoCtrlDlg.h"
#include "UserCtrDlg.h"
#include "TerminalCtrlDlg.h"
#include "DoRWorkDlg.h"
#include "LightCtrlDlg.h"
#include "ModifyName.h"
#include "SetNewUser.h"
#include "MapInfomation.h"
#include "MapViewDlg.h"
// CMFC_SDI_ClientApp:
// 有关此类的实现，请参阅 MFC_SDI_Client.cpp
class CFileView;
class CMFC_SDI_ClientApp : public CWinAppEx
{
public:
	CMFC_SDI_ClientApp();
	HANDLE m_hConnectThred;	
	SOCKET m_ConnectSock;
	int	   m_port;//端口号
	int    nRCount;
	int    nWarningCount;//警告个数
	char   m_ip[20];//IP地址
	CString strNamePass;
	bool m_return;
	bool m_InitTrue;
	bool m_connected;
	HANDLE h1;
	char TID[6];
	char GID[5];
	char nStatus[4];//托盘状态4种
	GInfo m_GInfo[8];//GPRS个数8 GName GID
	TInfo m_TInfo[32];//终端个数32 TName TID
	RInfo m_RInfo[196];//路的个数196 RName RID
	LInfo m_LInfo[1024];//单灯个数1024 LName LID
	TerminalInfo m_TerminalInfo[4];//1个GPRS下4个终端，最大
	ZigbeeInfo* m_ZigbeeInfo[512];//一个节点的信息最大512个显示的信息
	RoadListViewInfo*   m_RoadListInfo[12];//1个终端下最大8路的信息
	U8  m_lightPack[LENTH*1000];//包的缓冲大小定义成30K
	U8  m_warningPack[WARNLENGTH*1000];//警告的缓冲
	U8  m_MapInfoPack[MAPLENGTH*64];//警告的缓冲
	WarningInfo m_WarningInfo[256];//一次性最多可以获取历史警告256个
	NumInfo m_NumInfo;
	/*static int warningpack;*/
	CWaitDlg			m_WaitDlg;
	CFileView*			m_pFileView;
	CLightView*			m_pLightListView;
//	CCVListView*		m_pCVListView;
	CTabListView*		m_pTabListView;
	CRoudInfoView*		m_pRoadView;
	CMainSetDlg*        m_pMainSetDlg;
	CGPRSLocaInfomation*m_pLocalInfoDlg;
	CWarningDlg*        m_pWarningInfoView;
	CGprsInfoCtrlDlg*   m_pGCInfoDlg;
	CUserCtrDlg*        m_pUserCtrl;
	CTerminalCtrlDlg*   m_pTerminalDlg;
	CDoRWorkDlg*        m_pDoRoadDld;
	CLightCtrlDlg*      m_pLightCtrlDlg;
	CModifyName*        m_pModifyNameDlg;
	CSetNewUser*        m_pSetNewUserDlg;
	CMapInfomation*     m_pMapInfoDlg;
	CMapViewDlg*        m_pMapViewDlg;
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
	afx_msg void OnRenew();
};

extern CMFC_SDI_ClientApp theApp;
