// ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__1DEBAAC6_037A_4AF3_AF1D_BB66A2564C97__INCLUDED_)
#define AFX_CLIENTDLG_H__1DEBAAC6_037A_4AF3_AF1D_BB66A2564C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ShellDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

class CClientDlg : public CDialog
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	CImageList m_image;
	CStatusBarCtrl m_bar;
	
// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CListCtrl	m_IocpList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnRclickIocplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShell();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_num;//主机数量
	CShellDlg *m_shellDlg;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__1DEBAAC6_037A_4AF3_AF1D_BB66A2564C97__INCLUDED_)
