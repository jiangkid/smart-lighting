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

/************************************************************************/
/* 功能:得到一条路下所有的灯的名字和ID
返回格式:灯的个数的高位(char) 灯的个数的低位(char){灯1的名字}(灯的ID)..........{灯n的名字}(灯n的ID)#
例如:05 00{一区第1终端一号路1号灯}(0101010000000001){一区第1终端一号路2号灯}(0101010000000002){一区第1终端一号路3号灯}(0101010000000003){一区第1终端一号路4号灯}(0101010000000004){一区第1终端一号路5号灯}(0101010000000005)#*/

/************************************************************************/
char *CRoadRecordset::GetAllLightsOfRoad(CString roadID)
{
	lightInfo *pLightInfo= (lightInfo *)malloc(LIGHTINFOLEN);
	CString roadSQL;
	CString lightSQL;
	CString terminalSQL;
	CString areaSQL;
	CString strIDRoad;
	int intIDRoad;
	CString strIDTerminal;
	int intIDTerminal;
	_variant_t vIDArea;
	int intIDArea;

	int count = 0;  
	char countLow = 0x00;
	char countHigh = 0x00;
	_RecordsetPtr pLightRs;
	_RecordsetPtr pTerminalRs;
	_RecordsetPtr pAreaRs;
	_variant_t vLightName;
	CString strLightName;
	_variant_t vLightID;
	CString strLightID;
	_variant_t vMainLightStatus;
	bool bMainLightStatus;
	_variant_t vAssitLightStatus;
	bool bAssitLightStatus;
	_variant_t vUpdateStatus;
	/*	bool bUpdateStatus;*/
	_variant_t vUpdateCurrent;
	/*	bool bUpdateCurrent;*/
	// 	_variant_t vUpdateTime;
	// 	CString strUpdateTime;
	_variant_t vCurrentValue;
	CString strRoadName;
	_variant_t vTerminalName;
	CString strTerminalName;
	_variant_t vAreaName;
	CString strAreaName;
	_variant_t vAreaID;
	CString strAreaID;

	ZeroMemory(pLightInfo,LIGHTINFOLEN);
	//打开这条路的记录
	roadSQL.Format("Select * From Roads Where RoadID = '%s'",roadID);
	if (Open(roadSQL))
	{
		strRoadName = GetAsString("RoadName");
		strIDRoad = GetAsString("ID");
		intIDRoad = atoi(strIDRoad);              //获得与灯建立一对多关系的ID值

		strIDTerminal = GetAsString("IDTerminal");    //与终端建立一对多关系的ID值
		intIDTerminal = atoi(strIDTerminal);
	}

	terminalSQL.Format("Select * From Terminals Where [ID] = %d",intIDTerminal);
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
		vTerminalName = pTerminalRs->GetCollect("TerminalName");
		vIDArea = pTerminalRs->GetCollect("IDArea");
		if ((vTerminalName.vt != VT_NULL)&&(vTerminalName.vt != VT_EMPTY))
		{			
			strTerminalName=(LPCTSTR)(_bstr_t)vTerminalName;
		}
		if ((vIDArea.vt != VT_NULL)&&(vIDArea.vt != VT_EMPTY))
		{			
			intIDArea = vIDArea.intVal;
		}
	}
	// 	if (Open(terminalSQL))
	// 	{
	// 		strTerminalName = GetAsString("TerminalName");
	// 		strIDArea = GetAsString("IDArea");
	// 		intIDArea = atoi(strIDArea);
	// 	}

	areaSQL.Format("Select * From Areas Where ID = %d",intIDArea);
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
		vAreaName = pAreaRs->GetCollect("AreaName");
		vAreaID = pAreaRs->GetCollect("AreaID");
		if ((vAreaName.vt != VT_NULL)&&(vAreaName.vt != VT_EMPTY))
		{			
			strAreaName=(LPCTSTR)(_bstr_t)vAreaName;
		}
		if ((vAreaID.vt != VT_NULL)&&(vAreaID.vt != VT_EMPTY))
		{			
			strAreaID=(LPCTSTR)(_bstr_t)vAreaID;
		}
	}


	// 	if (Open(areaSQL))
	// 	{
	// 		strAreaName = GetAsString("AreaName");
	// 		strAreaID = GetAsString("AreaID");
	// 	}

	//打开Terminals表中TerminalID为指定值(terminalID)的记录
	lightSQL.Format("Select * From Lights Where IDRoad =%d",intIDRoad);
	try
	{
		pLightRs.CreateInstance("ADODB.RecordSet");
		pLightRs->Open((_variant_t)lightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}

	while (!pLightRs->adoEOF)
	{
		memcpy(pLightInfo->areaID,(U8 *)strAreaID.GetBuffer(),strAreaID.GetLength());
		memcpy(pLightInfo->areaName,(U8 *)strAreaName.GetBuffer(),strAreaName.GetLength());
		memcpy(pLightInfo->terminalName,(U8 *)strTerminalName.GetBuffer(),strTerminalName.GetLength());
		memcpy(pLightInfo->roadName,(U8 *)strRoadName.GetBuffer(),strRoadName.GetLength());

		vLightName=pLightRs->GetCollect("LightName");   //LightName字段的值
		if ((vLightName.vt != VT_NULL)&&(vLightName.vt != VT_EMPTY))
		{			
			strLightName=(LPCTSTR)(_bstr_t)vLightName;
			/*pLightInfo->lightName = (U8 *)strLightName.GetBuffer();*/
			memcpy(pLightInfo->lightName,(U8 *)strLightName.GetBuffer(),strLightName.GetLength());
		}

		vLightID=pLightRs->GetCollect("LightID");     //LightID字段的值
		if ((vLightID.vt != VT_NULL)&&(vLightID.vt != VT_EMPTY))
		{
			strLightID=(LPCTSTR)(_bstr_t)vLightID;
			memcpy(pLightInfo->lightID,(U8 *)strLightID.GetBuffer(),strLightID.GetLength());
		}

		vMainLightStatus = pLightRs->GetCollect("MainLightStatus");  //MainLightStatus字段的值
		pLightRs ->PutCollect("MainLightStatus","0");
		if ((vMainLightStatus.vt != VT_NULL) && (vMainLightStatus.vt != VT_EMPTY))
		{
			if (vMainLightStatus.boolVal ==VARIANT_TRUE )
			{
				bMainLightStatus = true;
			}
			else
			{
				bMainLightStatus = false;
			}
			/*			bMainLightStatus = vMainLightStatus.boolVal;*/
			pLightInfo->mainStatus = bMainLightStatus;
		}

		vAssitLightStatus = pLightRs->GetCollect("AssitLightStatus");  //AssitLightStatus字段的值
		pLightRs ->PutCollect("AssitLightStatus","0");
		if ((vAssitLightStatus.vt != VT_NULL) && (vAssitLightStatus.vt != VT_EMPTY))
		{
			if (vAssitLightStatus.boolVal == VARIANT_TRUE)
			{
				bAssitLightStatus = true;
			}
			else
			{
				bAssitLightStatus = false;
			}
			/*bAssitLightStatus = vAssitLightStatus.boolVal;*/
			pLightInfo->assitStatus = bAssitLightStatus;
		}

		vUpdateStatus = pLightRs->GetCollect("UpdateStatus");       //主辅灯的状态更新位
		pLightRs ->PutCollect("UpdateStatus","0");
		if ((vUpdateStatus.vt != VT_NULL)&&(vUpdateStatus.vt != VT_EMPTY))
		{
			if (vUpdateStatus.boolVal == VARIANT_TRUE)
			{
				pLightInfo->update = pLightInfo->update |= 0x80;
			}
		}

		vUpdateCurrent = pLightRs->GetCollect("UpdateCurrent");        //电流状态更新位
		pLightRs ->PutCollect("UpdateCurrent","0");
		if ((vUpdateCurrent.vt != VT_NULL)&&(vUpdateCurrent.vt != VT_EMPTY))
		{
			if (vUpdateCurrent.boolVal == VARIANT_TRUE)
			{
				pLightInfo->update = pLightInfo->update |= 0x40;
			}
		}

		vCurrentValue = pLightRs->GetCollect("CurrentValue");
		if ((vCurrentValue.vt != VT_NULL)&&(vCurrentValue.vt != VT_EMPTY))
		{
			pLightInfo->current = vCurrentValue.fltVal;
		}

		// 		vUpdate = pLightRs->GetCollect("Update");  //Update字段的值
		// 		if ((vUpdate.vt != VT_NULL) && (vUpdate.vt != VT_EMPTY))
		// 		{
		// 			if (vUpdate.boolVal == VARIANT_TRUE)
		// 			{
		// 				bUpdate = true;
		// 			}
		// 			else
		// 			{
		// 				bUpdate = false;
		// 			}
		// 		/*	bUpdate = vUpdate.boolVal;*/
		// 			pLightInfo->update = bUpdate;
		// 		}
		// 
		// 		vUpdateTime=pLightRs->GetCollect("UpdateTime");   //UpdateTime字段的值
		// 		if ((vUpdateTime.vt != VT_NULL)&&(vUpdateTime.vt != VT_EMPTY))
		// 		{			
		// 			strUpdateTime = (LPCTSTR)(_bstr_t)vUpdateTime;
		// 			memcpy(pLightInfo->updateTime,(U8 *)strUpdateTime.GetBuffer(),strUpdateTime.GetLength());
		// 		}

		memcpy(m_allLights+(count * LIGHTINFOLEN)+2,pLightInfo,LIGHTINFOLEN);
		ZeroMemory(pLightInfo,LIGHTINFOLEN);
		count++;
		countLow ++;
		if (countLow == 0xff)
		{
			countLow = 0;
			countHigh ++;
		}	
		pLightRs->MoveNext();
	}
	
	memcpy(m_allLights,&countHigh,1);
	memcpy(m_allLights+1,&countLow,1);
	memcpy(m_allLights+(count * LIGHTINFOLEN)+2,"#",1);
	pLightRs->Close();
	pTerminalRs->Close();
	pAreaRs->Close();
/********清空更新状态位********/
// 	lightSQL.Format("Select * From Lights Where IDRoad =%d",intIDRoad);
// 	try
// 	{
// 		pLightRs.CreateInstance("ADODB.RecordSet");
// 		pLightRs->Open((_variant_t)lightSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
// 	}
// 	catch(_com_error&e)
// 	{
// 		AfxMessageBox(e.Description());
// 	}
// 	while (!pLightRs->adoEOF)
// 	{
// 		
//		pLightRs->MoveNext();
// 	}

	return m_allLights;
}

