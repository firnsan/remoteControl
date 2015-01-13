; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Client.h"

ClassCount=2
Class1=CClientApp
Class2=CClientDlg

ResourceCount=4
Resource2=IDR_MAINFRAME
Resource3=IDD_CLIENT_DIALOG
Resource4=IDR_MENU1

[CLS:CClientApp]
Type=0
HeaderFile=Client.h
ImplementationFile=Client.cpp
Filter=N

[CLS:CClientDlg]
Type=0
HeaderFile=ClientDlg.h
ImplementationFile=ClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CClientDlg



[DLG:IDD_CLIENT_DIALOG]
Type=1
Class=CClientDlg
ControlCount=1
Control1=IDC_IOCPLIST,SysListView32,1350631425

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_WJGL
CommandCount=1

