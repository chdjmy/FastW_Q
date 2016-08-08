// FastW-QDlg.h : header file
//

#pragma once
#include "afxcmn.h"

const int POINT_NUM =10000;      //设备数量
const int DATA_NUM = 10000;      //首次提交时，每个设备的数据量
const int INSERT_NUM = 2000;     //重启提交时，每个设备的数据量
const float NO_VALUE = -1.00001f;//表示某设备在某时间点没有值，设置小数点后5位，避免和数据冲突

//批量提交 数据格式说明的数据结构，用于创建索引
struct tagIndex 
{
	int iPoint;        //设备号
	long long tStart;  //开始时间
	long long tOffset; //该设备的时间间隔
};

//断面提交 数据格式说明的数据结构
struct shotIndex
{
	int ptStart;      //开始设备号
	int ptEnd;        //结束设备号
	long long tStart; //开始时间
	long long tOffset;//时间间隔
};

//断面查询时缓存数据的结构体
struct readData
{
	float* pbufData;   //缓存首次提交的数据
	float* pbufInsert; //缓存批量提交的数据
};

//记录查询结果的数据结构
struct Data
{
	int iPoint;	     //设备号    
	long long lTime; //时间
	float fValue;    //数值

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

	int writeTagdata();    //批量提交
	int writeShotdata();   //断面提交
	int writeInsertdata(); //重启提交

	int queryByPoint();    //批量查询
	int queryByTime();     //断面查询

	unsigned int hashData(int iPoint);    //设备号到数据文件中文件指针的映射
	unsigned int hashInsert(int iPoint);  //设备号到重启提交文件中文件指针的映射
	int readIndex(void* pIndex);          //读索引的函数

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
