// FastW-QDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FastW-Q.h"
#include "FastW-QDlg.h"

#include <ShellAPI.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFastWQDlg dialog




CFastWQDlg::CFastWQDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastWQDlg::IDD, pParent)
	, m_Radio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDataPath = "";
	m_type = 0;
	g_bInserted = 0;	

}

void CFastWQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_WRITE, m_Process);
}

BEGIN_MESSAGE_MAP(CFastWQDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CFastWQDlg::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CFastWQDlg::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_WRITESHOT, &CFastWQDlg::OnBnClickedButtonWriteshot)
	ON_BN_CLICKED(IDC_BUTTON_WRITEINSERT, &CFastWQDlg::OnBnClickedButtonWriteinsert)
	ON_BN_CLICKED(IDC_BUTTON_QUERYSHOT, &CFastWQDlg::OnBnClickedButtonQueryshot)
	ON_BN_CLICKED(IDC_BUTTON_TESTDATA, &CFastWQDlg::OnBnClickedButtonTestdata)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFastWQDlg::OnBnClickedButtonStart)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, &CFastWQDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFastWQDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CFastWQDlg message handlers

BOOL CFastWQDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CString picPathname = L"res\\main_bkg.bmp";
	HBITMAP hbgBitmap = (HBITMAP)::LoadImage(NULL,picPathname,
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_bgBitmap.Attach(hbgBitmap);

	BITMAP bm;
	m_bgBitmap.GetBitmap(&bm);
	MoveWindow(0,0,bm.bmWidth,bm.bmHeight);

	GetDlgItem(IDC_BUTTON_WRITE)->MoveWindow(20,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITESHOT)->MoveWindow(100,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->MoveWindow(180,95,60,60);
	GetDlgItem(IDC_BUTTON_QUERY)->MoveWindow(260,95,60,60);	
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->MoveWindow(340,95,60,60);

	GetDlgItem(IDC_STATIC_CONTENTS)->MoveWindow(bm.bmWidth/2+50,bm.bmHeight/3+50,200,50);
	GetDlgItem(IDC_STATIC_CONTENTS)->SetWindowText(L"请选择测试数据所在的文件夹\n");
	GetDlgItem(IDC_BUTTON_TESTDATA)->MoveWindow(bm.bmWidth/2+50,bm.bmHeight/3+100,200,80);

	GetDlgItem(IDC_PROGRESS_WRITE)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2,bm.bmWidth/2,30);
	GetDlgItem(IDC_BUTTON_START)->MoveWindow(bm.bmWidth/5*4,bm.bmHeight/2-40,70,70);
	GetDlgItem(IDC_STATIC_FILENAME)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2-40,bm.bmWidth/2,30);

	//批量查询
	GetDlgItem(IDC_STATIC_POINT)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2-40,140,30);
	GetDlgItem(IDC_STATIC_STARTTIME)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2-40,140,30);
	GetDlgItem(IDC_STATIC_ENDTIME)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2-40,140,30);
	GetDlgItem(IDC_EDIT_POINT)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2,140,30);
	GetDlgItem(IDC_EDIT_STARTTIME)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2,140,30);
	GetDlgItem(IDC_EDIT_ENDTIME)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2,140,30);

	//断面查询
	GetDlgItem(IDC_STATIC_TQUERY)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2-70,80,30);
	GetDlgItem(IDC_EDIT_TQUERY)->MoveWindow(bm.bmWidth/4+80,bm.bmHeight/2-80,140,30);
	GetDlgItem(IDC_RADIO1)->MoveWindow(bm.bmWidth/4+30,bm.bmHeight/2,140,30);
	GetDlgItem(IDC_STATIC_PTSTART)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2-20,140,20);
	GetDlgItem(IDC_EDIT_PTSTART)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2,140,30);
	GetDlgItem(IDC_STATIC_PTEND)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2-20,140,20);
	GetDlgItem(IDC_EDIT_PTEND)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2,140,30);
	GetDlgItem(IDC_RADIO2)->MoveWindow(bm.bmWidth/4+30,bm.bmHeight/2+80,140,30);
	GetDlgItem(IDC_STATIC_PTNUM)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2+60,140,20);
	GetDlgItem(IDC_EDIT_PTNUM)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2+80,140,30);
	GetDlgItem(IDC_STATIC_POINTS)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2+60,200,20);
	GetDlgItem(IDC_EDIT_POINTS)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2+80,200,30);
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	BOOL bOk =  CreateDirectory(L"Data", NULL); //存放持久化文件
	bOk =  CreateDirectory(L"Result", NULL);    //存放查询结果

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFastWQDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFastWQDlg::OnPaint()
{
	CPaintDC dc(this); 

	BITMAP bm;
	m_bgBitmap.GetBitmap(&bm);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(&m_bgBitmap);
	dc.BitBlt(0,0,bm.bmWidth,bm.bmHeight,&dcMem,0,0,SRCCOPY);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFastWQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFastWQDlg::OnBnClickedButtonWrite()
{
	// TODO: Add your control notification handler code here

	m_type = 1;

	GetDlgItem(IDC_BUTTON_WRITE)->MoveWindow(20,100,80,80);
	GetDlgItem(IDC_BUTTON_WRITESHOT)->MoveWindow(120,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->MoveWindow(200,95,60,60);
	GetDlgItem(IDC_BUTTON_QUERY)->MoveWindow(280,95,60,60);	
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->MoveWindow(360,95,60,60);

	GetDlgItem(IDC_PROGRESS_WRITE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始提交");
	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"测试数据： "+m_strDataPath+L"\\批量提交");

	GetDlgItem(IDC_STATIC_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ENDTIME)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINTS)->ShowWindow(SW_HIDE);

	m_Process.SetPos(0);
	m_Process.SetRange(0,10000); 
	m_Process.SetStep(1);

}


void CFastWQDlg::OnBnClickedButtonWriteshot()
{
	// TODO: Add your control notification handler code here
	m_type = 2;


	GetDlgItem(IDC_BUTTON_WRITE)->MoveWindow(20,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITESHOT)->MoveWindow(100,100,80,80);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->MoveWindow(200,95,60,60);
	GetDlgItem(IDC_BUTTON_QUERY)->MoveWindow(280,95,60,60);	
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->MoveWindow(360,95,60,60);

	GetDlgItem(IDC_PROGRESS_WRITE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始提交");

	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"测试数据： "+m_strDataPath+L"\\断面提交");

	GetDlgItem(IDC_STATIC_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ENDTIME)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINTS)->ShowWindow(SW_HIDE);

	m_Process.SetPos(0);
	m_Process.SetRange(0,10); 
	m_Process.SetStep(1);

	Invalidate();
}

