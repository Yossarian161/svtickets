// svTicketC.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "12306_help.h"

int _tmain(int argc, _TCHAR* argv[])
{
	

	getchar();
	return 0;
}


/*
// ������֤��ͼƬ(250��)
std::ostringstream oss_path;
for (int idx = 0; idx < 250; ++idx)
{
	ticket_manage _manage;
	for (int i = 0; i < 10; ++i,++idx)
	{
		oss_path.str("");
		oss_path << "images/pass_0" << idx+1 << ".png";

		_manage.passcode_reflush(oss_path.str());
		SVLOGGER_DBG << oss_path.str() << "\tend.";
		Sleep(rand()%1000);
	}
}
*/