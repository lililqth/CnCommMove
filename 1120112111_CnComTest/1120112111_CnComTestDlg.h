
// 1120112111_CnComTestDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include"cnComm.h"
// CMy1120112111_CnComTestDlg 对话框
class CMy1120112111_CnComTestDlg : public CDialogEx
{
// 构造
public:
	CMy1120112111_CnComTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY1120112111_CNCOMTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	short speed[5];//用于存储五个电机的转速
	HICON m_hIcon;

	// 生成的消息映射函数
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
