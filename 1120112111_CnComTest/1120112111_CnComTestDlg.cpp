
// 1120112111_CnComTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "1120112111_CnComTest.h"
#include "1120112111_CnComTestDlg.h"
#include "afxdialogex.h"
#include "VoyCmd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CnComm m_CnComm;
CVoyCmd m_cmd;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy1120112111_CnComTestDlg 对话框



CMy1120112111_CnComTestDlg::CMy1120112111_CnComTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy1120112111_CnComTestDlg::IDD, pParent)
	, m_strEditTXDataInput(_T(""))
	, m_strEditRXDataDisp(_T(""))
	, changecycle(_T(""))
	, RXnum(_T(""))
	, TXnum(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bSerialPortOpened=FALSE;//初始串口没有打开
}

void CMy1120112111_CnComTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXDATAINPUT, m_strEditTXDataInput);
	DDX_Text(pDX, IDC_EDIT_RXDATADISP, m_strEditRXDataDisp);
	DDX_Control(pDX, IDC_COMBO_PORTNO, m_CtrlComboPortNO);
	DDX_Control(pDX, IDC_COMBObote, botevalue);
	DDX_Control(pDX, IDC_COMBOshujuwei, shujuweivalue);
	DDX_Control(pDX, IDC_COMBOtingzhiwei, tingzhiweivalue);
	DDX_Text(pDX, IDC_EDITinputzhouqi, changecycle);
	DDX_Text(pDX, IDC_EDITRX, RXnum);
	DDX_Text(pDX, IDC_EDITTX, TXnum);
}

BEGIN_MESSAGE_MAP(CMy1120112111_CnComTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, &CMy1120112111_CnComTestDlg::OnBnClickedButtonOpenport)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEPORT, &CMy1120112111_CnComTestDlg::OnBnClickedButtonCloseport)
	ON_BN_CLICKED(IDC_BUTTON_SENDDATA, &CMy1120112111_CnComTestDlg::OnBnClickedButtonSenddata)
	ON_EN_CHANGE(IDC_EDIT_RXDATADISP, &CMy1120112111_CnComTestDlg::OnEnChangeEditRxdatadisp)
	ON_MESSAGE(ON_COM_RECEIVE, OnReceive)
	ON_CBN_SELCHANGE(IDC_COMBO_PORTNO, &CMy1120112111_CnComTestDlg::OnCbnSelchangeComboPortno)
	ON_BN_CLICKED(IDC_BUTTONclearreceive, &CMy1120112111_CnComTestDlg::OnBnClickedButtonclearreceive)
	ON_BN_CLICKED(IDC_BUTTONclearinput, &CMy1120112111_CnComTestDlg::OnBnClickedButtonclearinput)
	ON_BN_CLICKED(IDC_CHECKzidong, &CMy1120112111_CnComTestDlg::OnBnClickedCheckzidong)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTONjishuqingling, &CMy1120112111_CnComTestDlg::OnBnClickedButtonjishuqingling)
	
	ON_BN_CLICKED(IDC_CHECKStop, &CMy1120112111_CnComTestDlg::OnBnClickedCheckstop)
	ON_BN_CLICKED(IDC_BUTTONKick, &CMy1120112111_CnComTestDlg::OnBnClickedButtonkick)
	ON_BN_CLICKED(IDC_BUTTONTurnRight, &CMy1120112111_CnComTestDlg::OnBnClickedButtonturnright)
	ON_BN_CLICKED(IDC_BUTTONStrait, &CMy1120112111_CnComTestDlg::OnBnClickedButtonstrait)
	ON_BN_CLICKED(IDC_BUTTONTurnLeft, &CMy1120112111_CnComTestDlg::OnBnClickedButtonturnleft)
	ON_BN_CLICKED(IDC_BUTTONStop, &CMy1120112111_CnComTestDlg::OnBnClickedButtonstop)
END_MESSAGE_MAP()


// CMy1120112111_CnComTestDlg 消息处理程序

