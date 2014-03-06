// VoyCmd.cpp: implementation of the CVoyCmd class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VoyCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CnComm m_CnComm;
//自动查询超声传感器线程
UINT QueryUSonicThread(LPVOID pParam)
{
	CVoyCmd * pcmd = (CVoyCmd *)pParam;
	const UINT Time = pcmd->QueryUSonicTime;
	while (Time == pcmd->QueryUSonicTime && FALSE == pcmd->bToEndThreads)
	{
		pcmd->QueryUltrasonicSensor();		
		Sleep(Time);
	}
	return 0L;
}

//自动查询红外传感器线程
UINT QueryInfraRedThread(LPVOID pParam)
{
	CVoyCmd * pcmd = (CVoyCmd *)pParam;
	const UINT Time = pcmd->QueryInfraRedTime;
	while (Time == pcmd->QueryInfraRedTime && FALSE == pcmd->bToEndThreads)
	{
		pcmd->QueryInfrared();
		Sleep(Time);
	}
	return 0L;
}

//自动查询罗盘信息线程
UINT QueryCompassThread(LPVOID pParam)
{
	CVoyCmd * pcmd = (CVoyCmd *)pParam;
	const UINT Time = pcmd->QueryCompassTime;
	while (Time == pcmd->QueryCompassTime && FALSE == pcmd->bToEndThreads)
	{
		pcmd->QueryCompass();
		Sleep(Time);
	}
	return 0L;
}

//罗盘标定线程
UINT DemarcateForCmps(LPVOID pParam)
{
	CVoyCmd * pcmd = (CVoyCmd *)pParam;
	Sleep(500);
	pcmd->SetBothMotorsSpeed(-700,700);
	Sleep(20000);
	pcmd->Brake(1);
	return 0L;
}

CVoyCmd::CVoyCmd()
{
	m_charUltrasonic=new UCHAR[ULTRASONICAMOUNT];
	EnableUSonic= new BOOL[ULTRASONICAMOUNT];
	ValUSonic = new DOUBLE[ULTRASONICAMOUNT];

	m_charInfrared= new UCHAR[INFRAREDCHAR];
	EnableInfrared = new BOOL[INFRAREDMOUNT];
	ValInfrared = new BOOL[INFRAREDMOUNT];

	for (int i=0;i<ULTRASONICAMOUNT;i++)
	{
		ValUSonic[i] = 0;
	}

	for (int i=0;i<ULTRASONICAMOUNT;i++)
	{
		EnableUSonic[i] = TRUE;
		EnableInfrared[i] = TRUE;
	}

	for (int i=0;i<INFRAREDMOUNT;i++)
	{
		ValInfrared[i] = FALSE;
	}

	for (int i=0;i<8;i++)
	{
		ValServMotor[i] = 90;
	}
	
	event=CreateEvent(NULL,TRUE,FALSE,NULL);

	nState =0;
	m_iLspeed = m_iRspeed = 0;
	//m_fSpReduc = 14.6;
	m_fLSpdCoe = 1;
	m_fRSpdCoe = 1;

	m_pRcvBuf=new UCHAR[MAX_BUF];
	m_pSendBuf=new UCHAR[MAX_BUF];
	m_nRcvIndex = 0;

	m_pBeh = NULL;
	m_pPhy = NULL;
	m_pCap = NULL;

	QueryUSonicTime = 0;
	QueryInfraRedTime = 0;
	QueryCompassTime = 0;

	bToEndThreads = FALSE;
/*
	CmdShow_Send = NULL;
	CmdShow_Rec = NULL;
	pieSensor = NULL;

	USDataShow = NULL;
	IRDataShow = NULL;
	CmpsDataShow = NULL;
	pvoytxt = NULL;
	*/
}

