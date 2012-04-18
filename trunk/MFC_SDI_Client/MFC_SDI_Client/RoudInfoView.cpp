// RoudInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "RoudInfoView.h"


// CRoudInfoView dialog

IMPLEMENT_DYNAMIC(CRoudInfoView, CDialog)

CRoudInfoView::CRoudInfoView(CWnd* pParent /*=NULL*/)
	: CDialog(CRoudInfoView::IDD, pParent)
	,m_szLID1(_T(""))
{

}

CRoudInfoView::~CRoudInfoView()
{
}

void CRoudInfoView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_R_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CRoudInfoView, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CRoudInfoView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_Updata_ALL, &CRoudInfoView::OnBnClickedUpdataAll)
	ON_NOTIFY(NM_DBLCLK, IDC_R_LIST, &CRoudInfoView::OnNMDblclkRList)
	ON_NOTIFY(NM_RCLICK, IDC_R_LIST, &CRoudInfoView::OnNMRClickRList)
	ON_COMMAND(ID_RC_Updata, &CRoudInfoView::OnRcUpdata)
	ON_COMMAND(ID_RS_Updata, &CRoudInfoView::OnRsUpdata)
	ON_COMMAND(ID_RC2_Updata, &CRoudInfoView::OnRc2Updata)
	ON_COMMAND(ID_RC3_Updata, &CRoudInfoView::OnRc3Updata)
END_MESSAGE_MAP()


// CRoudInfoView message handlers

BOOL CRoudInfoView::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pRoadView=this;
	m_List.SetHeadings("控制箱名称,80;路名,130;路ID,110;路的状态,110;电流1,110;电流2,110;电流3,110;");
//	m_List.SetHeadings("控制箱名称,80;路名,130;路ID,110;路的状态,110;电流,110;第一支路电流,110;第二支路电流,110;第三支路电流,110");
	m_List.SetGridLines(TRUE);
//	m_List.SetCheckboxes(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
}
void CRoudInfoView::RoadInfoToView(int nRCount)
{
	nCount=nRCount;
 	m_List.DeleteAllItems();
 	for (int i=0;i<nRCount;i++)
	{
 		CString strTName=_T("");
 		CString strRName=_T("");
		CString strCurrent1=_T("");
		CString strCurrent2=_T("");
		CString strCurrent3=_T("");
 		CString strRID  =_T("");
		strCurrent1.Format("%0.3fA",(float)theApp.m_RoadListInfo[i]->nCurrent1/1000);
		strCurrent2.Format("%0.3fA",(float)theApp.m_RoadListInfo[i]->nCurrent2/1000);
		strCurrent3.Format("%0.3fA",(float)theApp.m_RoadListInfo[i]->nCurrent3/1000);
 		for (int n=0;n<20;n++)
 		{
 			strRName+=theApp.m_RoadListInfo[i]->m_RoadName[n];
 		}
 		for (int m=0;m<20;m++)
		{
			strTName+=theApp.m_RoadListInfo[i]->m_TernimalName[m];
		}
 		for(int k=0;k<6;k++)
 		{
 			strRID+=theApp.m_RoadListInfo[i]->m_RoadID[k];
 		}
		switch (theApp.m_RoadListInfo[i]->m_Update)
		{
		case 0x00:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,UNUPDATA,UNUPDATA,UNUPDATA);
			break;
		case 0x10:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,UNUPDATA,UNUPDATA,strCurrent3);
			break;
		case 0x20:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,UNUPDATA,strCurrent2,UNUPDATA);
			break;
		case 0x30:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,UNUPDATA,strCurrent2,strCurrent3);
			break;
		case 0x40:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,strCurrent1,UNUPDATA,UNUPDATA);
			break;
		case 0x50:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,strCurrent1,UNUPDATA,strCurrent3);
			break;
		case 0x60:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,strCurrent1,strCurrent2,UNUPDATA);
			break;
		case 0x70:
			m_List.InsertItem(i,strTName,strRName,strRID,
				UNUPDATA,strCurrent1,strCurrent2,strCurrent3);
			break;
		case 0x80:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,UNUPDATA,UNUPDATA,UNUPDATA);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,UNUPDATA,UNUPDATA,UNUPDATA);
			}
			break;
		case 0x90:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,UNUPDATA,UNUPDATA,strCurrent3);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,UNUPDATA,UNUPDATA,strCurrent3);
			}
			break;
		case 0xA0:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,UNUPDATA,strCurrent2,UNUPDATA);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,UNUPDATA,strCurrent2,UNUPDATA);
			}
			break;
		case 0xB0:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,UNUPDATA,strCurrent2,strCurrent3);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,UNUPDATA,strCurrent2,strCurrent3);
			}
			break;
		case 0xC0:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,strCurrent1,UNUPDATA,UNUPDATA);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,strCurrent1,UNUPDATA,UNUPDATA);
			}
			break;
		case 0xD0:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,strCurrent1,UNUPDATA,strCurrent3);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,strCurrent1,UNUPDATA,strCurrent3);
			}
			break;
		case 0xE0:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,strCurrent1,strCurrent2,UNUPDATA);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,strCurrent1,strCurrent2,UNUPDATA);
			}
			break;
		case 0xF0:
			if (theApp.m_RoadListInfo[i]->m_RoadStatus)
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					ON,strCurrent1,strCurrent2,strCurrent3);
			}
			else
			{
				m_List.InsertItem(i,strTName,strRName,strRID,
					OFF,strCurrent1,strCurrent2,strCurrent3);
			}
			break;
		default:
			break;
		}
		//free(theApp.m_RoadListInfo[i]);
	}
}
void CRoudInfoView::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//m_List.SelectAllItems();
	m_List.SetFocus();
}

