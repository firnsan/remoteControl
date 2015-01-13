/**
 * CBasicManager是CKernelManger、CShellManager的基类，
 * 与CClient是组合关系，通过CClient对象实现通信，
 * 子类通过覆盖基类的函数扩展功能。
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
	int m_id;//自己的id
	BOOL b_quit;//表示是否结束自己
	CClient	*m_pClient;
	void Post(char* strPostData, int size);
	CBasicManager(CClient*);
	virtual ~CBasicManager();
	virtual void OnReceive(char*,int);
	void WaitEvent();

};

#endif