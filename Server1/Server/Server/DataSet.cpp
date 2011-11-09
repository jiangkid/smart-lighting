#include "StdAfx.h"
#include "DataSet.h"

CDataSet::CDataSet(void)
{
	//初始化RecordSet对象
	m_pRecordset.CreateInstance("ADODB.RecordSet");
	m_Actived = FALSE;
}

CDataSet::~CDataSet(void)
{
	//释放RecordSet对象
	m_pRecordset.Release();
}

BOOL CDataSet::Open(CString SQL)
{
	ASSERT(m_cnn);
	ASSERT(m_cnn->m_Actived);
	try
	{
		m_pRecordset->Open(_variant_t(SQL),_variant_t((IDispatch*)m_cnn,true),adOpenStatic,adLockOptimistic,adCmdText);
		m_Actived = TRUE;
		return TRUE;
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
}

void CDataSet::MovFirst()
{
	m_pRecordset->MoveFirst();
}

void CDataSet::MoveLast()
{
	m_pRecordset->MoveLast();
}

void CDataSet::MovNext()
{
	m_pRecordset->MoveNext();
}

void CDataSet::MovePrevious()
{
	m_pRecordset->MovePrevious();
}

BOOL CDataSet::IsEOF()
{
	return m_pRecordset->adoEOF;
}

BOOL CDataSet::IsBOF()
{
	return m_pRecordset->adoBOF;
}

//
//读取数据表内的数据
//
CString CDataSet::GetAsString(CString FieldName)
{
	CString sResult;
	ASSERT(!IsBOF()&&!IsEOF());
	try
	{	
		sResult = m_pRecordset->GetCollect(_variant_t(FieldName));
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
	return sResult;
}

/*
BOOL CDataSet::LoadData()
{
	return FALSE;
}
*/

void CDataSet::Close()
{
	if (m_Actived)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
	}
}

void CDataSet::SetAsString(CString FieldName,CString Value) 
{
	try
	{
		if (!IsBOF()&&!IsEOF())
		{
			m_pRecordset->PutCollect(_variant_t(FieldName),_variant_t(Value));
		}
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

void CDataSet::Update()
{
	ASSERT(!IsEOF()&&!IsBOF());
	m_pRecordset->Update(); 
}