BOOL CMy1120112111_CnComTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_CtrlComboPortNO.SetCurSel(0);
	botevalue.SetCurSel(0);
	shujuweivalue.SetCurSel(0);
	tingzhiweivalue.SetCurSel(0);
	

	GetDlgItem(IDC_BUTTON_OPENPORT)->ShowWindow(!m_bSerialPortOpened); 
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->ShowWindow(m_bSerialPortOpened);
	GetDlgItem(IDC_CHECKzidong)->EnableWindow(m_bSerialPortOpened);
	GetDlgItem(IDC_BUTTON_SENDDATA)->EnableWindow(m_bSerialPortOpened);
	
	RXnum="0";
	TXnum="0";
	changecycle="1000";
	stop = false;
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy1120112111_CnComTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy1120112111_CnComTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy1120112111_CnComTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy1120112111_CnComTestDlg::OnBnClickedButtonOpenport()
{
	// TODO: 在此添加控件通知处理程序代码
	/*UpdateData(TRUE);
	if(m_Com.IsOpen)
	{
		m_Com.Close();
	}
	//m_Com.Open(m_CtrlComboPortNO.GetCurSel()+1,"9600,8,n,1");
	m_Com.Open();*/
	int nPortNo = m_CtrlComboPortNO.GetCurSel()+1;
	/*CString  botevalue1;
	CString  shujuweivalue1;
	CString  tingzhiweivalue1;
	botevalue.GetLBText( botevalue.GetCurSel(),botevalue1);
	shujuweivalue.GetLBText(shujuweivalue.GetCurSel(),shujuweivalue1);
	tingzhiweivalue.GetLBText(tingzhiweivalue.GetCurSel(),tingzhiweivalue1);
	int a=_tstoi(LPCTSTR(botevalue1));
	int b=_tstoi( LPCTSTR(shujuweivalue1));
	int c=_tstoi(LPCTSTR(tingzhiweivalue1));*/
	m_CnComm.Open(nPortNo);
	m_CnComm.SetState(2000000);
	if(m_CnComm.IsOpen())
	{
		AfxMessageBox(_T("串口已经成功打开"));
		m_CnComm.SetWnd(this->m_hWnd);
		m_bSerialPortOpened=TRUE;
	}
	else
	{
		AfxMessageBox(_T("串口打开失败"));
		m_bSerialPortOpened=FALSE;
	}
	GetDlgItem(IDC_BUTTON_OPENPORT)->ShowWindow(!m_bSerialPortOpened); 
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->ShowWindow(m_bSerialPortOpened);
	GetDlgItem(IDC_CHECKzidong)->EnableWindow(m_bSerialPortOpened);
	GetDlgItem(IDC_BUTTON_SENDDATA)->EnableWindow(m_bSerialPortOpened);
	

}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonCloseport()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_CnComm.IsOpen())
	{
		m_CnComm.Close();
		m_bSerialPortOpened=false;
	}
	if(!m_CnComm.IsOpen())
	{
		AfxMessageBox(_T("串口关闭成功"));
		GetDlgItem(IDC_BUTTON_OPENPORT)->ShowWindow(!m_bSerialPortOpened);
		GetDlgItem(IDC_BUTTON_CLOSEPORT)->ShowWindow(m_bSerialPortOpened);
	}
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonSenddata()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_bSerialPortOpened)
	{
		AfxMessageBox(_T("请先打开串口"));
		return;
	}
	UpdateData(TRUE);
	//AfxMessageBox(m_strEditTXDataInput);
	if(this->IsDlgButtonChecked(IDC_CHECKHEX)==BST_CHECKED)
	{
		char data[512];
		int len=StrHex(m_strEditTXDataInput,data);
		m_CnComm.Write(data,len);
	}
	else
	{
		CStringA straTemp;
		straTemp = m_strEditTXDataInput; //UniCode字符串与ANSI字符串之间的转换
		m_CnComm.Write(straTemp);
	}
	
	CString str;
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDITTX);
	pBoxOne->GetWindowText(str);
	int a=_ttoi(LPCTSTR(str));
	a++;
	TXnum.Format(_T("%d"), a);
	UpdateData(FALSE);

}


void CMy1120112111_CnComTestDlg::OnEnChangeEditRxdatadisp()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

//#1015

	// TODO:  在此添加控件通知处理程序代码
	char cRx[1024];  
    CString strTemp;  
 
	m_CnComm.Read(cRx,1024); 
 
    strTemp.Format(_T("%s"),cRx);  
    m_strEditRXDataDisp = m_strEditRXDataDisp + strTemp;  
 
    UpdateData(FALSE);  //显示接收内容  
} 
LRESULT CMy1120112111_CnComTestDlg::OnReceive(WPARAM port, LPARAM ch)
{
	char str[100];
	m_CnComm.ReadString(str, 100);
	if(!stop)
	{
		m_strEditRXDataDisp += str;
		CString str1;
		CEdit* pBoxOne;
		pBoxOne = (CEdit*)GetDlgItem(IDC_EDITRX);
		pBoxOne->GetWindowText(str1);
		int a=_ttoi(LPCTSTR(str1));
		a++;
		RXnum.Format(_T("%d"), a);
		UpdateData(FALSE);
	}
	return 0;
}



void CMy1120112111_CnComTestDlg::OnCbnSelchangeComboPortno()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonclearreceive()
{
	// TODO: 在此添加控件通知处理程序代码
	 m_strEditRXDataDisp ="";  
    UpdateData(FALSE);  //显示接收内容  
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonclearinput()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strEditTXDataInput=""; //UniCode字符串与ANSI字符串之间的转换
	UpdateData(FALSE);
}


