
// 1120112111_CnComTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "1120112111_CnComTest.h"
#include "1120112111_CnComTestDlg.h"
#include "afxdialogex.h"
#include "VoyCmd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CnComm m_CnComm;
CVoyCmd m_cmd;
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMy1120112111_CnComTestDlg �Ի���



CMy1120112111_CnComTestDlg::CMy1120112111_CnComTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy1120112111_CnComTestDlg::IDD, pParent)
	, m_strEditTXDataInput(_T(""))
	, m_strEditRXDataDisp(_T(""))
	, changecycle(_T(""))
	, RXnum(_T(""))
	, TXnum(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bSerialPortOpened=FALSE;//��ʼ����û�д�
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


// CMy1120112111_CnComTestDlg ��Ϣ�������

BOOL CMy1120112111_CnComTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy1120112111_CnComTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy1120112111_CnComTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy1120112111_CnComTestDlg::OnBnClickedButtonOpenport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(_T("�����Ѿ��ɹ���"));
		m_CnComm.SetWnd(this->m_hWnd);
		m_bSerialPortOpened=TRUE;
	}
	else
	{
		AfxMessageBox(_T("���ڴ�ʧ��"));
		m_bSerialPortOpened=FALSE;
	}
	GetDlgItem(IDC_BUTTON_OPENPORT)->ShowWindow(!m_bSerialPortOpened); 
	GetDlgItem(IDC_BUTTON_CLOSEPORT)->ShowWindow(m_bSerialPortOpened);
	GetDlgItem(IDC_CHECKzidong)->EnableWindow(m_bSerialPortOpened);
	GetDlgItem(IDC_BUTTON_SENDDATA)->EnableWindow(m_bSerialPortOpened);
	

}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonCloseport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_CnComm.IsOpen())
	{
		m_CnComm.Close();
		m_bSerialPortOpened=false;
	}
	if(!m_CnComm.IsOpen())
	{
		AfxMessageBox(_T("���ڹرճɹ�"));
		GetDlgItem(IDC_BUTTON_OPENPORT)->ShowWindow(!m_bSerialPortOpened);
		GetDlgItem(IDC_BUTTON_CLOSEPORT)->ShowWindow(m_bSerialPortOpened);
	}
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonSenddata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_bSerialPortOpened)
	{
		AfxMessageBox(_T("���ȴ򿪴���"));
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
		straTemp = m_strEditTXDataInput; //UniCode�ַ�����ANSI�ַ���֮���ת��
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

//#1015

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char cRx[1024];  
    CString strTemp;  
 
	m_CnComm.Read(cRx,1024); 
 
    strTemp.Format(_T("%s"),cRx);  
    m_strEditRXDataDisp = m_strEditRXDataDisp + strTemp;  
 
    UpdateData(FALSE);  //��ʾ��������  
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonclearreceive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 m_strEditRXDataDisp ="";  
    UpdateData(FALSE);  //��ʾ��������  
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonclearinput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strEditTXDataInput=""; //UniCode�ַ�����ANSI�ַ���֮���ת��
	UpdateData(FALSE);
}


void CMy1120112111_CnComTestDlg::OnBnClickedCheckzidong()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	int a;
	switch (this->IsDlgButtonChecked(IDC_CHECKzidong))
	{
	case BST_CHECKED:
		CEdit* pBoxOne;
		pBoxOne = (CEdit*) GetDlgItem(IDC_EDITinputzhouqi);
		//��ֵ
		
		pBoxOne->GetWindowText(str);
		a=_ttoi(LPCTSTR(str));
		SetTimer(1,a, NULL);
		break;
	case BST_UNCHECKED:
		//AfxMessageBox("δѡ��");
		KillTimer(1);
		break;
	case BST_INDETERMINATE:
		//AfxMessageBox("�ѻһ����ɲ���");
		break;
	}
}

void CMy1120112111_CnComTestDlg::OnTimer(UINT_PTR nIDEvent)
{
  
	if(!m_bSerialPortOpened)
	{
		AfxMessageBox(_T("���ȴ򿪴���"));
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
		straTemp = m_strEditTXDataInput; //UniCode�ַ�����ANSI�ַ���֮���ת��
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
	
	//KillTimer(1);  // ɾ���˼�ʱ���������ʱ���ж������Զ����¿�ʼ��ʱ������һ���ж�ʱ���ᷢ���µ��ж�
               

   
}

void CMy1120112111_CnComTestDlg::OnBnClickedButtonjishuqingling()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		if((t==16)||(t1==16))//16 ��ӦDEL��������ͨ��
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
		return c-0x30;//0x30Ϊ0
	else if((c>='A')&&(c<='F'))
		return c-'A'+10;
	else if((c>='a')&&(c<='f'))
		return c-'a'+10;
	else 
		return 0x10;
}

void CMy1120112111_CnComTestDlg::OnBnClickedCheckstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch (this->IsDlgButtonChecked(IDC_CHECKStop))
	{
	case BST_CHECKED:
		stop = true;
		MessageBox(_T("�ر���ʾ"));
		break;
	case BST_UNCHECKED:
		stop = false;
		MessageBox(_T("����ʾ"));
		break;
	case BST_INDETERMINATE:
		//AfxMessageBox("�ѻһ����ɲ���");
		break;
	default:
		break;
	}
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonkick()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_cmd.Kick(4000);
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonturnright()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	speed[0] = -500;
	speed[1] = -500;
	speed[2] = -500;
	speed[3] = 7000;
	speed[4] = -7000;
	m_cmd.SetFiveMotorsSpeed(speed);
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonstrait()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	speed[0] = 500;
	speed[1] = -500;
	speed[2] = 0;
	m_cmd.SetFiveMotorsSpeed(speed);

}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonturnleft()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	speed[0] = 500;
	speed[1] = 500;
	speed[2] = 500;
	speed[3] = 7000;
	speed[4] = -7000;
	m_cmd.SetFiveMotorsSpeed(speed);
}


void CMy1120112111_CnComTestDlg::OnBnClickedButtonstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	speed[0] = 0;
	speed[1] = 0;
	speed[2] = 0;
	m_cmd.SetFiveMotorsSpeed(speed);
}