CVoyCmd::~CVoyCmd()
{
	QueryUSonicTime = 0;
	QueryInfraRedTime = 0;
	
	bToEndThreads = TRUE;

	CloseHandle(event);
	
	delete[] m_charUltrasonic;
	delete[] EnableUSonic;
	delete[] ValUSonic;
	delete[] m_charInfrared;
	delete[] ValInfrared;
	delete[] EnableInfrared;
	m_charInfrared = m_charUltrasonic = NULL;

	delete []m_pRcvBuf;
	delete []m_pSendBuf;

	//延时，给查询线程足够的时间退出
	if (QueryUSonicTime > QueryInfraRedTime)
	{
		Sleep(QueryUSonicTime+10);
	}
	else
	{
		Sleep(QueryInfraRedTime+10);
	}
	CoUninitialize();
}

//将接受到的数据分割组合成完整指令
void CVoyCmd::m_ParseBuffer(const UCHAR buf)
{
	//search for frame start tag
		if (buf==(UCHAR)0xaa && m_cLast==(UCHAR)0x55 && !m_bFrameStart)
		{
			m_pRcvBuf[0] = 0x55;
			m_pRcvBuf[1] = 0xAA;
			m_bFrameStart=true;
			return;
		}
			
		if (m_bFrameStart)
		{
			//put received data into buffer
			m_cLast=0x00;
			m_pRcvBuf[m_nRcvIndex+2]=buf;
			if (m_nRcvIndex==1)
			{
				m_nFrameLength=(buf & 0x3f)+4;
			}
			m_nRcvIndex++;
			//receive one frame, invoke ParseFrame to parse
			if (m_nRcvIndex==m_nFrameLength)
			{ 
				m_ParseFrame(m_pRcvBuf+2, m_nRcvIndex); 
				m_ResetRcvBuf(); 
			}

			//receive buffer overflow
				if (m_nRcvIndex>=MAX_BUF) 
				{
					m_ResetRcvBuf();
				}
		}
		else
			m_cLast=buf;
}

