// LightView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "LightView.h"


// CLightView dialog

IMPLEMENT_DYNAMIC(CLightView, CDialog)

CLightView::CLightView(CWnd* pParent /*=NULL*/)
	: CDialog(CLightView::IDD, pParent)
{

}

CLightView::~CLightView()
{
}

void CLightView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Light_View, m_List);
}


BEGIN_MESSAGE_MAP(CLightView, CDialog)

	ON_BN_CLICKED(IDC_BTN_ALL, &CLightView::OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_BTN_UPDATA, &CLightView::OnBnClickedBtnUpdata)
	ON_NOTIFY(NM_DBLCLK, IDC_Light_View, &CLightView::OnNMDblclkLightView)
	ON_NOTIFY(NM_RCLICK, IDC_Light_View, &CLightView::OnNMRClickLightView)
	ON_COMMAND(ID_Updata_Current, &CLightView::OnUpdataCurrent)
	ON_COMMAND(ID_Updata_Status, &CLightView::OnUpdataStatus)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_Light_View, &CLightView::OnLvnItemchangedLightView)
END_MESSAGE_MAP()


// CLightView message handlers

BOOL CLightView::OnInitDialog()
{
	CDialog::OnInitDialog();
 	theApp.m_pLightListView=this;
	m_List.SetHeadings("控制箱编号,80;控制箱名称,80;区域名,80;路名,120;灯,80;灯ID,110;主灯状态,110;辅灯状态,110;电流,110");
	m_List.SetGridLines(TRUE);
//	m_List.SetCheckboxes(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLightView::LightToView(int nLCount)
{
	m_List.DeleteAllItems();
	nCount=nLCount;
	for (int i=0;i<nLCount;i++)
	{
		theApp.m_ZigbeeInfo[i]->Update=0xC0;
		CString strID=_T("");
		CString strName=_T("");
		CString strGID=_T("");
		CString strGName=_T("");
		CString strTName=_T("");
		CString strRName=_T("");
		CString strCurrent=_T("");
		for (int n=0;n<16;n++)
		{
			strID+=theApp.m_ZigbeeInfo[i]->LID[n];
		}
		for (int m=0;m<50;m++)
		{
			strName+=theApp.m_ZigbeeInfo[i]->LName[m];
		}
		for (int k=0;k<20;k++)
		{
			strRName+=theApp.m_ZigbeeInfo[i]->RName[k];
		}
		for (int l=0;l<2;l++)
		{
			strGID+=theApp.m_ZigbeeInfo[i]->m_GID[l];
		}
		for (int o=0;o<20;o++)
		{
			strGName+=theApp.m_ZigbeeInfo[i]->m_GName[o];
		}
		for (int p=0;p<20;p++)
		{
			strTName+=theApp.m_ZigbeeInfo[i]->m_TName[p];
		}
		strCurrent.Format("%0.3fA",theApp.m_ZigbeeInfo[i]->current/1000);
		if (theApp.m_ZigbeeInfo[i]->Update==0xC0)
		{
			if (theApp.m_ZigbeeInfo[i]->MainStatus && theApp.m_ZigbeeInfo[i]->AssistStatus)
			{
				m_List.InsertItem(i,strGID,strGName,strTName,strRName,
					strName,strID,ON,ON,strCurrent);
			}
			else
				if (!theApp.m_ZigbeeInfo[i]->MainStatus && theApp.m_ZigbeeInfo[i]->AssistStatus)
				{
					m_List.InsertItem(i,strGID,strGName,strTName,strRName,
						strName,strID,OFF,ON,strCurrent);
				}
				else
					if (theApp.m_ZigbeeInfo[i]->MainStatus && !theApp.m_ZigbeeInfo[i]->AssistStatus)
					{
						m_List.InsertItem(i,strGID,strGName,strTName,
							strRName,strName,strID,ON,OFF,strCurrent);
					}
					else
						if (!theApp.m_ZigbeeInfo[i]->MainStatus && !theApp.m_ZigbeeInfo[i]->AssistStatus)
						{
							m_List.InsertItem(i,strGID,strGName,strTName,
								strRName,strName,strID,OFF,OFF,strCurrent);
						}
		}
		if (theApp.m_ZigbeeInfo[i]->Update==0x40)
		{
			m_List.InsertItem(i,strGID,strGName,strTName,
				strRName,strName,strID,UNUPDATA,UNUPDATA,strCurrent);
		}
		if (theApp.m_ZigbeeInfo[i]->Update==0x00)
		{
			m_List.InsertItem(i,strGID,strGName,strTName,
				strRName,strName,strID,UNUPDATA,UNUPDATA,UNUPDATA);
		}
		if (theApp.m_ZigbeeInfo[i]->Update==0x80)
		{
			if (theApp.m_ZigbeeInfo[i]->MainStatus && theApp.m_ZigbeeInfo[i]->AssistStatus)
			{
				m_List.InsertItem(i,strGID,strGName,strTName,strRName,
					strName,strID,ON,ON,UNUPDATA);
			}
			else
				if (!theApp.m_ZigbeeInfo[i]->MainStatus && theApp.m_ZigbeeInfo[i]->AssistStatus)
				{
					m_List.InsertItem(i,strGID,strGName,strTName,strRName,
						strName,strID,OFF,ON,UNUPDATA);
				}
				else
					if (theApp.m_ZigbeeInfo[i]->MainStatus && !theApp.m_ZigbeeInfo[i]->AssistStatus)
					{
						m_List.InsertItem(i,strGID,strGName,strTName,strRName,
							strName,strID,ON,OFF,UNUPDATA);
					}
					else
						if (!theApp.m_ZigbeeInfo[i]->MainStatus && !theApp.m_ZigbeeInfo[i]->AssistStatus)
						{
							m_List.InsertItem(i,strGID,strGName,strTName,strRName,
								strName,strID,OFF,OFF,UNUPDATA);
						}
		}
	}
}
bool CLightView::ItemdataProc(DWORD dwData, LPARAM lParam)
{
	return TRUE;
}

void CLightView::OnBnClickedBtnAll()
{
	// TODO: Add your control notification handler code here
	m_List.SelectAllItems();
	m_List.SetFocus();
}

void CLightView::OnBnClickedBtnUpdata()
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<nCount;i++)
	{
		if (theApp.m_ZigbeeInfo[i]->Update==0x00)
		{
			ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
			pGetRInfo->m_First[0]=0x2F;
			pGetRInfo->m_First[1]=0x43;
			pGetRInfo->m_First[2]=0x2F;
			pGetRInfo->m_First[3]=0x01;
			for (int n=0;n<16;n++)
			{
				pGetRInfo->m_ID[n]=theApp.m_ZigbeeInfo[i]->LID[n];
			}
			pGetRInfo->m_OrderType[0]=0x0A;
			pGetRInfo->m_OrderObject[0]=0xA3;
			pGetRInfo->m_ActiveType[0]=0xBD;
			pGetRInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetRInfo);
			Sleep(50);
			ConTrlInfo* pGetRInfo1 = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo1,sizeof(ConTrlInfo));
			pGetRInfo1->m_First[0]=0x2F;
			pGetRInfo1->m_First[1]=0x43;
			pGetRInfo1->m_First[2]=0x2F;
			pGetRInfo1->m_First[3]=0x01;
			for (int n=0;n<16;n++)
			{
				pGetRInfo1->m_ID[n]=theApp.m_ZigbeeInfo[i]->LID[n];
			}
			pGetRInfo1->m_OrderType[0]=0x0A;
			pGetRInfo1->m_OrderObject[0]=0xAE;
			pGetRInfo1->m_ActiveType[0]=0xBD;
			pGetRInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetRInfo1);
			Sleep(50);
			free(pGetRInfo);
			free(pGetRInfo1);
		}
		if (theApp.m_ZigbeeInfo[i]->Update==0x80)
		{
			ConTrlInfo* pGetRInfo1 = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo1,sizeof(ConTrlInfo));
			pGetRInfo1->m_First[0]=0x2F;
			pGetRInfo1->m_First[1]=0x43;
			pGetRInfo1->m_First[2]=0x2F;
			pGetRInfo1->m_First[3]=0x01;
			for (int n=0;n<16;n++)
			{
				pGetRInfo1->m_ID[n]=theApp.m_ZigbeeInfo[i]->LID[n];
			}
			pGetRInfo1->m_OrderType[0]=0x0A;
			pGetRInfo1->m_OrderObject[0]=0xAE;
			pGetRInfo1->m_ActiveType[0]=0xBD;
			pGetRInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetRInfo1);
			Sleep(50);
			free(pGetRInfo1);
		}
		if (theApp.m_ZigbeeInfo[i]->Update==0x40)
		{
			ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
			pGetRInfo->m_First[0]=0x2F;
			pGetRInfo->m_First[1]=0x43;
			pGetRInfo->m_First[2]=0x2F;
			pGetRInfo->m_First[3]=0x01;
			for (int n=0;n<16;n++)
			{
				pGetRInfo->m_ID[n]=theApp.m_ZigbeeInfo[i]->LID[n];
			}
			pGetRInfo->m_OrderType[0]=0x0A;
			pGetRInfo->m_OrderObject[0]=0xA3;
			pGetRInfo->m_ActiveType[0]=0xBD;
			pGetRInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetRInfo);
			Sleep(50);
			free(pGetRInfo);
		}
	}

}
void CLightView::OnNMDblclkLightView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_szLID=_T("");
	m_szLName=_T("");
	m_szRName=_T("");
	m_szGName=_T("");
	m_szMainStatus=_T("");
	m_szAssistStatus=_T("");
	if (pNMItemActivate->iItem == -1) 
	{ 
		return; 
	} 
	else 
	{ 
		//获取所点击的行和列的例子。 
		//strText.Format("你点击了第%d行", pNMItemActivate->iItem); 
		m_szLID=m_List.GetItemText(pNMItemActivate->iItem,5);
		m_szLName=m_List.GetItemText(pNMItemActivate->iItem,4);
		m_szRName=m_List.GetItemText(pNMItemActivate->iItem,3);
		m_szGName=m_List.GetItemText(pNMItemActivate->iItem,1);
		m_szMainStatus=m_List.GetItemText(pNMItemActivate->iItem,6);
		m_szAssistStatus=m_List.GetItemText(pNMItemActivate->iItem,7);
// 		pDlg = (CLightCtrlDlg *)malloc(sizeof(CLightCtrlDlg));
// 		pDlg->CLightCtrlDlg::CLightCtrlDlg();
// 		pDlg->Create(IDD_LIGHT_DLG, this);
// 		pDlg->CenterWindow();
// 		pDlg->ShowWindow(SW_SHOW);
		CLightCtrlDlg dlg;
		dlg.DoModal();
	} 
	m_szLID=_T("");
	m_szLName=_T("");
	m_szRName=_T("");
	m_szGName=_T("");
	m_szMainStatus=_T("");
	m_szAssistStatus=_T("");
	*pResult = 0;
}

