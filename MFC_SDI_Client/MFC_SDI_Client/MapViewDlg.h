#if !defined(AFX_MFCHTMLDLG_H__48A3710B_BDFF_4289_990F_8D3B8028D38A__INCLUDED_)
#define AFX_MFCHTMLDLG_H__48A3710B_BDFF_4289_990F_8D3B8028D38A__INCLUDED_
// CMapViewDlg dialog


#pragma once
#include "MapInfomation.h"
#include "afxcmn.h"
#include "ReportCtrl.h"

#include "MapCtrlDlg.h"
#include "afxwin.h"
class CMapViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapViewDlg)

public:
	CMapViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapViewDlg();


	CMapInfomation* m_pMapInfo ;
// Dialog Data
	enum { IDD = IDD_MAP_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CString m_lat;
	CString m_lng;
	void ShowMessage(CString str1,CString str2);
	afx_msg void OnBnClickedButton2();
	void ShowInfomation(int nItem, MAPInfo* pGetInfo);
	CReportCtrl m_List;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
//	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_ComBox;
};
#endif