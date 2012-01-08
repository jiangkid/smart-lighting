#include "StdAfx.h"
#include "AreaRecordset.h"

CAreaRecordset::CAreaRecordset(void)
{
}

CAreaRecordset::~CAreaRecordset(void)
{
}

//
//根据区域的ID设置区域的名称和区域下的路
//
BOOL CAreaRecordset::SetAreaByAreaID(CString AreaID, CString AreaName, CString Terminals)
{
	CString SQL;
	SQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(SQL))
	{
		SetAsString("AreaName",AreaName);
		SetAsString("Roads",Terminals);
		return TRUE;
	}
	return FALSE;
}

//
//根据区域的ID来获取该区域下的所有终端的ID
//
CString CAreaRecordset::GetTerminalIDsByAreaID(CString AreaID)
{
	CString AreaSQL;
	CString TerminalSQL;
	CString strID;
	CString strTerminals;
	int intID;
	_variant_t vTerminalID;
	_RecordsetPtr pRs;

	AreaSQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(AreaSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}
	TerminalSQL.Format("Select * From Terminals Where IDArea=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)TerminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vTerminalID=pRs->GetCollect("TerminalID");
		if (vTerminalID.vt!=VT_NULL)
		{
			//strTerminals+="+";
			strTerminals+=(LPCTSTR)(_bstr_t)vTerminalID;
		}
		pRs->MoveNext();
	}
	return strTerminals;
	pRs->Close();
}

//根据区域的名字获得区域的ID
CString CAreaRecordset::GetAreaIDByAreaName(CString AreaName)
{
	CString SQL;
	CString AreaID;

	SQL.Format("Select * From Areas Where AreaName = '%s'",AreaName);
	if (Open(SQL))
	{
		AreaID=GetAsString("AreaID");
		return AreaID; 
	}
	return NULL;
}

//
//根据区域的ID来获取该区域下所有终端的名称
//
CString CAreaRecordset::GetTerminalNamesByAreaID(CString AreaID)
{
	CString AreaSQL;
	CString TerminalSQL;
	CString strID;
	CString strTerminals;
	int intID;
	_variant_t vTerminalName;
	_RecordsetPtr pRs;

	AreaSQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(AreaSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	TerminalSQL.Format("Select * From Terminals Where IDArea=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)TerminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vTerminalName=pRs->GetCollect("TerminalName");
		if (vTerminalName.vt!=VT_NULL)
		{
			strTerminals+="+";
			strTerminals+=(LPCTSTR)(_bstr_t)vTerminalName;
		}
		pRs->MoveNext();
	}
	return strTerminals;

	pRs->Close();
}
/************函数功能：获得所有的区域ID**************/
CString CAreaRecordset::GetAllAreaIDAndCount()
{
	CString AreaSQL;
	_RecordsetPtr pAreaRs;
	char count=0x00;
	CString strCount;
	CString AreaIDAndCount;
	CString strAreaID;
	_variant_t vAreaID;
	AreaSQL.Format("Select * From Areas");

	try
	{
		pAreaRs.CreateInstance("ADODB.Recordset");
		pAreaRs->Open((_variant_t)AreaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pAreaRs->adoEOF)
	{
		vAreaID=pAreaRs->GetCollect("AreaID");
		if (vAreaID.vt!=NULL)
		{
			//strAreaID+="<";
			strAreaID+=(LPCTSTR)(_bstr_t)vAreaID;
			//strAreaID+=">";
		}
		count++;
		pAreaRs->MoveNext();
	}
	AreaIDAndCount=count+strAreaID;
	AreaIDAndCount+="#";
	return AreaIDAndCount;
}

//根据区域的ID来获取该区域下的所有终端的ID和终端的数目
//
CString CAreaRecordset::GetTerminalIDsAndCountByAreaID(CString AreaID)
{
	CString AreaSQL;
	CString TerminalSQL;
	CString strID;
	CString strTerminalIDs;
	int intID;
	char count=0;
	//CString strCount;

	_variant_t vTerminalID;
	_RecordsetPtr pRs;

	AreaSQL.Format("Select * From Areas Where AreaID='%s'",AreaID);
	if (Open(AreaSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	TerminalSQL.Format("Select * From Terminals Where IDArea=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)TerminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vTerminalID=pRs->GetCollect("TerminalID");
		if (vTerminalID.vt!=VT_NULL)
		{
			//strTerminalIDs+="<";
			strTerminalIDs+=(LPCTSTR)(_bstr_t)vTerminalID;
			//strTerminalIDs+=">";
		}
		count++;
		pRs->MoveNext();
	}
	//strCount.Format("%d",count);
	strTerminalIDs=count+strTerminalIDs;
	return strTerminalIDs;
	pRs->Close();

}

//根据区域的ID(areaID)设置区域的名称(areaName)并将它与指定的用户(userName)建立关系
//
BOOL CAreaRecordset::SetAreaNameAndIDUser(CString areaID,CString areaName,CString userName)
{
	CString userSQL;
	CString areaSQL;
	_RecordsetPtr pUserRs;
	_RecordsetPtr pAreaRs;

	_variant_t vID;
	int idUser;

	userSQL.Format("Select * From Users Where Name= '%s'",userName);
	areaSQL.Format("Select * From Areas Where AreaID='%s'",areaID);

	try
	{
		pUserRs.CreateInstance("ADODB.RecordSet");
		pUserRs->Open((_variant_t)userSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}

	//获得Users表中Name为userName的ID字段的值
	try
	{
		if (!pUserRs->adoEOF)
		{
			vID=pUserRs->GetCollect("ID");
			if (vID.vt!=NULL)
			{
				idUser=vID.intVal;
			}
		}
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
		return FALSE;
	}
	pUserRs->Close();
	pUserRs=NULL;

	if (Open(areaSQL))
	{
		SetAsString("AreaName",areaName);
		SetAsInt("IDUser",idUser);
		return TRUE;
	}
	return TRUE;
}
/************************************************************************************
功能:获得所有的区域的ID和名称及其数量
返回的格式:0x00-0xFF<名称1>{01}<名称2>{01}.......<名称n>{0n}#
*************************************************************************************/
CString CAreaRecordset::GetAllAreaAndCount()
{
	CString areaSQL;
	CString allAreas;
	_variant_t vAreaName;
	_variant_t vAreaID;
	char count=0x00;
	_RecordsetPtr pAreaRs;

	areaSQL.Format("Select * From Areas");

	//打开Areas表
	try
	{
		pAreaRs.CreateInstance("ADODB.Recordset");
		pAreaRs->Open((_variant_t)areaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	while (!pAreaRs->adoEOF)
	{
		vAreaName=pAreaRs->GetCollect("AreaName");   //获得AreaName字段的值
		if (vAreaName.vt!=NULL)
		{
			allAreas+="<";
			allAreas+=(LPCTSTR)(_bstr_t)vAreaName;
			allAreas+=">";
		}
		vAreaID=pAreaRs->GetCollect("AreaID");     //获得AreaID字段的值
		if (vAreaID.vt!=NULL)
		{
			allAreas+="{";
			allAreas+=(LPCTSTR)(_bstr_t)vAreaID;
			allAreas+="}";
		}
		count++;
		pAreaRs->MoveNext();
	}
	pAreaRs->Close();

	allAreas=count+allAreas;
	allAreas+="#";
	return allAreas;
}