void CRoudInfoView::OnBnClickedUpdataAll()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<nCount;i++)
	{
		if (theApp.m_RoadListInfo[i]->m_Update==0x10)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x32;
			pGetInfo->m_ActiveType[0]=0xBD;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);
			Sleep(5000);

			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			U8* ID;
			ID=GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2);
			memcpy(pGetInfo1->m_CheckData,ID,1);
			free(ID);
			pGetInfo1->m_CheckData[1]=0x01;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);

			ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
			memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo2->m_First[0]=0x2F;
			pGetInfo2->m_First[1]=0x43;
			pGetInfo2->m_First[2]=0x2F;
			pGetInfo2->m_First[3]=0x01;
			pGetInfo2->m_OrderType[0]=0x1A;
			pGetInfo2->m_OrderObject[0]=0x33;
			pGetInfo2->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo2->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo2->m_CheckData[1]=0x02;
			pGetInfo2->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo2);
			Sleep(500);
			free(pGetInfo2);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0x20)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x32;
			pGetInfo->m_ActiveType[0]=0xBD;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);

			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x01;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);

			ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
			memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo2->m_First[0]=0x2F;
			pGetInfo2->m_First[1]=0x43;
			pGetInfo2->m_First[2]=0x2F;
			pGetInfo2->m_First[3]=0x01;
			pGetInfo2->m_OrderType[0]=0x1A;
			pGetInfo2->m_OrderObject[0]=0x33;
			pGetInfo2->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo2->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo2->m_CheckData[1]=0x03;
			pGetInfo2->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo2);
			Sleep(500);
			free(pGetInfo2);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0x30)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x32;
			pGetInfo->m_ActiveType[0]=0xBD;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);

			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x01;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0x40)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x32;
			pGetInfo->m_ActiveType[0]=0xBD;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);

			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x02;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);

			ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
			memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo2->m_First[0]=0x2F;
			pGetInfo2->m_First[1]=0x43;
			pGetInfo2->m_First[2]=0x2F;
			pGetInfo2->m_First[3]=0x01;
			pGetInfo2->m_OrderType[0]=0x1A;
			pGetInfo2->m_OrderObject[0]=0x33;
			pGetInfo2->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo2->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo2->m_CheckData[1]=0x03;
			pGetInfo2->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo2);
			Sleep(500);
			free(pGetInfo2);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0x50)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x32;
			pGetInfo->m_ActiveType[0]=0xBD;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);

			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x02;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0x60)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x32;
			pGetInfo->m_ActiveType[0]=0xBD;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);

			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x03;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);
		}
		if (theApp.m_RoadListInfo[i]->m_Update=0x70)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x32;
			pGetInfo->m_ActiveType[0]=0xBD;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0x80)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x33;
			pGetInfo->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo->m_CheckData[1]=0x01;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);


			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x02;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);

			ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
			memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo2->m_First[0]=0x2F;
			pGetInfo2->m_First[1]=0x43;
			pGetInfo2->m_First[2]=0x2F;
			pGetInfo2->m_First[3]=0x01;
			pGetInfo2->m_OrderType[0]=0x1A;
			pGetInfo2->m_OrderObject[0]=0x33;
			pGetInfo2->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo2->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo2->m_CheckData[1]=0x03;
			pGetInfo2->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo2);
			Sleep(500);
			free(pGetInfo2);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0x90)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x33;
			pGetInfo->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo->m_CheckData[1]=0x01;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);


			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x02;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0xA0)
		{
			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
			memcpy(pGetInfo->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo->m_First[0]=0x2F;
			pGetInfo->m_First[1]=0x43;
			pGetInfo->m_First[2]=0x2F;
			pGetInfo->m_First[3]=0x01;
			pGetInfo->m_OrderType[0]=0x1A;
			pGetInfo->m_OrderObject[0]=0x33;
			pGetInfo->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo->m_CheckData[1]=0x01;
			pGetInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
			free(pGetInfo);

			ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
			memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo2->m_First[0]=0x2F;
			pGetInfo2->m_First[1]=0x43;
			pGetInfo2->m_First[2]=0x2F;
			pGetInfo2->m_First[3]=0x01;
			pGetInfo2->m_OrderType[0]=0x1A;
			pGetInfo2->m_OrderObject[0]=0x33;
			pGetInfo2->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo2->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo2->m_CheckData[1]=0x03;
			pGetInfo2->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo2);
			Sleep(500);
			free(pGetInfo2);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0xB0)
		{
			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x01;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0xC0)
		{
			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x02;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);

			ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
			memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo2->m_First[0]=0x2F;
			pGetInfo2->m_First[1]=0x43;
			pGetInfo2->m_First[2]=0x2F;
			pGetInfo2->m_First[3]=0x01;
			pGetInfo2->m_OrderType[0]=0x1A;
			pGetInfo2->m_OrderObject[0]=0x33;
			pGetInfo2->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo2->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo2->m_CheckData[1]=0x03;
			pGetInfo2->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo2);
			Sleep(500);
			free(pGetInfo2);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0xD0)
		{
			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
			memcpy(pGetInfo1->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo1->m_First[0]=0x2F;
			pGetInfo1->m_First[1]=0x43;
			pGetInfo1->m_First[2]=0x2F;
			pGetInfo1->m_First[3]=0x01;
			pGetInfo1->m_OrderType[0]=0x1A;
			pGetInfo1->m_OrderObject[0]=0x33;
			pGetInfo1->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo1->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo1->m_CheckData[1]=0x02;
			pGetInfo1->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo1);
			Sleep(500);
			free(pGetInfo1);
		}
		if (theApp.m_RoadListInfo[i]->m_Update==0xE0)
		{
			ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
			ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
			memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
			pGetInfo2->m_First[0]=0x2F;
			pGetInfo2->m_First[1]=0x43;
			pGetInfo2->m_First[2]=0x2F;
			pGetInfo2->m_First[3]=0x01;
			pGetInfo2->m_OrderType[0]=0x1A;
			pGetInfo2->m_OrderObject[0]=0x33;
			pGetInfo2->m_ActiveType[0]=0xBD;
			memcpy(pGetInfo2->m_CheckData,GPRSTranslationID(theApp.m_RoadListInfo[i]->m_RoadID+4,2),1);
			pGetInfo2->m_CheckData[1]=0x03;
			pGetInfo2->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGetInfo2);
			Sleep(500);
			free(pGetInfo2);
		}
	}
}

