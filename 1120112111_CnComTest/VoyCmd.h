// VoyCmd.h: interface for the CVoyCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOYCMD_H__8007E941_6DAD_4AA1_88D7_21A255E426F0__INCLUDED_)
#define AFX_VOYCMD_H__8007E941_6DAD_4AA1_88D7_21A255E426F0__INCLUDED_

#include "IPhy.h"
#include "IBehavior.h"
#include "CnComm.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BUF				1024		//���ܻ��峤��
#define MAX_SEND			20			//���ͻ��峤��

#define ULTRASONICAMOUNT	24			//��������
#define INFRAREDCHAR		3			//3�ֽ�24λ
#define INFRAREDMOUNT		24			

//�������˶�״̬���
#define STOP		0					//ֹͣ״̬
#define FORWARD		1					//��ֱǰ��
#define BACKWARD	2					//��ֱ����
#define RIGHT		3					//ԭ����ת
#define LEFT		4					//ԭ����ת
#define RIGHTFRONT	5					//��תǰ��
#define LEFTFRONT	6					//��תǰ��
#define RIGHTBACK	7					//��ת����
#define LEFTBACK	8					//��ת����

#define SIG_MOTOR_INDEX	   1
#define CMD_LEN  32
#define CMD_NUM  2
#define SIG_KICK_INDEX	0

typedef struct SigCmd
{
	UCHAR buf[CMD_LEN];
	int Length;
	bool bToBeSend;
}SigCmd;

class CCaptureVideo;
class IBehavior;

class CVoyCmd  
{
	friend class CSampleGrabberCB;
public:
	//�Լ���ӵĺ����Ͷ���
	void SetFiveMotorsSpeed(short* inSpeed);//ͬʱ������������ֵ
	void m_GenerateSigCmd(UINT inIndex, UCHAR inID, UCHAR inLen, UCHAR inMode, UCHAR inMethod, UCHAR *data);//���ʹ�����Ϣ
	void move();
	SigCmd  m_arSigCmd[CMD_NUM];
	//////////////////////////////////////////////////////
	void SetServMotor(int inSpeed);
	void SetRMotorPos(int inPos, int inSpeed);
	void SetLMotorPos(int inPos,int inSpeed);
	void DemarcateForCompass();
	void Demarcate();								//�����ǵı궨
	void Kick(int st);									//������
	void AutoQueryCompass(UINT TimeGap);			//�Զ���ѯ������Ϣ
	void QueryCompass();							//���β�ѯ������Ϣ
	void CircleByGyro(int inAngel,int inSpeed);		//ԭ��ת�򣨻������ݣ�
	void SpeedByGyro(int inSpeed);					//ֱ�У��������ݣ�
	void ShowSensor();								//�ڴ�������ʾ�ؼ�����ʾ��Ϣ
	void AutoQueryInfraRed(UINT TimeGap);			//�Զ���ѯ���⴫������Ϣ
	void AutoQueryUSonic(UINT TimeGap);				//�Զ���ѯ������������Ϣ
	UINT GetState();								//���ص�ǰ������״̬
	void SetBehavior(IBehavior * pBeh);				//�趨��������Ϊ����
	void QueryUltrasonicSensor();					//���β�ѯ����������
	void QueryInfrared();							//���β�ѯ���⴫����
	void SetRMotorSpeed(int inrightspeed);			//�����ҵ���ٶ�
	void SetLMotorSpeed(int inleftspeed);			//���������ٶ�
	void SetBothMotorsSpeed(int inleftspeed, int inrightspeed);		//ͬʱ������������ٶ�
	void Brake(UCHAR breakmode);					//ɲ��
	HANDLE event;

	void Parse(void *buf, int length);				//�������յ���ָ��
	
	CVoyCmd();
	virtual ~CVoyCmd();

	IPhy * m_pPhy;					//ͨѶӲ������ָ��
	IBehavior * m_pBeh;				//��Ϊ����ָ��
	CCaptureVideo * m_pCap;			//��Ƶ��׽����ָ��
	
