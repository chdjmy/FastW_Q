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
	GetDlgItem(IDC_STATIC_CONTENTS)->SetWindowText(L"��ѡ������������ڵ��ļ���\n");
	GetDlgItem(IDC_BUTTON_TESTDATA)->MoveWindow(bm.bmWidth/2+50,bm.bmHeight/3+100,200,80);

	GetDlgItem(IDC_PROGRESS_WRITE)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2,bm.bmWidth/2,30);
	GetDlgItem(IDC_BUTTON_START)->MoveWindow(bm.bmWidth/5*4,bm.bmHeight/2-40,70,70);
	GetDlgItem(IDC_STATIC_FILENAME)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2-40,bm.bmWidth/2,30);

	//������ѯ
	GetDlgItem(IDC_STATIC_POINT)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2-40,140,30);
	GetDlgItem(IDC_STATIC_STARTTIME)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2-40,140,30);
	GetDlgItem(IDC_STATIC_ENDTIME)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2-40,140,30);
	GetDlgItem(IDC_EDIT_POINT)->MoveWindow(bm.bmWidth/4,bm.bmHeight/2,140,30);
	GetDlgItem(IDC_EDIT_STARTTIME)->MoveWindow(bm.bmWidth/4+180,bm.bmHeight/2,140,30);
	GetDlgItem(IDC_EDIT_ENDTIME)->MoveWindow(bm.bmWidth/4+340,bm.bmHeight/2,140,30);

	//�����ѯ
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

	BOOL bOk =  CreateDirectory(L"Data", NULL); //��ų־û��ļ�
	bOk =  CreateDirectory(L"Result", NULL);    //��Ų�ѯ���

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
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ�ύ");
	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"�������ݣ� "+m_strDataPath+L"\\�����ύ");

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
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ�ύ");

	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"�������ݣ� "+m_strDataPath+L"\\�����ύ");

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
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ�ύ");
	GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"�������ݣ� "+m_strDataPath+L"\\ֹͣ�������ύ����");

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
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ��ѯ");

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
	GetDlgItem(IDC_BUTTON_START)->SetWindowText(L"��ʼ��ѯ");

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

	//��ʼ����ڲ���bi��ʼ

	bi.hwndOwner = NULL;

	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ�����

	bi.pszDisplayName = Buffer;//�˲�����ΪNULL������ʾ�Ի���

	bi.lpszTitle = L"��ѡ�������������Ŀ¼";

	bi.ulFlags = BIF_EDITBOX;//���༭��ķ��

	bi.lpfn = NULL;

	bi.lParam = 0;

	bi.iImage=IDR_MAINFRAME;

	//��ʼ����ڲ���bi����

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���

	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��		 
		 int iRet = MessageBox(Buffer,L"ѡ��Ŀ¼",MB_OKCANCEL|MB_ICONQUESTION);
		 if (iRet==IDOK)
		 {
			 m_strDataPath = Buffer;//��·��������һ��CString������

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
			 GetDlgItem(IDC_BUTTON_TESTDATA)->SetWindowText(L"�޸Ĳ�������");

			 MessageBox(L"�������ݵ���ɹ���",L"����ɹ�",MB_OK);
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
	/**********************���������ύ�Ĳ�������*********************/

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


	//�ַ�ת��
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, NULL, 0, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char* psDataPath = NULL;	
	psDataPath = new char[dwNum];//������Ҫ�Ĵ�С��̬����
	
	WideCharToMultiByte (CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, psDataPath, dwNum, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	CString strFileName;

	for (int k = 0;k<10;k++)
	{
		sprintf_s(szFileName,"%s\\�����ύ\\tag_%d-%d.txt",
			psDataPath,k*1000+1,(k+1)*1000+1);

		fopen_s(&fp,szFileName,"r");

		if(fp==NULL)
		{
			MessageBox(L"Cannot openfile!",L"Error",MB_OK);

			return -1;
		}

		strFileName.Format(L"\\�����ύ\\tag_%d-%d.txt",k*1000+1,(k+1)*1000+1);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"���ڶ��룺 "+m_strDataPath+strFileName);	

		for (int i=0;i<1000;i++)//ÿ���ļ�1000���豸
		{
			fscanf_s(fp,"%d",&pIndex[i+1000*k].iPoint);
			fscanf_s(fp,"%I64d",&pIndex[i+1000*k].tStart);
			fscanf_s(fp,"%I64d",&pIndex[i+1000*k].tOffset);

			m_Process.StepIt();

			for (int j=0;j<DATA_NUM;j++)//ÿ���豸10000������
			{
				fscanf_s(fp,"%f",&pbuf[j+10000*i+10000*1000*k]);
			}
		}
		fclose(fp);
	}
	delete[] psDataPath;

	

	/*************�������ڴ��10000*10000�����ύ���ݵ��ļ�************/

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"����д�������ļ�");

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
	strTime.Format(L"�����ύ��10000*10000 ��\n��      ʱ��%d ms\n",endClock-startClock);
