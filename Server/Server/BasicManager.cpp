#include "BasicManager.h"
//#include "StdAfx.h"

CBasicManager::CBasicManager(CClient* pclient)
{

	b_quit=FALSE;
	m_pClient=pclient;
	m_pClient->setManagerCallBack(this);
	m_hEvent = CreateEvent(NULL, true, false, NULL);
	
}

CBasicManager::~CBasicManager()
{
	CloseHandle(m_hEvent);
}

void CBasicManager::OnReceive(char * lpBuff,int size)
{
	
}
void CBasicManager::Post(char* strPostData, int size)
{
	m_pClient->post(strPostData, size);
}

void CBasicManager::WaitEvent()
{
	WaitForSingleObject(m_hEvent, INFINITE);
}