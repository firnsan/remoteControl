#pragma once


// CShellDlg �Ի���

class CShellDlg : public CDialog
{
	DECLARE_DYNAMIC(CShellDlg)

public:
	CShellDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShellDlg();

// �Ի�������
	enum { IDD = IDD_SHELLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_shelltxt;
	CString m_cmd;
	int m_sock;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCancel();
};
