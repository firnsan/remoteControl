// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, m_num(0)
	, m_shellDlg(NULL)
{
	//{{AFX_DATA_INIT(CClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_IOCPLIST, m_IocpList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_NOTIFY(NM_RCLICK, IDC_IOCPLIST, OnRclickIocplist)
	ON_WM_TIMER()
	ON_COMMAND(ID_Shell, OnShell)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// ������
	LONG lstyle = m_IocpList.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE); //��ȡlistctrl����ʽ
	lstyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_IocpList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)lstyle); //����listctrl��ʽ������

	m_IocpList.InsertColumn(0,"id",LVCFMT_LEFT,50);
	m_IocpList.InsertColumn(1,"���ض˰汾",LVCFMT_LEFT,110);
	m_IocpList.InsertColumn(2,"���������",LVCFMT_LEFT,110);
	m_IocpList.InsertColumn(3,"IP��ַ",LVCFMT_LEFT,110);		
	m_IocpList.InsertColumn(4,"����ϵͳ",LVCFMT_LEFT,110);
	m_IocpList.InsertColumn(5,"����λ��",LVCFMT_LEFT,110);

	//----------------------------״̬��-------------------------------------------
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW,CRect(0,0,0,0),this,1024); // ״̬���Ĵ���
	int strPart[3] = {200,600,-1};
	m_bar.SetParts(3,strPart);
	//m_bar.SetText("��������:0̨",2,0);
	//m_bar.SetText("���ݿ����",1,0);
	
	AfxBeginThread((AFX_THREADPROC)CEIocpinitialization,this);                //Afxbeginthread�����µ��߳�ִ��
	// TODO: Add extra initialization here
	//SetTimer(0,500,NULL);                                                     //SetTimer 500�������һ��Ontimer
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



int getValue(char *str, const char *key, char *value, int size)
{
	memset(value, 0, size);
	char *newstr=strstr(str,key);
	if (newstr==NULL)
		return -1;
	char *value_start=newstr+strlen(key)+1; //id=XXX&name=XXX
	char *value_end=strchr(value_start,'&');
	if (value_end==NULL) //���һ��key
		strcpy(value, value_start);
	else 
		memcpy(value, value_start, (int)(value_end-value_start));

	return 0;
	
}