void CFastWQDlg::OnBnClickedButtonWriteinsert()
{
	// TODO: Add your control notification handler code here
	m_type = 3;


	GetDlgItem(IDC_BUTTON_WRITE)->MoveWindow(20,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITESHOT)->MoveWindow(100,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->MoveWindow(180,100,80,80);
	GetDlgItem(IDC_BUTTON_QUERY)->MoveWindow(280,95,60,60);	
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->MoveWindow(360,95,60,60);

	GetDlgItem(IDC_PROGRESS_WRITE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始提交");
	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"测试数据： "+m_strDataPath+L"\\停止后重启提交数据");

	GetDlgItem(IDC_STATIC_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ENDTIME)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINTS)->ShowWindow(SW_HIDE);

	m_Process.SetPos(0);
	m_Process.SetRange(0,10000); 
	m_Process.SetStep(1);

	Invalidate();

	
}

void CFastWQDlg::OnBnClickedButtonQuery()
{
	// TODO: Add your control notification handler code here
	m_type = 4;


	GetDlgItem(IDC_BUTTON_WRITE)->MoveWindow(20,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITESHOT)->MoveWindow(100,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->MoveWindow(180,95,60,60);
	GetDlgItem(IDC_BUTTON_QUERY)->MoveWindow(260,100,80,80);	
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->MoveWindow(360,95,60,60);

	GetDlgItem(IDC_PROGRESS_WRITE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始查询");

	GetDlgItem(IDC_STATIC_POINT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_STARTTIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_POINT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_STARTTIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_ENDTIME)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TQUERY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTSTART)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PTNUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINTS)->ShowWindow(SW_HIDE);

}

void CFastWQDlg::OnBnClickedButtonQueryshot()
{
	// TODO: Add your control notification handler code here
	m_type =5;


	GetDlgItem(IDC_BUTTON_WRITE)->MoveWindow(20,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITESHOT)->MoveWindow(100,95,60,60);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->MoveWindow(180,95,60,60);
	GetDlgItem(IDC_BUTTON_QUERY)->MoveWindow(260,95,60,60);	
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->MoveWindow(340,100,80,80);

	GetDlgItem(IDC_PROGRESS_WRITE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"开始查询");

	GetDlgItem(IDC_STATIC_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ENDTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_POINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_STARTTIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ENDTIME)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_TQUERY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_TQUERY)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PTSTART)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_PTSTART)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PTEND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_PTEND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PTNUM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_PTNUM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_POINTS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_POINTS)->ShowWindow(SW_SHOW);



}

void CFastWQDlg::OnBnClickedButtonTestdata()
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;

	TCHAR Buffer[MAX_PATH];

	//初始化入口参数bi开始

	bi.hwndOwner = NULL;

	bi.pidlRoot =NULL;//初始化制定的root目录很不容易

	bi.pszDisplayName = Buffer;//此参数如为NULL则不能显示对话框

	bi.lpszTitle = L"请选择测试数据所在目录";

	bi.ulFlags = BIF_EDITBOX;//带编辑框的风格

	bi.lpfn = NULL;

	bi.lParam = 0;

	bi.iImage=IDR_MAINFRAME;

	//初始化入口参数bi结束

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框

	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//取得文件夹路径到Buffer里		 
		 int iRet = MessageBox(Buffer,L"选择目录",MB_OKCANCEL|MB_ICONQUESTION);
		 if (iRet==IDOK)
		 {
			 m_strDataPath = Buffer;//将路径保存在一个CString对象里

			 GetDlgItem(IDC_STATIC_CONTENTS)->ShowWindow(SW_HIDE);

			 GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(TRUE);
			 GetDlgItem(IDC_BUTTON_WRITESHOT)->EnableWindow(TRUE);
 			 GetDlgItem(IDC_BUTTON_WRITEINSERT)->EnableWindow(FALSE);
    		 GetDlgItem(IDC_BUTTON_QUERY)->EnableWindow(FALSE);
    		 GetDlgItem(IDC_BUTTON_QUERYSHOT)->EnableWindow(FALSE);

			 g_bInserted = 0; 

			 CRect rc;
			 GetClientRect(rc);
			 GetDlgItem(IDC_BUTTON_TESTDATA)->MoveWindow(rc.right-170,rc.bottom-80,80,40);
			 GetDlgItem(IDC_BUTTON_TESTDATA)->SetWindowText(L"修改测试数据");

			 MessageBox(L"测试数据导入成功！",L"导入成功",MB_OK);
		 }	 
	}
	UpdateData(FALSE);
}

void CFastWQDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	switch(m_type)
	{
	case 1:
		writeTagdata();
		break;

	case 2:
		writeShotdata();		
		break;

	case 3:
		writeInsertdata();	
		break;

	case 4:
		queryByPoint();
		break;

	case 5:
		queryByTime();
		break;

	default:
		break;
	}
}

