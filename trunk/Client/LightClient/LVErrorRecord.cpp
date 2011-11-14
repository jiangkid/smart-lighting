// LVErrorRecord.cpp : implementation file
//

#include "stdafx.h"
#include "LightClient.h"
#include "LVErrorRecord.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLightClientApp theApp;       // �ڴ�����Ӧ�����е�theApp����ȡ������ָ��
/////////////////////////////////////////////////////////////////////////////
// CLVErrorRecord

IMPLEMENT_DYNCREATE(CLVErrorRecord, CListView)

CLVErrorRecord::CLVErrorRecord()
{
	m_nRecordNum = 0;
}

CLVErrorRecord::~CLVErrorRecord()
{
}


BEGIN_MESSAGE_MAP(CLVErrorRecord, CListView)
	//{{AFX_MSG_MAP(CLVErrorRecord)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLVErrorRecord drawing

void CLVErrorRecord::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLVErrorRecord diagnostics

#ifdef _DEBUG
void CLVErrorRecord::AssertValid() const
{
	CListView::AssertValid();
}

void CLVErrorRecord::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLVErrorRecord message handlers

int CLVErrorRecord::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ErrorRecord = &GetListCtrl();
	m_ErrorRecord->SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP 
							| LVS_EX_ONECLICKACTIVATE |LVS_EX_GRIDLINES);
	//m_ErrorRecord->ShowScrollBar(SB_VERT);
	static int csLength[] = {0,75,75,75,75,175,125,393};
	static CString csText[] = {_T(" "),_T("���"),_T("��������"),_T("����¥��"),_T("�����ڵ�"),
								_T("����ʱ��"),_T("��������"),_T("��ϸ��Ϣ")};
	for(int i=0; i<8; i++)
	{
		LV_COLUMN lvcol;
		lvcol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lvcol.fmt = LVCFMT_CENTER;
		lvcol.cx = csLength[i];
		lvcol.pszText = (char *)(LPCTSTR)csText[i];
		lvcol.iSubItem = i;
		m_ErrorRecord->InsertColumn(i,&lvcol);
	}
	// ʹ��ADO�������ݿ��¼��
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	// ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
	// ��Ϊ����ʱ�ᾭ������һЩ�벻���Ĵ���jingzhou xu
	try
	{
		m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
							theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
		{
			if(!m_pRecordset->BOF)
				m_pRecordset->MoveFirst();
			// ������и��ֶβ������б����
			//m_nRecordNum = 0;
			while(!m_pRecordset->adoEOF)
			{
				m_nRecordNum++;
				m_pRecordset->MoveNext();
			}
			OnRead();
		}
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	//����ͨ�Ŷ˿�CMSComm��m_PrintCom
	DWORD style=WS_VISIBLE | WS_CHILD;
	if(!m_PrintCom.Create(NULL,style,CRect(0,0,0,0),this,ID_MSCOMM_PRINT,NULL))
	{
		TRACE0("Failed to create OLE Communications Control\n");
		return -1;
	}
	
	CLightClientApp *pApp=(CLightClientApp *)AfxGetApp();
	pApp->pRecord = this;
	return 0;
}

BOOL CLVErrorRecord::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_REPORT | LVS_ALIGNTOP | LVS_SHOWSELALWAYS;// | LVS_NOSCROLL;
	
	return CListView::PreCreateWindow(cs);
}