/*****************************************************************************/
/*  �Զ��庯��CEIocpinitialization                                           */
/*/ �����ʼ��SOCKET��������ɶ˿ڣ�����SOCKET����ʾ������Ϣ                 */
/*/                                                                          */
/*****************************************************************************/
SOCKADDR_IN      saRemote;                                                        //�����õ�
SOCKET           Accept;                                                          //������
int              RemoteLen;                                                       //��ʼ����
DWORD WINAPI CEIocpinitialization(void *m_dlg)
{
	CClientDlg *dlg = (CClientDlg *)m_dlg;
	


    HANDLE       CEm_IocpPort;                                                    //��ɶ˿ڱ���
	WSADATA      CEm_Wsd;                                                         // SOCKET��
	SYSTEM_INFO  systeminfo;                                                      //  
	SOCKADDR_IN  CEm_listenDdr;                                                   // SOCKET
    SOCKET       CEm_listenSck;                                                   //�����õ�SOCKET
	int          CEm_Port=2015;                                                   //�˿ڱ���
	CString      initialization,initialization1,initialization2;                  //��ʽ�������
	WORD ver =MAKEWORD(2,2);                                                      //��ʼ��

	if(WSAStartup(ver,&CEm_Wsd) !=0 )                                             //��ʼ��SOCKET��
	{	
		initialization="��ʼ��SOCKET�����!";
		dlg->m_bar.SetText(initialization,0,0);
		return 0;
	}

	CEm_IocpPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);            //������ɶ˿�

	GetSystemInfo(&systeminfo);                                                    //ȷ������CPU

	for (int i=0; i < (int)systeminfo.dwNumberOfProcessors *2 + 2;i++)             //�����ǰ�̲߳���CPU*2+2�������̣߳�
	{
		HANDLE GongZuoXianCheng;
	
		GongZuoXianCheng=CreateThread(NULL,0,CEIocpWorkThread,CEm_IocpPort,0,NULL);//�����������̺߳���
	
		CloseHandle(GongZuoXianCheng); // �ر�
	}

     CEm_listenSck=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);    //���������׽���
	 
	 if (CEm_listenSck == INVALID_SOCKET)
	 {
	
		 return 0;
	 }
	 CEm_listenDdr.sin_family=AF_INET;
	 CEm_listenDdr.sin_port=htons(CEm_Port);
	 CEm_listenDdr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	 
	 int CEm_bind,CEm_Listen;

	 CEm_bind=bind(CEm_listenSck,(SOCKADDR*)&CEm_listenDdr,sizeof(CEm_listenDdr)); //�󶨼����׽���
	 if (CEm_bind == SOCKET_ERROR)
	 {
	
		 initialization1.Format(_T("%d:��ʧ��!"),CEm_Port);
		 dlg->m_bar.SetText(initialization1,0,0);
		 return 0;
	
	 }

	 CEm_Listen=listen(CEm_listenSck,5);                                             //����
	 if (CEm_Listen == SOCKET_ERROR)
	 {
		 initialization2.Format(_T("%d:�˿ڼ���ʧ��!"),CEm_Port);
		 dlg->m_bar.SetText(initialization2,0,0);
		 return 0;
	 }
	 else
	 {	
		 initialization2.Format(_T("     ���ڼ��� 0.0.0.0:%d"),CEm_Port);
	     dlg->m_bar.SetText(initialization2,0,0);
	 }
	

	 while(TRUE)
	 {
            PER_HANDLE_DATA  *perhandledata =NULL;                                              //�ṹ��=��
          

			RemoteLen=sizeof(saRemote);                                                         //�õ��˰ɣ�
		
			Accept=accept(CEm_listenSck,(SOCKADDR*)&saRemote,&RemoteLen);                       //��������
		
			perhandledata=(LPPER_HANDLE_DATA)GlobalAlloc(GPTR,sizeof(PER_HANDLE_DATA));
         
			perhandledata->m_connectionSck=Accept;                                               //�ṹ���е�SOCKET��ACCEPT����
			memcpy(&(perhandledata->m_connectionDdr), &saRemote,RemoteLen);                      //��������
			
			CreateIoCompletionPort((HANDLE)Accept,CEm_IocpPort,(DWORD)perhandledata,0);          //�׽��ֺ���ɶ˿ڹ���
	
	
			DWORD  RecvBytes = 0;                                                          
			DWORD  Flags     = 0;

			LPPER_IO_DATA periodata = NULL;

			periodata =(LPPER_IO_DATA)GlobalAlloc(GPTR,sizeof(PER_IO_DATA));

			ZeroMemory(&(periodata->Overlapped),sizeof(OVERLAPPED));

			periodata->DataBuf.len =296;
			periodata->DataBuf.buf =periodata->buffer;
			periodata->OperationType =0;
			
			WSARecv(perhandledata->m_connectionSck,&(periodata->DataBuf),1,&(periodata->DataBuf.len),&Flags,&(periodata->Overlapped),NULL); //Ͷ���첽��������
			
		
	 }
	
	return 0;  

}