int CFastWQDlg::writeTagdata()
{
	/**********************读入批量提交的测试数据*********************/

	char szFileName[MAX_PATH] = {0}; 

	FILE* fp = NULL;

	int fileSize = sizeof(float)*DATA_NUM*POINT_NUM;

	int allocSize = 4*1024*1024 * 96;//4*10000*10000

	void* pBase = VirtualAlloc(NULL,allocSize,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);

	if (pBase==NULL)
	{
//		cout<<"Alloc Failed!"<<endl;
		MessageBox(L"Alloc Failed!",L"Error",MB_OK);

		return -1;
	}

	float* pbuf = (float*)pBase;

	tagIndex* pIndex = new tagIndex[POINT_NUM];
	if (pIndex==NULL)
	{
//		cout<<"Failed to new pIndex";
		MessageBox(L"Failed to new pIndex!",L"Error",MB_OK);
		return -1;
	}


	//字符转换
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, NULL, 0, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char* psDataPath = NULL;	
	psDataPath = new char[dwNum];//根据需要的大小动态申请
	
	WideCharToMultiByte (CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, psDataPath, dwNum, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	CString strFileName;

	for (int k = 0;k<10;k++)
	{
		sprintf_s(szFileName,"%s\\批量提交\\tag_%d-%d.txt",
			psDataPath,k*1000+1,(k+1)*1000+1);

		fopen_s(&fp,szFileName,"r");

		if(fp==NULL)
		{
			MessageBox(L"Cannot openfile!",L"Error",MB_OK);

			return -1;
		}

		strFileName.Format(L"\\批量提交\\tag_%d-%d.txt",k*1000+1,(k+1)*1000+1);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"正在读入： "+m_strDataPath+strFileName);	

		for (int i=0;i<1000;i++)//每个文件1000个设备
		{
			fscanf_s(fp,"%d",&pIndex[i+1000*k].iPoint);
			fscanf_s(fp,"%I64d",&pIndex[i+1000*k].tStart);
			fscanf_s(fp,"%I64d",&pIndex[i+1000*k].tOffset);

			m_Process.StepIt();

			for (int j=0;j<DATA_NUM;j++)//每个设备10000个数据
			{
				fscanf_s(fp,"%f",&pbuf[j+10000*i+10000*1000*k]);
			}
		}
		fclose(fp);
	}
	delete[] psDataPath;

	

	/*************创建用于存放10000*10000批量提交数据的文件************/

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"正在写入数据文件");

	DWORD startClock = GetTickCount();

	TCHAR szFile[] = L"Data\\data.flo" ;

	HANDLE hFile = CreateFile(szFile,GENERIC_WRITE,FILE_SHARE_READ,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING,NULL);

	if (hFile==INVALID_HANDLE_VALUE)
	{
//		printf("Create File Failed!\n");
		MessageBox(L"Create File Failed!",L"Error",MB_OK);

		return -1;
	}

	DWORD dwBytes = 0;	

	WriteFile(hFile,pbuf,fileSize,&dwBytes,NULL);

	VirtualFree(pBase,0,MEM_RELEASE);

	CloseHandle(hFile);

	DWORD endClock = GetTickCount();

	CString strTime;;
	CString strEfficient;
	strTime.Format(L"批量提交：10000*10000 条\n用      时：%d ms\n",endClock-startClock);
//	printf("写入数据 ：10000*10000 条\n用时：%d ms\n",endClock-startClock);
	if (endClock-startClock!=0)
	{
//		printf("写入效率：%d W/s\n",10000*10000/10/(endClock-startClock));
		strEfficient.Format(L"写入效率：%d W/s\n",10000*10000/10/(endClock-startClock));
	}
	

	/**********创建文件来存放各个设备的时间间隔**********************/

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"正在写入索引文件");

	TCHAR szFileIndex[] = L"Data\\index.int";

	HANDLE hFileIndex = CreateFile(szFileIndex,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFileIndex==INVALID_HANDLE_VALUE)
	{
//		printf("Failed to Create index.int");
		MessageBox(L"Failed to Create index.int!",L"Error",MB_OK);

		return -1;
	}

	WriteFile(hFileIndex,pIndex,sizeof(tagIndex)*10000,&dwBytes,NULL);

	CloseHandle(hFileIndex);

	delete[] pIndex;

	CString strResult;
	strResult = strTime+strEfficient;
	strResult += L"\n本次提交生成的持久化文件存放于当前工作目录Data文件夹\n数据文件是data.flo，索引文件是insert.int";

	MessageBox(strResult,L"提交成功",MB_OK|MB_ICONINFORMATION);

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"提交成功!  "+strEfficient);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->EnableWindow(TRUE);

	Invalidate();

	return 0;

}

