#include "StdAfx.h"
#include "DBRecordset.h"

CDBRecordset::CDBRecordset(void)
{
	//初始化RecordSet对象
	m_pRs.CreateInstance("ADODB.RecordSet");
	m_Actived = FALSE;
}

CDBRecordset::~CDBRecordset(void)
{
	//释放RecordSet对象
	m_pRs.Release();
}
BOOL CDBRecordset::Open(CString SQL) 
{
	ASSERT(m_cnn);
	ASSERT(m_cnn->m_Actived);
	HRESULT hResult;
	try
	{
		m_pRs->Open(_variant_t(SQL),_variant_t(m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);/*==S_OK )*/
		m_Actived = TRUE; 
		if (m_pRs->RecordCount>0)
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		::AfxMessageBox(e.ErrorMessage());
		return FALSE;
	}
}


void CDBRecordset::MoveFirst()
{
	m_pRs->MoveFirst();
}

void CDBRecordset::MoveLast()
{
	m_pRs->MoveLast();
}

void CDBRecordset::MoveNext()
{
	m_pRs->MoveNext();
}

void CDBRecordset::MovePrevious()
{
	m_pRs->MovePrevious();
}

BOOL CDBRecordset::IsBOF()
{
	return m_pRs->adoBOF;
}

BOOL CDBRecordset::IsEOF()
{
	return m_pRs->adoEOF;
}

CString CDBRecordset::GetAsString(CString FieldName)
{
//	ASSERT(!IsEOF()&&!IsBOF());

	_variant_t vValue = m_pRs->Fields->Item[_variant_t(FieldName)]->Value;

	//如果为空值则返回空
	if ((V_VT(&vValue)==VT_NULL)||(V_VT(&vValue)==VT_EMPTY))
	{
		return "";
	}
	return _com_util::ConvertBSTRToString(_bstr_t(vValue));
}

BOOL CDBRecordset::LoadData()
{
	return FALSE;
}

void CDBRecordset::Close()
{
	if (m_Actived)
	{
		m_pRs->Close();
	}
}

void CDBRecordset::SetAsString(CString FieldName,CString strValue)
{
//	ASSERT(!IsBOF()&&!IsEOF());	
	BSTR bstrValue=strValue.AllocSysString();
	m_pRs->Fields->Item[_variant_t(FieldName)]->Value = bstrValue;//有错误
	m_pRs->Update();
	SysFreeString(bstrValue);
}

void CDBRecordset::Update()
{
	ASSERT(!IsBOF()&&!IsEOF());
	m_pRs->Update();
}

void CDBRecordset::SetAsInt(CString fieldName,int intValue)
{
	CString strValue;
	strValue.Format("%d",intValue);
	BSTR bstrValue=strValue.AllocSysString();
	ASSERT(!IsBOF()&&!IsEOF());
	m_pRs->Fields->Item[_variant_t(fieldName)]->Value=bstrValue;
	m_pRs->Update();
	SysFreeString(bstrValue);
}

//  void CDBRecordset::SetAsFloat(CString fieldName,int intValue)
 //  {
 // 	 CString strValue;
 // 	 strValue.Format("%f",intValue);
 // 	 BSTR bstrValue=strValue.AllocSysString();
 // 	 ASSERT(!IsBOF()&&!IsEOF());
 // 	 m_pRs->Fields->Item[_variant_t(fieldName)]->Value=bstrValue;
 // 	 m_pRs->Update();
 // 	 SysFreeString(bstrValue);
 //  }