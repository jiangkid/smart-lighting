#include "StdAfx.h"
#include "RoadRecordset.h"

CRoadRecordset::CRoadRecordset(void)
{
}

CRoadRecordset::~CRoadRecordset(void)
{
}

//加载路这张表中的所有数据
BOOL CRoadRecordset::LoadRoadData()
{
	return Open("Select * From Roads");
}

//设置路的名称
BOOL CRoadRecordset::SetRoadName(CString RoadID,CString RoadName)
{
	CString SQL;
	SQL.Format("Select * From  Roads Where [RoadID] = '%s'",RoadID);
	if (Open(SQL))
	{
		SetAsString("RoadName",RoadName);
		return TRUE;
	}
	return FALSE;
}


//根据ID来查看路的名称

CString CRoadRecordset::GetRoadName(CString RoadID)
{
	CString SQL;
	CString RoadName;
	SQL.Format("Select * From  Roads Where [RoadID] = '%s'",RoadID);
	if (Open(SQL))
	{
		RoadName=GetAsString("Name");
		return RoadName;
	}
	return NULL;
}

//根据路名得到该条路下所有灯的名称
//
CString CRoadRecordset::GetLightNamesByRoadName(CString RoadName)
{
	CString RoadSQL;
	CString LightSQL;
	CString strID;
	CString strLights;
	int intID;
	_variant_t vLightName;
	_RecordsetPtr pRs;

	RoadSQL.Format("Select * From Roads Where [RoadName]='%s'",RoadName);
	if (Open(RoadSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	LightSQL.Format("Select * From Lights Where IDRoad=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)LightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vLightName=pRs->GetCollect("LightName");
		if (vLightName.vt!=VT_NULL)
		{
			strLights+="+";
			strLights+=(LPCTSTR)(_bstr_t)vLightName;
		}
		pRs->MoveNext();
	}
	return strLights;
	pRs->Close();
}

//根据路名得到该条路下所有灯的ID
//
CString CRoadRecordset::GetLightIDsByRoadName(CString RoadName)
{
	CString RoadSQL;
	CString LightSQL;
	CString strID;
	CString strLights;
	int intID;
	_variant_t vLightID;
	_RecordsetPtr pRs;

	RoadSQL.Format("Select * From Roads Where [RoadName]='%s'",RoadName);
	if (Open(RoadSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	LightSQL.Format("Select * From Lights Where IDRoad=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)LightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vLightID=pRs->GetCollect("LightID");
		if (vLightID.vt!=VT_NULL)
		{
			strLights+="+";
			strLights+=(LPCTSTR)(_bstr_t)vLightID;
		}
		pRs->MoveNext();
	}
	return strLights;
	pRs->Close();

}

//根据路的ID获得灯的ID及其数目,格式为(count<LightID1><LightID2><LightID3>....<LightIDn>#)

CString CRoadRecordset::GetLightIDsAndCountByRoadID(CString roadID)
{
	CString roadSQL;
	CString lightSQL;
	CString strID;
	CString strLightIDs;
	char count=0x00;
	//CString strCount;
	int intID;
	_variant_t vLightID;
	_RecordsetPtr pRs;

	roadSQL.Format("Select * From Roads Where [RoadID]='%s'",roadID);
	if (Open(roadSQL))
	{
		strID=GetAsString("ID");
		intID=atoi(strID);
	}

	lightSQL.Format("Select * From Lights Where IDRoad=%d",intID);
	try
	{
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)lightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		vLightID=pRs->GetCollect("LightID");
		if (vLightID.vt!=VT_NULL)
		{
			//strLightIDs+="<";
			strLightIDs+=(LPCTSTR)(_bstr_t)vLightID;
			//strLightIDs+=">";
		}
		count++;
		pRs->MoveNext();
	}
	//strCount.Format("%d",count);
	strLightIDs=count+strLightIDs;
	strLightIDs+="#";
	return strLightIDs;
	pRs->Close();
}

/************************************************************************/
/* 功能:返回所有路的名称、ID及其所属终端的ID和路的数目
返回格式:0x00-0xFF<路1的名称>{路1的ID}{所属终端的ID}......<路n的名称>{路n的ID}{所属终端的ID}#*/
/************************************************************************/

CString CRoadRecordset::GetAllRoadsAndCount()
{
	CString roadSQL;
	CString terminalSQL;
	CString allRoads;
	_RecordsetPtr pRoadRs;
	_RecordsetPtr pTerminalRs;
	_variant_t vRoadName;
	_variant_t vRoadID;
	_variant_t vIDTerminal;
	_variant_t vTerminalID;
	CString strAreaID;
	int intIDTerminal=0;
	char count=0x00;    //记录数目

	//打开Roads表
	roadSQL="Select * From Roads";
	try
	{
		pRoadRs.CreateInstance("ADODB.Recordset");
		pRoadRs->Open((_variant_t)roadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	while (!pRoadRs->adoEOF)
	{
		vRoadName=pRoadRs->GetCollect("RoadName");   //RoadName字段的值
		if ((vRoadName.vt != VT_NULL)&&(vRoadName.vt != VT_EMPTY))
		{
			allRoads+="<";
			allRoads+=(LPCTSTR)(_bstr_t)vRoadName;
			allRoads+=">";
		}

		vRoadID=pRoadRs->GetCollect("RoadID");     //RoadID字段的值
		if (vRoadID.vt!=VT_NULL)
		{
			allRoads+="{";
			allRoads+=(LPCTSTR)(_bstr_t)vRoadID;
			allRoads+="}";
		}

		vIDTerminal=pRoadRs->GetCollect("IDTerminal");         //IDArea字段的值
		if((vIDTerminal.vt != VT_NULL)&&(vIDTerminal.vt != VT_EMPTY))
		{
			intIDTerminal=vIDTerminal.intVal;
		} 

		//取得Terminals表中ID为与Road关联的IDTerminal的记录集
		terminalSQL.Format("Select * From Terminals Where [ID] = %d",intIDTerminal);
		try
		{
			pTerminalRs.CreateInstance("ADODB.Recordset");
			pTerminalRs->Open((_variant_t)terminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
		}
		catch(_com_error&e)
		{
			AfxMessageBox(e.Description());
		}

		if(!pTerminalRs->adoEOF)
		{
			vTerminalID=pTerminalRs->GetCollect("TerminalID");   //TerminalID字段的值
			if ((vTerminalID.vt != VT_NULL)&&(vTerminalID.vt != VT_EMPTY))
			{
				allRoads+="(";
				allRoads+=(LPCTSTR)(_bstr_t)vTerminalID;
				allRoads+=")";
			}
		}
		count++;
		pRoadRs->MoveNext();
		pTerminalRs->Close();
	}
	
	pRoadRs->Close();
	allRoads=count+allRoads;
	allRoads+="#";
	return allRoads;
}

/************************************************************************/
/* 功能: 设置指定路的名称并将它和指定的终端建立一对多的关系*/
/*
参数  指定路:roadID  设定名称:roadName  指定区域:terminalID
*/
/************************************************************************/

BOOL CRoadRecordset::SetRoadNameAndIDTerminal(CString roadID, CString roadName, CString terminalID)
{
	CString roadSQL;
	CString terminalSQL;
	_RecordsetPtr pTerminalRs;
	_variant_t vID;
	int intID;

	//打开Terminals表中TerminalID为指定值(terminalID)的记录
	terminalSQL.Format("Select * From Terminals Where TerminalID='%s'",terminalID);
	try
	{
		pTerminalRs.CreateInstance("ADODB.RecordSet");
		pTerminalRs->Open((_variant_t)terminalSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	if (!pTerminalRs->adoEOF)
	{
		vID=pTerminalRs->GetCollect("ID");  //与Terminal建立一对多关系的IDArea值
		intID=vID.intVal;
	}
	else
		return FALSE;

	//打开Roads表中RoadID为指定值(RoadID)的记录
	roadSQL.Format("Select * From Roads Where RoadID='%s'",roadID);
	if(Open(roadSQL))
	{
		SetAsString("RoadName",roadName);  //设置终端的名称
		SetAsInt("IDTerminal",intID);                  //设置与Areas关联的IDArea值,以此绑定
		return TRUE;
	}
	else
		return FALSE;

}