void CVoyCmd::m_ParseFrame(UCHAR *buf, int length)
{
	CString str,strTemp;

	if (!m_ValidFrame(buf,length))
	{
		m_ResponseError();
		return;
	}

	//通过局域网转发指令(已废弃)
// 	if (m_netsend.isConnecting())
// 	{
// 		m_netsend.Send(m_pRcvBuf,length+2);
// 	}

	bool ctrlcodevalid=true;
	switch (buf[2])
	{
		//there are two section of response
		//first is for query response
	case 0x30:
		{
			//query ultrasonic sensor response
			for (int i=0;i<ULTRASONICAMOUNT;i++)
			{
				m_charUltrasonic[i]=buf[i+3];
				ValUSonic[i] = m_CalDistance(m_charUltrasonic[i]);	//换算成距离
			}

			ShowSensor();

			if (m_pBeh != NULL)
			{
				m_pBeh->AfterUpdateUSonic(ValUSonic,EnableUSonic,nState);
			}
			break;
		}
	case 0x34:
		{	//罗盘陀螺仪返回数据

			/*WORD CmpsData;
			CmpsData = buf[5];
			CmpsData<<=8;
			CmpsData |= buf[6];
			//计算角度
			/*m_angle = ((FLOAT)CmpsData/180)*3.1415926;

			if (compass != NULL)
			{
				compass->Update(&buf[5],2);
				compass->Invalidate();
			}
			/*if (CmpsDataShow != NULL)
			{
				if (buf[3] < 0x80)
				{
					str.Format("X轴滚转角度：%d°\n",buf[3]);
					m_XRoll = buf[3];
				}
				else
				{
					str.Format("X轴滚转角度：%d°\n",-(0xff-buf[3]));
					m_XRoll = -(0xff-buf[3]);
				}

				if (buf[4] < 0x80)
				{
					strTemp.Format("Y轴滚转角度：%d°",buf[4]);
					m_YRoll = buf[4];
				}
				else
				{
					strTemp.Format("Y轴滚转角度：%d°",-(0xff-buf[4]));
					m_YRoll = -(0xff-buf[4]);
				}
				str += strTemp;
				CmpsDataShow->SetWindowText(str);
			}*/

			float temp;
			int t = 0 ;

			//俯仰
			if (buf[5]>0x7f)
			{
				buf[5] = 0xff - buf[5];
				buf[6] = 0xff - buf[6];
				t = (int)buf[5]<<8;
				t ^= (int)buf[6];
				temp = (FLOAT)t/3141.59;
				m_YRoll = -temp*180;
			}
			else
			{
				t = (int)buf[5]<<8;
				t ^= (int)buf[6];
				temp = (FLOAT)t/3141.59;
				m_YRoll = temp*180;
			}

			//横滚
			if (buf[7] > 0x7F)
			{
				buf[7] = 0xff - buf[7];
				buf[8] = 0xFF - buf[8];
				t = (int)buf[7]<<8;
				t ^= (int)buf[8];
				temp = (FLOAT)t/3141.59;
				m_XRoll = -temp*180;
			}
			else
			{
				t = (int)buf[7]<<8;
				t ^= (int)buf[8];
				temp = (FLOAT)t/3141.59;
				m_XRoll = temp*180;
			}
/*
			if (CmpsDataShow != NULL)
			{
				CString str;
				str.Format("X轴滚转角度：%.1f°\nY轴滚转角度：%.1f°",m_XRoll,m_YRoll);
				CmpsDataShow->SetWindowText(str);
			}
*/			
			//偏北角
			t = 0;
			t = (int)buf[3]<<8;
			t ^= (int)buf[4];
			m_angle = (FLOAT)t/100;
/*
			if (compass != NULL)
			{
				compass->Update(m_angle,2);
				compass->Invalidate();
			}

			if (m_pBeh != NULL)
			{
				m_pBeh->AfterUpdateAttitude(m_angle,m_XRoll,m_YRoll);
			}
*/
			break;
		}
	case 0x36:
		{
			//query infrared sensor response
			for (int i=0;i<INFRAREDCHAR;i++)
			{
				m_charInfrared[i]=buf[i+3];
				for (int j=7;j>=0;j--)
				{
				
					if ((m_charInfrared[i] & 0x01)==0x01)
					{
						ValInfrared[j+8*i]=false;
					}
					else
					{
						ValInfrared[j+8*i]=true;
					}

					m_charInfrared[i]=m_charInfrared[i] >> 1;
				}
				m_charInfrared[i]=m_charInfrared[i] >> 1;	//使m_charInfrared恢复到原来的位顺序
			}

			ShowSensor();
			
			if (m_pBeh != NULL)
			{
				m_pBeh->AfterUpdateInfrared(m_charInfrared,EnableInfrared,nState);
			}
			break;
		}
		//second is for setting response
	case 0x21: break;
	case 0x24: break;
	case 0x25: break;
	case 0x26: break;
	case 0x2c: break;	//罗盘闭环直行回执
	case 0x2d: break;	//罗盘闭环旋转回执
	case 0x2F: break;	//踢球
	case 0x33: break;
	default:
		{
			ctrlcodevalid=false;
			break;
		}
	}
	
	SetEvent(event);

	if (ctrlcodevalid)
		m_Response(buf,length);
	else
		m_ResponseError();
}

void CVoyCmd::Parse(void *buf, int length)
{
	if (length==0 || buf==NULL) return;
			
		for (int i=0;i<length;i++)
			m_ParseBuffer(((UCHAR*)buf)[i]);
}

void CVoyCmd::m_ResetRcvBuf()
{
	memset(m_pRcvBuf, 0, MAX_BUF);
	m_nRcvIndex=0;
	m_bFrameStart=false;
	m_cLast=0x00;
	m_nFrameLength=0;
}

void CVoyCmd::m_ResetSendBuf()
{
	memset(m_pSendBuf, 0, MAX_BUF); 
	m_nSendlength=0;
}

BOOL CVoyCmd::m_ValidFrame(UCHAR *buf, int length)
{
	if ((buf[1] & 0x3f)!=length-4)
		return false;

	int sum=0x000000ff;

	for (int i=0;i<length-1;i++)
		sum+=buf[i];

	if (buf[length-1]!=(UCHAR)(sum & 0x000000ff))
		return false;

	return true;
}