	BOOL bToEndThreads;				//�˳��������̱߳�ǣ��˳�����ǰ��TRUE

	UINT QueryUSonicTime;			//������������ѯʱ����
	UINT QueryInfraRedTime;			//���⴫������ѯʱ����
	UINT QueryCompassTime;			//������Ϣ��ѯʱ����
	
	UCHAR *m_charUltrasonic;		//����������ԭʼ��Ϣ����
	BOOL *EnableUSonic;				//����������ʹ������
	DOUBLE * ValUSonic;				//������������Ϣ
	BOOL *ValInfrared;				//������������
	BOOL *EnableInfrared;			//����ʹ������
	
	FLOAT m_angle;					//���̽Ƕ�
/*
	CListBox* CmdShow_Send;			//��ʾ���͵�ָ��
	CListBox* CmdShow_Rec;			//��ʾ���յ���ָ��
	CPieSensor* pieSensor;			//��������Ϣͼ
	CCompasee* compass;				//������Ϣͼ

	CStatic * USDataShow;			//������ֵ�ı���ʾ����
	CStatic * IRDataShow;			//������ֵ�ı���ʾ����
	CStatic * CmpsDataShow;			//��������ֵ��ʾ
*/
	int m_iLspeed;					//�����ٶ�
	int m_iRspeed;					//�ҵ���ٶ�
	//FLOAT m_fSpReduc;				//������ٱ�
	float m_fLSpdCoe;
	float m_fRSpdCoe;

	FLOAT m_XRoll;					//X��ת��
	FLOAT m_YRoll;					//Y��ת��

//	CNetSend m_netsend;				//���緢�Ͷ���

	BYTE ValServMotor[8];			//�ŷ����ת��

//	Voy2Txt* pvoytxt;
protected:
	//�Լ���ӵĺ���
	void m_Split2Bytes(unsigned char *inTarg, short inSrc);//ת��Ϊ�ֽ���
	short speed[5];
	////////////////////
	void m_UpdateState();			//���ݵ���ٶ�ֵ����״̬���
	WORD m_CalculateSpeed( int speed);					//���������ٶȻ�������ֽ�
	virtual void m_ParseFrame(UCHAR* buf, int length);	//�����ջ������ݽ����з�

	UCHAR *m_pRcvBuf;				//���ݽ��ջ���
	UCHAR *m_pSendBuf;				//���ݷ��ͻ���
	unsigned int m_nSendlength;		//�ѷ����ֽڼ���
	unsigned int m_nRcvIndex;		//�ѽ����ֽڼ���

	BOOL m_bFrameStart;				//ָ��������ַ����
	UCHAR m_cLast;					//����ͷ�����ֽ��õ��ĸ�������
	unsigned int m_nFrameLength;	//��ǰָ���

	UINT nState;					//������״̬���

private:	
	DOUBLE m_CalDistance(UCHAR inUSChar);		//�ɳ���������ֵ�������
	UCHAR m_CalSum(int length);					//Ϊָ������У���
	void m_GenerateSendBuffer(UCHAR addr, UCHAR status, UCHAR length, UCHAR ctrlcode, UCHAR *data);
	void m_Response(UCHAR *recbuf, int length);	//�ܵ�ָ���Ĵ�����
	void m_ResponseError();						//�յ�������Э���ָ��Ĵ�����
	BOOL m_ValidFrame(UCHAR *buf, int length);	//�ж�һ��ָ���Ƿ����Э��
	void m_ParseBuffer(const UCHAR buf);		//��һƬ���������ݽ���ָ���з�
	void m_ResetSendBuf();						//��շ��ͻ�����
	void m_ResetRcvBuf();						//��ս��ջ�����
	
	UCHAR *m_charInfrared;						//���⴫����������Ϣ����
};

#endif // !defined(AFX_VOYCMD_H__8007E941_6DAD_4AA1_88D7_21A255E426F0__INCLUDED_)