//	printf("д������ ��10000*10000 ��\n��ʱ��%d ms\n",endClock-startClock);
	if (endClock-startClock!=0)
	{
//		printf("д��Ч�ʣ�%d W/s\n",10000*10000/10/(endClock-startClock));
		strEfficient.Format(L"д��Ч�ʣ�%d W/s\n",10000*10000/10/(endClock-startClock));
	}
	

	/**********�����ļ�����Ÿ����豸��ʱ����**********************/

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"����д�������ļ�");

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
	strResult += L"\n�����ύ���ɵĳ־û��ļ�����ڵ�ǰ����Ŀ¼Data�ļ���\n�����ļ���data.flo�������ļ���insert.int";

	MessageBox(strResult,L"�ύ�ɹ�",MB_OK|MB_ICONINFORMATION);

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"�ύ�ɹ�!  "+strEfficient);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->EnableWindow(TRUE);

	Invalidate();

	return 0;

}

// �� �� ����readShotdata
// �������ܣ��Ѷ����ύ��ȫ�����ݰ���ƵĽṹ����Data\\data.flo
// �� �� ֵ������int ����0��ʾ��ѯ�ɹ������ط�0��ʾʧ��
// ��    ������
int CFastWQDlg::writeShotdata()
{
	/**********************�򿪶����ύ�Ĳ�������*********************/

	float* pbuf = new float[DATA_NUM*POINT_NUM];//�������10000*10000������
	if (pbuf==NULL)
	{
//		printf("new pbuf failed");
		MessageBox(L"new pbuf failed!",L"Error",MB_OK);
		return -1;
		
	} 

	shotIndex* pIndex = new shotIndex[10];//�������һ������������
	if (pIndex==NULL)
	{
//		printf("memory full");
		MessageBox(L"memory full!",L"Error",MB_OK);
		return -1;
	}

	//�ַ�ת��
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, NULL, 0, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char* psDataPath = NULL;	
	psDataPath = new char[dwNum];//������Ҫ�Ĵ�С��̬����

	WideCharToMultiByte (CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, psDataPath, dwNum, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char szFileName[MAX_PATH] = {0}; 
	FILE* fp;

	CString strFileName;

	for (int k = 0;k<10;k++)
	{
		sprintf_s(szFileName,"%s\\�����ύ\\shots_tag_%d-%d.txt",
			psDataPath,k*1000+1,(k+1)*1000+1);

		fopen_s(&fp,szFileName,"r");

		if (fp==NULL)
		{
//			cout<<"Cannot openfile!";
			MessageBox(L"Cannot openfile!",L"Error",MB_OK);

			return -1;
		}

//		cout<<szFileName<<endl;//����ļ�������Ļ
		m_Process.StepIt();
		strFileName.Format(L"\\�����ύ\\tag_%d-%d.txt",k*1000+1,(k+1)*1000+1);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"���ڶ��룺 "+m_strDataPath+strFileName);

		fscanf_s(fp,"%d",&pIndex[k].ptStart);//��ʼ�豸��
		fscanf_s(fp,"%d",&pIndex[k].ptEnd);  //�����豸��
		fscanf_s(fp,"%I64d",&pIndex[k].tStart); //��ʼʱ��
		fscanf_s(fp,"%I64d",&pIndex[k].tOffset);//ʱ����

		for (int i=0;i<1000*10000;i++)
		{
			fscanf_s(fp,"%f",&pbuf[i+1000*10000*k]);
		}

		fclose(fp);
	}

	DWORD startClock = GetTickCount();//���ݶ�����Ϻ�ʼ��ʱ

	//�Ѱ�������������ݴ���ɰ���������ĸ�ʽ
	//���Ȱ�10*1000*10000�����ݲ�ֳ�100��1000*1000�ľ���,ʹͬһ�豸��1000����������
	float fTemp = 0; 
	float* p = NULL;
	for (int k=0;k<100;k++)
	{
		p = pbuf+1000*1000*k;

		for (int i=0;i<1000;i++)  //����ת��
		{
			for (int j=i;j<1000;j++)
			{	
				fTemp = p[i+j*1000];
				p[i+j*1000] = p[j+i*1000];
				p[j+i*1000] = fTemp;	
			}
		}

	}

	//�ٰ����ݲ��Ϊ10��1000*10000���ݿ飬ÿ�����ݿ������10000*1000�ĸ�ʽ
	//ʹͬһ�豸��10000����������
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

	/*************�������ڴ��10000*10000�����ύ���ݵ��ļ�************/

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
	strTime.Format(L"�����ύ��10000*10000 ��\n��      ʱ��%d ms\n",endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"д��Ч�ʣ�%d W/s\n",10000*10000/10/(endClock-startClock));
	}


	/**********�����ļ�����Ÿ����豸��ʱ����**********************/


	//�Ѷ����ύ����������ɺ������ύһ�µ�����
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