/************************************************************************/
/* 功能:向下查询电流时将返回的电流值存入数据库
参数说明: roadID               01--08 表示第几个继电器
				roadNum          01--03 表示那一小路
				currentValue     电流值*/
/************************************************************************/
BOOL CRoadRecordset::UpdateRoadCurrent(CString roadID,U8 roadNum,U8 currentValue[2])
{
	CString strUpdateStatus;
	CString strUpdateI1;
	CString strUpdateI2;
	CString strUpdateI3;
	CString strUpdateTime;
	CTime timer;
	CString sql;
	int intCurrentValue;
	CString strCurrentValue;
	sql.Format("Select * From Roads Where RoadID = '%s'",roadID);

	intCurrentValue = currentValue[0]*256+currentValue[1];
	strCurrentValue.Format("%d",intCurrentValue);
	//更新标志位
	strUpdateStatus = "-1";
	strUpdateI1 = "-1";
	strUpdateI2 = "-1";
	strUpdateI3 = "-1";

	//更新时间	
	timer = CTime::GetCurrentTime();       //获得系统的当前时间
	strUpdateTime = timer.Format("%Y-%m-%d %H:%M:%S");

	if (Open(sql))
	{
		SetAsString("UpdateStatus",strUpdateStatus);
		SetAsString("UpdateTime",strUpdateTime);

		if (roadNum == 0x01)
		{
			SetAsString("I1",strCurrentValue);
			SetAsString("UpdateI1",strUpdateI1);
		}
		else if (roadNum == 0x02)
		{
			SetAsString("I2",strCurrentValue);
			SetAsString("UpdateI2",strUpdateI2);
		}
		else if (roadNum == 0x03)
		{
			SetAsString("I3",strCurrentValue);
			SetAsString("UpdateI3",strUpdateI3);
		}
		return TRUE;
	}
	return FALSE;
}