void CVoyCmd::m_ResponseError()
{	
	//解析不合协议的指令回调

}

void CVoyCmd::m_Response(UCHAR *recbuf, int length)
{	//解析合法指令回调
	m_pPhy->bSending = FALSE;
/*
	//显示接收指令
	if (CmdShow_Rec != NULL)
	{
		CString cmd,str;
		CTime tm;
		tm = CTime::GetCurrentTime();
		cmd = tm.Format("%X:");
		for (int i=0;i<length+2;i++)
		{
			str.Format("%.2X ",m_pRcvBuf[i]);
			cmd += str;
		}
		CmdShow_Rec->AddString(cmd);
		CmdShow_Rec->SetCurSel(CmdShow_Rec->GetCount()-1);
	}
	*/
}

UCHAR CVoyCmd::m_CalSum(int length)
{
	int temp=0;
	for (int i=0;i<length;i++)
	temp+=m_pSendBuf[i];

	UCHAR ret;
	ret=(UCHAR)(temp & 0x000000ff);
	return ret;
}

DOUBLE CVoyCmd::m_CalDistance(UCHAR inUSChar)
{
	DOUBLE Distance;
	//Distance = (DOUBLE) inUSChar*0.02174;
	Distance = (DOUBLE) inUSChar*0.0344;

	return Distance;
}

void CVoyCmd::m_GenerateSendBuffer(UCHAR addr, UCHAR status, UCHAR length, UCHAR ctrlcode, UCHAR *data)
{
	m_pSendBuf[0]=(UCHAR)0x55;
	m_pSendBuf[1]=(UCHAR)0xaa;
	m_pSendBuf[2]=addr;
	m_pSendBuf[3]=((status << 6) & 0xc0) | (length & 0x3f);
	m_pSendBuf[4]=ctrlcode;
	if (length>0 && data!=NULL)
	{
		memcpy(&m_pSendBuf[5],data,length);
	}
	m_pSendBuf[length+5]=m_CalSum(length+5);

	m_nSendlength=length+6;

	//发送指令
	if (m_pPhy != NULL )
	{
		m_pPhy->Send(m_pSendBuf,m_nSendlength);
		for(int i=0;i<5;i++)
		{
			printf("%x",m_pSendBuf[i]);
		}
/*
		//显示发送指令
		if (CmdShow_Send != NULL)
		{
			CString cmd,str;
			CTime tm;
			tm = CTime::GetCurrentTime();
			cmd = tm.Format("%X: ");
			for (int i=0;i<m_nSendlength;i++)
			{
				str.Format("%.2X ",m_pSendBuf[i]);
				cmd += str;
			}
			CmdShow_Send->AddString(cmd);
			CmdShow_Send->SetCurSel(CmdShow_Send->GetCount()-1);
			
		}
*/	}
/*	
	if (m_pBeh!= NULL)
	{
		m_pBeh->AfterSendCommand(m_pSendBuf,m_nSendlength,nState);
	}
	*/
}
void CVoyCmd::Brake(UCHAR breakmode)
{
	m_GenerateSendBuffer((UCHAR)0x01,0,1,(UCHAR)0x21,&breakmode);
	m_iLspeed = 0;
	m_iRspeed = 0;
	nState = STOP;
}

void CVoyCmd::SetBothMotorsSpeed(int inleftspeed, int inrightspeed)
{
	if (inleftspeed == m_iLspeed && inrightspeed == m_iRspeed)
	{
		return;
	}
	// printf("Fuck\n");
	m_iLspeed = inleftspeed;
	m_iRspeed = inrightspeed;
	
    WORD leftspeed = m_CalculateSpeed((int)m_iLspeed*m_fLSpdCoe);
	WORD rightspeed = m_CalculateSpeed((int)m_iRspeed*m_fRSpdCoe);

	UCHAR bothspeed[4];	
	
	bothspeed[0]=(UCHAR)((leftspeed >> 8) & 0x00ff);
	bothspeed[1]=(UCHAR)(leftspeed & 0x00ff);

	bothspeed[2]=(UCHAR)((rightspeed >> 8) & 0x00ff);
	bothspeed[3]=(UCHAR)(rightspeed & 0x00ff);	

	m_GenerateSendBuffer((UCHAR)0x01,0,4,(UCHAR)0x26,bothspeed);
	m_UpdateState();
}