/*****************************************************************************/
/*  �Զ��庯��CEIocpWorkThread                                               */
/*/ ���������̣߳�IOCP��д����                                             */
/*/                                                                          */
/*****************************************************************************/
DWORD WINAPI CEIocpWorkThread(LPVOID lpgram)
{  

	HANDLE WorkIocpPort =(HANDLE)lpgram;                                          //��ɶ˿�=����lpgram
   	CClientDlg *dlg=(CClientDlg*)AfxGetApp()->m_pMainWnd;
	DWORD BytesTransferred;                                                          //˫�ֽڱ���
    LPOVERLAPPED lpOverlapped;                                                       //�ṹ��
    LPPER_HANDLE_DATA PerHandleData = NULL;                                          //�ṹ�ȵ��ڿ�
    LPPER_IO_DATA PerIoData = NULL;                                                  //��

    DWORD RecvBytes;                                                                                            
    DWORD Flags;
    BOOL bRet = FALSE;                                                               //�����ͱ���=��
                                               //������
	while(TRUE)                                                                      //��ѭ��
	{
		
		bRet = GetQueuedCompletionStatus(WorkIocpPort,&BytesTransferred,(LPDWORD)&PerHandleData,(LPOVERLAPPED*)&lpOverlapped,INFINITE);
		
	
		PerIoData =(LPPER_IO_DATA)CONTAINING_RECORD(lpOverlapped,PER_IO_DATA,Overlapped);
		if (0 == BytesTransferred)
        {
	
            closesocket(PerHandleData->m_connectionSck);
            GlobalFree(PerHandleData);
            GlobalFree(PerIoData);
			
            continue;
        }
		
        Flags = 0;
		//...
		//Ӧ����������Ӵ��룬�յ����ض˷��������ݺ����Э��ͷ�ĸ�ʽ�����жϱ��ض˷��͹�������ʲô����
	
		char value[128];
		getValue(PerIoData->DataBuf.buf, "cmd", value, 128);
		int cmd=atoi(value);

		if(PerHandleData->m_connectionSck!= SOCKET_ERROR )
		{			
			switch(cmd)
			{
				case COMMAND_SIGN:	
					AddHost(PerIoData->DataBuf.buf, PerHandleData->m_connectionSck);

					char PostData[1024];
					sprintf(PostData,"cmd=%03d&id=%03d",COMMAND_SIGN,dlg->m_num);
					send(PerHandleData->m_connectionSck, PostData, strlen(PostData)+1, 0);
					break;
				case COMMAND_SHELL:
					UpdateShellDlg(PerIoData->DataBuf.buf, PerHandleData->m_connectionSck);
					break;
			}

		}
        // Ϊ��һ���ص����ý�����I/O��������
        ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->DataBuf.len = 296;
        PerIoData->DataBuf.buf = PerIoData->buffer;
        PerIoData->OperationType = 0; // read
	
        WSARecv(PerHandleData->m_connectionSck,&(PerIoData->DataBuf),1,&RecvBytes,&Flags,&(PerIoData->Overlapped),NULL);
		
	
	}
	return 0;
}

/*****************************************************************************/
/*  �Զ��庯��AddHost                                                        */
/*/ ����������Ϣ���������Է���˵�SOCKET��IP��ַ��Ϣ                         */
/*/                                                                          */
/*****************************************************************************/
void AddHost(char *data, int sock)
{
	char name[128],ip[128], system[128];
	getValue(data, "name", name, 128);
	getValue(data, "ip", ip, 128);
	getValue(data, "system", system, 128);

	CClientDlg *dlg=(CClientDlg*)AfxGetApp()->m_pMainWnd;
	int n=dlg->m_IocpList.GetItemCount();
	char buf[4];
	_itoa(n+1, buf, 10);
	dlg->m_IocpList.InsertItem(n,buf);
	dlg->m_IocpList.SetItemText(n,1,"1.00"); 
	dlg->m_IocpList.SetItemText(n,2,name);    
	dlg->m_IocpList.SetItemText(n,3,ip);	
	dlg->m_IocpList.SetItemText(n,4,system); 
	dlg->m_IocpList.SetItemData(n,sock);
	dlg->m_num=n+1;
                                                
}


