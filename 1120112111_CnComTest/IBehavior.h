// IBehavior.h: interface for the IBehavior class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBEHAVIOR_H__B5768320_B81E_4929_98DB_11CB134E7E1F__INCLUDED_)
#define AFX_IBEHAVIOR_H__B5768320_B81E_4929_98DB_11CB134E7E1F__INCLUDED_

#include <afx.h>
#include "VoyCmd.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVoyCmd;

class IBehavior  
{
public:
	IBehavior(){m_pCmd = NULL;};
	virtual ~IBehavior(){};
	void SetCmd(CVoyCmd * pCmd){m_pCmd = pCmd;};																//����ָ�������	
	virtual void AfterUpdateInfrared(UCHAR *Infrared,BOOL *EnableIR,UINT nState){};								//���⴫������Ϣ���º�Ĵ�����
	virtual void AfterUpdateUSonic(DOUBLE *Ultrasonic,BOOL *EnableUS,UINT nState){};							//������������Ϣ���º�Ĵ�����
	virtual void AfterUpdateVideoSample(BYTE * pBuffer, long lWidth,long lHeight,double dbTime,UINT nState){};	//��ǰ������װ�ý��յ�һ��ͼ���Ĵ�����
	virtual void AfterUpdateOverlook(BYTE * pBuffer, long lWidth,long lHeight,double dbTime,UINT nState){};		//ȫ������װ�ý��յ�һ��ͼ���Ĵ�����
	virtual void AfterSendCommand(BYTE * pBuffer, int iLength,UINT nState){};									//����ָ����Ϻ�Ĵ�����
	virtual void AfterUpdateAttitude(FLOAT inAngle,FLOAT inXRoll,FLOAT inYRoll){};							//��̬��Ϣ����
	CVoyCmd * m_pCmd;																							//ָ�������ָ��
};

#endif // !defined(AFX_IBEHAVIOR_H__B5768320_B81E_4929_98DB_11CB134E7E1F__INCLUDED_)
