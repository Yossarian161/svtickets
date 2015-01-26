// svTicketC.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "12306_help.h"

int _tmain(int argc, _TCHAR* argv[])
{
	

	getchar();
	return 0;
}


/*
// 下载验证码图片(250张)
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