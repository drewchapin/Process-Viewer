// Process ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Process Viewer.h"
#include "Process ViewerDlg.h"
#include <tlhelp32.h>

//#define MAKEQWORD(a, b)      ((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessViewerDlg dialog

CProcessViewerDlg::CProcessViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessViewerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessViewerDlg)
	DDX_Control(pDX, IDC_WINDOW_SHOW, m_WinShow);
	DDX_Control(pDX, IDC_WINDOW_HIDE, m_WinHide);
	DDX_Control(pDX, IDC_WINDOW_REFRESH, m_WinRefresh);
	DDX_Control(pDX, IDC_WINDOW_CLOSE, m_WinClose);
	DDX_Control(pDX, IDC_PROC_TERMINATE, m_ProcTerminate);
	DDX_Control(pDX, IDC_EXIT, m_Exit);
	DDX_Control(pDX, IDC_PROC_REFRESH, m_ProcRefresh);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProcessViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessViewerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabChange)
	ON_BN_CLICKED(IDC_PROC_REFRESH, OnProcRefresh)
	ON_BN_CLICKED(IDC_PROC_TERMINATE, OnProcTerminate)
	ON_BN_CLICKED(IDC_WINDOW_HIDE, OnWindowHide)
	ON_BN_CLICKED(IDC_WINDOW_SHOW, OnWindowShow)
	ON_BN_CLICKED(IDC_WINDOW_REFRESH, OnWindowRefresh)
	ON_BN_CLICKED(IDC_WINDOW_CLOSE, OnWindowClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessViewerDlg message handlers

BOOL CProcessViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Tab.InsertItem( 0, "&Processes" );
	m_Tab.InsertItem( 1, "&Windows" );
	m_List.SetBkColor( RGB( 0, 0, 0 ) );
	m_List.SetTextBkColor( RGB( 0, 0, 0 ) );
	m_List.SetTextColor( RGB( 0, 255, 0 ) );
	m_List.SetExtendedStyle( m_List.GetExtendedStyle() | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT );
	SetForProc();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProcessViewerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProcessViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CProcessViewerDlg::SetForProc()
{

	ResetList();

	m_List.InsertColumn( List_Cols++, "Process", LVCFMT_LEFT, 150, -1 );
	m_List.InsertColumn( List_Cols++, "PID", LVCFMT_LEFT, 70, -1 );
	m_List.InsertColumn( List_Cols++, "Base Priority", LVCFMT_LEFT, 70, -1 );
	m_List.InsertColumn( List_Cols++, "Threads", LVCFMT_LEFT, 60, -1 );
	m_List.InsertColumn( List_Cols++, "Full Path", LVCFMT_LEFT, 400, -1 );

	m_List.InsertItem(0,"test");

	m_WinClose.ShowWindow( SW_HIDE );
	m_WinRefresh.ShowWindow( SW_HIDE );
	m_WinShow.ShowWindow( SW_HIDE );
	m_WinHide.ShowWindow( SW_HIDE );
	m_ProcRefresh.ShowWindow( SW_SHOW );
	m_ProcTerminate.ShowWindow( SW_SHOW );

	GetProcesses();

}

void CProcessViewerDlg::SetForWin()
{

	ResetList();

	m_List.InsertColumn( List_Cols++, "Title", LVCFMT_LEFT, 140, -1 );
	m_List.InsertColumn( List_Cols++, "Handle", LVCFMT_CENTER, 50, -1 );
	m_List.InsertColumn( List_Cols++, "Visible", LVCFMT_CENTER, 60, -1 );
	m_List.InsertColumn( List_Cols++, "Class Name", LVCFMT_LEFT, 120, -1 );
	m_List.InsertColumn( List_Cols++, "Position", LVCFMT_LEFT, 80, -1 );
	m_List.InsertColumn( List_Cols++, "Size", LVCFMT_LEFT, 80, -1 );

	m_WinClose.ShowWindow( SW_SHOW );
	m_WinRefresh.ShowWindow( SW_SHOW );
	m_WinShow.ShowWindow( SW_SHOW );
	m_WinHide.ShowWindow( SW_SHOW );
	m_ProcRefresh.ShowWindow( SW_HIDE );
	m_ProcTerminate.ShowWindow( SW_HIDE );

	GetWindows();

}

void CProcessViewerDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	OnClose();

}

void CProcessViewerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int mbResult = MessageBox( "Are you sure you want to exit?", NULL, MB_YESNO );
	if( mbResult == IDYES ) {

		EndDialog( 0 );
		CDialog::OnClose();

	}
}

void CProcessViewerDlg::OnTabChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if( m_Tab.GetCurSel() == 1 )
	{
	
		SetForWin();

	}
	else
	{

		SetForProc();

	}

	*pResult = 0;
}

void CProcessViewerDlg::ResetList()
{

	m_List.DeleteAllItems();

	for( int i = 0; i < List_Cols; i++ )
	{
	
		m_List.DeleteColumn( 0 );

	}

	List_Cols = 0;


}

void CProcessViewerDlg::GetProcesses()
{

	PROCESSENTRY32 pEntry32;

	m_List.DeleteAllItems();
	
	hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPALL, NULL );
	pEntry32.dwSize = sizeof( pEntry32 );
	
	pRes = Process32First( hSnapShot, &pEntry32 );

	do
	{

		_splitpath( pEntry32.szExeFile, Drive, Dir, fName, Ext );

		m_List.InsertItem( 0, NULL );
		
		sprintf( ProcName, "%s%s ", fName, Ext );
		m_List.SetItemText( 0, 0, ProcName );
		
		sprintf( ProcId, "%08X", pEntry32.th32ProcessID );
		m_List.SetItemText( 0, 1, ProcId );
		
		sprintf( ProcPriority, "%d", pEntry32.pcPriClassBase );
		m_List.SetItemText( 0, 2, ProcPriority );
		
		sprintf( ProcThreads, "%d", pEntry32.cntThreads );
		m_List.SetItemText( 0, 3, ProcThreads );
		//TCHAR f[255];
		//GetModuleFileName( (HMODULE)pEntry32.th32ParentProcessID, f, 255 );
		m_List.SetItemText( 0, 4, pEntry32.szExeFile  );

		pRes = Process32Next( hSnapShot, &pEntry32 );

	}
	while( pRes );

	CloseHandle( hSnapShot );

	m_List.SortItems( NULL, NULL );

}

void CProcessViewerDlg::GetWindows(){

	m_List.DeleteAllItems();
	EnumWindows( ListWindows, (LPARAM) this );
	m_List.SortItems( NULL, NULL );

}

BOOL CALLBACK CProcessViewerDlg::ListWindows( HWND hwnd, LPARAM lParam )
{

	CProcessViewerDlg * me = (CProcessViewerDlg*)lParam;

	return me->ListWindows( hwnd );

}

BOOL CProcessViewerDlg::ListWindows( HWND hwnd )
{

	char handle[256];
	char title[256];
	char classname[256];
	char pos[256];
	char size[256];
	RECT lpRect;
	char visible[256];

	::GetClassName( hwnd, classname, sizeof(classname) );
	::GetWindowText( hwnd, title, sizeof(title) );
	::GetWindowRect( hwnd, &lpRect );

	sprintf( handle, "%04d", hwnd );
	sprintf( pos, "%04d, %04d", lpRect.left, lpRect.top );
	sprintf( size, "%04dx%04d", lpRect.right - lpRect.left, lpRect.bottom - lpRect.top );
	sprintf( visible, ::IsWindowVisible( hwnd ) ? "Visible" : "Hidden" );

	if( ::GetWindowTextLength( hwnd ) == 0 )
	{

		sprintf( title, "<Not Set>" );

	}

	m_List.InsertItem( 0, NULL );
	m_List.SetItemText( 0, 0, title );
	m_List.SetItemText( 0, 1, handle );
	m_List.SetItemText( 0, 2, visible );
	m_List.SetItemText( 0, 3, classname );
	m_List.SetItemText( 0, 4, pos );
	m_List.SetItemText( 0, 5, size );

	return TRUE;

}

