// IPhy.h: interface for the IPhy class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>

#if !defined(AFX_IPHY_H__EE441FE2_5225_4F87_B168_6312E1427795__INCLUDED_)
#define AFX_IPHY_H__EE441FE2_5225_4F87_B168_6312E1427795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

class CVoyCmd;

class IPhy
{
public:
	IPhy(){IsRuning = FALSE;};
	virtual ~IPhy(){};
	virtual void SetCmd(CVoyCmd * pCmd){};							//����ָ�������
	virtual void Send(const void *pBuffer, const int iLength){};	//����ָ�������������ã�
	virtual void SigWrite(){};
	BOOL IsRuning;													//����״̬���
	BOOL bSending;													//���ͱ��
};

#endif // !defined(AFX_IPHY_H__EE441FE2_5225_4F87_B168_6312E1427795__INCLUDED_)