// 函 数 名：readShotdata
// 函数功能：把断面提交的全部数据按设计的结构存入Data\\data.flo
// 返 回 值：类型int 返回0表示查询成功，返回非0表示失败
// 参    数：无
int CFastWQDlg::writeShotdata()
{
	/**********************打开断面提交的测试数据*********************/

	float* pbuf = new float[DATA_NUM*POINT_NUM];//用来存放10000*10000个数据
	if (pbuf==NULL)
	{
//		printf("new pbuf failed");
		MessageBox(L"new pbuf failed!",L"Error",MB_OK);
		return -1;
		
	} 

	shotIndex* pIndex = new shotIndex[10];//用来存放一个整理后的数据
	if (pIndex==NULL)
	{
//		printf("memory full");
		MessageBox(L"memory full!",L"Error",MB_OK);
		return -1;
	}

	//字符转换
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, NULL, 0, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char* psDataPath = NULL;	
	psDataPath = new char[dwNum];//根据需要的大小动态申请

	WideCharToMultiByte (CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, psDataPath, dwNum, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char szFileName[MAX_PATH] = {0}; 
	FILE* fp;

	CString strFileName;

	for (int k = 0;k<10;k++)
	{
		sprintf_s(szFileName,"%s\\断面提交\\shots_tag_%d-%d.txt",
			psDataPath,k*1000+1,(k+1)*1000+1);

		fopen_s(&fp,szFileName,"r");

		if (fp==NULL)
		{
//			cout<<"Cannot openfile!";
			MessageBox(L"Cannot openfile!",L"Error",MB_OK);

			return -1;
		}

//		cout<<szFileName<<endl;//输出文件名到屏幕
		m_Process.StepIt();
		strFileName.Format(L"\\断面提交\\tag_%d-%d.txt",k*1000+1,(k+1)*1000+1);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"正在读入： "+m_strDataPath+strFileName);

		fscanf_s(fp,"%d",&pIndex[k].ptStart);//开始设备号
		fscanf_s(fp,"%d",&pIndex[k].ptEnd);  //结束设备号
		fscanf_s(fp,"%I64d",&pIndex[k].tStart); //开始时间
		fscanf_s(fp,"%I64d",&pIndex[k].tOffset);//时间间隔

		for (int i=0;i<1000*10000;i++)
		{
			fscanf_s(fp,"%f",&pbuf[i+1000*10000*k]);
		}

		fclose(fp);
	}

	DWORD startClock = GetTickCount();//数据读入完毕后开始计时

	//把按断面排序的数据处理成按批量排序的格式
	//首先把10*1000*10000的数据拆分成100个1000*1000的矩阵,使同一设备的1000个数据相邻
	float fTemp = 0; 
	float* p = NULL;
	for (int k=0;k<100;k++)
	{
		p = pbuf+1000*1000*k;

		for (int i=0;i<1000;i++)  //矩阵转置
		{
			for (int j=i;j<1000;j++)
			{	
				fTemp = p[i+j*1000];
				p[i+j*1000] = p[j+i*1000];
				p[j+i*1000] = fTemp;	
			}
		}

	}

	//再把数据拆分为10个1000*10000数据块，每个数据块整理成10000*1000的格式
	//使同一设备的10000个数据相邻
	float* pTemp = new float[1000*10000];
	for (int k=0;k<10;k++)
	{
		p = pbuf+1000*10000*k;
		memcpy(pTemp,p,sizeof(float)*1000*10000);

		for (int i=0;i<10;i++)
		{
			for (int j=0;j<1000;j++)
			{
				memcpy(p+j*10*1000+i*1000,pTemp+i*1000*1000+j*1000,sizeof(float)*1000);
			}
		}
	}
	delete[] pTemp;

	/*************创建用于存放10000*10000断面提交数据的文件************/

	TCHAR szFileShot[] = L"Data\\data.flo";

	HANDLE hFile = CreateFile(szFileShot,GENERIC_WRITE,FILE_SHARE_READ,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile==INVALID_HANDLE_VALUE)
	{
		//printf("Create File Failed!\n");
		MessageBox(L"Create File Failed!",L"Error",MB_OK);

		return -1;
	}

	DWORD dwBytes = 0;	

	WriteFile(hFile,pbuf,sizeof(float)*DATA_NUM*DATA_NUM,&dwBytes,NULL);

	delete[] pbuf;
	CloseHandle(hFile);

	DWORD endClock = GetTickCount();
	CString strTime;;
	CString strEfficient;
	strTime.Format(L"断面提交：10000*10000 条\n用      时：%d ms\n",endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"写入效率：%d W/s\n",10000*10000/10/(endClock-startClock));
	}


	/**********创建文件来存放各个设备的时间间隔**********************/


	//把断面提交的索引处理成和批量提交一致的索引
	tagIndex* ptagIndex =  new tagIndex[POINT_NUM];
	if (ptagIndex==NULL)
	{
//		cout<<"Failed to new tagIndex";
		MessageBox(L"Failed to new tagIndex!",L"Error",MB_OK);
		return -1;
	}
	for (int i=0;i<10;i++)
	{
		for (int j=0;j<1000;j++)
		{
			ptagIndex[i*1000+j].iPoint = pIndex[i].ptStart+j;
			ptagIndex[i*1000+j].tStart = pIndex[i].tStart;
			ptagIndex[i*1000+j].tOffset = pIndex[i].tOffset;
		}
	}

	TCHAR szFileIndex[] = L"Data\\index.int";
	HANDLE hIndex = CreateFile(szFileIndex,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hIndex==INVALID_HANDLE_VALUE)
	{
		//printf("Failed to Create shotIndex.int");
		MessageBox(L"Failed to Create shotIndex.int!",L"Error",MB_OK);

		return -1;
	}

	WriteFile(hIndex,ptagIndex,sizeof(tagIndex)*DATA_NUM,&dwBytes,NULL);

	CloseHandle(hIndex);

	delete[] pIndex;
	delete[] ptagIndex;

//	cout<<"\n本次提交生成的持久化文件存放目录为Data文件夹"
//		<<"\n数据文件是data.flo，索引文件是insert.int，";
	CString strResult;
	strResult = strTime+strEfficient;
	strResult += L"\n本次提交生成的持久化文件存放于当前工作目录Data文件夹\n数据文件是data.flo，索引文件是insert.int";

	MessageBox(strResult,L"提交成功",MB_OK|MB_ICONINFORMATION);

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"提交成功!  "+strEfficient);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->EnableWindow(TRUE);

	Invalidate();

	return 0;
}

int CFastWQDlg::writeInsertdata()
{
	/**********************打开停止后重启提交的测试数据*********************/

	float* pbuf = new float[INSERT_NUM*POINT_NUM+200];
	if (pbuf==NULL)
	{
//		cout<<"Failed to new pbuf";
		MessageBox(L"Failed to new pbuf!",L"Error",MB_OK);
		return -1;
	}

	//字符转换
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, NULL, 0, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char* psDataPath = NULL;	
	psDataPath = new char[dwNum];//根据需要的大小动态申请

	WideCharToMultiByte (CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, psDataPath, dwNum, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char szFileName[MAX_PATH] = {0}; 
	FILE* fp;
	tagIndex indexTemp = {0};

	int k = 0;
	int i = 0;
	int j = 0;

	CString strFileName;

	for (k = 0;k<10;k++)
	{
		sprintf_s(szFileName,"%s\\停止后重启提交数据\\insert_tag_%d-%d.txt",
			psDataPath,k*1000+1,(k+1)*1000+1);

		fopen_s(&fp,szFileName,"r");
		if(fp==NULL)
		{
//			cout<<"Cannot openfile!";
			MessageBox(L"Cannot openfile!",L"Error",MB_OK);

			return -1;
		}
//		cout<<szFileName<<endl;//输出文件名
		strFileName.Format(L"\\停止后重启提交数据\\tag_%d-%d.txt",k*1000+1,(k+1)*1000+1);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"正在读入： "+m_strDataPath+strFileName);

		for (i=0;i<1000;i++)//每个文件1000个设备
		{
			m_Process.StepIt();

			fscanf_s(fp,"%d",&indexTemp.iPoint);
			fscanf_s(fp,"%I64d",&indexTemp.tStart);
			fscanf_s(fp,"%I64d",&indexTemp.tOffset);

			for (j=0;j<INSERT_NUM;j++)//每个设备2000个数据
			{
				fscanf_s(fp,"%f",&(pbuf[j+2000*i+2000*1000*k]));
			}

		}
		fclose(fp);
	}

	DWORD startClock = GetTickCount();//开始计时

	/*************创建用于存放2000*10000批量提交数据的文件************/

	TCHAR szFileInsert[] = L"Data\\insert.flo" ;

	HANDLE hFileInsert = CreateFile(szFileInsert,GENERIC_WRITE,FILE_SHARE_READ,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFileInsert==INVALID_HANDLE_VALUE)
	{
		//printf("Create File Failed!\n");
		MessageBox(L"Create File Failed!",L"Error",MB_OK);

		return -1;
	}

	DWORD dwBytes = 0;	
	WriteFile(hFileInsert,pbuf,sizeof(float)*INSERT_NUM*POINT_NUM,&dwBytes,NULL);

	delete[] pbuf;
	CloseHandle(hFileInsert);

	g_bInserted = 1;

	DWORD endClock = GetTickCount();

	CString strTime;;
	CString strEfficient;
	strTime.Format(L"停止后重启提交：2000*10000 条\n用      时：%d ms\n",endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"写入效率：%d W/s\n",2000*10000/10/(endClock-startClock));
	}