void CLVErrorRecord::OnRead()
{
	//��ȡAccess���ݿ�,��ListCtrl����ʾ����
	try             // ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
	{				// ��Ϊ����ʱ�ᾭ������һЩ�벻���Ĵ���jingzhou xu
		if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
		{
			if(!m_pRecordset->BOF)
				m_pRecordset->MoveFirst();
			// ������и��ֶβ������б����
			//m_nRecordNum = 0;
			unsigned int ucLVStart,ucCount = 0;	
			if(m_nRecordNum >20) ucLVStart = m_nRecordNum-20;
			else ucLVStart = 0;
			m_pRecordset->Move(ucLVStart,vtMissing);
			while(!m_pRecordset->adoEOF)
			{
				CString str;
				str.Format("%d",ucLVStart+1);
				m_ErrorRecord->InsertItem(ucCount,"");
				m_ErrorRecord->SetItemText(ucCount,1,str);
				m_ErrorRecord->SetItemText(ucCount,2,(char*)(_bstr_t)m_pRecordset->GetCollect("��������"));
				m_ErrorRecord->SetItemText(ucCount,3,(char*)(_bstr_t)m_pRecordset->GetCollect("����¥��"));
				m_ErrorRecord->SetItemText(ucCount,4,(char*)(_bstr_t)m_pRecordset->GetCollect("�����ڵ�"));
				m_ErrorRecord->SetItemText(ucCount,5,(char*)(_bstr_t)m_pRecordset->GetCollect("����ʱ��"));
				m_ErrorRecord->SetItemText(ucCount,6,(char*)(_bstr_t)m_pRecordset->GetCollect("��������"));
				_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("��ϸ��Ϣ"));
				if(vtmp.vt != VT_NULL)
				{
					str = (char*)(_bstr_t)vtmp;
					m_ErrorRecord->SetItemText(ucCount,7,str);
				}
				ucLVStart++;
				ucCount++;
				m_pRecordset->MoveNext();
			}
			m_ErrorRecord->EnsureVisible(ucCount-1,TRUE);  //ȷ�����һ��ITEM�ɼ�
			m_ErrorRecord->Invalidate(FALSE);
		}
		// �ر�ADO����״̬
	    //m_pRecordset->Close();
        //m_pRecordset = NULL; 
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

//BOOL bF = FALSE;
void CLVErrorRecord::OnPrintAlarmRecord() 
{
	PrintFunc(0,0);
	PrintFunc(0,0);
	LPCSTR  lpszSend = "        ****����****";
	m_PrintCom.SetOutput(COleVariant(lpszSend));
	PrintFunc(0,0);
	CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
	if(!(pMain->m_bAllPart))
	{
		//****************��ӡ���б�����¼"*****************
		m_nNum = m_nRecordNum;
		try             // ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
		{				// ��Ϊ����ʱ�ᾭ������һЩ�벻���Ĵ���jingzhou xu
			m_pRecordset->Close();
			m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
							theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
			if(!m_pRecordset->adoEOF)
				m_pRecordset->MoveLast();
			while(!m_pRecordset->BOF)
			{
				unsigned char ucChn = 0;
				CString str = (char*)(_bstr_t)m_pRecordset->GetCollect("�����ڵ�");
				if(str.Find('/') != -1)
				{
					ucChn = atoi(str.Right(1));
				}
				unsigned char ucB= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("��������"));
			    unsigned char ucF = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("����¥��"));
				unsigned char ucM = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("�����ڵ�"));
				str.Format("         %d-%d-%d   ",ucB,ucF,ucM);
				if(ucB<10) str.Insert(9,0x30);
				if(ucF<100) 
				{
					str.Insert(12,0x30);
					if(ucF<10) str.Insert(13,0x30);
				}
				if(ucM<100) 
				{
					str.Insert(16,0x30);
					if(ucM<10) str.Insert(17,0x30);
				}
				if(ucChn > 0)
				{
					str.Insert(19,"/");
					str.Insert(20,0x30+ucChn);
				}
				else 
				{
					str.Insert(19,0x20);
					str.Insert(20,0x20);
				}
				m_PrintCom.SetOutput(COleVariant(str));
				str = (char*)(_bstr_t)m_pRecordset->GetCollect("��������");
				if(str == "ͨ�Ź���") 
					str = "ͨ�Ź���";
				else
				{
					_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("��ϸ��Ϣ"));
					if(vtmp.vt == VT_NULL)
					{
						str = str.Mid(0,4)+"����";
					}
					else
					{
						str = (char*)(_bstr_t)vtmp;
						str = str.Mid(31,5);
					}
				}
				m_PrintCom.SetOutput(COleVariant(str));
				PrintFunc(0,0);
				PrintFunc(1,10);
				str.Format("%d",m_nNum);
				if(str.GetLength() == 1) str = "00"+str+"      ";
				else if(str.GetLength() == 2) str = "0"+str+"      ";
				else str += "      ";
				m_PrintCom.SetOutput(COleVariant(str));
				str = (char*)(_bstr_t)m_pRecordset->GetCollect("����ʱ��");
				str.Insert(11,"    ");
				m_PrintCom.SetOutput(COleVariant(str));
				PrintFunc(0,0);
				m_pRecordset->MovePrevious();
				m_nNum--;
				PrintFunc(1,3);
			}
			// �ر�ADO����״̬
			m_pRecordset->Close();
			m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
							theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}                   
	else
	{
		if(!(pMain->m_bNumDate))
		{
			//****************����Ŵ�ӡָ��������¼"*****************
			unsigned int nFirst = _ttoi(pMain->m_lpszNum1);
			unsigned int nLast  = _ttoi(pMain->m_lpszNum2);
			try             // ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
			{				// ��Ϊ����ʱ�ᾭ������һЩ�벻���Ĵ���jingzhou xu
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
								theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
				if(!m_pRecordset->BOF)
					m_pRecordset->MoveFirst();			
				m_pRecordset->Move(nLast-1,vtMissing);
				m_nNum = nLast;
				while(m_nNum >= nFirst)
				{
					unsigned char ucChn = 0;
					CString str = (char*)(_bstr_t)m_pRecordset->GetCollect("�����ڵ�");
					if(str.Find('/') != -1)
					{
						ucChn = atoi(str.Right(1));
					}
					unsigned char ucB= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("��������"));
					unsigned char ucF = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("����¥��"));
					unsigned char ucM = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("�����ڵ�"));
					str.Format("         %d-%d-%d   ",ucB,ucF,ucM);
					if(ucB<10) str.Insert(9,0x30);
					if(ucF<100) 
					{
						str.Insert(12,0x30);
						if(ucF<10) str.Insert(13,0x30);
					}
					if(ucM<100) 
					{
						str.Insert(16,0x30);
						if(ucM<10) str.Insert(17,0x30);
					}
					if(ucChn > 0)
					{
						str.Insert(19,"/");
						str.Insert(20,0x30+ucChn);
					}
					else 
					{
						str.Insert(19,0x20);
						str.Insert(20,0x20);
					}
					m_PrintCom.SetOutput(COleVariant(str));
					str = (char*)(_bstr_t)m_pRecordset->GetCollect("��������");
					if(str == "ͨ�Ź���") 
						str = "ͨ�Ź���";
					else
					{
						_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("��ϸ��Ϣ"));
						if(vtmp.vt == VT_NULL)
						{
							str = str.Mid(0,4)+"����";
						}
						else
						{
							str = (char*)(_bstr_t)vtmp;
							str = str.Mid(31,5);
						}
					}
					m_PrintCom.SetOutput(COleVariant(str));
					PrintFunc(0,0);
					PrintFunc(1,10);
					str.Format("%d",m_nNum);
					if(str.GetLength() == 1) str = "00"+str+"      ";
					else if(str.GetLength() == 2) str = "0"+str+"      ";
					else str += "      ";
					m_PrintCom.SetOutput(COleVariant(str));
					str = (char*)(_bstr_t)m_pRecordset->GetCollect("����ʱ��");
					str.Insert(11,"    ");
					m_PrintCom.SetOutput(COleVariant(str));
					PrintFunc(0,0);
					m_pRecordset->MovePrevious();
					m_nNum--;
					PrintFunc(1,3);
				}
				// �ر�ADO����״̬
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
								theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
			}
			catch(_com_error *e)
			{
					AfxMessageBox(e->ErrorMessage());
			}
		}
		else
		{
			//****************�����ڴ�ӡָ��������¼"*****************
			try             // ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
			{				// ��Ϊ����ʱ�ᾭ������һЩ�벻���Ĵ���jingzhou xu
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
								theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
				if(!m_pRecordset->adoEOF)
					m_pRecordset->MoveLast();
				m_nNum = m_nRecordNum;
				while(!m_pRecordset->BOF)
				{
					CString str = (char*)(_bstr_t)m_pRecordset->GetCollect("����ʱ��");
					str = str.Left(10);
					LPTSTR lpszDate = new TCHAR[str.GetLength()+1];
					lstrcpy( lpszDate, str);
					if((strcmp(pMain->m_lpszDate2,lpszDate) >= 0) && (strcmp(lpszDate,pMain->m_lpszDate1) >= 0))
					{
						unsigned char ucChn = 0;
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("�����ڵ�");
						if(str.Find('/') != -1)
						{
							ucChn = atoi(str.Right(1));
						}
						unsigned char ucB= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("��������"));
						unsigned char ucF = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("����¥��"));
						unsigned char ucM = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("�����ڵ�"));
						str.Format("         %d-%d-%d   ",ucB,ucF,ucM);
						if(ucB<10) str.Insert(9,0x30);
						if(ucF<100) 
						{
							str.Insert(12,0x30);
							if(ucF<10) str.Insert(13,0x30);
						}
						if(ucM<100) 
						{
							str.Insert(16,0x30);
							if(ucM<10) str.Insert(17,0x30);
						}
						if(ucChn > 0)
						{
							str.Insert(19,"/");
							str.Insert(20,0x30+ucChn);
						}
						else 
						{
							str.Insert(19,0x20);
							str.Insert(20,0x20);
						}
						m_PrintCom.SetOutput(COleVariant(str));
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("��������");
						if(str == "ͨ�Ź���") 
							str = "ͨ�Ź���";
						else
						{
							_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("��ϸ��Ϣ"));
							if(vtmp.vt == VT_NULL)
							{
								str = str.Mid(0,4)+"����";
							}
							else
							{
								str = (char*)(_bstr_t)vtmp;
								str = str.Mid(31,5);
							}
						}
						m_PrintCom.SetOutput(COleVariant(str));
						PrintFunc(0,0);
						PrintFunc(1,10);
						str.Format("%d",m_nNum);
						if(str.GetLength() == 1) str = "00"+str+"      ";
						else if(str.GetLength() == 2) str = "0"+str+"      ";
					    else str += "      ";
						m_PrintCom.SetOutput(COleVariant(str));
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("����ʱ��");
						str.Insert(11,"    ");
						m_PrintCom.SetOutput(COleVariant(str));
						PrintFunc(0,0);
					}
					delete lpszDate;
					lpszDate = NULL;
					m_pRecordset->MovePrevious();
					m_nNum--;
					PrintFunc(1,3);
				}
				// �ر�ADO����״̬
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
								theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
			}
			catch(_com_error *e)
			{
					AfxMessageBox(e->ErrorMessage());
			}
		}
	}
	PrintFunc(1,3);
	lpszSend = "            �ڵ�          ����";
	m_PrintCom.SetOutput(COleVariant(lpszSend));
	PrintFunc(0,0);
	lpszSend = "���        ����          ʱ��";
	m_PrintCom.SetOutput(COleVariant(lpszSend));
	//PrintFunc(1,10);
	//PrintFunc(0,0);
	lpszSend = "          ****������¼****";
	m_PrintCom.SetOutput(COleVariant(lpszSend));
	PrintFunc(0,0);
	PrintFunc(0,0);
	PrintFunc(0,0);
	PrintFunc(0,0);
	PrintFunc(0,0);
}

