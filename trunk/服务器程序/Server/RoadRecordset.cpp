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
		SetAsString("Name",RoadName);
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
			strLightIDs+="<";
			strLightIDs+=(LPCTSTR)(_bstr_t)vLightID;
			strLightIDs+=">";
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