void CVoyCmd::SetLMotorSpeed(int inleftspeed)
{
	if (inleftspeed == m_iLspeed)
	{
		return;
	}

	m_iLspeed = inleftspeed;

	WORD leftspeed = m_CalculateSpeed((int)inleftspeed*m_fLSpdCoe);

	UCHAR lspeed[2];

	lspeed[1]=(UCHAR)(leftspeed & 0x00ff);
	lspeed[0]=(UCHAR)((leftspeed >> 8) & 0x00ff);

	m_GenerateSendBuffer((UCHAR)0x01,0,2,(UCHAR)0x24,lspeed);
	m_UpdateState();
}

void CVoyCmd::SetRMotorSpeed(int inrightspeed)
{
	if (inrightspeed == m_iRspeed)
	{
		return;
	}
	m_iRspeed = inrightspeed;

	WORD rightspeed = m_CalculateSpeed((int)inrightspeed*m_fRSpdCoe);

	UCHAR rspeed[2];

	rspeed[1]=(UCHAR)(rightspeed & 0x00ff);
	rspeed[0]=(UCHAR)((rightspeed >> 8) & 0x00ff);

	m_GenerateSendBuffer((UCHAR)0x01,0,2,(UCHAR)0x25,rspeed);
	m_UpdateState();
}

void CVoyCmd::QueryInfrared()
{
	m_GenerateSendBuffer((UCHAR)0x02,0,0,(UCHAR)0x36,NULL);
}

void CVoyCmd::QueryUltrasonicSensor()
{	
	UCHAR ultrasonicchar[3];
	int i;
	for (int i=0;i<3;i++)
	{
		ultrasonicchar[i]=0xff;
	}

	for (i=0;i<24;i++)
	{
		if (EnableUSonic[i] == FALSE)
		{
			ultrasonicchar[2-i/8] &= ~(0x01<<(i%8));
		}
	}

	/*CString a;
	a.Format("%d,%d,%d",ultrasonicchar[0],ultrasonicchar[1],ultrasonicchar[2]);
	AfxMessageBox(a);*/

	m_GenerateSendBuffer((UCHAR)0x02,0,3,(UCHAR)0x30,ultrasonicchar);
}

void CVoyCmd::SetBehavior(IBehavior *pBeh)
{
	/*IBehavior * oldpBeh = m_pBeh;
	if (m_pBeh != NULL)
	{
		m_pBeh = NULL;
		Sleep(100);
		oldpBeh->SetCmd(NULL);
	}*/
	
	m_pBeh = pBeh;

	if (m_pBeh != NULL)
	{
		m_pBeh->SetCmd(this);
	}
}

WORD CVoyCmd::m_CalculateSpeed(int speed)
{
	//speed *= m_fSpdCoe;
	BOOL forward;
	if (speed >= 0)
	{
		forward = TRUE;
	}
	else
	{
		forward = FALSE;
		speed = -speed;
	}

	if (speed > 30000)
	{
		speed = 30000;
	}

	WORD ret=0;
	speed &=0x7fff;

	if (forward)
		speed &= 0x7fff;
	else
		speed |= 0x8000;

	ret=speed;
	return ret;
}

void CVoyCmd::m_UpdateState()
{
	//左右速度相等
	if (m_iLspeed == m_iRspeed)
	{
		if (0 == m_iRspeed)
		{
			nState = STOP;
			return;
		}
		if (m_iLspeed > 0)
		{
			nState = FORWARD;
		}
		else
		{
			nState = BACKWARD;
		}
		return;
	}

	//左右电机大小相等，方向相反
	if (m_iLspeed == -m_iRspeed)
	{
		if (m_iLspeed > 0)
		{
			nState = RIGHT;
		}
		else
		{
			nState = LEFT;
		}
		return;
	}

	//右电机比左电机大（正方向上）
	if (m_iLspeed < m_iRspeed)
	{
		if (m_iLspeed + m_iRspeed > 0)
		{	//右前进速度较大
			nState = LEFTFRONT;
		} 
		else
		{	//左后退速度较大
			nState = RIGHTBACK;
		}
	}
	else
	{	//m_iLspeed > m_iRspeed
		if (m_iLspeed + m_iRspeed > 0)
		{	//左前进速度较大
			nState = RIGHTFRONT;
		} 
		else
		{	//右后退速度较大
			nState = LEFTBACK;
		}		
	}
}

