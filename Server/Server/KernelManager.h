

#ifndef KERNELMANAGER_H
#define KERNELMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicManager.h"

class CKernelManager : public CBasicManager  
{
public:
	BOOL b_signed;//��ʾ�Լ��Ƿ��Ѿ�����

	HANDLE	m_hThread[32]; // �㹻����
	UINT	m_nThreadCount;

	CKernelManager(CClient *pClient);
	virtual ~CKernelManager();
	virtual void OnReceive(char *lpBuff,int size);

private:
	
	
	
};

#endif 
