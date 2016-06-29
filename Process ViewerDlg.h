// Process ViewerDlg.h : header file
//

#if !defined(AFX_PROCESSVIEWERDLG_H__887FCDBF_6788_45F6_9DF3_91E4D3A58D19__INCLUDED_)
#define AFX_PROCESSVIEWERDLG_H__887FCDBF_6788_45F6_9DF3_91E4D3A58D19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CProcessViewerDlg dialog

class CProcessViewerDlg : public CDialog
{
// Construction
public:
	static BOOL CALLBACK ListWindows( HWND hwnd, LPARAM lParam );
	BOOL ListWindows( HWND hwnd );
	void GetProcesses();
	void GetWindows();
	void ResetList();
	int List_Cols;
	void SetForWin();
	void SetForProc();
	HANDLE hSnapShot;
	char ProcName[256];
	char ProcId[256];
	char ProcThreads[256];
	char ProcPriority[256];
	char Drive[_MAX_DRIVE];
	char Dir[_MAX_DIR];
	char fName[_MAX_FNAME];
	char Ext[_MAX_EXT];
	int pRes;

	CProcessViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CProcessViewerDlg)
	enum { IDD = IDD_PROCESSVIEWER_DIALOG };
	CButton	m_WinShow;
	CButton	m_WinHide;
	CButton	m_WinRefresh;
	CButton	m_WinClose;
	CButton	m_ProcTerminate;
	CButton	m_Exit;
	CButton	m_ProcRefresh;
	CListCtrl	m_List;
	CTabCtrl	m_Tab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProcessViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnExit();
	afx_msg void OnClose();
	afx_msg void OnTabChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProcRefresh();
	afx_msg void OnProcTerminate();
	afx_msg void OnWindowHide();
	afx_msg void OnWindowShow();
	afx_msg void OnWindowRefresh();
	afx_msg void OnWindowClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSVIEWERDLG_H__887FCDBF_6788_45F6_9DF3_91E4D3A58D19__INCLUDED_)
