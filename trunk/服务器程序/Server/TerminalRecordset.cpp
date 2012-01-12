#include "StdAfx.h"
#include "TerminalRecordset.h"

CTerminalRecordset::CTerminalRecordset(void)
{
}

CTerminalRecordset::~CTerminalRecordset(void)
{
}

//
//加载Terminal表中所有的数据
//
BOOL CTerminalRecordset::LoadTerminalData()
{
	//使用Open方法加载所有数据
	return Open("Select * From Terminals");
}

//
//设置终端的ID来设置终端的名称,成功时返回TRUE，失败时返回FALSE
//
BOOL CTerminalRecordset::SetTerminalName(CString TerminalID,CString TerminalName)
{
	CString SQL;
	SQL.Format("Select * From  Terminals Where [TerminalID] = '%s'",TerminalID);
	if (Open(SQL))
	{
		SetAsString("TerminalName",TerminalName);
		return TRUE;
	}
	return FALSE;
}

//
//根据ID查看路的名称,返回为路的名称
//
CString CTerminalRecordset::GetTerminalName(CString TerminalID)
{
	CString SQL;
	CString TerminalName;
	SQL.Format("Select * From  Terminals Where [TerminalID] = '%s'",TerminalID);
	if (Open(SQL))
	{
		TerminalName=GetAsString("Name");
		return TerminalName;
	}
	return NULL;
	
}

