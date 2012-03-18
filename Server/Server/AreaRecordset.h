#pragma once
#include "dbrecordset.h"

#define   U8                       unsigned char    
#define   AREAINFOLEN    sizeof(areaInfo)   

typedef struct _areaInfo 
{
	U8 areaID[2];
	U8 areaName[20];
	U8 dutyArea[20];
	U8 telephone[11];
	U8 installTime[17];
	U8 installLocation[20];
}areaInfo,*pAreaInfo;


class CAreaRecordset :
	public CDBRecordset
{
public:
	CAreaRecordset(void);
	~CAreaRecordset(void);
	
	//���������ID������������ƺ������µ�·
	BOOL SetAreaByAreaID(CString AreaID,CString AreaName,CString Terminals);

	//���������ID����ȡ�������µ��ն�ID
	CString GetTerminalIDsByAreaID(CString AreaID);

	//������������������ID
	CString GetAreaIDByAreaName(CString AreaName);

	//���������ID����ȡ�������µ��ն�����
	CString GetTerminalNamesByAreaID(CString AreaID);
	/********�������ܣ�������е�����ID*********/
	CString GetAllAreaIDAndCount();
	//���������ID����ȡ�������µ��ն�ID���ն���Ŀ
	CString GetTerminalIDsAndCountByAreaID(CString AreaID);
	//���������ID���û����������������Ϣ�Լ�������û�����
	BOOL SetAreaInfoByAreaID(CString areaID,CString areaName,CString dutyArea,CString telephone,CString installTime,CString installLocation,CString userName);
	//������е������ID�������ƺ�����Ŀ
	CString GetAllAreaAndCount();
	//���������ID(areaID)�����֮�������û���(userName)
	CString GetUserNamebyAreaID(CString areaID);

	//���������ID������������Ϣ
	char *GetAreaInfoByAreaID(CString areaID);

private:
	char mAreaInfo[90];
};