/***********************************************************************
功能:更新一个终端下八个继电器的状态
参数说明:  terminalID 终端ID   status  八个继电器的状态
***********************************************************************/
BOOL CRoadRecordset::UpdateRoadsStatus(CString terminalID,U8 status)
{
	CString roadSQL1,roadSQL2,roadSQL3,roadSQL4,roadSQL5,roadSQL6,roadSQL7,roadSQL8;
	CString roadID1,roadID2,roadID3,roadID4,roadID5,roadID6,roadID7,roadID8;
	int intStatus[8];
	_RecordsetPtr pRoadRs;
	roadID1 =  terminalID+"01";
	roadID2 = terminalID +"02";
	roadID3 = terminalID +"03";
	roadID4 =terminalID + "04";
	roadID5 = terminalID + "05";
	roadID6 = terminalID + "06";
	roadID7 = terminalID + "07";
	roadID8 =terminalID + "08";

	roadSQL1.Format("Select * From Roads Where RoadID = '%s'",roadID1);
	roadSQL2.Format("Select * From Roads Where RoadID = '%s'",roadID2);
	roadSQL3.Format("Select * From Roads Where RoadID = '%s'",roadID3);
	roadSQL4.Format("Select * From Roads Where RoadID = '%s'",roadID4);
	roadSQL5.Format("Select * From Roads Where RoadID = '%s'",roadID5);
	roadSQL6.Format("Select * From Roads Where RoadID = '%s'",roadID6);
	roadSQL7.Format("Select * From Roads Where RoadID = '%s'",roadID7);
	roadSQL8.Format("Select * From Roads Where RoadID = '%s'",roadID8);
	for (int i = 0; i<8; i++)
	{
		if (((status>>i)&0x01)==1)
		{
			intStatus[i] =0;
		}
		else
		{
			intStatus[i] = -1;
		}
	}

	if (Open(roadSQL1))
	{
		SetAsInt("Status",intStatus[0]);
		/*return TRUE;*/
	}
	Close();

	if (Open(roadSQL2))
	{
		SetAsInt("Status",intStatus[1]);
		/*return TRUE;*/
	}
	Close();

	if (Open(roadSQL3))
	{
		SetAsInt("Status",intStatus[2]);
		/*return TRUE;*/
	}
	Close();

	if (Open(roadSQL4))
	{
		SetAsInt("Status",intStatus[3]);
		/*return TRUE;*/
	}
	Close();

	if (Open(roadSQL5))
	{
		SetAsInt("Status",intStatus[4]);
		/*return TRUE;*/
	}
	Close();

	if (Open(roadSQL6))
	{
		SetAsInt("Status",intStatus[5]);
		/*return TRUE;*/
	}
	Close();

	if (Open(roadSQL7))
	{
		SetAsInt("Status",intStatus[6]);
		/*return TRUE;*/
	}
	Close();

	if (Open(roadSQL8))
	{
		SetAsInt("Status",intStatus[7]);
		return TRUE;
	}
	return FALSE;
}