void CRoudInfoView::OnNMDblclkRList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_szLID=_T("");
	m_szLName=_T("");
	m_szRName=_T("");
	m_szGName=_T("");
	if(pNMItemActivate->iItem == -1)
	{
		return;
	}
	else
	{ 
		//获取所点击的行和列的例子。 
		//strText.Format("你点击了第%d行", pNMItemActivate->iItem); 
		m_szGName=m_List.GetItemText(pNMItemActivate->iItem,3);
		m_szLID=m_List.GetItemText(pNMItemActivate->iItem,2);
		m_szRName=m_List.GetItemText(pNMItemActivate->iItem,1);
		pDlg = (CDoRWorkDlg *)malloc(sizeof(CDoRWorkDlg));
		pDlg->CDoRWorkDlg::CDoRWorkDlg();
		pDlg->Create(IDD_DoRWorkDlg, this);
		pDlg->CenterWindow();
		pDlg->ShowWindow(SW_SHOW);
	} 
	m_szLID=_T("");
	m_szLName=_T("");
	m_szRName=_T("");
	m_szGName=_T("");
	*pResult = 0;
}
void CRoudInfoView::OnNMRClickRList(NMHDR *pNMHDR, LRESULT *pResult)
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
		pSubMenu = menu.GetSubMenu(1); //获取第一个弹出菜单 
		CPoint oPoint; //定义一个用于确定光标位置的位置
		GetCursorPos(&oPoint); //获取当前光标的位置 
		//在指定位置显示弹出菜单
		m_szLID1=_T("");
		m_szLID1=m_List.GetItemText(pNMItemActivate->iItem,2);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this); 

	}
	*pResult = 0;
}