//	cout<<"\n�����ύ���ɵĳ־û��ļ����Ŀ¼ΪData�ļ���"
//		<<"\n�����ļ���data.flo�������ļ���insert.int��";
	CString strResult;
	strResult = strTime+strEfficient;
	strResult += L"\n�����ύ���ɵĳ־û��ļ�����ڵ�ǰ����Ŀ¼Data�ļ���\n�����ļ���data.flo�������ļ���insert.int";

	MessageBox(strResult,L"�ύ�ɹ�",MB_OK|MB_ICONINFORMATION);

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"�ύ�ɹ�!  "+strEfficient);
	GetDlgItem(IDC_BUTTON_WRITEINSERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERYSHOT)->EnableWindow(TRUE);

	Invalidate();

	return 0;
}

int CFastWQDlg::writeInsertdata()
{
	/**********************��ֹͣ�������ύ�Ĳ�������*********************/

	float* pbuf = new float[INSERT_NUM*POINT_NUM+200];
	if (pbuf==NULL)
	{
//		cout<<"Failed to new pbuf";
		MessageBox(L"Failed to new pbuf!",L"Error",MB_OK);
		return -1;
	}

	//�ַ�ת��
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, m_strDataPath.GetBuffer()
		, -1, NULL, 0, NULL, FALSE);
	m_strDataPath.ReleaseBuffer();

	char* psDataPath = NULL;	
	psDataPath = new char[dwNum];//������Ҫ�Ĵ�С��̬����

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
		sprintf_s(szFileName,"%s\\ֹͣ�������ύ����\\insert_tag_%d-%d.txt",
			psDataPath,k*1000+1,(k+1)*1000+1);

		fopen_s(&fp,szFileName,"r");
		if(fp==NULL)
		{
//			cout<<"Cannot openfile!";
			MessageBox(L"Cannot openfile!",L"Error",MB_OK);

			return -1;
		}
