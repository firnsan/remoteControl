/**
 * CClient��װ���뱻�ض�ͨ��ϸ�ڣ�
 * Ϊ�ⲿ�ṩ�������ݵ�post()�ӿڣ�
 * �Լ��������ݵ���ʱ�Ļص�������
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