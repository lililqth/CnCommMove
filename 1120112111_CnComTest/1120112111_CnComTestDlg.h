
// 1120112111_CnComTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include"cnComm.h"
// CMy1120112111_CnComTestDlg �Ի���
class CMy1120112111_CnComTestDlg : public CDialogEx
{
// ����
public:
	CMy1120112111_CnComTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY1120112111_CNCOMTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	short speed[5];//���ڴ洢��������ת��
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnReceive(WPARAM port, LPARAM ch);
	DECLARE_MESSAGE_MAP()
public:
	CString m_strEditTXDataInput;
	CString m_strEditRXDataDisp;
	CComboBox m_CtrlComboPortNO;
	BOOL m_bSerialPortOpened;
	afx_msg void OnBnClickedButtonOpenport();
	afx_msg void OnBnClickedButtonCloseport();
	afx_msg void OnBnClickedButtonSenddata();
	afx_msg void OnEnChangeEditRxdatadisp();
	afx_msg void OnCbnSelchangeComboPortno();
	CComboBox botevalue;
	CComboBox shujuweivalue;
	CComboBox tingzhiweivalue;
	afx_msg void OnBnClickedButtonclearreceive();
	afx_msg void OnBnClickedButtonclearinput();
	CString changecycle;
	afx_msg void OnBnClickedCheckzidong();
	void OnTimer(UINT_PTR nIDEvent);
	CString RXnum;
	CString TXnum;
	afx_msg void OnBnClickedButtonjishuqingling();
	int StrHex(CString str, char* data);
	char toHex(char c);
	afx_msg void OnBnClickedCheckstop();
	bool stop;
	afx_msg void OnBnClickedButtonkick();
	afx_msg void OnBnClickedButtonturnright();
	afx_msg void OnBnClickedButtonstrait();
	afx_msg void OnBnClickedButtonturnleft();
	afx_msg void OnBnClickedButtonstop();
};