//		cout<<szFileName<<endl;//����ļ���
		strFileName.Format(L"\\ֹͣ�������ύ����\\tag_%d-%d.txt",k*1000+1,(k+1)*1000+1);
		GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"���ڶ��룺 "+m_strDataPath+strFileName);

		for (i=0;i<1000;i++)//ÿ���ļ�1000���豸
		{
			m_Process.StepIt();

			fscanf_s(fp,"%d",&indexTemp.iPoint);
			fscanf_s(fp,"%I64d",&indexTemp.tStart);
			fscanf_s(fp,"%I64d",&indexTemp.tOffset);

			for (j=0;j<INSERT_NUM;j++)//ÿ���豸2000������
			{
				fscanf_s(fp,"%f",&(pbuf[j+2000*i+2000*1000*k]));
			}

		}
		fclose(fp);
	}

	DWORD startClock = GetTickCount();//��ʼ��ʱ

	/*************�������ڴ��2000*10000�����ύ���ݵ��ļ�************/

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
	strTime.Format(L"ֹͣ�������ύ��2000*10000 ��\n��      ʱ��%d ms\n",endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"д��Ч�ʣ�%d W/s\n",2000*10000/10/(endClock-startClock));
	}

//	cout<<"\n�����ύ���ɵ������ļ���insert.flo�����Ŀ¼ΪData�ļ���";
	CString strResult;
	strResult = strTime+strEfficient;
	strResult += L"\n�����ύ���ɵĳ־û��ļ�����ڵ�ǰ����Ŀ¼Data�ļ���\n�����ļ���insert.flo";

	MessageBox(strResult,L"�ύ�ɹ�",MB_OK|MB_ICONINFORMATION);

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(L"�ύ�ɹ�!  "+strEfficient);

	Invalidate();

	return 0;
}

// �� �� ����queryByPoint
// �������ܣ����ά�ȵĲ�ѯ�����ѯһ��Pointһ��ʱ���ڵ���������
//           ��ѯ��������ļ���Result��
// �� �� ֵ������int ����0��ʾ��ѯ�ɹ������ط�0��ʾʧ��
// ��    ������
int CFastWQDlg::queryByPoint()
{
	int iPoint = 0;             //�豸��
	long long startTime = 0;    //��ʼʱ��
	long long endTime = 0;      //����ʱ��

	CString strPoint;
	CString strStartTime;
	CString strEndTime;

	/**********************************  ����׼���׶�  ****************************/

	//��1���������豸������
	tagIndex* pIndex = NULL;  
	int iRet = readIndex((void*)&pIndex);
	if (iRet==-1)//��ȡ����ʧ��
	{
		return -1;
	}

	//��2�����û������豸��
//	cout<<"������ѯ��\n�������豸��(1~10000)��";
//	cin>>iPoint;
	GetDlgItem(IDC_EDIT_POINT)->GetWindowText(strPoint);
	iPoint = _ttoi(strPoint);

	while(iPoint<1||iPoint>POINT_NUM)
	{
//		cout<<"�豸�Ų��ڹ涨��Χ��,����������"<<endl;
		MessageBox(L"�豸�Ų��ڹ涨��Χ��,����������!",L"Error",MB_OK);
		return -1;
	}

	//��3�����Ѹ��豸��������׼�����ڴ�
	float* pbuf= NULL;
	pbuf = new float[DATA_NUM+g_bInserted*INSERT_NUM];
	if (pbuf == NULL)
	{
//		cout<<"Failed to new pbuf";
		MessageBox(L"Failed to new pbuf !",L"Error",MB_OK);
		return -1;
	}

	TCHAR szFileData[] = L"Data\\data.flo";//�����ļ�
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
		TCHAR szFileInsert[] = L"Data\\insert.flo";//�����ύ�������ļ�

		HANDLE hFileInsert= CreateFile(szFileInsert,GENERIC_READ,FILE_SHARE_READ,
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFileInsert==INVALID_HANDLE_VALUE)
		{
//			cout<<"Failed to OpenFile insert.flo,���ύ�����������";
			MessageBox(L"Failed to OpenFile insert.flo,���ύ�����������!",L"Error",MB_OK);
			g_bInserted = 0;
			return -1;
		}

		SetFilePointer(hFileInsert,hashInsert(iPoint),NULL,FILE_BEGIN);
		ReadFile(hFileInsert,pbuf+DATA_NUM,sizeof(float)*INSERT_NUM,&dwBytes,NULL);

		CloseHandle(hFileInsert);
	}
	/**********************************  �������׼��  ****************************/