void CRoudInfoView::OnRcUpdata()
{
	// TODO: Add your command handler code here
	ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
	memcpy(pGetInfo2->m_ID,m_szLID1.GetBuffer(),4);
	pGetInfo2->m_First[0]=0x2F;
	pGetInfo2->m_First[1]=0x43;
	pGetInfo2->m_First[2]=0x2F;
	pGetInfo2->m_First[3]=0x01;
	pGetInfo2->m_OrderType[0]=0x1A;
	pGetInfo2->m_OrderObject[0]=0x33;
	pGetInfo2->m_ActiveType[0]=0xBD;
	U8 c[2];
	ZeroMemory(c,2);
	memcpy(c,m_szLID1.GetBuffer()+4,2);
	memcpy(&pGetInfo2->m_CheckData[0],GPRSTranslationID(c,2),1);
	pGetInfo2->m_CheckData[1]=0x01;
	pGetInfo2->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo2);
	Sleep(500);
	free(pGetInfo2);

}
void CRoudInfoView::OnRsUpdata()
{
	// TODO: Add your command handler code here
	ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
	//memcpy(pGetInfo2->m_ID,m_szLID1.GetBuffer(),4);
	pGetInfo->m_First[0]=0x2F;
	pGetInfo->m_First[1]=0x43;
	pGetInfo->m_First[2]=0x2F;
	pGetInfo->m_First[3]=0x01;
	memcpy(pGetInfo->m_ID,theApp.TID+1,4);
	pGetInfo->m_OrderType[0]=0x1A;
	pGetInfo->m_OrderObject[0]=0x32;
	pGetInfo->m_ActiveType[0]=0xBD;
	pGetInfo->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo);
	Sleep(500);
	free(pGetInfo);
}

U8* CRoudInfoView::GPRSTranslationID(U8* buffer, int Length){	U8 c[1];	ZeroMemory(c,1);	int nCount = 0;	CString temp;	int i,j;	for (i=0;i<Length/2;i++)	{		CString temp="";		for (j=nCount*2;j<nCount*2+2;j++)		{			temp+=buffer[j];		}		sscanf(temp,"%2x",&c[nCount]);		nCount++;	}	return c;}

void CRoudInfoView::OnRc2Updata()
{
	// TODO: Add your command handler code here
	ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
	//memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
	memcpy(pGetInfo2->m_ID,m_szLID1.GetBuffer(),4);
	pGetInfo2->m_First[0]=0x2F;
	pGetInfo2->m_First[1]=0x43;
	pGetInfo2->m_First[2]=0x2F;
	pGetInfo2->m_First[3]=0x01;
	pGetInfo2->m_OrderType[0]=0x1A;
	pGetInfo2->m_OrderObject[0]=0x33;
	pGetInfo2->m_ActiveType[0]=0xBD;
	U8 c[2];
	ZeroMemory(c,2);
	memcpy(c,m_szLID1.GetBuffer()+4,2);
	memcpy(&pGetInfo2->m_CheckData[0],GPRSTranslationID(c,2),1);
	pGetInfo2->m_CheckData[1]=0x02;
	pGetInfo2->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo2);
	Sleep(500);
	free(pGetInfo2);
}