void CMy1120112111_CnComTestDlg::OnBnClickedCheckzidong()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int a;
	switch (this->IsDlgButtonChecked(IDC_CHECKzidong))
	{
	case BST_CHECKED:
		CEdit* pBoxOne;
		pBoxOne = (CEdit*) GetDlgItem(IDC_EDITinputzhouqi);
		//付值
		
		pBoxOne->GetWindowText(str);
		a=_ttoi(LPCTSTR(str));
		SetTimer(1,a, NULL);
		break;
	case BST_UNCHECKED:
		//AfxMessageBox("未选中");
		KillTimer(1);
		break;
	case BST_INDETERMINATE:
		//AfxMessageBox("已灰化不可操作");
		break;
	}
}

void CMy1120112111_CnComTestDlg::OnTimer(UINT_PTR nIDEvent)
{
  
	if(!m_bSerialPortOpened)
	{
		AfxMessageBox(_T("请先打开串口"));
		return;
	}
	UpdateData(TRUE);
	//AfxMessageBox(m_strEditTXDataInput);
	if(this->IsDlgButtonChecked(IDC_CHECKHEX)==BST_CHECKED)
	{
		char data[512];
		int len=StrHex(m_strEditTXDataInput,data);
		m_CnComm.Write(data,len);
	}
	else
	{
		CStringA straTemp;
		straTemp = m_strEditTXDataInput; //UniCode字符串与ANSI字符串之间的转换
		m_CnComm.Write(straTemp);
	}
	CString str;
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDITTX);
	pBoxOne->GetWindowText(str);
	int a=_ttoi(LPCTSTR(str));
	a++;
	TXnum.Format(_T("%d"), a);
	UpdateData(FALSE);
	
	//KillTimer(1);  // 删除此计时器，否则计时器中断完后会自动重新开始计时，到下一次中断时还会发生新的中断
               

   
}

void CMy1120112111_CnComTestDlg::OnBnClickedButtonjishuqingling()
{
	// TODO: 在此添加控件通知处理程序代码
	TXnum="0";
	RXnum="0";
	UpdateData(FALSE);
}



int CMy1120112111_CnComTestDlg::StrHex(CString str, char* data)
{
	int t,t1;
	int rlen=0,len=str.GetLength();
	//data.SetSize(len/2);
	for(int i=0;i<len;)
	{
		char l,h=str[i];
		if(h==' ')
		{
			i++;
			continue;
		}
		i++;
		if(i>=len)
			break;
		l=str[i];
		t=toHex(h);
		t1=toHex(l);
		if((t==16)||(t1==16))//16 对应DEL跳出数据通信
			break;
		else 
			t=t*16+t1;
		i++;
		data[rlen]=(char)t;
		rlen++;
	}
	return rlen;

}

char CMy1120112111_CnComTestDlg::toHex(char c)
{
	if((c>='0')&&(c<='9'))
		return c-0x30;//0x30为0
	else if((c>='A')&&(c<='F'))
		return c-'A'+10;
	else if((c>='a')&&(c<='f'))
		return c-'a'+10;
	else 
		return 0x10;
}

void CMy1120112111_CnComTestDlg::OnBnClickedCheckstop()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (this->IsDlgButtonChecked(IDC_CHECKStop))
	{
	case BST_CHECKED:
		stop = true;
		MessageBox(_T("关闭显示"));
		break;
	case BST_UNCHECKED:
		stop = false;
		MessageBox(_T("打开显示"));
		break;
	case BST_INDETERMINATE:
		//AfxMessageBox("已灰化不可操作");
		break;
	default:
		break;
	}
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonkick()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cmd.Kick(4000);
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonturnright()
{
	// TODO: 在此添加控件通知处理程序代码
	speed[0] = -500;
	speed[1] = -500;
	speed[2] = -500;
	speed[3] = 7000;
	speed[4] = -7000;
	m_cmd.SetFiveMotorsSpeed(speed);
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonstrait()
{
	// TODO: 在此添加控件通知处理程序代码
	speed[0] = 500;
	speed[1] = -500;
	speed[2] = 0;
	m_cmd.SetFiveMotorsSpeed(speed);

}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonturnleft()
{
	// TODO: 在此添加控件通知处理程序代码
	speed[0] = 500;
	speed[1] = 500;
	speed[2] = 500;
	speed[3] = 7000;
	speed[4] = -7000;
	m_cmd.SetFiveMotorsSpeed(speed);
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonstop()
{
	// TODO: 在此添加控件通知处理程序代码
	speed[0] = 0;
	speed[1] = 0;
	speed[2] = 0;
	m_cmd.SetFiveMotorsSpeed(speed);
}
