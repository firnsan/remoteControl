// ShellDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ShellDlg.h"


// CShellDlg 对话框

IMPLEMENT_DYNAMIC(CShellDlg, CDialog)

CShellDlg::CShellDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShellDlg::IDD, pParent)
	, m_shelltxt(_T(""))
	, m_cmd(_T(""))
{

}

CShellDlg::~CShellDlg()
{
}

void CShellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_shelltxt);
	DDX_Text(pDX, IDC_EDIT2, m_cmd);
}


BEGIN_MESSAGE_MAP(CShellDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CShellDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CShellDlg 消息处理程序

void CShellDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString str=m_cmd+"\r\n";
	send(m_sock,str, str.GetLength()+1,0);
	m_cmd="";
	UpdateData(FALSE);
}


void CShellDlg::OnCancel()
{
	CString str("quit");
	send(m_sock,str, str.GetLength()+1,0);
	CDialog::OnCancel();
}