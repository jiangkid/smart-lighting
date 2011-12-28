#include "StdAfx.h"
#include "AreaRecordset.h"

CAreaRecordset::CAreaRecordset(void)
{
}

CAreaRecordset::~CAreaRecordset(void)
{
}

//
//加载Area表中的所有数据
//
BOOL CAreaRecordset::LoadAreaData()
{
	//使用Open方法加载所有的数据
	return Open("Select * From Areas");
}

//
//设置区域的ID
//
void CAreaRecordset::SetAreaID(int ID)
{
	CString strID;
	strID.Format("%d",ID);
	SetAsString("AreaID",strID);
}

//
//设置区域的名称
//
void CAreaRecordset::SetAreaName(CString AreaName)
{
	SetAsString("AreaName",AreaName);
}

