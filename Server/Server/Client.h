/**
 * CClient封装了与被控端通信细节，
 * 为外部提供发送数据的post()接口，
 * 以及设置数据到达时的回调函数。
**/
#ifndef CLIENT_H
#define CLIENT_H

#include "BasicManager.h"

class CClient
{

	friend class CBasicManager;
	CBasicManager *m_pManager;
	SOCKET m_sock;
	bool m_bIsRunning;

public:
	HANDLE m_hWorkerThread;

	CClient();
	~CClient();

	bool Connect(char *host, int port);
	int post(char *msg, int size);
	void setManagerCallBack(CBasicManager *pManager);
	void OnReceive(char* buff, int size);
	//bool isRunning();

	static DWORD WINAPI WorkThread(LPVOID lparam);

};

#endif