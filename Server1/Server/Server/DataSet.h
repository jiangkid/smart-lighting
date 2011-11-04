
//
//数据库访问模块
//

#include "DBConnection.h"
#pragma once

class CDataSet
{
public:
	CDataSet(void);
	~CDataSet(void);

	void Update();                                                                 //更新用户修改
	void SetAsString(CString FieldName,CString Value);                                           //设置FieldName指定的字段的值
	void Close();                                                    //关闭数据集
	virtual BOOL LoadData();                                 //加载数据集中数据
	CString GetAsString(CString FieldName);         //获得FieldName指定字段的值
	BOOL IsBOF();                                                 //当前游标是否处于第一条记录之前           
	BOOL IsEOF();                                                 //当前游标是否处于处于最后一条记录之后
	void MovePrevious();                                      //移到前一条记录
	void MovNext();                                              //移到下一条记录
	void MoveLast();                                             //移到最后一条记录
	void MovFirst();                                              //移动到第一条记录

	CDBConnection *m_cnn;       //数据库连接对象

protected:
	BOOL Open(CString SQL);    //打开数据集

private:
	BOOL m_Actived;      //是否处于打开状态
	_RecordsetPtr m_pRs;
};