UINT CVoyCmd::GetState()
{
	return nState;
}

void CVoyCmd::AutoQueryUSonic(UINT TimeGap)
{
	if (TimeGap == QueryUSonicTime)
	{
		return;
	}
	QueryUSonicTime = TimeGap;

	if (QueryUSonicTime != 0)
	{
		UCHAR sw = 0x01;
		m_GenerateSendBuffer(2,0,1,0x31,&sw);
		AfxBeginThread(QueryUSonicThread,(LPVOID)this,THREAD_PRIORITY_TIME_CRITICAL);
	}
	else
	{	//参数为0 ，停止超声硬件工作
		UCHAR sw = 0x00;
		m_GenerateSendBuffer(2,0,1,0x31,&sw);
	}
}

void CVoyCmd::AutoQueryInfraRed(UINT TimeGap)
{
	if (TimeGap == QueryInfraRedTime)
	{
		return;
	}
	QueryInfraRedTime = TimeGap;
	if (QueryInfraRedTime != 0)
	{
		AfxBeginThread(QueryInfraRedThread,(LPVOID)this,THREAD_PRIORITY_NORMAL);
	}
}

void CVoyCmd::AutoQueryCompass(UINT TimeGap)
{
	if (TimeGap == QueryCompassTime)
	{
		return;
	}
	QueryCompassTime = TimeGap;
	if (QueryCompassTime != 0)
	{
		AfxBeginThread(QueryCompassThread,(LPVOID)this,THREAD_PRIORITY_NORMAL);
	}
}

void CVoyCmd::ShowSensor()
{
	CString strUS,strIR;
	CString temp;
	int i;

	/*
	if (USDataShow != NULL)
	{
		for (i=0;i<ULTRASONICAMOUNT;i++)
		{
			if (EnableUSonic[i] == FALSE)
			{
				continue;
			}
			
			temp.Format("%2d号:%2.2f   ",i+1,ValUSonic[i]);
			strUS += temp;*/
			/*if ((i+1)%3 == 0)
			{
				*inUSstr += "\r";
			}*/
	/*
		}
		USDataShow->SetWindowText(strUS);
	}

	if (IRDataShow != NULL)
	{
		for (i=0;i<INFRAREDMOUNT;i++)
		{
			if (EnableInfrared[i] == FALSE)
			{
				continue;
			}
			
			temp.Format("%2d号:",i+1);
			if (ValInfrared[i] == TRUE)
			{
				temp+="有  ";
			} 
			else
			{
				temp+="无  ";
			}
			strIR += temp;*/
			/*if (i%2 == 1)
			{
				*inIRstr += "\r";
			}*/
	/*	}
		IRDataShow->SetWindowText(strIR);
	}

	if (pieSensor != NULL)
	{
		pieSensor->UpdateUntrasonic(ValUSonic,EnableUSonic);
		pieSensor->UpdateInfrared(ValInfrared);
		pieSensor->Invalidate();
	}

	
	if (pvoytxt != NULL)
	{
		for (i=0;i<24;i++)
		{
			//pvoytxt->SensorEnable[i] = EnableUSonic[i];
			pvoytxt->ValUSonic[i] = ValUSonic[i];
			pvoytxt->ValInfrared[i] = ValInfrared[i];
		}
	}
	*/
	
}

