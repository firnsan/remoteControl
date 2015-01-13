#include "Client.h"

CClient::CClient()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	m_bIsRunning=false;
	m_sock=INVALID_SOCKET;
}

CClient::~CClient()
{
	
}

bool CClient::Connect(char *host, int port)
{
	m_sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock==SOCKET_ERROR)
		return false;
	hostent *pHostent=NULL;
	pHostent=gethostbyname(host);
	
	sockaddr_in clientAddr;
	clientAddr.sin_family=AF_INET;
	clientAddr.sin_port=htons(port);
	clientAddr.sin_addr=*((struct in_addr*)pHostent->h_addr);
	if (connect(m_sock, (struct sockaddr*)&clientAddr, sizeof(clientAddr)))
		return false;
	m_bIsRunning = true;
	m_hWorkerThread = (HANDLE)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, (LPVOID)this, 0, NULL);

	return true;

}

int CClient::post(char *msg, int size)
{
	int val;
	if ( (val=send(m_sock,msg,size,0))==SOCKET_ERROR)
		return -1;
	return val;
}

void CClient::setManagerCallBack(CBasicManager *pManager)
{
	m_pManager = pManager;
}

DWORD WINAPI CClient::WorkThread(LPVOID lparam)   
{
	CClient *pThis = (CClient *)lparam;
	char	buff[MAX_BUFF];
	fd_set fdSocket;
	FD_ZERO(&fdSocket);
	FD_SET(pThis->m_sock, &fdSocket);
	//while (pThis->isRunning())
	while(1)
	{
		fd_set fdRead = fdSocket;
		int nRet = select(NULL, &fdRead, NULL, NULL, NULL);
		if (nRet == SOCKET_ERROR)
		{
			//pThis->Disconnect();
			break;
		}
		if (nRet > 0)
		{
			memset(buff, 0, sizeof(buff));
			int nSize = recv(pThis->m_sock, buff, sizeof(buff), 0);
			if (nSize <= 0)
			{
				//pThis->Disconnect();
				break;
			}
			if (nSize > 0)pThis->OnReceive((char*)buff, nSize);
		}
	}

	return -1;
}

void CClient::OnReceive(char *buff, int size)
{
	m_pManager->OnReceive(buff, size);
}