void CRoudInfoView::OnRc3Updata()
{
	// TODO: Add your command handler code here
	ConTrlInfo* pGetInfo2=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo2,sizeof(ConTrlInfo));
	//memcpy(pGetInfo2->m_ID,theApp.m_RoadListInfo[i]->m_RoadID,4);
	memcpy(pGetInfo2->m_ID,m_szLID1.GetBuffer(),4);
	pGetInfo2->m_First[0]=0x2F;
	pGetInfo2->m_First[1]=0x43;
	pGetInfo2->m_First[2]=0x2F;
	pGetInfo2->m_First[3]=0x01;
	pGetInfo2->m_OrderType[0]=0x1A;
	pGetInfo2->m_OrderObject[0]=0x33;
	pGetInfo2->m_ActiveType[0]=0xBD;
	U8 c[2];
	ZeroMemory(c,2);
	memcpy(c,m_szLID1.GetBuffer()+4,2);
	memcpy(&pGetInfo2->m_CheckData[0],GPRSTranslationID(c,2),1);
	pGetInfo2->m_CheckData[1]=0x03;
	pGetInfo2->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo2);
	Sleep(500);
	free(pGetInfo2);
}
void CRoudInfoView::UpdataOneRoad(RoadListViewInfo* pGetInfo)
{
	CString str1;
	int nRow = m_List.GetItemCount();
	for (int i=0;i<6;i++)
	{
		str1+=pGetInfo->m_RoadID[i];
	}
	
	for (int n=0;n<nRow;n++)
	{
		CString str2=_T("");
		str2=m_List.GetItemText(n,2);
		if(strcmp(str1,str2)==0)
		{
			m_List.DeleteItem(n);
			CString strTName=_T("");
			CString strRName=_T("");
			CString strCurrent1=_T("");
			CString strCurrent2=_T("");
			CString strCurrent3=_T("");
			CString strRID  =_T("");
			strCurrent1.Format("%0.3fA",pGetInfo->nCurrent1/1000);
			strCurrent2.Format("%0.3fA",pGetInfo->nCurrent2/1000);
			strCurrent3.Format("%0.3fA",pGetInfo->nCurrent3/1000);
			for (int j=0;j<20;j++)
			{
				strRName+=pGetInfo->m_RoadName[j];
			}
			for (int m=0;m<20;m++)
			{
				strTName+=pGetInfo->m_TernimalName[m];
			}
			for(int k=0;k<6;k++)
			{
				strRID+=pGetInfo->m_RoadID[k];
			}
			switch (pGetInfo->m_Update)
			{
			case 0x00:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,UNUPDATA,UNUPDATA,UNUPDATA);
				break;
			case 0x10:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,UNUPDATA,UNUPDATA,strCurrent3);
				break;
			case 0x20:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,UNUPDATA,strCurrent2,UNUPDATA);
				break;
			case 0x30:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,UNUPDATA,strCurrent2,strCurrent3);
				break;
			case 0x40:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,strCurrent1,UNUPDATA,UNUPDATA);
				break;
			case 0x50:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,strCurrent1,UNUPDATA,strCurrent3);
				break;
			case 0x60:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,strCurrent1,strCurrent2,UNUPDATA);
				break;
			case 0x70:
				m_List.InsertItem(n,strTName,strRName,strRID,
					UNUPDATA,strCurrent1,strCurrent2,strCurrent3);
				break;
			case 0x80:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,UNUPDATA,UNUPDATA,UNUPDATA);
				}
				else
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						OFF,UNUPDATA,UNUPDATA,UNUPDATA);
				}
				break;
			case 0x90:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,UNUPDATA,UNUPDATA,strCurrent3);
				}
				else
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						OFF,UNUPDATA,UNUPDATA,strCurrent3);
				}
				break;
			case 0xA0:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,UNUPDATA,strCurrent2,UNUPDATA);
				}
				else
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						OFF,UNUPDATA,strCurrent2,UNUPDATA);
				}
				break;
			case 0xB0:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,UNUPDATA,strCurrent2,strCurrent3);
				}
				else
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						OFF,UNUPDATA,strCurrent2,strCurrent3);
				}
				break;
			case 0xC0:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,strCurrent1,UNUPDATA,UNUPDATA);
				}
				else
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						OFF,strCurrent1,UNUPDATA,UNUPDATA);
				}
				break;
			case 0xD0:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,strCurrent1,UNUPDATA,strCurrent3);
				}
				else
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						OFF,strCurrent1,UNUPDATA,strCurrent3);
				}
				break;
			case 0xE0:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,strCurrent1,strCurrent2,UNUPDATA);
				}
				else
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						OFF,strCurrent1,strCurrent2,UNUPDATA);
				}
				break;
			case 0xF0:
				if (pGetInfo->m_RoadStatus)
				{
					m_List.InsertItem(n,strTName,strRName,strRID,
						ON,strCurrent1,strCurrent2,strCurrent3);
				}
				else
				{
					m_List.InsertItem(0,strTName,strRName,strRID,
						OFF,strCurrent1,strCurrent2,strCurrent3);
				}
				break;
			default:
				break;
				}
		}
	}
}