//
//根据终端的ID得到该终端下所有路的ID
//
CString CTerminalRecordset::GetRoadIDsByTerminalID(CString TerminalID)
{
	CString TerminalSQL;
	CString RoadSQL;
	CString strID;
	CString strRoads;
	int intID;
	_variant_t vRoadID;
	_RecordsetPtr pRs;

	TerminalSQL.Format("Select * From Terminals Where [TerminalID]='%s'",TerminalID);
	if (Open(TerminalSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	RoadSQL.Format("Select * From Roads Where IDTerminal=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)RoadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vRoadID=pRs->GetCollect("RoadID");
		if (vRoadID.vt!=VT_NULL)
		{
			strRoads+="+";
			strRoads+=(LPCTSTR)(_bstr_t)vRoadID;
		}
		pRs->MoveNext();
	}
	return strRoads;
	pRs->Close();
}

//
//根据终端的ID得到该终端下所有路的名称
//
CString CTerminalRecordset::GetRoadNamesByTerminalID(CString TerminalID)
{
	CString TerminalSQL;
	CString RoadSQL;
	CString strID;
	CString strRoads;
	int intID;
	_variant_t vRoadName;
	_RecordsetPtr pRs;

	TerminalSQL.Format("Select * From Terminals Where [TerminalID]='%s'",TerminalID);
	if (Open(TerminalSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	RoadSQL.Format("Select * From Roads Where IDTerminal=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)RoadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vRoadName=pRs->GetCollect("RoadName");
		if (vRoadName.vt!=VT_NULL)
		{
			strRoads+="+";
			strRoads+=(LPCTSTR)(_bstr_t)vRoadName;
		}
		pRs->MoveNext();
	}
	return strRoads;

	pRs->Close();
}

//根据终端的ID得到该终端下所有路的ID和数目
//
CString CTerminalRecordset::GetRoadIDsAndCountByTerminalID(CString TerminalID)
{
	CString TerminalSQL;
	CString RoadSQL;
	CString strID;
	CString strRoads;
	char count=0;
	//CString strCount;
	int intID;
	_variant_t vRoadID;
	_RecordsetPtr pRs;

	TerminalSQL.Format("Select * From Terminals Where [TerminalID]='%s'",TerminalID);
	if (Open(TerminalSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	RoadSQL.Format("Select * From Roads Where IDTerminal=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)RoadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vRoadID=pRs->GetCollect("RoadID");
		if (vRoadID.vt!=VT_NULL)
		{
			//strRoads+="<";
			strRoads+=(LPCTSTR)(_bstr_t)vRoadID;
			//strRoads+=">";
		}
		count++;
		pRs->MoveNext();
	}
	//strCount.Format("%d",count);
	strRoads=count+strRoads;
	strRoads+="#";
	return strRoads;
	pRs->Close();
}

/************************************************************************/
/* 功能:返回所有的终端名称、ID及其所属区域和终端的数目
返回格式:0x00-0xFF<终端名称1>{终端1ID}{所属区域的ID}......<终端名称n>{终端nID}{所属区域的ID}#*/
/************************************************************************/

CString CTerminalRecordset::GetAllTerminalsAndCount()
{
	CString terminalSQL;
	CString areaSQL;
	CString allTerminals;
	_RecordsetPtr pTerminalRs;
	_RecordsetPtr pAreaRS;
	_variant_t vTerminalName;
	_variant_t vTerminalID;
	_variant_t vIDArea;
	_variant_t vAreaID;
	CString strAreaID;
	int intIDArea=0;
	char count=0x00;    //记录数目

	//打开Terminals表
	terminalSQL="Select * From Terminals";
	try
	{
		pTerminalRs.CreateInstance("ADODB.Recordset");
		pTerminalRs->Open((_variant_t)terminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	while (!pTerminalRs->adoEOF)
	{
		vTerminalName=pTerminalRs->GetCollect("TerminalName");   //TerminalName字段的值
		if ((vTerminalName.vt != VT_NULL)&&(vTerminalName.vt != VT_EMPTY))
		{
			allTerminals+="<";
			allTerminals+=(LPCTSTR)(_bstr_t)vTerminalName;
			allTerminals+=">";
		}

		vTerminalID=pTerminalRs->GetCollect("TerminalID");     //TerminalID字段的值
		if ((vTerminalID.vt != VT_NULL)&&(vTerminalID.vt != VT_EMPTY))
		{
			allTerminals+="{";
			allTerminals+=(LPCTSTR)(_bstr_t)vTerminalID;
			allTerminals+="}";
		}

		vIDArea=pTerminalRs->GetCollect("IDArea");         //IDArea字段的值
		if((vIDArea.vt != VT_NULL)&&(vIDArea.vt != VT_EMPTY))
		{
			intIDArea=vIDArea.intVal;
		} 

		//打开区域这张表
		areaSQL.Format("Select * From Areas Where [ID] = %d",intIDArea);
		try
		{
			pAreaRS.CreateInstance("ADODB.Recordset");
			pAreaRS->Open((_variant_t)areaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
		}
		catch(_com_error&e)
		{
			AfxMessageBox(e.Description());
		}

		if(!pAreaRS->adoEOF)
		{
			vAreaID=pAreaRS->GetCollect("AreaID");   //AreaID字段的值
			if ((vAreaID.vt != VT_NULL)&&(vAreaID.vt != VT_EMPTY))
			{
				allTerminals+="(";
				allTerminals+=(LPCTSTR)(_bstr_t)vAreaID;
				allTerminals+=")";
			}
		}
		count++;
		pTerminalRs->MoveNext();
	}
	pTerminalRs->Close();
	allTerminals=count+allTerminals;
	allTerminals+="#";
	return allTerminals;
}
/************************************************************************/
/* 功能: 设置指定终端的名称并将它和指定的区域建立一对多的关系
参数  指定终端:terminalID  设定名称:terminalName  指定区域:areaID*/
/************************************************************************/
BOOL CTerminalRecordset::SetTerminalNameAndIDArea(CString terminalID,CString terminalName,CString areaID)
{
	CString areaSQL;
	CString terminalSQL;
	_RecordsetPtr pAreaRs;
	_variant_t vID;
	int intID;

	//打开Areas表中AreaID为指定值的记录
	areaSQL.Format("Select * From Areas Where AreaID='%s'",areaID);
	try
	{
		pAreaRs.CreateInstance("ADODB.RecordSet");
		pAreaRs->Open((_variant_t)areaSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}


	if (!pAreaRs->adoEOF)
	{
		vID=pAreaRs->GetCollect("ID");  //与Terminal建立一对多关系的IDArea值
		intID=vID.intVal;
	}
	terminalSQL.Format("Select * From Terminals Where TerminalID='%s'",terminalID);
	if(Open(terminalSQL))
	{
		SetAsString("TerminalName",terminalName);  //设置终端的名称
		SetAsInt("IDArea",intID);                  //设置与Areas关联的IDArea值,以此绑定
		return TRUE;
	}
	else
		return FALSE;
}