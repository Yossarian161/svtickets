// svTicketC.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "12306_help.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	ticket_manage _manage;
	
	if (_manage.login_init() != 1)
	{
		std::cout << "input pass code: ";
		std::string spasscode;
		std::cin >> spasscode;
		
		if (_manage.passcode_verify(spasscode))
		{
			if (_manage.login_verify("785192612@qq.com","1201qiao", spasscode))
			{
				//登录成功
				SVLOGGER_INFO << "登录成功";
			}
			else
			{
				// 登录失败
				SVLOGGER_ERR << _manage.get_error_buffer();
				goto _error;
			}
		}
		else
		{
			SVLOGGER_ERR << _manage.get_error_buffer();
			goto _error;
		}
	}

	//
	_manage.query_passengers();

//	_manage.login_out();
_error:
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