//	cout<<"\n本次提交生成的数据文件是insert.flo，存放目录为Data文件夹";
	CString strResult;
	strResult = strTime+strEfficient;
	strResult += L"\n本次提交生成的持久化文件存放于当前工作目录Data文件夹\n数据文件是insert.flo";

	MessageBox(strResult,L"提交成功",MB_OK|MB_ICONINFORMATION);

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"提交成功!  "+strEfficient);

	Invalidate();

	return 0;
}

// 函 数 名：queryByPoint
// 函数功能：测点维度的查询，如查询一个Point一段时间内的所有数据
//           查询结果放在文件夹Result中
// 返 回 值：类型int 返回0表示查询成功，返回非0表示失败
// 参    数：无
int CFastWQDlg::queryByPoint()
{
	int iPoint = 0;             //设备号
	long long startTime = 0;    //开始时间
	long long endTime = 0;      //结束时间

	CString strPoint;
	CString strStartTime;
	CString strEndTime;

	/**********************************  数据准备阶段  ****************************/

	//第1步：读入设备索引表
	tagIndex* pIndex = NULL;  
	int iRet = readIndex((void*)&pIndex);
	if (iRet==-1)//读取索引失败
	{
		return -1;
	}

	//第2步：用户输入设备号
//	cout<<"批量查询：\n请输入设备号(1~10000)：";
//	cin>>iPoint;
	GetDlgItem(IDC_EDIT_POINT)->GetWindowText(strPoint);
	iPoint = _ttoi(strPoint);

	while(iPoint<1||iPoint>POINT_NUM)
	{
//		cout<<"设备号不在规定范围内,请重新输入"<<endl;
		MessageBox(L"设备号不在规定范围内,请重新输入!",L"Error",MB_OK);
		return -1;
	}

	//第3步：把该设备所有数据准备到内存
	float* pbuf= NULL;
	pbuf = new float[DATA_NUM+g_bInserted*INSERT_NUM];
	if (pbuf == NULL)
	{
//		cout<<"Failed to new pbuf";
		MessageBox(L"Failed to new pbuf !",L"Error",MB_OK);
		return -1;
	}

	TCHAR szFileData[] = L"Data\\data.flo";//数据文件
	HANDLE hFileData= CreateFile(szFileData,GENERIC_READ,FILE_SHARE_READ,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFileData==INVALID_HANDLE_VALUE)
	{
//		cout<<"Failed to openfile Data\\data.flo";
		MessageBox(L"Failed to openfile Data\\data.flo!",L"Error",MB_OK);
		return -1;
	}

	DWORD dwBytes = 0;
	SetFilePointer(hFileData,hashData(iPoint),NULL,FILE_BEGIN);
	ReadFile(hFileData,pbuf,sizeof(float)*DATA_NUM,&dwBytes,NULL);

	CloseHandle(hFileData);

	if (g_bInserted==1)
	{
		TCHAR szFileInsert[] = L"Data\\insert.flo";//重启提交的数据文件

		HANDLE hFileInsert= CreateFile(szFileInsert,GENERIC_READ,FILE_SHARE_READ,
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFileInsert==INVALID_HANDLE_VALUE)
		{
//			cout<<"Failed to OpenFile insert.flo,请提交重启后的数据";
			MessageBox(L"Failed to OpenFile insert.flo,请提交重启后的数据!",L"Error",MB_OK);
			g_bInserted = 0;
			return -1;
		}

		SetFilePointer(hFileInsert,hashInsert(iPoint),NULL,FILE_BEGIN);
		ReadFile(hFileInsert,pbuf+DATA_NUM,sizeof(float)*INSERT_NUM,&dwBytes,NULL);

		CloseHandle(hFileInsert);
	}
	/**********************************  完成数据准备  ****************************/


//	cout<<"请输入起始时间 ：";
//	cin>>startTime;
//	cout<<"请输入结束时间 ：";
//	cin>>endTime;
	GetDlgItem(IDC_EDIT_STARTTIME)->GetWindowText(strStartTime);
	GetDlgItem(IDC_EDIT_ENDTIME)->GetWindowText(strEndTime);
	startTime = _ttoi(strStartTime);
	endTime = _ttoi(strEndTime);

	while (endTime<startTime)
	{
//		cout<<"结束时间不能小于开始时间,请重新输入结束时间"<<endl;
//		cin>>endTime;
		MessageBox(L"结束时间不能小于开始时间,请重新输入结束时间!",L"Error",MB_OK);
		delete[] pbuf;
		delete[] pIndex;
		return -1;
	}

	DWORD startClock = GetTickCount();//输入结束，开始计时

	//从索引表里获得设备开始时间和时间间隔
	long long tStart = pIndex[iPoint-1].tStart;
	long long tOffset = pIndex[iPoint-1].tOffset;
	delete[] pIndex;


	//根据查询条件定位数据

	//iFirstHit是第一个大于或等于查询起始时间的有效点的编号
	//有效点指有数据的时间点，比如 设备的开始时间是0号的有效点
	long long iFirstHit = (startTime-tStart)/tOffset;
	if (startTime > tStart+iFirstHit*tOffset)
	{
		iFirstHit++;
	}
	//边界处理：如果查询起始时间超过该设备的时间上界，则查询结果为空
	if (iFirstHit >= DATA_NUM+g_bInserted*INSERT_NUM)
	{
//		cout<<"查询起始时间超出时间上界\n";
		delete[] pbuf;
		return -1;
	}
	//如果查询的时间段小于设备的时间间隔，且该时间段内不包含有效点，则查询结果为空	
	if (endTime < tStart+tOffset*iFirstHit)
	{
		printf("设备%d (开始时间:%I64d 时间间隔:%I64d)在 %I64d到%I64d 时间内没有数据\n",
			iPoint,tStart,tOffset,startTime,endTime);
		return -1;
	}

	//计算该时间范围内的有效点的个数
	long long iHitNum = (endTime-iFirstHit*tOffset-tStart)/tOffset+1;

	//如果所查数据的范围超过已有数据量，则查询结果只有数据范围内的值
	if (iFirstHit+iHitNum > DATA_NUM+g_bInserted*INSERT_NUM)
	{
		iHitNum = DATA_NUM+g_bInserted*INSERT_NUM-iFirstHit;
	}

	//申请内存空间存放查询结果
	Data* pResult = new Data[(int)iHitNum];
	if (pResult==NULL)
	{
//		cout<<"Failed to new pResult";
		MessageBox(L"Failed to new pResult!",L"Error",MB_OK);
		return -1;
	}

	//根据数据定位获得查询结果
	for (int i=0;i<iHitNum;i++)
	{
		pResult[i].iPoint = iPoint;
		pResult[i].lTime = iFirstHit*tOffset+tStart+tOffset*i;
		pResult[i].fValue = pbuf[iFirstHit+i];
	}
	delete[] pbuf;

	DWORD endClock = GetTickCount();//查询结束后计算总效率

	CString strTime;;
	CString strEfficient;
	strTime.Format(L"查询结果：%d 条\n用      时：%d ms\n",iHitNum,endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"查询效率：%d W/s\n",iHitNum/(endClock-startClock)/10);
	}
	else
	{
		strEfficient.Format(L"查询效率：无穷大\n");
	}

