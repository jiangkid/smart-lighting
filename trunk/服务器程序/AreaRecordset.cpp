#include "StdAfx.h"
#include "AreaRecordset.h"

CAreaRecordset::CAreaRecordset(void)
{
}

CAreaRecordset::~CAreaRecordset(void)
{
}

//
//����Area���е���������
//
BOOL CAreaRecordset::LoadAreaData()
{
	//ʹ��Open�����������е�����
	return Open("Select * From Areas");
}

//
//���������ID
//
void CAreaRecordset::SetAreaID(int ID)
{
	CString strID;
	strID.Format("%d",ID);
	SetAsString("AreaID",strID);
}

//
//�������������
//
void CAreaRecordset::SetAreaName(CString AreaName)
{
	SetAsString("AreaName",AreaName);
}

