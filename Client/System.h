//包含WINSOCKE头文件，并在StdAfx.h头文件里包含本头文件，System.h
#include "winsock2.h"
#pragma comment (lib,"ws2_32.lib")

#define BUF_LEN 296

#define MAX_BUFF 81920

enum
{
	COMMAND_SIGN = 0x01,
	COMMAND_SHELL,
};




typedef struct PER_HANDLE_DATA     
{
	SOCKET m_connectionSck;
	SOCKADDR_IN m_connectionDdr;
}PER_HANDLE_DAT,*LPPER_HANDLE_DATA;

/*****************************************************************************/
/*  自定义IOCP结构体，                                                       */
/*/                                                                          */
/*****************************************************************************/
typedef struct PER_IO_DATA          
{
	OVERLAPPED    Overlapped;
	WSABUF        DataBuf;
	char          buffer[MAX_BUFF];
	int           BufferLen;
	int           OperationType;
}PER_IO_DATA,*LPPER_IO_DATA;

/*****************************************************************************/
/*  自定义命令结构体，                                                       */
/*/  发送和接收来自服务端的命令                                              */
/*****************************************************************************/

typedef struct					//文件结构
{
	char		FileName[MAX_PATH];
	int			FileLen;
	char		Time[50];
	BOOL		IsDir;
	BOOL		Error;
	HICON		hIcon;			//图标句柄
	
}FILEINFO;
/************************************************************************/           
/*                                  
/*  以下信息为自定义各种函数。                      
/*                                  
/************************************************************************/  
DWORD WINAPI CEIocpWorkThread(LPVOID lpgram);                            //工作者线程
DWORD WINAPI CEIocpinitialization(void *m_dlg);                          //初始化
DWORD WINAPI CheckOnlineProc(void *m_dlg);                               //动态计数
void AddHost(char *data, int sock);
void UpdateShellDlg(char *data, int sock);
/************************************************************************/           
/*                                  
/*  以下信息为自定义各种变量。                      
/*                                  
/************************************************************************/  


/////////////////////////////////////////////////////////////////////////////