#pragma once


// CShellDlg 对话框

class CShellDlg : public CDialog
{
	DECLARE_DYNAMIC(CShellDlg)

public:
	CShellDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShellDlg();

// 对话框数据
	enum { IDD = IDD_SHELLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_shelltxt;
	CString m_cmd;
	int m_sock;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCancel();
};