void CProcessViewerDlg::OnProcRefresh() 
{
	// TODO: Add your control notification handler code here
	GetProcesses();
}

void CProcessViewerDlg::OnProcTerminate() 
{
	// TODO: Add your control notification handler code here

	PROCESSENTRY32 pEntry32;
	HANDLE hProc;
	char temp[256];
	pEntry32.dwSize = sizeof(pEntry32);

	hSnapShot = CreateToolhelp32Snapshot( TH32CS_SNAPALL, NULL );

	pRes = Process32First( hSnapShot, &pEntry32 );

	do
	{
		
		sprintf( ProcId, "%08X", pEntry32.th32ProcessID );

		for( int i = 0; i < m_List.GetItemCount(); i++ )
		{
			if( m_List.GetItemState( i, LVNI_SELECTED ) == LVNI_SELECTED )
			{
				if( !strcmp( m_List.GetItemText( i, 1 ), ProcId ) )
				{
					sprintf( temp, "Are You want to terminate\n%s", pEntry32.szExeFile );
					if( IDYES == MessageBox( temp, NULL, MB_YESNO ) )
					{
						hProc = OpenProcess( SYNCHRONIZE|PROCESS_TERMINATE, FALSE, pEntry32.th32ProcessID );
						if( !TerminateProcess( hProc, 0 ) )
						{
							sprintf( temp, "Failed to terminate\n%s", pEntry32.szExeFile );
							MessageBox( temp );
						}
						else
						{

							CloseHandle( hProc );

						}
					}
				}
			}
		}

		pRes = Process32Next( hSnapShot, &pEntry32 );
	}
	while( pRes );

	CloseHandle( hSnapShot );
	Sleep( 500 );
	GetProcesses();

/*
	int nIndex = m_List.GetNextItem (-1, LVNI_ALL | LVNI_SELECTED);
	char temp[128];
	while (nIndex != -1) {
		sprintf (temp, "Item %d is selected\n", nIndex);
		MessageBox( temp );
		nIndex = m_List.GetNextItem (nIndex, LVNI_ALL | LVNI_SELECTED);
	}
*/
}

void CProcessViewerDlg::OnWindowHide() 
{
	// TODO: Add your control notification handler code here

	for( int i = 0; i < m_List.GetItemCount(); i++ )
	{

		if( m_List.GetItemState( i, LVNI_SELECTED ) == LVNI_SELECTED )
		{

			::ShowWindow( (HWND) atoi( m_List.GetItemText( i, 1 ) ), SW_HIDE );

		}

	}

	GetWindows();

}

void CProcessViewerDlg::OnWindowShow() 
{
	// TODO: Add your control notification handler code here

	for( int i = 0; i < m_List.GetItemCount(); i++ )
	{

		if( m_List.GetItemState( i, LVNI_SELECTED ) == LVNI_SELECTED )
		{

			::ShowWindow( (HWND) atoi( m_List.GetItemText( i, 1 ) ), SW_SHOW );

		}

	}

	GetWindows();

}

void CProcessViewerDlg::OnWindowRefresh() 
{
	// TODO: Add your control notification handler code here

	GetWindows();


}

void CProcessViewerDlg::OnWindowClose() 
{
	// TODO: Add your control notification handler code here
	

	for( int i = 0; i < m_List.GetItemCount(); i++ )
	{

		if( m_List.GetItemState( i, LVNI_SELECTED ) == LVNI_SELECTED )
		{

			if( ::PostMessage( (HWND) atoi( m_List.GetItemText( i, 1 ) ), WM_CLOSE, 0, 0 ) == 0 )
			{

				MessageBox( "Couldn't close the window" );

			}

		}

	}

	GetWindows();

}