void CVoyCmd::SpeedByGyro(int inSpeed)
{
	float fspdcoe;
	if (m_fLSpdCoe > m_fRSpdCoe)
	{
		fspdcoe = m_fRSpdCoe;
	} 
	else
	{
		fspdcoe = m_fLSpdCoe;
	}

	WORD wspeed = m_CalculateSpeed(inSpeed*fspdcoe);
	UCHAR speed[4];
	speed[0] = 0x00;
	speed[1] = 0x00;
	speed[3] = (UCHAR)(wspeed & 0x00ff);
	speed[2] = (UCHAR)((wspeed>>8) & 0x00ff);

	m_GenerateSendBuffer((UCHAR) 0x01,0,4,0x2C,speed);
}

void CVoyCmd::CircleByGyro(int inAngle, int inSpeed)
{
	if (inAngle <-360 || inAngle > 360)
	{
		inAngle = inAngle%360;
	}

	float fspdcoe;
	if (m_fLSpdCoe > m_fRSpdCoe)
	{
		fspdcoe = m_fRSpdCoe;
	} 
	else
	{
		fspdcoe = m_fLSpdCoe;
	}

	WORD wAngle,wSpeed;
	UCHAR toSend[4];

	wAngle = m_CalculateSpeed(inAngle*39000/3445);
	toSend[1] = (UCHAR) wAngle&0x00ff;
	toSend[0] = (UCHAR) (wAngle>>8)&0x00ff;

	wSpeed = m_CalculateSpeed(inSpeed*fspdcoe);
	toSend[3] = (UCHAR)(wSpeed & 0x00ff);
	toSend[2] = (UCHAR)((wSpeed>>8) & 0x00ff);

	m_GenerateSendBuffer((UCHAR) 0x01,0,4,0x2D,toSend);
}

void CVoyCmd::QueryCompass()
{
	m_GenerateSendBuffer(0x03,0,0,0x34,NULL);
}

void CVoyCmd::Kick(int st)
{
	static UCHAR val[2];
	m_Split2Bytes(val,(short)st);
	m_GenerateSigCmd(SIG_KICK_INDEX,0x38,0x02,0x09,0x70,(UCHAR*)val);
}

void CVoyCmd::Demarcate()
{
	m_GenerateSendBuffer(0x01,0,0,0x2f,NULL);
}

void CVoyCmd::DemarcateForCompass()
{
	m_GenerateSendBuffer(0x03,0,0,0x33,NULL);
	AfxBeginThread(DemarcateForCmps,(LPVOID)this,THREAD_PRIORITY_NORMAL);
}

void CVoyCmd::SetLMotorPos(int inPos, int inSpeed)
{
	UCHAR buf[6];
	WORD  speed;
	UINT  Pos;
	if (inSpeed < 0)
	{
		speed = (WORD)(-inSpeed*m_fLSpdCoe);
	}
	else
	{
		speed = (WORD)(inSpeed*m_fLSpdCoe);
	}
	buf[0] = speed<<8;
	buf[1] = speed;

	if (inPos < 0 )
	{	//反向位置
		Pos = (-inPos/360)*m_fLSpdCoe*4096;
		buf[2] = Pos <<24;
		buf[3] = Pos <<16;
		buf[4] = Pos <<8;
		buf[5] = Pos;
		buf[2] |= 0x80;
	}
	else
	{	//正向位置
		Pos = (inPos/360)*m_fLSpdCoe*4096;
		buf[2] = Pos <<24;
		buf[3] = Pos <<16;
		buf[4] = Pos <<8;
		buf[5] = Pos;
		buf[2] &= 0x7f;
	}

	m_GenerateSendBuffer((UCHAR)0x01,0,6,(UCHAR)0x22,buf);
}

