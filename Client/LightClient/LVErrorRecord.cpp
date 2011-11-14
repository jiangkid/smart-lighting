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

extern CLightClientApp theApp;       // 在此引用应用类中的theApp来获取库连接指针
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
	static CString csText[] = {_T(" "),_T("编号"),_T("报警建筑"),_T("报警楼层"),_T("报警节点"),
								_T("报警时间"),_T("报警类型"),_T("详细信息")};
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
	// 使用ADO创建数据库记录集
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	// 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
	// 因为它有时会经常出现一些想不到的错误。jingzhou xu
	try
	{
		m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
							theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
		if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
		{
			if(!m_pRecordset->BOF)
				m_pRecordset->MoveFirst();
			// 读入库中各字段并加入列表框中
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

	//创建通信端口CMSComm类m_PrintCom
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
	//读取Access数据库,在ListCtrl中显示出来
	try             // 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
	{				// 因为它有时会经常出现一些想不到的错误。jingzhou xu
		if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
		{
			if(!m_pRecordset->BOF)
				m_pRecordset->MoveFirst();
			// 读入库中各字段并加入列表框中
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
				m_ErrorRecord->SetItemText(ucCount,2,(char*)(_bstr_t)m_pRecordset->GetCollect("报警建筑"));
				m_ErrorRecord->SetItemText(ucCount,3,(char*)(_bstr_t)m_pRecordset->GetCollect("报警楼层"));
				m_ErrorRecord->SetItemText(ucCount,4,(char*)(_bstr_t)m_pRecordset->GetCollect("报警节点"));
				m_ErrorRecord->SetItemText(ucCount,5,(char*)(_bstr_t)m_pRecordset->GetCollect("报警时间"));
				m_ErrorRecord->SetItemText(ucCount,6,(char*)(_bstr_t)m_pRecordset->GetCollect("报警类型"));
				_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("详细信息"));
				if(vtmp.vt != VT_NULL)
				{
					str = (char*)(_bstr_t)vtmp;
					m_ErrorRecord->SetItemText(ucCount,7,str);
				}
				ucLVStart++;
				ucCount++;
				m_pRecordset->MoveNext();
			}
			m_ErrorRecord->EnsureVisible(ucCount-1,TRUE);  //确保最后一个ITEM可见
			m_ErrorRecord->Invalidate(FALSE);
		}
		// 关闭ADO连接状态
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
	LPCSTR  lpszSend = "        ****结束****";
	m_PrintCom.SetOutput(COleVariant(lpszSend));
	PrintFunc(0,0);
	CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
	if(!(pMain->m_bAllPart))
	{
		//****************打印所有报警记录"*****************
		m_nNum = m_nRecordNum;
		try             // 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
		{				// 因为它有时会经常出现一些想不到的错误。jingzhou xu
			m_pRecordset->Close();
			m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
							theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
			if(!m_pRecordset->adoEOF)
				m_pRecordset->MoveLast();
			while(!m_pRecordset->BOF)
			{
				unsigned char ucChn = 0;
				CString str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警节点");
				if(str.Find('/') != -1)
				{
					ucChn = atoi(str.Right(1));
				}
				unsigned char ucB= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警建筑"));
			    unsigned char ucF = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警楼层"));
				unsigned char ucM = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警节点"));
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
				str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警类型");
				if(str == "通信故障") 
					str = "通信故障";
				else
				{
					_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("详细信息"));
					if(vtmp.vt == VT_NULL)
					{
						str = str.Mid(0,4)+"报警";
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
				str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警时间");
				str.Insert(11,"    ");
				m_PrintCom.SetOutput(COleVariant(str));
				PrintFunc(0,0);
				m_pRecordset->MovePrevious();
				m_nNum--;
				PrintFunc(1,3);
			}
			// 关闭ADO连接状态
			m_pRecordset->Close();
			m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
							theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
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
			//****************按序号打印指定报警记录"*****************
			unsigned int nFirst = _ttoi(pMain->m_lpszNum1);
			unsigned int nLast  = _ttoi(pMain->m_lpszNum2);
			try             // 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
			{				// 因为它有时会经常出现一些想不到的错误。jingzhou xu
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
								theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
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
					CString str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警节点");
					if(str.Find('/') != -1)
					{
						ucChn = atoi(str.Right(1));
					}
					unsigned char ucB= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警建筑"));
					unsigned char ucF = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警楼层"));
					unsigned char ucM = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警节点"));
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
					str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警类型");
					if(str == "通信故障") 
						str = "通信故障";
					else
					{
						_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("详细信息"));
						if(vtmp.vt == VT_NULL)
						{
							str = str.Mid(0,4)+"报警";
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
					str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警时间");
					str.Insert(11,"    ");
					m_PrintCom.SetOutput(COleVariant(str));
					PrintFunc(0,0);
					m_pRecordset->MovePrevious();
					m_nNum--;
					PrintFunc(1,3);
				}
				// 关闭ADO连接状态
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
								theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
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
			//****************按日期打印指定报警记录"*****************
			try             // 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
			{				// 因为它有时会经常出现一些想不到的错误。jingzhou xu
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
								theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
				if(!m_pRecordset->adoEOF)
					m_pRecordset->MoveLast();
				m_nNum = m_nRecordNum;
				while(!m_pRecordset->BOF)
				{
					CString str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警时间");
					str = str.Left(10);
					LPTSTR lpszDate = new TCHAR[str.GetLength()+1];
					lstrcpy( lpszDate, str);
					if((strcmp(pMain->m_lpszDate2,lpszDate) >= 0) && (strcmp(lpszDate,pMain->m_lpszDate1) >= 0))
					{
						unsigned char ucChn = 0;
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警节点");
						if(str.Find('/') != -1)
						{
							ucChn = atoi(str.Right(1));
						}
						unsigned char ucB= atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警建筑"));
						unsigned char ucF = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警楼层"));
						unsigned char ucM = atoi((char*)(_bstr_t)m_pRecordset->GetCollect("报警节点"));
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
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警类型");
						if(str == "通信故障") 
							str = "通信故障";
						else
						{
							_variant_t vtmp = m_pRecordset->GetCollect(_variant_t("详细信息"));
							if(vtmp.vt == VT_NULL)
							{
								str = str.Mid(0,4)+"报警";
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
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警时间");
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
				// 关闭ADO连接状态
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
								theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
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
	lpszSend = "            节点          报警";
	m_PrintCom.SetOutput(COleVariant(lpszSend));
	PrintFunc(0,0);
	lpszSend = "序号        日期          时间";
	m_PrintCom.SetOutput(COleVariant(lpszSend));
	//PrintFunc(1,10);
	//PrintFunc(0,0);
	lpszSend = "          ****报警记录****";
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
    if(ucFunc == 0)                             //0,输出0x0D,打印换行
	{
		str.Format("%c",0x0D);
		m_PrintCom.SetOutput(COleVariant(str));
	}
    if(ucFunc == 1)								//1,输出0x01B,0X31,n,设置n点行间距
	{
		str.Format("%c%c%c",0x1B,0x31,n);
		m_PrintCom.SetOutput(COleVariant(str));
	}
}

void CLVErrorRecord::OpenPrintCom()
{
    if(m_PrintCom.GetPortOpen())       //若串口已打开,则先关闭
		m_PrintCom.SetPortOpen(FALSE);
	CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
	m_PrintCom.SetCommPort(3);   //设定为所选的串口
		
	if(!m_PrintCom.GetPortOpen())       //打开串口
	{
		m_PrintCom.SetPortOpen(TRUE);
		m_PrintCom.SetSettings("9600,N,8,1");  //9600,n,8,1
		m_PrintCom.SetRThreshold(2); //参数n表示,每当串口接收缓冲区中有多余或等于n个字符时,将引发
									//一个接收数据的OnComm事件
		m_PrintCom.SetInputMode(1); //二进制模式或文本模式
		m_PrintCom.SetInputLen(0); //设置当前接收区数据长度为0
		m_PrintCom.GetInput(); //先预读缓冲区以清除残留数据
	}
	else AfxMessageBox("无法打开打印端口");
}

void CLVErrorRecord::OnDelAlarmRecord()
{
	// TODO: Add your specialized creation code here
	CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
	if(AfxMessageBox("您确定要删除报警记录吗？",MB_OKCANCEL|MB_ICONEXCLAMATION) == IDOK)
	{
		try             // 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
		{				// 因为它有时会经常出现一些想不到的错误。jingzhou xu
			// 关闭ADO重新打开
			m_pRecordset->Close();
			m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
							theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
			if(!pMain->m_bAllPart) //删除所有记录
			{
				if(!m_pRecordset->BOF)
					m_pRecordset->MoveFirst();
				while(!m_pRecordset->adoEOF)
				{
					m_pRecordset->Update();
					m_pRecordset->Delete(adAffectCurrent);
					m_pRecordset->MoveNext();
				}
				// 关闭ADO重新打开
				m_pRecordset->Close();
				m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
								theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
								adOpenDynamic,
								adLockOptimistic,
								adCmdText);
				m_nRecordNum = 0;
			}
			else
			{   //删除指定记录
				if(!pMain->m_bNumDate)//按序号删除记录
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
					// 关闭ADO重新打开
					m_pRecordset->Close();
					m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
									theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
									adOpenDynamic,
									adLockOptimistic,
									adCmdText);
					if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
					{
						if(!m_pRecordset->BOF)
							m_pRecordset->MoveFirst();
						// 读入库中各字段并加入列表框中
						m_nRecordNum = 0;
						while(!m_pRecordset->adoEOF)
						{
							m_nRecordNum++;
							m_pRecordset->MoveNext();
						}
					}
					else m_nRecordNum = 0;
				}
				else //按日期删除记录
				{
					if(!m_pRecordset->adoEOF)
						m_pRecordset->MoveFirst();
					CString str;
					while((!m_pRecordset->adoEOF))
					{
						str = (char*)(_bstr_t)m_pRecordset->GetCollect("报警时间");
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
					// 关闭ADO重新打开
					m_pRecordset->Close();
					m_pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
									theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
									adOpenDynamic,
									adLockOptimistic,
									adCmdText);
					if(!((m_pRecordset->BOF)&&(m_pRecordset->adoEOF)))
					{
						if(!m_pRecordset->BOF)
							m_pRecordset->MoveFirst();
						// 读入库中各字段并加入列表框中
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
