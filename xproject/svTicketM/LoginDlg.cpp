// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "svTicketM.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "svTicketMDlg.h"

#include "ximage.hpp"
#include "xdecaptcha.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_auto_login = true;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_01, &CLoginDlg::OnBnClickedBtn01)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_PASSCODE, &CLoginDlg::OnStnClickedStaticPasscode)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CEdit *)GetDlgItem(IDC_EDIT_USERNAME))->SetLimitText(100);
	((CEdit *)GetDlgItem(IDC_EDIT_PASSWORD))->SetLimitText(100);
	((CEdit *)GetDlgItem(IDC_EDIT_PASSCODE))->SetLimitText(4);

// 	if (!gl_manage.login_init())
// 	{
// 		return FALSE;
// 	}

	SetTimer(1, 100, NULL);
	std::string pass_str = decaptchaImage("./data/pass.png");
	SetDlgItemText(IDC_EDIT_PASSCODE, win32_A2U(pass_str.c_str()));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		onFlushPassCode();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CLoginDlg::OnBnClickedBtn01()
{
	CString spasscode;
	CString susername,spassword;
	GetDlgItemText(IDC_EDIT_PASSCODE, spasscode);
	GetDlgItemText(IDC_EDIT_USERNAME, susername);
	GetDlgItemText(IDC_EDIT_PASSWORD, spassword);

	if (m_auto_login)
	{
		std::string pass_str = win32_U2A(spasscode.GetBuffer(0));
		while (!gl_manage.login_passcode_verify(pass_str))
		{
			gl_manage.login_passcode_reflush();
			pass_str = xdecaptcha::get_instance()->get_vcode_from_file(std::string("./data/pass.png"));
			SetDlgItemText(IDC_EDIT_PASSCODE, win32_A2U(pass_str.c_str()));
			UpdateData(FALSE);
		}

		if (!gl_manage.login_verify(win32_U2A(susername.GetBuffer(0)), win32_U2A(spassword.GetBuffer(0)), win32_U2A(spasscode.GetBuffer(0))))
		{
			AfxMessageBox(win32_A2U((gl_manage.get_error_buffer()).c_str()));

			gl_manage.login_init();
			std::string pass_str = decaptchaImage("./data/pass.png");
			SetDlgItemText(IDC_EDIT_PASSCODE, win32_A2U(pass_str.c_str()));
		}
		else
		{
			//登录成功
			OnOK();
		}
	}
	else
	{
		if (!gl_manage.login_passcode_verify(win32_U2A(spasscode.GetBuffer(0))))
		{
			AfxMessageBox(win32_A2U((gl_manage.get_error_buffer()).c_str()));
		}
		else
		{
			//AfxMessageBox("验证码正确");
			if (!gl_manage.login_verify(win32_U2A(susername.GetBuffer(0)), win32_U2A(spassword.GetBuffer(0)), win32_U2A(spasscode.GetBuffer(0))))
			{
				AfxMessageBox(win32_A2U((gl_manage.get_error_buffer()).c_str()));

				gl_manage.login_init();
				std::string pass_str = decaptchaImage("./data/pass.png");
				SetDlgItemText(IDC_EDIT_PASSCODE, win32_A2U(pass_str.c_str()));
			}
			else
			{
				//登录成功
				OnOK();
			}
		}
	}
}

void CLoginDlg::onFlushPassCode()
{
	xImage::get_instance()->draw_image(GetDlgItem(IDC_STATIC_PASSCODE), win32_utils::AnsiToUnicode("./data/pass.png"));
}

void CLoginDlg::OnStnClickedStaticPasscode()
{
	gl_manage.login_passcode_reflush();
	//Sleep(500);
	std::string pass_str = decaptchaImage("./data/pass.png");
	SetDlgItemText(IDC_EDIT_PASSCODE, win32_A2U(pass_str.c_str()));

	UpdateData(FALSE);
}

std::string CLoginDlg::decaptchaImage(const std::string& image_path)
{
	//xdecaptcha::get_instance();
	
	std::string str_code = xdecaptcha::get_instance()->get_vcode_from_file(std::string("./data/pass.png"));

 	return str_code;
}
