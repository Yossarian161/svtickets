
// svTicketM.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CsvTicketMApp:
// �йش����ʵ�֣������ svTicketM.cpp
//

class CsvTicketMApp : public CWinApp
{
public:
	CsvTicketMApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CsvTicketMApp theApp;