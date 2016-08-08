// FastW-QDlg.h : header file
//

#pragma once
#include "afxcmn.h"

const int POINT_NUM =10000;      //�豸����
const int DATA_NUM = 10000;      //�״��ύʱ��ÿ���豸��������
const int INSERT_NUM = 2000;     //�����ύʱ��ÿ���豸��������
const float NO_VALUE = -1.00001f;//��ʾĳ�豸��ĳʱ���û��ֵ������С�����5λ����������ݳ�ͻ

//�����ύ ���ݸ�ʽ˵�������ݽṹ�����ڴ�������
struct tagIndex 
{
	int iPoint;        //�豸��
	long long tStart;  //��ʼʱ��
	long long tOffset; //���豸��ʱ����
};

//�����ύ ���ݸ�ʽ˵�������ݽṹ
struct shotIndex
{
	int ptStart;      //��ʼ�豸��
	int ptEnd;        //�����豸��
	long long tStart; //��ʼʱ��
	long long tOffset;//ʱ����
};

//�����ѯʱ�������ݵĽṹ��
struct readData
{
	float* pbufData;   //�����״��ύ������
	float* pbufInsert; //���������ύ������
};

//��¼��ѯ��������ݽṹ
struct Data
{
	int iPoint;	     //�豸��    
	long long lTime; //ʱ��
	float fValue;    //��ֵ

};

// CFastWQDlg dialog
class CFastWQDlg : public CDialog
{
// Construction
public:
	CFastWQDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FASTWQ_DIALOG };

	CBitmap m_bgBitmap;

	CString m_strDataPath;
	bool m_bDataPath;

	int m_type;
	int  g_bInserted;   

	int writeTagdata();    //�����ύ
	int writeShotdata();   //�����ύ
	int writeInsertdata(); //�����ύ

	int queryByPoint();    //������ѯ
	int queryByTime();     //�����ѯ

	unsigned int hashData(int iPoint);    //�豸�ŵ������ļ����ļ�ָ���ӳ��
	unsigned int hashInsert(int iPoint);  //�豸�ŵ������ύ�ļ����ļ�ָ���ӳ��
	int readIndex(void* pIndex);          //�������ĺ���

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonWriteshot();
	afx_msg void OnBnClickedButtonWriteinsert();
	afx_msg void OnBnClickedButtonQueryshot();
	afx_msg void OnBnClickedButtonTestdata();
	CProgressCtrl m_Process;
	afx_msg void OnBnClickedButtonStart();
	int m_Radio;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
