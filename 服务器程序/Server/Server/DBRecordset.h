#pragma once
#include "DBConnection.h"

class CDBRecordset
{
protected:
	BOOL Open(CString SQL);   //�����ݼ�

public:
	void Update();  	//�����û��޸�
	void SetAsString(CString FieldName,CString Value);     //����FieldNameָ���ֶε�ֵ
	void Close();	//�ر����ݼ�
	virtual BOOL LoadData();     //�������ݼ��е�����
	CString GetAsString(CString FiledName);     //���FieldNameָ���ֶε�ֵ
	BOOL IsBOF();     //��ǰ�α��Ƿ��ڵ�һ����¼֮ǰ	
	BOOL IsEOF();    //��ǰ�α��Ƿ��ڵ�һ����¼֮��
	void MovePrevious();   //�ƶ���ǰһ����¼  
	void MoveNext();    //�ƶ�����һ����¼
	void MoveLast();     //�ƶ������һ����¼
	void MoveFirst();    //�ƶ�����һ����¼
	//���ݿ����Ӷ���
	CDBConnection *m_cnn;

private:
	//�Ƿ��ڴ�״̬
	BOOL m_Actived;
	_RecordsetPtr m_pRs;

public:
	CDBRecordset(void);
	~CDBRecordset(void);
};