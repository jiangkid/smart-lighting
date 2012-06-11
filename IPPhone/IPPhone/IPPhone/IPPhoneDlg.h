// IPPhoneDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Communication.h"

// CIPPhoneDlg �Ի���
class CIPPhoneDlg : public CDialog
{
// ����
public:
	CIPPhoneDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IPPHONE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CImageList    m_ImageList;

public:
	CButton m_BtnCallGroup;
	CButton m_BtnCallMany;
	CButton m_BtnRecentCall;
	CButton m_BtnContacts;
	CButton m_BtnCall;
	CButton m_BtnStopCall;
	CButton m_BtnRevoke;

	CCommunication  m_Communication;
};