void CLightView::OnNMRClickLightView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMItemActivate->iItem==-1)
	{
		return;
	}
	else
	{
		CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象
		menu.LoadMenu(IDR_List_Menu); //装载自定义的右键菜单
		pSubMenu = menu.GetSubMenu(0); //获取第一个弹出菜单 
		CPoint oPoint; //定义一个用于确定光标位置的位置
		GetCursorPos(&oPoint); //获取当前光标的位置 
		m_szLID1=m_List.GetItemText(pNMItemActivate->iItem,5);//在指定位置显示弹出菜单
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}
	*pResult = 0;
}

void CLightView::OnUpdataCurrent()
{
	// TODO: Add your command handler code here
	ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
	memcpy(pGetInfo->m_ID,m_szLID1.GetBuffer(),16);
	pGetInfo->m_First[0]=0x2F;
	pGetInfo->m_First[1]=0x43;
	pGetInfo->m_First[2]=0x2F;
	pGetInfo->m_First[3]=0x01;
	pGetInfo->m_OrderType[0]=0x0A;
	pGetInfo->m_OrderObject[0]=0xAE;
	pGetInfo->m_ActiveType[0]=0xBD;
	pGetInfo->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo);
	Sleep(50);
	free(pGetInfo);

}

void CLightView::OnUpdataStatus()
{
	// TODO: Add your command handler code here
	ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
	memcpy(pGetInfo->m_ID,m_szLID1.GetBuffer(),16);
	pGetInfo->m_First[0]=0x2F;
	pGetInfo->m_First[1]=0x43;
	pGetInfo->m_First[2]=0x2F;
	pGetInfo->m_First[3]=0x01;
	pGetInfo->m_OrderType[0]=0x0A;
	pGetInfo->m_OrderObject[0]=0xA3;
	pGetInfo->m_ActiveType[0]=0xBD;
	pGetInfo->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo);
	Sleep(50);
	free(pGetInfo);
}
void CLightView::UpdataOneLight(ZigbeeInfo* pGetInfo)
{
	CString str1=_T("");
	for (int n=0;n<16;n++)
	{
		str1+=pGetInfo->LID[n];
	}
	int nRow = m_List.GetItemCount();
	for (int i(0);i<nRow;i++)
	{
		CString m_strID=_T("");
		m_strID=m_List.GetItemText(i,5);
		if (strcmp(str1,m_strID)==0)
		{
			m_List.DeleteItem(i);
			CString strID=_T("");
			CString strName=_T("");
			CString strGID=_T("");
			CString strGName=_T("");
			CString strTName=_T("");
			CString strRName=_T("");
			CString strCurrent=_T("");
			for (int n=0;n<16;n++)
			{
				strID+=pGetInfo->LID[n];
			}
			for (int m=0;m<50;m++)
			{
				strName+=pGetInfo->LName[m];
			}
			for (int k=0;k<20;k++)
			{
				strRName+=pGetInfo->RName[k];
			}
			for (int l=0;l<2;l++)
			{
				strGID+=pGetInfo->m_GID[l];
			}
			for (int o=0;o<20;o++)
			{
				strGName+=pGetInfo->m_GName[o];
			}
			for (int p=0;p<20;p++)
			{
				strTName+=pGetInfo->m_TName[p];
			}
			strCurrent.Format("%0.3fA",pGetInfo->current/1000);
			if (pGetInfo->Update==0xC0)
			{
				if (pGetInfo->MainStatus && pGetInfo->AssistStatus)
				{
					m_List.InsertItem(i,strGID,strGName,strTName,strRName,
						strName,strID,ON,ON,strCurrent);
				}
				else
					if (!pGetInfo->MainStatus && pGetInfo->AssistStatus)
					{
						m_List.InsertItem(i,strGID,strGName,strTName,strRName,
							strName,strID,OFF,ON,strCurrent);
					}
					else
						if (pGetInfo->MainStatus && !pGetInfo->AssistStatus)
						{
							m_List.InsertItem(i,strGID,strGName,strTName,
								strRName,strName,strID,ON,OFF,strCurrent);
						}
						else
							if (!pGetInfo->MainStatus && !pGetInfo->AssistStatus)
							{
								m_List.InsertItem(i,strGID,strGName,strTName,
									strRName,strName,strID,OFF,OFF,strCurrent);
							}
			}
			if (pGetInfo->Update==0x40)
			{
				m_List.InsertItem(i,strGID,strGName,strTName,
					strRName,strName,strID,UNUPDATA,UNUPDATA,strCurrent);
			}
			if (pGetInfo->Update==0x00)
			{
				m_List.InsertItem(i,strGID,strGName,strTName,
					strRName,strName,strID,UNUPDATA,UNUPDATA,UNUPDATA);
			}
			if (pGetInfo->Update==0x80)
			{
				if (pGetInfo->MainStatus && pGetInfo->AssistStatus)
				{
					m_List.InsertItem(i,strGID,strGName,strTName,strRName,
						strName,strID,ON,ON,UNUPDATA);
				}
				else
					if (!pGetInfo->MainStatus && pGetInfo->AssistStatus)
					{
						m_List.InsertItem(i,strGID,strGName,strTName,strRName,
							strName,strID,OFF,ON,UNUPDATA);
					}
					else
						if (pGetInfo->MainStatus && !pGetInfo->AssistStatus)
						{
							m_List.InsertItem(i,strGID,strGName,strTName,strRName,
								strName,strID,ON,OFF,UNUPDATA);
						}
						else
							if (!pGetInfo->MainStatus && !pGetInfo->AssistStatus)
							{
								m_List.InsertItem(i,strGID,strGName,strTName,strRName,
									strName,strID,OFF,OFF,UNUPDATA);
							}
			}
		}
	}
}
void CLightView::OnLvnItemchangedLightView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