// 	printf("结果 %d 条\n",iHitNum);
// 	printf("用时 %d 毫秒\n",endClock-startClock);
// 	if (endClock-startClock!=0)
// 	{
// 		printf("效率 %d W/s\n",iHitNum/(endClock-startClock)/10);
// 	}
// 	else
// 	{
// 		printf("效率 无穷大\n");
// 	}

	/******************************  查询结果写入文件  ***************************/

	char szFileResult[MAX_PATH] = {0};
	sprintf_s(szFileResult,"Result\\批量查询_设备%d_时间%I64d-%I64d.txt",iPoint,startTime,endTime);

	FILE* fp = NULL;
	fopen_s(&fp,szFileResult,"w");

	if (fp==NULL)
	{
//		cout<<"Failed to Create File "<<szFileResult;
		MessageBox(L"Failed to Create File !",L"Error",MB_OK);

		return -1;
	}
	for (int i=0;i<iHitNum;i++)
	{
		fprintf(fp,"time:%I64d  value:%0.4f\n",pResult[i].lTime,pResult[i].fValue);
	}
	fclose(fp);

//	cout<<"本次查询结果已写入文件"<<szFileResult<<endl;
	CString strResult;
	strResult.Format(L"\n本次查询结果已写入到文件\nResult\\批量查询_设备%d_时间%I64d-%I64d.txt",iPoint,startTime,endTime);
	int iResult = MessageBox(strTime+strEfficient+strResult+L"\n是否打开查询结果的文件目录？",
		L"批量查询结果",MB_YESNO|MB_ICONINFORMATION);
	if(iResult==IDYES)
	{
		ShellExecute(NULL,L"open",L".\\Result",NULL,NULL,SW_SHOW);
	}


	GetDlgItem(IDC_EDIT_POINT)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_STARTTIME)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_ENDTIME)->SetWindowText(L"");

	

	return 0;

}
// 函 数 名：queryByTime
// 函数功能：断面查询，查询批量设备一个时间点的数据
//           查询结果写在文件夹Result中
// 返 回 值：类型int 返回0表示查询成功，返回非0表示失败
// 参    数：无
int CFastWQDlg::queryByTime()
{
	/********************************  数据准备阶段 *********************************/

	//第1步.读入索引信息
	tagIndex* pIndex = NULL;  
	int iRet = readIndex((void*)&pIndex);
	if (iRet==-1)//读取索引失败
	{
		MessageBox(L"Failed to read Index !",L"Error",MB_OK);
		return -1;
	}

	//第2步.读入首次提交的数据
	readData* pData = new readData;
	pData->pbufData = new float[DATA_NUM*POINT_NUM];
	if (pData->pbufData==NULL)
	{
//		cout <<"Failed to new pData->pbufData";
		MessageBox(L"Failed to new pData->pbufData !",L"Error",MB_OK);
		return -1;
	}

	TCHAR szFileData[] = L"Data\\data.flo";//首次提交的10000*10000个数据
	HANDLE hFileData= CreateFile(szFileData,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFileData == INVALID_HANDLE_VALUE)
	{
//		cout<<"Failed to OpenFile data.flo";
		MessageBox(L"Failed to OpenFile data.flo !",L"Error",MB_OK);
		return -1;
	}

	DWORD dwBytes = 0;
	ReadFile(hFileData,pData->pbufData,sizeof(float)*DATA_NUM*POINT_NUM,&dwBytes,NULL);
	CloseHandle(hFileData);

	//第3步.如果有重启提交，则读重启提交数据
	if (g_bInserted==1)
	{
		pData->pbufInsert = new float[INSERT_NUM*POINT_NUM];

		TCHAR szFileInsert[] = L"Data\\insert.flo";//首次提交的10000*10000个数据
		HANDLE hFileInsert= CreateFile(szFileInsert,GENERIC_READ,FILE_SHARE_READ,NULL,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

		if (hFileInsert == INVALID_HANDLE_VALUE)
		{
//			cout<<"Failed to OpenFile insert.flo";
			MessageBox(L"Failed to OpenFile insert.flo !",L"Error",MB_OK);
			return -1;
		}
		ReadFile(hFileInsert,pData->pbufInsert,sizeof(float)*INSERT_NUM*POINT_NUM,&dwBytes,NULL);
		CloseHandle(hFileInsert);
	}

	/********************************  完成数据准备 *********************************/


	DWORD startClock = 0;
	DWORD endClock = 0;

	//输入设备号的方式
	int iType = m_Radio;
//	cout<<"断面查询：\n请选择批量设备输入方式：1-连续设备号 2-离散设备号：";
//	cin>>iType;

	long long tQuery = 0;    //查询的时间点
	int ptNum = 0;           //查询的设备的数量
//	list<Data> liResult;     //用来存放查询结果的链表
	Data* pResult = NULL;

	CString strTQuery;
	GetDlgItem(IDC_EDIT_TQUERY)->GetWindowText(strTQuery);

	Data dataTemp = {0};     //暂存一个查询结果

	if (iType==0)//连续设备号
	{
		int ptStart = 0;  //开始设备号
		int ptEnd = 0;    //结束设备号

		CString strPtStart;
		CString strPtEnd;

		GetDlgItem(IDC_EDIT_PTSTART)->GetWindowText(strPtStart);
		GetDlgItem(IDC_EDIT_PTEND)->GetWindowText(strPtEnd);

//		cout<<"输入开始设备号：";
//		cin>>ptStart;
		ptStart = _ttoi(strPtStart);

		while(ptStart>10000||ptStart<0)
		{
//			cout<<"起始设备号超界，请重新输入：";
			MessageBox(L"起始设备号超界，请重新输入!",L"Error",MB_OK);
			return -1;
//			cin>>ptStart;
		}

//		cout<<"输出结束设备号：";
//		cin>>ptEnd;
		ptEnd = _ttoi(strPtEnd);
		while(ptEnd>10000||ptEnd<0)
		{
			MessageBox(L"起始设备号超界，请重新输入!",L"Error",MB_OK);
			return -1;
//			cout<<"结束设备号超界，请重新输入：";
//			cin>>ptEnd;
		}

		ptNum = ptEnd-ptStart+1;     //计算设备数量	
		pResult = new Data[ptNum];
		if (pResult==NULL)
		{
			MessageBox(L"Memory Full",L"Error",MB_OK);
			return -1;
		}

//		cout<<"输入查询的时间点：";
//		cin>>tQuery;
		tQuery = _ttoi(strTQuery);

		startClock = GetTickCount();//数据准备完成后开始计时

		dataTemp.lTime = tQuery;
		long long iCount = 0;            //tQuery时刻是设备的第iCount个有效点（从0开始记）
		long long tStart = 0;   
		long long tOffset = 0; 

		for (int i=ptStart;i<ptStart+ptNum;i++)
		{
			dataTemp.iPoint = i;
			tStart = pIndex[i-1].tStart;   //从索引表获得开始时间和时间间隔
			tOffset = pIndex[i-1].tOffset;

			if ((tQuery-tStart)%tOffset==0)//tQuery是设备i的有效点
			{
				iCount = (tQuery-tStart)/tOffset;

				if (iCount<DATA_NUM)//如果查询点在前10000个数据里，则在pbufData里查找数据
				{					
					dataTemp.fValue = pData->pbufData[(i-1)*DATA_NUM+iCount];
				}
				else if (g_bInserted==1&&iCount>=DATA_NUM&&iCount<DATA_NUM+INSERT_NUM)
				{                   //如果有重启提交且查询点在后2000个数据里就在pbufInsert查找数据
					iCount = iCount - DATA_NUM;
					dataTemp.fValue = pData->pbufInsert[(i-1)*INSERT_NUM+iCount];
				}
				else//查询的时间点超界，设备i在该时刻没有值
				{
					dataTemp.fValue = NO_VALUE;
				}						
			}
			else   //tQuery不是设备i的有效点
			{
				dataTemp.fValue = NO_VALUE;
			}
			//liResult.push_back(dataTemp);	
			pResult[i-ptStart] = dataTemp;
		}
		endClock = GetTickCount();
	}
	else//离散设备号
	{
//		cout<<"请输入查询的设备数量:";
//		cin>>ptNum;
		CString strPtNum;
		CString strPoints;
		GetDlgItem(IDC_EDIT_PTNUM)->GetWindowText(strPtNum);
		GetDlgItem(IDC_EDIT_POINTS)->GetWindowText(strPoints);


		ptNum = _ttoi(strPtNum);

		pResult = new Data[ptNum];
		if (pResult==NULL)
		{
			MessageBox(L"Memory Full",L"Error",MB_OK);
			return -1;
		}

		int* ptArray = new int[ptNum];
		int iLength = strPoints.GetLength();
		int iSet = 0;
		for (int i=0;i<ptNum;i++)
		{
//			cin>>ptArray[i];
			iSet = strPoints.Find(' ');
			if (iSet==-1)
			{
				iSet = 1;
			}
			ptArray[i] = _ttoi(strPoints.Left(iSet));
			iLength = iLength - iSet - 1;
			strPoints = strPoints.Right(iLength);
		}

//		cout<<"输入查询的时间点：";
//		cin>>tQuery;
		tQuery = _ttoi(strTQuery);

		startClock = GetTickCount();//输入完成后开始计时

		int iPoint = 0;
		long long iCount = 0;//tQuery时刻的值是某设备的第iCount个数据（从0开始记）		
		long long tStart = 0;
		long long tOffset = 0;

		for (int i=0;i<ptNum;i++)
		{
			iPoint = ptArray[i]; //获得索引表里的信息 设备号 开始时间 时间间隔
			tStart = pIndex[iPoint-1].tStart;
			tOffset = pIndex[iPoint-1].tOffset;	

			dataTemp.iPoint = iPoint;

			if ((tQuery-tStart)%tOffset==0)//tQuery是设备iPoint的有效点
			{
				iCount = (tQuery-tStart)/tOffset;

				if (iCount<DATA_NUM)//如果查询点在前10000个数据里，则在bufData里查找数据
				{					
					dataTemp.fValue = pData->pbufData[(iPoint-1)*DATA_NUM+iCount];
				}
				else if (g_bInserted==1&&iCount>=DATA_NUM&&iCount<DATA_NUM+INSERT_NUM)
				{                   //如果有重启提交且查询点在后2000个数据里就在pbufInsert查找数据
					iCount -= DATA_NUM;
					dataTemp.fValue = pData->pbufInsert[(iPoint-1)*INSERT_NUM+iCount];
				}
				else//查询的时间点超界，设备iPoint该时刻没有值
				{
					dataTemp.fValue = NO_VALUE;
				}							
			}
			else//tQuery不是设备iPoint的有效点
			{
				dataTemp.fValue = NO_VALUE;
			}
		//	liResult.push_back(dataTemp);
			pResult[i] = dataTemp;
		}
		endClock = GetTickCount();
	}

	delete[] pIndex;
	delete[] pData->pbufData;
	if (g_bInserted==1)
	{
		delete[] pData->pbufInsert;
	}
	delete pData;

//	cout<<"查询结果："<<ptNum<<"条\n";
//	cout<<"用    时："<<endClock - startClock <<"  ms\n";
//	if (endClock-startClock!=0)
//	{
//		cout<<"效    率："<<ptNum/(endClock-startClock)/10<<"  W/s\n";
//	}
//	else
//	{
//		cout<<"效    率：无穷大\n";
//	}
	CString strTime;;
	CString strEfficient;
	strTime.Format(L"查询结果：%d 条\n用      时：%d ms\n",ptNum,endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"查询效率：%d W/s\n",ptNum/(endClock-startClock)/10);
	}
	else
	{
		strEfficient.Format(L"查询效率：无穷大\n");
	}

	//**********************创建存放查询结果的文件*****************************//
	char szFileResult[MAX_PATH] = {0};
	sprintf_s(szFileResult,"Result\\断面查询_时刻%I64d.txt",tQuery);

	FILE* fp = NULL;		
	fopen_s(&fp,szFileResult,"w");
	if (fp==NULL)
	{
//		cout<<"Failed to Create File "<<szFileResult;
		MessageBox(L"Failed to Create File !",L"Error",MB_OK);

		return -1;
	}

	int iInvalidTime = 0;//记录查询结果中无效值的个数
//	list<Data>::iterator it;
//	for (it=liResult.begin();it!=liResult.end();it++)
	for (int i=0;i<ptNum;i++)
	{
		if (pResult[i].fValue!=NO_VALUE)
		{
			fprintf(fp,"time:%I64d  point:%d  value:%0.4f\n",tQuery,pResult[i].iPoint,pResult[i].fValue);
		}
		else
		{
			fprintf(fp,"time:%I64d  point:%d  value: 空值\n",tQuery,pResult[i].iPoint);
			iInvalidTime++;
		}	
	}
	fclose(fp);

	if (iInvalidTime>0)
	{
		printf("%d条查询结果中包含%d条无效记录（在查询时间点没有值）\n",ptNum,iInvalidTime);
	}

//	cout<<"本次查询结果已写入文件"<<szFileResult<<endl;
	CString strResult;
	strResult.Format(L"\n本次查询结果已写入到文件\nResult\\断面查询_时刻%I64d.txt",tQuery);
	int iResult = MessageBox(strTime+strEfficient+strResult+L"\n是否打开查询结果的文件目录？",
		L"断面查询结果",MB_YESNO|MB_ICONINFORMATION);

	if(iResult==IDYES)
	{
		ShellExecute(NULL,L"open",L".\\Result",NULL,NULL,SW_SHOW);
	}


	GetDlgItem(IDC_EDIT_TQUERY)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_PTNUM)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_POINTS)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_PTSTART)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_PTEND)->SetWindowText(L"");

	return 0;

}