void CLVErrorRecord::PrintFunc(unsigned char ucFunc, unsigned char n)
{
	CString str;
    if(ucFunc == 0)                             //0,���0x0D,��ӡ����
	{
		str.Format("%c",0x0D);
		m_PrintCom.SetOutput(COleVariant(str));
	}
    if(ucFunc == 1)								//1,���0x01B,0X31,n,����n���м��
	{
		str.Format("%c%c%c",0x1B,0x31,n);
		m_PrintCom.SetOutput(COleVariant(str));
	}
}

void CLVErrorRecord::OpenPrintCom()
{
    if(m_PrintCom.GetPortOpen())       //�������Ѵ�,���ȹر�
		m_PrintCom.SetPortOpen(FALSE);
	CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
	m_PrintCom.SetCommPort(3);   //�趨Ϊ��ѡ�Ĵ���
		
	if(!m_PrintCom.GetPortOpen())       //�򿪴���
	{
		m_PrintCom.SetPortOpen(TRUE);
		m_PrintCom.SetSettings("9600,N,8,1");  //9600,n,8,1
		m_PrintCom.SetRThreshold(2); //����n��ʾ,ÿ�����ڽ��ջ��������ж�������n���ַ�ʱ,������
									//һ���������ݵ�OnComm�¼�
		m_PrintCom.SetInputMode(1); //������ģʽ���ı�ģʽ
		m_PrintCom.SetInputLen(0); //���õ�ǰ���������ݳ���Ϊ0
		m_PrintCom.GetInput(); //��Ԥ���������������������
	}
	else AfxMessageBox("�޷��򿪴�ӡ�˿�");
}

