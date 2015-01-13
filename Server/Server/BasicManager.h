/**
 * CBasicManager��CKernelManger��CShellManager�Ļ��࣬
 * ��CClient����Ϲ�ϵ��ͨ��CClient����ʵ��ͨ�ţ�
 * ����ͨ�����ǻ���ĺ�����չ���ܡ�
**/
#ifndef BASICMANAGER_H
#define BASICMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "Client.h"
#include "map.h"
#ifdef _CONSOLE
#include <stdio.h>
#endif

class CBasicManager 
{
	friend class CClient;
	
public:
	HANDLE m_hEvent;
	int m_id;//�Լ���id
	BOOL b_quit;//��ʾ�Ƿ�����Լ�
	CClient	*m_pClient;
	void Post(char* strPostData, int size);
	CBasicManager(CClient*);
	virtual ~CBasicManager();
	virtual void OnReceive(char*,int);
	void WaitEvent();

};

#endif