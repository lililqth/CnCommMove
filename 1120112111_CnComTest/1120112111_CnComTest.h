
// 1120112111_CnComTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy1120112111_CnComTestApp:
// �йش����ʵ�֣������ 1120112111_CnComTest.cpp
//

class CMy1120112111_CnComTestApp : public CWinApp
{
public:
	CMy1120112111_CnComTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy1120112111_CnComTestApp theApp;