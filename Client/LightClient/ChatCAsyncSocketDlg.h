#if !defined(AFX_CHATCASYNCSOCKETDLG_H__BFB37FB2_A801_41B9_BF2E_8BF1023F53A5__INCLUDED_)
#define AFX_CHATCASYNCSOCKETDLG_H__BFB37FB2_A801_41B9_BF2E_8BF1023F53A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatCAsyncSocketDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatCAsyncSocketDlg dialog
#include "ListenSocket.h"
#include "DataSocket.h"
#include <AFXTEMPL.H>

class CChatCAsyncSocketDlg : public CDialog
{
// Construction
public:
	CChatCAsyncSocketDlg(CWnd* pParent = NULL);   // standard constructor
	void OnSocketReceive(CDataSocket *pSocket);
	void OnSocketClose(CDataSocket *pSocket);
	void OnServerAccept();
	void OnClientConnect(int iResult);
// Dialog Data
	//{{AFX_DATA(CChatCAsyncSocketDlg)
	enum { IDD = IDD_CONNECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
private:
	void OnServerReceive(CDataSocket *pSocket);
	void OnServerClose(CDataSocket *pSocket);
	void OnClientReceive();
	void OnClientClose();
	BOOL m_bActive;
	BOOL m_bClient;
	CString m_ChatWords;
	CDataSocket   m_ClientSocket;
	CListenSocket m_ListenSocket;
	CArray<CDataSocket*, CDataSocket*> m_DataSockets;
	void GetErrorReason(int nErrorCode);
	void ExitChat();
	void RefreshScreen();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatCAsyncSocketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CChatCAsyncSocketDlg)
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnConnect();
		afx_msg void OnListen();
		afx_msg	void SendMessageToPeer();
		virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATCASYNCSOCKETDLG_H__BFB37FB2_A801_41B9_BF2E_8BF1023F53A5__INCLUDED_)
