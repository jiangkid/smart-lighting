#pragma once
#include "DBConnection.h"

class CDBRecordset
{
protected:
	BOOL Open(CString SQL);   //打开数据集

public:
	void Update();  	//更新用户修改
	void SetAsString(CString FieldName,CString Value);     //设置FieldName指定字段的值
	void Close();	//关闭数据集
	virtual BOOL LoadData();     //加载数据集中的数据
	CString GetAsString(CString FiledName);     //获得FieldName指定字段的值
	BOOL IsBOF();     //当前游标是否处于第一条记录之前	
	BOOL IsEOF();    //当前游标是否处于第一条记录之后
	void MovePrevious();   //移动到前一条记录  
	void MoveNext();    //移动到后一条记录
	void MoveLast();     //移动到最后一条记录
	void MoveFirst();    //移动到第一条记录
	//数据库连接对象
	CDBConnection *m_cnn;

private:
	//是否处于打开状态
	BOOL m_Actived;
	_RecordsetPtr m_pRs;

public:
	CDBRecordset(void);
	~CDBRecordset(void);
};