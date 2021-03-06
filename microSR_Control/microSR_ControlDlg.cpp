
// microSR_ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "microSR_Control.h"
#include "microSR_ControlDlg.h"
#include "afxdialogex.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmicroSRControlDlg dialog



CmicroSRControlDlg::CmicroSRControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MICROSR_CONTROL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmicroSRControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmicroSRControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CmicroSRControlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CmicroSRControlDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CmicroSRControlDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CmicroSRControlDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CmicroSRControlDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CmicroSRControlDlg message handlers

BOOL CmicroSRControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	SetDlgItemText(IDC_EDIT19, L"Environmental micro Sound Recorder Control");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CmicroSRControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CmicroSRControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmicroSRControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#include "Serial.h"

inline void CmicroSRControlDlg::loadPar(char c, int item)
{
	wchar_t text[40];
	char txt[40];
	int par;
	int nn=T_load(c, txt, 40); 
	sscanf_s(txt, "%d", &par); 
	swprintf(text, 40, L"%d", par);	
	SetDlgItemText(item, (const wchar_t *)text);

}

inline void CmicroSRControlDlg::loadStr(char c, int item)
{
	wchar_t text[40];
	char txt[40];
	int nn=T_load(c, txt, 40);
	size_t len;
	mbstowcs_s(&len, text, 16, txt, 16);
	SetDlgItemText(item, (const wchar_t *)text);
}

inline void CmicroSRControlDlg::storePar(char c, int item)
{
	wchar_t text[16];
	char txt[16];
	int par;
	// read from devive and show on gui
	GetDlgItemText(item, text, 16);
	swscanf_s((const wchar_t *)text, L"%d", &par);
	sprintf_s(txt, 16, "%d", par);
	T_store(c, txt);
}

inline void CmicroSRControlDlg::storeStr(char c, int item)
{
	wchar_t text[40];
	char txt[40];
	size_t len;
	GetDlgItemText(item, text, 16);
	wcstombs_s(&len, txt, 16, text, 16);
	T_store(c, txt);
}

void CmicroSRControlDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// Load parameters from Teensy
	if (!findTeensy(portName)) return;

	SYSTEMTIME lt;
	GetLocalTime(&lt);
	wchar_t text[40];
	swprintf(text, 40, L"%02d:%02d:%02d", lt.wHour, lt.wMinute, lt.wSecond);
	SetDlgItemText(IDC_EDIT18, (const wchar_t *)text);
	swprintf(text, 40, L"%04d/%02d/%02d", lt.wYear, lt.wMonth, lt.wDay);
	SetDlgItemText(IDC_EDIT17, (const wchar_t *)text);

	comOpen();

	// read from devive and show on gui
	loadPar('o', IDC_EDIT1);
	loadPar('a', IDC_EDIT2);
	loadPar('r', IDC_EDIT3);
	loadPar('1', IDC_EDIT4);
	loadPar('2', IDC_EDIT5);
	loadPar('3', IDC_EDIT6);
	loadPar('4', IDC_EDIT7);

	loadPar('c', IDC_EDIT22);
	loadPar('h', IDC_EDIT9);
	loadPar('w', IDC_EDIT10);
	loadPar('s', IDC_EDIT11);
	loadPar('e', IDC_EDIT12);
	loadPar('i', IDC_EDIT13);
	loadPar('k', IDC_EDIT14);
	loadPar('p', IDC_EDIT20);

	loadStr('n', IDC_EDIT8);
	loadStr('d', IDC_EDIT15);
	loadStr('t', IDC_EDIT16);
	comClose();
}


void CmicroSRControlDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	// Store parameters to Teensy
	if (!findTeensy(portName)) return;

	comOpen();

	// read from gui and store on device
	storePar('o', IDC_EDIT1);
	storePar('a', IDC_EDIT2);
	storePar('r', IDC_EDIT3);
	storePar('1', IDC_EDIT4);
	storePar('2', IDC_EDIT5);
	storePar('3', IDC_EDIT6);
	storePar('4', IDC_EDIT7);

	storePar('c', IDC_EDIT22);
	storePar('h', IDC_EDIT9);
	storePar('w', IDC_EDIT10);
	storePar('s', IDC_EDIT11);
	storePar('e', IDC_EDIT12);
	storePar('i', IDC_EDIT13);
	storePar('k', IDC_EDIT14);
	storePar('p', IDC_EDIT20);

	storeStr('n', IDC_EDIT8);

	comClose();
}


void CmicroSRControlDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	// Sync time stamp with Teensy
	if (!findTeensy(portName)) return;

	SYSTEMTIME lt;
	GetLocalTime(&lt);

	char txt[32];
	sprintf_s(txt,32,"%02d:%02d:%02d", lt.wHour, lt.wMinute, lt.wSecond);

	comOpen();
	T_store('t', txt);

	wchar_t text[32];
	size_t len;
	len = 0;
	mbstowcs_s(&len, text, 16, txt, 16);
	SetDlgItemText(IDC_EDIT16, (const wchar_t *)text);
	SetDlgItemText(IDC_EDIT18, (const wchar_t *)text);

	sprintf_s(txt, 32, "%04d/%02d/%02d", lt.wYear,lt.wMonth,lt.wDay);
	T_store('d', txt);
	comClose();

	len = 0;
	mbstowcs_s(&len, text, 16, txt, 16);
	SetDlgItemText(IDC_EDIT15, (const wchar_t *)text);
	SetDlgItemText(IDC_EDIT17, (const wchar_t *)text);
}


void CmicroSRControlDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	// start Teensy immediately (send x-1)
	DWORD written;
	comOpen();
	WriteFile(file, "x-1", 3, &written, NULL);
	comClose();
}

#include "monitor.h"
void CmicroSRControlDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	// monitor Teensy
	static int running=0;
	if (!running)
	{	running = 1;
		SetDlgItemText(IDC_EDIT21, L"monitor not implmented yet");
//		monitor(1);
	}
	else
	{	running = 0;
		//		monitor(0);
	}
}