void UpdateShellDlg(char *data, int sock){
	
	char output[MAX_BUFF];
	getValue(data, "output", output, MAX_BUFF);
	int len=strlen(output);
	int len2=strlen(data);
	CClientDlg *dlg = (CClientDlg*)AfxGetApp()->m_pMainWnd;
	CShellDlg *shellDlg=dlg->m_shellDlg;
	if (!dlg->m_shellDlg)
		return;
	CString csOutput(output);
	csOutput.Replace("\n", "\r\n");
	shellDlg->m_shelltxt+=csOutput;
	shellDlg->GetDlgItem(IDC_EDIT1)->SetWindowText(shellDlg->m_shelltxt);
	shellDlg->m_sock=sock;

}

/*****************************************************************************/
/*  �Զ��庯��CheckOnlineProc                                                */
/*/ ����̬��ʾ�����������Լ�LIST��̬���ɾ������                           */
/*/                                                                          */
/*****************************************************************************/
DWORD WINAPI CheckOnlineProc(void *m_dlg)
{
	CClientDlg *dlg = (CClientDlg *)m_dlg;                                          //������ָ��
	int nCount = dlg->m_IocpList.GetItemCount();                                    //����NCOUNT���ڻ��IOCPLIST��ITEM
	
	for(int i=0;i<nCount;i++)                                                       //���IС��ITEM��I+1
	{
		SOCKET ss = dlg->m_IocpList.GetItemData(i);                                 //SOCKET���ݵ���IOCPLIST������
		int nRet = send(ss,"A",1,0);                                                //NRET���ڷ���SOUKET��A��1.0
		if(nRet == SOCKET_ERROR)                                                    //SOCKET���ڴ���                                          
		{
			if(WSAGetLastError() == WSAEWOULDBLOCK)                                 //
			{
				Sleep(10);                                                          //10����
				continue;                                                           //ǿ��ִ������Ĵ���
			}
			dlg->m_IocpList.DeleteItem(i);                                          //ɾ��ITEM��I������
			nCount--;                                                               //��̬���ݼ�1
			i--;                                                                    //��1
		}
		CString str;                                                                //������Ǵ���ִ��
		str.Format("��������:%d̨",nCount);                                         //��ʽ�����NCOUNT��̬����
        dlg->m_bar.SetText(str,2,0);		                                        //�ŵ�BAR״̬���ڶ���
	}
	return 0;
}


/*****************************************************************************/
/*  ϵͳ���� OnRclickIocplist  RCLICK�Ҽ�ѡ��                              */
/*/ ��IOCPLIST�ؼ���ѡ�񵯳��Ҽ��˵�                                         */
/*/                                                                          */
/*****************************************************************************/
void CClientDlg::OnRclickIocplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu CaiDan;                                                                     //�˵���
	CaiDan.LoadMenu(IDR_MENU1);                                                       //�ò˵�����ز˵�ID
	CMenu *ZhiZhen =CaiDan.GetSubMenu(0);                                             //�˵�ָ���ò˵���
	POINT ptmenus;                                                                    //����
	GetCursorPos(&ptmenus);                                                           //����������
	ZhiZhen->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,ptmenus.x,ptmenus.y,this);  //�˵�������ص㵯��

	*pResult = 0;
}


/*****************************************************************************/
/*  ϵͳ���� OnTimer  ͨ�� SetTimer ���á�                                   */
/*/ ����̬����CheckOnlineProc������������Ƿ�������������                  */
/*/                                                                          */
/*****************************************************************************/
void CClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//AfxBeginThread((AFX_THREADPROC)CheckOnlineProc,this);                           	//��������Ƿ�����
	CDialog::OnTimer(nIDEvent);
}


void CClientDlg::OnShell()
{

	int i;
	int n=m_IocpList.GetItemCount();
	for (i=0;i<n;i++) {
		if (m_IocpList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			break;
	}
	if (i==n)
		return;

	int sock=m_IocpList.GetItemData(i);
	char PostData[1024];
	sprintf(PostData,"cmd=%03d",COMMAND_SHELL);
	send(sock, PostData, strlen(PostData)+1, 0);
	
	m_shellDlg= new CShellDlg;
	m_shellDlg->DoModal();


}