void CVoyCmd::SetRMotorPos(int inPos, int inSpeed)
{
	UCHAR buf[6];
	WORD  speed;
	UINT  Pos;
	if (inSpeed < 0)
	{
		speed = (WORD)(-inSpeed*m_fRSpdCoe);
	}
	else
	{
		speed = (WORD)(inSpeed*m_fRSpdCoe);
	}
	buf[0] = speed<<8;
	buf[1] = speed;

	if (inPos < 0 )
	{	//反向位置
		Pos = (-inPos/360)*m_fRSpdCoe*4096;
		buf[2] = Pos <<24;
		buf[3] = Pos <<16;
		buf[4] = Pos <<8;
		buf[5] = Pos;
		buf[2] |= 0x80;
	}
	else
	{	//正向位置
		Pos = (inPos/360)*m_fRSpdCoe*4096;
		buf[2] = Pos <<24;
		buf[3] = Pos <<16;
		buf[4] = Pos <<8;
		buf[5] = Pos;
		buf[2] &= 0x7f;
	}

	m_GenerateSendBuffer((UCHAR)0x01,0,6,(UCHAR)0x23,buf);
}

void CVoyCmd::SetServMotor(int inSpeed)
{
	UCHAR buf[9];
	if (inSpeed > 0xff)
	{
		buf[8] = 0xff;
	}
	else if (inSpeed < 0)
	{
		buf[8] = 0;
	}
	else
	{
		buf[8] = (BYTE) inSpeed;
	}

	for (int i=0;i<8;i++)
	{
		buf[i] = ValServMotor[i];
	}

	m_GenerateSendBuffer(0x06,0,10,0x01,buf);
}

void CVoyCmd::SetFiveMotorsSpeed(short* inSpeed)
{
	//55 AA 38 0A 08 70 1H 1L 2H 2L 3H 3L 4H 4L 5H 5L SUM	// 同时设置五电机速度
	unsigned char speedbuf[10];
	m_Split2Bytes(&speedbuf[0],inSpeed[0]);
	m_Split2Bytes(&speedbuf[2],inSpeed[1]);
	m_Split2Bytes(&speedbuf[4],inSpeed[2]);
	m_Split2Bytes(&speedbuf[6],inSpeed[3]);
	m_Split2Bytes(&speedbuf[8],inSpeed[4]);
	//m_GenerateSendBuffer(0x38,0x0a,10,0x08,speedbuf);
	m_GenerateSigCmd(SIG_MOTOR_INDEX,0x38,0x0a,0x08,0x70,speedbuf);
}

void CVoyCmd::m_Split2Bytes(unsigned char *inTarg, short inSrc)
{
	if (inTarg == NULL)
	{
		return;
	}
	
	static unsigned short temp;
	memcpy(&temp,&inSrc,sizeof(short));
	inTarg[1] = (unsigned char)temp&0x00ff;
	temp >>= 8;
	inTarg[0] = (unsigned char)temp&0x00ff;
}

void CVoyCmd::m_GenerateSigCmd(UINT inIndex, UCHAR inID, UCHAR inLen, UCHAR inMode, UCHAR inMethod, UCHAR *data)
{
	//55 AA 38 0A 08 70 1H 1L 2H 2L 3H 3L 4H 4L 5H 5L SUM	// 同时设置五电机速度
	//包头  数据  校验和
	m_pSendBuf[0]=(UCHAR)0x55;
	m_pSendBuf[1]=(UCHAR)0xaa;
	m_pSendBuf[2]=inID;
	m_pSendBuf[3]=inLen;
	m_pSendBuf[4]=inMode;
	m_pSendBuf[5]=inMethod;
	
	if (inLen>0 && data!=NULL)
	{
		memcpy(&m_pSendBuf[6],data,inLen);
	}
	m_pSendBuf[inLen+6]=m_CalSum(inLen+6);//添加校验和
	
	SigCmd* pSigCmd = &m_arSigCmd[inIndex];
	
	pSigCmd->Length=inLen+7;//输入信号的长度，加固定长度
	memcpy(pSigCmd->buf,m_pSendBuf,pSigCmd->Length);
	pSigCmd->bToBeSend = true;
	m_CnComm.Write(pSigCmd->buf,pSigCmd->Length);
}
void CVoyCmd::move()
{
	speed[0] = -100;//左右电机
	speed[1] = 100;
	speed[2] = 0;//后电机
	speed[3]=7000;//左右吸球器
	speed[4]=-7000;
	SetFiveMotorsSpeed(speed);
}