//	cout<<"��������ʼʱ�� ��";
//	cin>>startTime;
//	cout<<"���������ʱ�� ��";
//	cin>>endTime;
	GetDlgItem(IDC_EDIT_STARTTIME)->GetWindowText(strStartTime);
	GetDlgItem(IDC_EDIT_ENDTIME)->GetWindowText(strEndTime);
	startTime = _ttoi(strStartTime);
	endTime = _ttoi(strEndTime);

	while (endTime<startTime)
	{
//		cout<<"����ʱ�䲻��С�ڿ�ʼʱ��,�������������ʱ��"<<endl;
//		cin>>endTime;
		MessageBox(L"����ʱ�䲻��С�ڿ�ʼʱ��,�������������ʱ��!",L"Error",MB_OK);
		delete[] pbuf;
		delete[] pIndex;
		return -1;
	}

	DWORD startClock = GetTickCount();//�����������ʼ��ʱ

	//�������������豸��ʼʱ���ʱ����
	long long tStart = pIndex[iPoint-1].tStart;
	long long tOffset = pIndex[iPoint-1].tOffset;
	delete[] pIndex;


	//���ݲ�ѯ������λ����

	//iFirstHit�ǵ�һ�����ڻ���ڲ�ѯ��ʼʱ�����Ч��ı��
	//��Ч��ָ�����ݵ�ʱ��㣬���� �豸�Ŀ�ʼʱ����0�ŵ���Ч��
	long long iFirstHit = (startTime-tStart)/tOffset;
	if (startTime > tStart+iFirstHit*tOffset)
	{
		iFirstHit++;
	}
	//�߽紦�������ѯ��ʼʱ�䳬�����豸��ʱ���Ͻ磬���ѯ���Ϊ��
	if (iFirstHit >= DATA_NUM+g_bInserted*INSERT_NUM)
	{
//		cout<<"��ѯ��ʼʱ�䳬��ʱ���Ͻ�\n";
		delete[] pbuf;
		return -1;
	}
	//�����ѯ��ʱ���С���豸��ʱ�������Ҹ�ʱ����ڲ�������Ч�㣬���ѯ���Ϊ��	
	if (endTime < tStart+tOffset*iFirstHit)
	{
		printf("�豸%d (��ʼʱ��:%I64d ʱ����:%I64d)�� %I64d��%I64d ʱ����û������\n",
			iPoint,tStart,tOffset,startTime,endTime);
		return -1;
	}

	//�����ʱ�䷶Χ�ڵ���Ч��ĸ���
	long long iHitNum = (endTime-iFirstHit*tOffset-tStart)/tOffset+1;

	//����������ݵķ�Χ�������������������ѯ���ֻ�����ݷ�Χ�ڵ�ֵ
	if (iFirstHit+iHitNum > DATA_NUM+g_bInserted*INSERT_NUM)
	{
		iHitNum = DATA_NUM+g_bInserted*INSERT_NUM-iFirstHit;
	}

	//�����ڴ�ռ��Ų�ѯ���
	Data* pResult = new Data[(int)iHitNum];
	if (pResult==NULL)
	{
//		cout<<"Failed to new pResult";
		MessageBox(L"Failed to new pResult!",L"Error",MB_OK);
		return -1;
	}

	//�������ݶ�λ��ò�ѯ���
	for (int i=0;i<iHitNum;i++)
	{
		pResult[i].iPoint = iPoint;
		pResult[i].lTime = iFirstHit*tOffset+tStart+tOffset*i;
		pResult[i].fValue = pbuf[iFirstHit+i];
	}
	delete[] pbuf;

	DWORD endClock = GetTickCount();//��ѯ�����������Ч��

	CString strTime;;
	CString strEfficient;
	strTime.Format(L"��ѯ�����%d ��\n��      ʱ��%d ms\n",iHitNum,endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"��ѯЧ�ʣ�%d W/s\n",iHitNum/(endClock-startClock)/10);
	}
	else
	{
		strEfficient.Format(L"��ѯЧ�ʣ������\n");
	}

