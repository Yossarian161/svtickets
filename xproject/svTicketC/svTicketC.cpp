// svTicketC.cpp : �������̨Ӧ�ó������ڵ㡣
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
				//��¼�ɹ�
				SVLOGGER_INFO << "��¼�ɹ�";
			}
			else
			{
				// ��¼ʧ��
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