void CLVErrorRecord::OnDelAlarmRecord()
{
	// TODO: Add your specialized creation code here
	CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
	if(AfxMessageBox("��ȷ��Ҫɾ��������¼��",MB_OKCANCEL|MB_ICONEXCLAMATION) == IDOK)
	{
		try             // ��ADO�����н��������Ҫ����try...catch()�����������Ϣ��
		{				// ��Ϊ����ʱ�ᾭ������һЩ�벻���Ĵ���jingzhou xu
			// �ر�ADO���´�
			m_pRecordset->Close();
			m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
							theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
			if(!pMain->m_bAllPart) //ɾ�����м�¼
			{
				if(!m_pRecordset->BOF)
					m_pRecordset->MoveFirst();
				while(!m_pRecordset->adoEOF)
				{
					m_pRecordset->Update();
					m_pRecordset->Delete(adAffectCurrent);
					m_pRecordset->MoveNext();
				}
				// �ر�ADO���´�
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
								theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
				m_nRecordNum = 0;
			}
			else
			{   //ɾ��ָ����¼
				if(!pMain->m_bNumDate)//�����ɾ����¼
				{
					int nFirst = _ttoi(pMain->m_lpszNum1);
					int nLast  = _ttoi(pMain->m_lpszNum2);
					if(!m_pRecordset->adoEOF)
						m_pRecordset->MoveFirst();
					m_pRecordset->Move(nFirst-1);
					int nCnt = nFirst;
					while((!m_pRecordset->adoEOF) && (nCnt <= nLast))
					{
						nCnt++;
						m_pRecordset->Update();
						m_pRecordset->Delete(adAffectCurrent);
						m_pRecordset->MoveNext();
					}
					// �ر�ADO���´�
					m_pRecordset->Close();
					m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
									theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
									adOpenDynamic,
									adLockOptimistic,
									adCmdText);
					if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
					{
						if(!m_pRecordset->BOF)
							m_pRecordset->MoveFirst();
						// ������и��ֶβ������б����
						m_nRecordNum = 0;
						while(!m_pRecordset->adoEOF)
						{
							m_nRecordNum++;
							m_pRecordset->MoveNext();
						}
					}
					else m_nRecordNum = 0;
				}
				else //������ɾ����¼
				{
					if(!m_pRecordset->adoEOF)
						m_pRecordset->MoveFirst();
					CString str;
					while((!m_pRecordset->adoEOF))
					{
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("����ʱ��");
						str = str.Left(10);
						LPTSTR lpszDate = new TCHAR[str.GetLength()+1];
						lstrcpy( lpszDate, str);
						if((strcmp(pMain->m_lpszDate2,lpszDate) >= 0) && (strcmp(lpszDate,pMain->m_lpszDate1) >= 0))
						{
							m_pRecordset->Update();
							m_pRecordset->Delete(adAffectCurrent);
						}
						delete lpszDate;
						lpszDate = NULL;
						m_pRecordset->MoveNext();
					}
					// �ر�ADO���´�
					m_pRecordset->Close();
					m_pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
									theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
									adOpenDynamic,
									adLockOptimistic,
									adCmdText);
					if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
					{
						if(!m_pRecordset->BOF)
							m_pRecordset->MoveFirst();
						// ������и��ֶβ������б����
						m_nRecordNum = 0;
						while(!m_pRecordset->adoEOF)
						{
							m_nRecordNum++;
							m_pRecordset->MoveNext();
						}
					}
					else m_nRecordNum = 0;
				}
			}
			m_ErrorRecord->DeleteAllItems();
			OnRead();
			pMain->ShowWindow(TRUE);
			pMain->ShowWindow(SW_MAXIMIZE);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}
}