// 	printf("��� %d ��\n",iHitNum);
// 	printf("��ʱ %d ����\n",endClock-startClock);
// 	if (endClock-startClock!=0)
// 	{
// 		printf("Ч�� %d W/s\n",iHitNum/(endClock-startClock)/10);
// 	}
// 	else
// 	{
// 		printf("Ч�� �����\n");
// 	}

	/******************************  ��ѯ���д���ļ�  ***************************/

	char szFileResult[MAX_PATH] = {0};
	sprintf_s(szFileResult,"Result\\������ѯ_�豸%d_ʱ��%I64d-%I64d.txt",iPoint,startTime,endTime);

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

//	cout<<"���β�ѯ�����д���ļ�"<<szFileResult<<endl;
	CString strResult;
	strResult.Format(L"\n���β�ѯ�����д�뵽�ļ�\nResult\\������ѯ_�豸%d_ʱ��%I64d-%I64d.txt",iPoint,startTime,endTime);
	int iResult = MessageBox(strTime+strEfficient+strResult+L"\n�Ƿ�򿪲�ѯ������ļ�Ŀ¼��",
		L"������ѯ���",MB_YESNO|MB_ICONINFORMATION);
	if(iResult==IDYES)
	{
		ShellExecute(NULL,L"open",L".\\Result",NULL,NULL,SW_SHOW);
	}


	GetDlgItem(IDC_EDIT_POINT)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_STARTTIME)->SetWindowText(L"");
	GetDlgItem(IDC_EDIT_ENDTIME)->SetWindowText(L"");

	

	return 0;

}
// �� �� ����queryByTime
// �������ܣ������ѯ����ѯ�����豸һ��ʱ��������
//           ��ѯ���д���ļ���Result��
// �� �� ֵ������int ����0��ʾ��ѯ�ɹ������ط�0��ʾʧ��
// ��    ������
int CFastWQDlg::queryByTime()
{
	/********************************  ����׼���׶� *********************************/

	//��1��.����������Ϣ
	tagIndex* pIndex = NULL;  
	int iRet = readIndex((void*)&pIndex);
	if (iRet==-1)//��ȡ����ʧ��
	{
		MessageBox(L"Failed to read Index !",L"Error",MB_OK);
		return -1;
	}

	//��2��.�����״��ύ������
	readData* pData = new readData;
	pData->pbufData = new float[DATA_NUM*POINT_NUM];
	if (pData->pbufData==NULL)
	{
//		cout <<"Failed to new pData->pbufData";
		MessageBox(L"Failed to new pData->pbufData !",L"Error",MB_OK);
		return -1;
	}

	TCHAR szFileData[] = L"Data\\data.flo";//�״��ύ��10000*10000������
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

	//��3��.����������ύ����������ύ����
	if (g_bInserted==1)
	{
		pData->pbufInsert = new float[INSERT_NUM*POINT_NUM];

		TCHAR szFileInsert[] = L"Data\\insert.flo";//�״��ύ��10000*10000������
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

	/********************************  �������׼�� *********************************/


	DWORD startClock = 0;
	DWORD endClock = 0;

	//�����豸�ŵķ�ʽ
	int iType = m_Radio;
//	cout<<"�����ѯ��\n��ѡ�������豸���뷽ʽ��1-�����豸�� 2-��ɢ�豸�ţ�";
//	cin>>iType;

	long long tQuery = 0;    //��ѯ��ʱ���
	int ptNum = 0;           //��ѯ���豸������
//	list<Data> liResult;     //������Ų�ѯ���������
	Data* pResult = NULL;

	CString strTQuery;
	GetDlgItem(IDC_EDIT_TQUERY)->GetWindowText(strTQuery);

	Data dataTemp = {0};     //�ݴ�һ����ѯ���

	if (iType==0)//�����豸��
	{
		int ptStart = 0;  //��ʼ�豸��
		int ptEnd = 0;    //�����豸��

		CString strPtStart;
		CString strPtEnd;

		GetDlgItem(IDC_EDIT_PTSTART)->GetWindowText(strPtStart);
		GetDlgItem(IDC_EDIT_PTEND)->GetWindowText(strPtEnd);

//		cout<<"���뿪ʼ�豸�ţ�";
//		cin>>ptStart;
		ptStart = _ttoi(strPtStart);

		while(ptStart>10000||ptStart<0)
		{
//			cout<<"��ʼ�豸�ų��磬���������룺";
			MessageBox(L"��ʼ�豸�ų��磬����������!",L"Error",MB_OK);
			return -1;
//			cin>>ptStart;
		}

//		cout<<"��������豸�ţ�";
//		cin>>ptEnd;
		ptEnd = _ttoi(strPtEnd);
		while(ptEnd>10000||ptEnd<0)
		{
			MessageBox(L"��ʼ�豸�ų��磬����������!",L"Error",MB_OK);
			return -1;
//			cout<<"�����豸�ų��磬���������룺";
//			cin>>ptEnd;
		}

		ptNum = ptEnd-ptStart+1;     //�����豸����	
		pResult = new Data[ptNum];
		if (pResult==NULL)
		{
			MessageBox(L"Memory Full",L"Error",MB_OK);
			return -1;
		}

//		cout<<"�����ѯ��ʱ��㣺";
//		cin>>tQuery;
		tQuery = _ttoi(strTQuery);

		startClock = GetTickCount();//����׼����ɺ�ʼ��ʱ

		dataTemp.lTime = tQuery;
		long long iCount = 0;            //tQueryʱ�����豸�ĵ�iCount����Ч�㣨��0��ʼ�ǣ�
		long long tStart = 0;   
		long long tOffset = 0; 

		for (int i=ptStart;i<ptStart+ptNum;i++)
		{
			dataTemp.iPoint = i;
			tStart = pIndex[i-1].tStart;   //���������ÿ�ʼʱ���ʱ����
			tOffset = pIndex[i-1].tOffset;

			if ((tQuery-tStart)%tOffset==0)//tQuery���豸i����Ч��
			{
				iCount = (tQuery-tStart)/tOffset;

				if (iCount<DATA_NUM)//�����ѯ����ǰ10000�����������pbufData���������
				{					
					dataTemp.fValue = pData->pbufData[(i-1)*DATA_NUM+iCount];
				}
				else if (g_bInserted==1&&iCount>=DATA_NUM&&iCount<DATA_NUM+INSERT_NUM)
				{                   //����������ύ�Ҳ�ѯ���ں�2000�����������pbufInsert��������
					iCount = iCount - DATA_NUM;
					dataTemp.fValue = pData->pbufInsert[(i-1)*INSERT_NUM+iCount];
				}
				else//��ѯ��ʱ��㳬�磬�豸i�ڸ�ʱ��û��ֵ
				{
					dataTemp.fValue = NO_VALUE;
				}						
			}
			else   //tQuery�����豸i����Ч��
			{
				dataTemp.fValue = NO_VALUE;
			}
			//liResult.push_back(dataTemp);	
			pResult[i-ptStart] = dataTemp;
		}
		endClock = GetTickCount();
	}
	else//��ɢ�豸��
	{
//		cout<<"�������ѯ���豸����:";
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

//		cout<<"�����ѯ��ʱ��㣺";
//		cin>>tQuery;
		tQuery = _ttoi(strTQuery);

		startClock = GetTickCount();//������ɺ�ʼ��ʱ

		int iPoint = 0;
		long long iCount = 0;//tQueryʱ�̵�ֵ��ĳ�豸�ĵ�iCount�����ݣ���0��ʼ�ǣ�		
		long long tStart = 0;
		long long tOffset = 0;

		for (int i=0;i<ptNum;i++)
		{
			iPoint = ptArray[i]; //��������������Ϣ �豸�� ��ʼʱ�� ʱ����
			tStart = pIndex[iPoint-1].tStart;
			tOffset = pIndex[iPoint-1].tOffset;	

			dataTemp.iPoint = iPoint;

			if ((tQuery-tStart)%tOffset==0)//tQuery���豸iPoint����Ч��
			{
				iCount = (tQuery-tStart)/tOffset;

				if (iCount<DATA_NUM)//�����ѯ����ǰ10000�����������bufData���������
				{					
					dataTemp.fValue = pData->pbufData[(iPoint-1)*DATA_NUM+iCount];
				}
				else if (g_bInserted==1&&iCount>=DATA_NUM&&iCount<DATA_NUM+INSERT_NUM)
				{                   //����������ύ�Ҳ�ѯ���ں�2000�����������pbufInsert��������
					iCount -= DATA_NUM;
					dataTemp.fValue = pData->pbufInsert[(iPoint-1)*INSERT_NUM+iCount];
				}
				else//��ѯ��ʱ��㳬�磬�豸iPoint��ʱ��û��ֵ
				{
					dataTemp.fValue = NO_VALUE;
				}							
			}
			else//tQuery�����豸iPoint����Ч��
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

//	cout<<"��ѯ�����"<<ptNum<<"��\n";
//	cout<<"��    ʱ��"<<endClock - startClock <<"  ms\n";
//	if (endClock-startClock!=0)
//	{
//		cout<<"Ч    �ʣ�"<<ptNum/(endClock-startClock)/10<<"  W/s\n";
//	}
//	else
//	{
//		cout<<"Ч    �ʣ������\n";
//	}
	CString strTime;;
	CString strEfficient;
	strTime.Format(L"��ѯ�����%d ��\n��      ʱ��%d ms\n",ptNum,endClock-startClock);
	if (endClock-startClock!=0)
	{
		strEfficient.Format(L"��ѯЧ�ʣ�%d W/s\n",ptNum/(endClock-startClock)/10);
	}
	else
	{
		strEfficient.Format(L"��ѯЧ�ʣ������\n");
	}

	//**********************������Ų�ѯ������ļ�*****************************//
	char szFileResult[MAX_PATH] = {0};
	sprintf_s(szFileResult,"Result\\�����ѯ_ʱ��%I64d.txt",tQuery);

	FILE* fp = NULL;		
	fopen_s(&fp,szFileResult,"w");
	if (fp==NULL)
	{
//		cout<<"Failed to Create File "<<szFileResult;
		MessageBox(L"Failed to Create File !",L"Error",MB_OK);

		return -1;
	}

	int iInvalidTime = 0;//��¼��ѯ�������Чֵ�ĸ���
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
			fprintf(fp,"time:%I64d  point:%d  value: ��ֵ\n",tQuery,pResult[i].iPoint);
			iInvalidTime++;
		}	
	}
	fclose(fp);

	if (iInvalidTime>0)
	{
		printf("%d����ѯ����а���%d����Ч��¼���ڲ�ѯʱ���û��ֵ��\n",ptNum,iInvalidTime);
	}

//	cout<<"���β�ѯ�����д���ļ�"<<szFileResult<<endl;
	CString strResult;
	strResult.Format(L"\n���β�ѯ�����д�뵽�ļ�\nResult\\�����ѯ_ʱ��%I64d.txt",tQuery);
	int iResult = MessageBox(strTime+strEfficient+strResult+L"\n�Ƿ�򿪲�ѯ������ļ�Ŀ¼��",
		L"�����ѯ���",MB_YESNO|MB_ICONINFORMATION);

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
	TCHAR szFileIndex[] = L"Data\\index.int";//�����ļ�

	HANDLE hFileIndex = CreateFile(szFileIndex,GENERIC_READ,FILE_SHARE_READ,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFileIndex==INVALID_HANDLE_VALUE)
	{
// 		cout<<"Failed to OpenFile index.int";
// 		cout<<"������δ�ύ�����ύ����\n";
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
		return CreateSolidBrush(RGB(243, 234, 227));            //��������ˢ�� 
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
