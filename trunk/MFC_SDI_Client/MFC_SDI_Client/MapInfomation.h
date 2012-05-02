#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CMapInfomation dialog

class CMapInfomation : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CMapInfomation)

public:
	CMapInfomation(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapInfomation();
// Overrides
	void Func1(const CString strLongtitude,const CString strLatitude);

	virtual void OnFinalRelease();
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_HTMLMAP, IDH = IDR_HTML2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	bool CallJScript(const CString strFunc, _variant_t* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CString strArg1, _variant_t* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CString strArg1,const CString strArg2,_variant_t* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CString strArg1, const CString strArg2,const CString strArg3, const CString strArg4,
					 const CString strArg5,const CString strArg6,const CString strArg7,const CString strArg8,_variant_t* pVarResult);
	bool CallJScript(const CString strFunc , const CStringArray& paramArray, _variant_t* pVarResult = NULL);
	bool GetJScript(CComPtr<IDispatch>& spDisp);
};
