//����WINSOCKEͷ�ļ�������StdAfx.hͷ�ļ��������ͷ�ļ���System.h
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
/*  �Զ���IOCP�ṹ�壬                                                       */
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
/*  �Զ�������ṹ�壬                                                       */
/*/  ���ͺͽ������Է���˵�����                                              */
/*****************************************************************************/

typedef struct					//�ļ��ṹ
{
	char		FileName[MAX_PATH];
	int			FileLen;
	char		Time[50];
	BOOL		IsDir;
	BOOL		Error;
	HICON		hIcon;			//ͼ����
	
}FILEINFO;
/************************************************************************/           
/*                                  
/*  ������ϢΪ�Զ�����ֺ�����                      
/*                                  
/************************************************************************/  
DWORD WINAPI CEIocpWorkThread(LPVOID lpgram);                            //�������߳�
DWORD WINAPI CEIocpinitialization(void *m_dlg);                          //��ʼ��
DWORD WINAPI CheckOnlineProc(void *m_dlg);                               //��̬����
void AddHost(char *data, int sock);
void UpdateShellDlg(char *data, int sock);
/************************************************************************/           
/*                                  
/*  ������ϢΪ�Զ�����ֱ�����                      
/*                                  
/************************************************************************/  


/////////////////////////////////////////////////////////////////////////////