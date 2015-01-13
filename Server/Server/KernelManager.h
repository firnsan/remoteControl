

#ifndef KERNELMANAGER_H
#define KERNELMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicManager.h"

class CKernelManager : public CBasicManager  
{
public:
	BOOL b_signed;//表示自己是否已经上线

	HANDLE	m_hThread[32]; // 足够用了
	UINT	m_nThreadCount;

	CKernelManager(CClient *pClient);
	virtual ~CKernelManager();
	virtual void OnReceive(char *lpBuff,int size);

private:
	
	
	
};

#endif 
