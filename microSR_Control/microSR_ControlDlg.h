
// microSR_ControlDlg.h : header file
//

#pragma once


// CmicroSRControlDlg dialog
class CmicroSRControlDlg : public CDialog
{
// Construction
public:
	CmicroSRControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MICROSR_CONTROL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


private:
	inline void loadPar(char c, int item);
	inline void loadStr(char c, int item);
	inline void storePar(char c, int item);
	inline void storeStr(char c, int item);

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	afx_msg void OnBnClickedButton5();
};