unsigned int CFastWQDlg::hashData(int iPoint)
{
	return sizeof(float)*DATA_NUM*(iPoint-1);

}
unsigned int CFastWQDlg::hashInsert(int iPoint)
{
	return sizeof(float)*INSERT_NUM*(iPoint-1);

}
int CFastWQDlg::readIndex(void* pIndex)
{
	TCHAR szFileIndex[] = L"Data\\index.int";//索引文件

	HANDLE hFileIndex = CreateFile(szFileIndex,GENERIC_READ,FILE_SHARE_READ,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFileIndex==INVALID_HANDLE_VALUE)
	{
// 		cout<<"Failed to OpenFile index.int";
// 		cout<<"数据尚未提交，请提交数据\n";
		MessageBox(L"Failed to OpenFile index.int!",L"Error",MB_OK);
		return -1;
	}

	DWORD dwFileSize = GetFileSize(hFileIndex,NULL);

	BYTE** ppBuf = (BYTE**)pIndex;

	*ppBuf = new BYTE[dwFileSize];

	DWORD dwBytes = 0;

	ReadFile(hFileIndex,*ppBuf,dwFileSize,&dwBytes,NULL);

	CloseHandle(hFileIndex);

	return 0;
}

HBRUSH CFastWQDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	if (pWnd->GetDlgCtrlID()==IDC_RADIO1||pWnd->GetDlgCtrlID()==IDC_RADIO2)
	{
		return CreateSolidBrush(RGB(243, 234, 227));            //创建背景刷子 
	}
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC-> SetBkColor(RGB(243, 234, 227));
		return (HBRUSH)::GetStockObject(NULL_BRUSH); 
	}
	
	

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CFastWQDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	m_Radio = 0;

}

void CFastWQDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	m_Radio = 1;

}
