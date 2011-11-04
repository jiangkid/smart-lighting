#include "StdAfx.h"
#include "DataSet.h"

CDataSet::CDataSet(void)
{
	//初始化RecordSet对象
	m_pRs.CreateInstance("ADODB.RecordSet");
	m_Actived = FALSE;
}

CDataSet::~CDataSet(void)
{
	//释放RecordSet对象
	m_pRs.Release();
}

BOOL CDataSet::Open(CString SQL)
{
	ASSERT(m_cnn);
	ASSERT(m_cnn->m_Actived);
	try
	{
		m_pRs->Open(_variant_t(SQL),_variant_t(m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
		m_Actived = TRUE;
		return TRUE;
	}
	catch(_com_error&e)
	{
		::AfxMessageBox(e.ErrorMessage());
		return FALSE;
	}
}

void CDataSet::MovFirst()
{
	m_pRs->MoveFirst();
}

void CDataSet::MoveLast()
{
	m_pRs->MoveLast();
}

void CDataSet::MovNext()
{
	m_pRs->MoveNext();
}

void CDataSet::MovePrevious()
{
	m_pRs->MovePrevious();
}

BOOL CDataSet::IsEOF()
{
	return m_pRs->EndOfFile;
}

BOOL CDataSet::IsBOF()
{
	return m_pRs->BOF;
}

CString CDataSet::GetAsString(CString FieldName)
{
	ASSERT(!IsBOF()&&!IsEOF());

	_variant_t vValue = m_pRs->Fields->Item[_variant_t(FieldName)]->Value;

	//如果为空值则返回空
	if((V_VT(&vValue)==VT_NULL)||(V_VT(&vValue)==VT_EMPTY))
	{
		return  _T("");
	}
	return (CString)_com_util::ConvertBSTRToString(_bstr_t(vValue));
}

BOOL CDataSet::LoadData()
{
	return FALSE;
}

void CDataSet::Close()
{
	if (m_Actived)
	{
		m_pRs->Close(); 
	}
}

void CDataSet::SetAsString(CString FieldName,CString Value) 
{
	ASSERT(!IsBOF()&&!IsEOF());
	m_pRs->Fields->Item[_variant_t(FieldName)]->Value = _variant_t(Value);
}

void CDataSet::Update()
{
	ASSERT(!IsEOF()&&!IsBOF());
	m_pRs->Update();
}
