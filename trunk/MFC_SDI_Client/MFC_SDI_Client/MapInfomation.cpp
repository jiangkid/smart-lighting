// MapInfomation.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MapInfomation.h"


// CMapInfomation dialog

IMPLEMENT_DYNCREATE(CMapInfomation, CDHtmlDialog)

CMapInfomation::CMapInfomation(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CMapInfomation::IDD, CMapInfomation::IDH, pParent)
{
	EnableAutomation();
}

CMapInfomation::~CMapInfomation()
{
}

void CMapInfomation::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDHtmlDialog::OnFinalRelease();
}

void CMapInfomation::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CMapInfomation::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	theApp.m_pMapInfoDlg=this;
	SetExternalDispatch(GetIDispatch(TRUE));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CMapInfomation, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CMapInfomation)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()


BEGIN_DISPATCH_MAP(CMapInfomation, CDHtmlDialog)
	DISP_FUNCTION(CMapInfomation, "Func1", Func1, VT_EMPTY, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMapInfomation to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {77D95451-EBC7-42D5-99D4-7FF7485BE040}
static const IID IID_IMapInfomation =
{ 0x77D95451, 0xEBC7, 0x42D5, { 0x99, 0xD4, 0x7F, 0xF7, 0x48, 0x5B, 0xE0, 0x40 } };

BEGIN_INTERFACE_MAP(CMapInfomation, CDHtmlDialog)
	INTERFACE_PART(CMapInfomation, IID_IMapInfomation, Dispatch)
END_INTERFACE_MAP()


// CMapInfomation message handlers

HRESULT CMapInfomation::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CMapInfomation::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CMapInfomation::Func1(const CString strLongtitude,const CString strLatitude)
{
	//CString str;
	//str.Format("经度是：%s\n纬度是:%s",strLongtitude,strLatitude);
	//AfxMessageBox(_T(str));
	CString str1,str2;
	str1.Format("%s",strLongtitude);
	str2.Format("%s",strLatitude);
	theApp.m_pMapViewDlg->ShowMessage(str1,str2);
}
bool CMapInfomation::CallJScript(const CString strFunc, const CString strArg1, const CString strArg2,
								 const CString strArg3, const CString strArg4,
								 const CString strArg5, const CString strArg6,
								 const CString strArg7, const CString strArg8,_variant_t* pVarResult)
{
	CStringArray paramArray;
	paramArray.Add(strArg1);
	paramArray.Add(strArg2);
	paramArray.Add(strArg3);
	paramArray.Add(strArg4);
	paramArray.Add(strArg5);
	paramArray.Add(strArg6);
	paramArray.Add(strArg7);
	paramArray.Add(strArg8);
	return CallJScript(strFunc,paramArray,pVarResult);
}

bool CMapInfomation::CallJScript(const CString strFunc, _variant_t* pVarResult)
{
	CStringArray paramArray;
	return CallJScript(strFunc,pVarResult);
}

bool CMapInfomation::CallJScript(const CString strFunc, const CString strArg1, _variant_t* pVarResult)
{
	CStringArray paramArray;
	paramArray.Add(strArg1);
	return CallJScript(strFunc,paramArray,pVarResult);
}
bool CMapInfomation::CallJScript(const CString strFunc, const CString strArg1,const CString strArg2,_variant_t* pVarResult)
{
	CStringArray paramArray;
	paramArray.Add(strArg1);
	paramArray.Add(strArg2);
	return CallJScript(strFunc,paramArray,pVarResult);
}

bool CMapInfomation::CallJScript(const CString strFunc , const CStringArray& paramArray, _variant_t* pVarResult)
{
	CComPtr<IDispatch> spScript;
	if(!GetJScript(spScript))
	{
		MessageBox("函数GetJScrip调用失败！");
		return FALSE;
	}
	CComBSTR bstrFunc(strFunc);
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrFunc,1,
		LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		//MessageBox(GetSystemErrorMessage(hr));
		return FALSE;
	}

	INT_PTR arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = (UINT)arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];

	for( int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
	_variant_t vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg

	hr = spScript->Invoke(dispid,IID_NULL,0,
		DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	delete [] dispparams.rgvarg;
	if(FAILED(hr))
	{
		//MessageBox(GetSystemErrorMessage(hr));
		return FALSE;
	}

	if(pVarResult)
	{
		*pVarResult = vaResult;
	}
	return TRUE;

}

bool CMapInfomation::GetJScript(CComPtr<IDispatch>& spDisp)
{
	if( m_spHtmlDoc==NULL )
		return FALSE;

	HRESULT hr = this->m_spHtmlDoc->get_Script(&spDisp);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}
