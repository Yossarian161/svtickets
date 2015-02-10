
// svTicketMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "svTicketM.h"
#include "svTicketMDlg.h"
#include "NewCellTypes/GridURLCell.h"
#include "StopStationDlg.h"
#include "LoginDlg.h"
#include "PassengerDlg.h"

#include "ximage.hpp"
#include "xdecaptcha.h"

ticket_manage gl_manage;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsvTicketMDlg 对话框



CsvTicketMDlg::CsvTicketMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsvTicketMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsvTicketMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_FROM_STATION_NAME, m_from_station);
	DDX_Control(pDX, IDC_TO_STATION_NAME, m_to_station);
	DDX_Control(pDX, IDC_TRAIN_DATE, m_train_date);
	DDX_Control(pDX, IDC_PASSENGER_LIST, m_passenger_list);
}

BEGIN_MESSAGE_MAP(CsvTicketMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_QUERY, &CsvTicketMDlg::OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_CHANGE_STATION, &CsvTicketMDlg::OnBnClickedBtnChangeStation)
	ON_MESSAGE(WM_GRID_CELL_CLICK, &CsvTicketMDlg::OnGridCellClick)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_BTN_SELECT, &CsvTicketMDlg::OnStnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_SUBMIT_ORDER, &CsvTicketMDlg::OnBnClickedBtnSubmitOrder)
END_MESSAGE_MAP()


// CsvTicketMDlg 消息处理程序

BOOL CsvTicketMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 开启程序运行日志
	SVLOGGER_INIT_PATH("./data/svticket.log");
	// 获取ip地址
	ip138 _ip_138;
	CString ip_msg;
	ip_msg.Format(_T("%s"), win32_A2U(_ip_138.get_reponse_str().c_str()));
	svTicketRunLogPush(ip_msg);
	//
	if (gl_manage.login_init() != 1)
	{
		SetTimer(1, 100, NULL);
	}
	else
	{
		CString username = win32_A2U(gl_manage.get_username_cn().c_str());
		CString msg;
		msg.Format(_T("用户：[%s] 成功登录。"), username);
		svTicketRunLogPush(msg);
	}
	
	OnInitControl();

	// test data
	SetDlgItemText(IDC_FROM_STATION_NAME, _T("吉安"));
	SetDlgItemText(IDC_TO_STATION_NAME, _T("深圳"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsvTicketMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsvTicketMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsvTicketMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsvTicketMDlg::OnInitControl()
{
	gl_manage.get_station_name();

	// init train date
	m_train_date.SetFormat(_T("yyyy-MM-dd"));

	// init auto completa edit control. 
	m_from_station.Init();
	m_from_station.SetMode(_MODE_CURSOR_O_LIST_);
	m_from_station.AddSearchStrings(gl_manage.get_station_name_list());
	m_from_station.SetSelectOnly(true);

	m_to_station.Init();
	m_to_station.SetMode(_MODE_CURSOR_O_LIST_);
	m_to_station.AddSearchStrings(gl_manage.get_station_name_list());
	m_to_station.SetSelectOnly(true);

	// init passenger
	m_passenger_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_passenger_list.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 60);
	m_passenger_list.InsertColumn(1, _T("证件号码"), LVCFMT_CENTER, 150);
	m_passenger_list.InsertColumn(2, _T("席别"), LVCFMT_CENTER, 65);
	m_passenger_list.InsertColumn(3, _T("票种"), LVCFMT_CENTER, 60);

	m_passenger_list.SetTextBkColor(RGB(220,235,245));

	// init grid_ctrl.
	OnInitGridCtrl();

	// check order dlg.
	m_checkorder_dlg = new CCheckOrderDlg;
	m_checkorder_dlg->Create(IDD_CHECK_ORDER_DLG);
	m_checkorder_dlg->SetParent(this);
	m_checkorder_dlg->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BTN_SUBMIT_ORDER)->ShowWindow(SW_HIDE);
}

void CsvTicketMDlg::OnInitGridCtrl()
{
	int nCol = 16;

	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetTrackFocusCell(FALSE);
	m_Grid.SetFrameFocusCell(FALSE);
//	m_Grid.SetFixedRowSelection(TRUE);
//	m_Grid.EnableColumnHide();
	m_Grid.EnableSelection(FALSE);
//	m_Grid.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(222,233,241));
	//m_Grid.SetGridLineColor(RGB(0,255,0));
	m_Grid.SetGridBkColor(RGB(255,255,255));
	//m_Grid.SetAutoSizeStyle();
	m_Grid.SetScrollNoDisable(1, true);

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(nCol);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	CString TitleName[] = {
		_T("车次"),
		_T("发站"),
		_T("到站"),
		_T("历时"),
		_T("商务座"),
		_T("特等座"),
		_T("一等座"),
		_T("二等座"),
		_T("高级软卧"),
		_T("软卧"),
		_T("硬卧"),
		_T("软座"),
		_T("硬座"),
		_T("无座"),
		_T("其他"),
		_T("购票")
	};

	for (int irow = 0; irow < 1; ++irow)
	{
		for (int idx = 0; idx < 16; ++idx)
		{
			m_Grid.SetRowHeight(irow, 25);

			if (idx == 0)
			{
				m_Grid.SetColumnWidth(idx, 50);
			}
			if ((idx > 0 && idx < 3)|| idx == 15 )
			{
				m_Grid.SetColumnWidth(idx, 76);
			}
			if (idx > 3 && idx < 8)
			{
				m_Grid.SetColumnWidth(idx, 45);
			}
			if (idx == 8 || idx == 3)
			{
				m_Grid.SetColumnWidth(idx, 60);
			}
			if (idx > 8 && idx < 15)
			{
				m_Grid.SetColumnWidth(idx, 35);
			}			

			CString str;
			GV_ITEM Item;
			Item.mask = GVIF_TEXT;
			Item.row = irow;
			Item.col = idx;

			if (irow == 0)
			{
				str.Format(_T("%s"), TitleName[idx]);
			}

			Item.strText = str;

			m_Grid.SetItem(&Item);

			m_Grid.SetItemState(irow, idx, m_Grid.GetItemState(irow, idx) | GVIS_READONLY);
			m_Grid.SetItemFormat(irow,idx, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOCLIP);
		}
	}

// #if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
// 	m_Grid.AutoSize();
// #endif

	UpdateData(FALSE);
}


void CsvTicketMDlg::OnBnClickedBtnQuery()
{
	CString fstation_name, tstation_name;
	bool m_bret = false;
	ShowCheckOrderDialog(false);
	
	if (m_from_station.GetItemValue().IsEmpty() || m_to_station.GetItemValue().IsEmpty())
	{
		AfxMessageBox(_T("出发地或目的地不允许为空！"));
		return ;
	}
	bool m_surplus_query = true;	//启用余票查询

	CString stime;
	GetDlgItemText(IDC_TRAIN_DATE, stime);
	//AfxMessageBox(stime);

	// 清空grid历史数据
	m_Grid.DeleteNonFixedRows();

	GetDlgItemText(IDC_FROM_STATION_NAME, fstation_name);
	GetDlgItemText(IDC_TO_STATION_NAME, tstation_name);
	m_ticket_dto.set_from_station(win32_U2A(m_from_station.GetItemValue().GetBuffer(0)));
	m_ticket_dto.set_from_station_name(win32_U2A(fstation_name.GetBuffer(0)));
	m_ticket_dto.set_to_station(win32_U2A(m_to_station.GetItemValue().GetBuffer(0)));
	m_ticket_dto.set_to_station_name(win32_U2A(tstation_name.GetBuffer(0)));
	m_ticket_dto.set_train_date(win32_U2A(stime.GetBuffer(0)));


	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK_STUDENT))->GetCheck())
	{
		m_ticket_dto.set_purpose_codes(true);
	}
	else
		m_ticket_dto.set_purpose_codes();
	
	CString log_msg;
	log_msg.Format(_T("正在查询(%s, %s->%s)，请等待..."), win32_A2U(m_ticket_dto.get_train_date().c_str()),
		win32_A2U(m_ticket_dto.get_from_station_name().c_str()), win32_A2U(m_ticket_dto.get_to_station_name().c_str()));
	svTicketRunLogPush(log_msg);

	if (!m_surplus_query)
	{
		m_bret = gl_manage.query_train_data(m_ticket_dto);
	}
	else
	{
		m_bret = gl_manage.query_train_data_surplus(m_ticket_dto);
	}
	
	if (!m_bret)
	{
		log_msg.Format(_T("未查询到满足您查询设置中需求的车次或席位"));
		svTicketRunLogPush(log_msg);
		//svTicketRunLogPush(win32_A2U(gl_manage.get_error_buffer().c_str()));
		return ;
	}

	std::vector<train_data> data_list = m_surplus_query?
		gl_manage.get_train_data_surplus_list()
		:gl_manage.get_train_data_list();

	for (unsigned int idx = 0; idx < data_list.size(); ++idx)
	{
		int irow = idx + 1;
		m_Grid.InsertRow(_T(""));
		m_Grid.SetRowHeight(irow, 38);
		for (int icol = 0; icol < 16; ++icol)
		{
			CString str;
			GV_ITEM Item;
			Item.mask = GVIF_TEXT;
			Item.row = irow;
			Item.col = icol;

			switch (icol)
			{
			case 0:
				str = win32_A2U(data_list[idx].station_train_code.c_str());
				break;
			case 1:
				str = win32_A2U(data_list[idx].from_station_name.c_str());
				if (data_list[idx].start_station_telecode == data_list[idx].from_station_telecode)
				{
					str += _T("[始]\r\n");
				}
				else
				{
					str += "\r\n";
				}
				str += win32_A2U(data_list[idx].start_time.c_str());
				break;
			case 2:
				str = win32_A2U(data_list[idx].to_station_name.c_str());
				if (data_list[idx].to_station_telecode == data_list[idx].end_station_telecode)
				{
					str += "[终]\r\n";
				}
				else
				{
					str += "\r\n";
				}
				str += win32_A2U(data_list[idx].arrive_time.c_str());
				break;
			case 3:
				str = win32_A2U(data_list[idx].lishi.c_str());
				if (data_list[idx].day_difference == "0")
				{
					str += "\r\n当日到达";
				}
				else if (data_list[idx].day_difference == "1")
				{
					str += "\r\n次日到达";
				}
				else if (data_list[idx].day_difference == "2")
				{
					str += "\r\n两日到达";
				}
				else if (data_list[idx].day_difference == "3")
				{
					str += "\r\n三日到达";
				}

				break;
			case 4:
				str = win32_A2U(data_list[idx].swz_num.c_str());
				break;
			case 5:
				str = win32_A2U(data_list[idx].tz_num.c_str());
				break;
			case 6:
				str = win32_A2U(data_list[idx].zy_num.c_str());
				break;
			case 7:
				str = win32_A2U(data_list[idx].ze_num.c_str());
				break;
			case 8:
				str = win32_A2U(data_list[idx].gr_num.c_str());
				break;
			case 9:
				str = win32_A2U(data_list[idx].rw_num.c_str());
				break;
			case 10:
				str = win32_A2U(data_list[idx].yw_num.c_str());
				break;
			case 11:
				str = win32_A2U(data_list[idx].rz_num.c_str());
				break;
			case 12:
				str = win32_A2U(data_list[idx].yz_num.c_str());
				break;
			case 13:
				str = win32_A2U(data_list[idx].wz_num.c_str());
				break;
			case 14:
				str = win32_A2U(data_list[idx].qt_num.c_str());
				break;
			case 15:
				str = win32_A2U(data_list[idx].buttonTextInfo.c_str());
				str.Replace(_T("<br/>"), _T("\r\n"));
				str.Replace(_T("起售"), _T("售"));
				break;
			default:
				break;
			}

			Item.strText =str;

//  		if (rand() % 10 == 1)
//  		{
//  			COLORREF clr = RGB(rand()%128 + 128, rand()%128 + 128, rand()%128 + 128);
//  			Item.crBkClr = clr;             // or - m_Grid.SetItemBkColour(row, col, clr);
//  			Item.crFgClr = RGB(255,0,0);    // or - m_Grid.SetItemFgColour(row, col, RGB(255,0,0));				    
//  			Item.mask    |= (GVIF_BKCLR|GVIF_FGCLR);
//  		}

			m_Grid.SetItem(&Item);

			m_Grid.SetItemState(irow, icol, m_Grid.GetItemState(irow, icol) | GVIS_READONLY);
			if (icol == 1 || icol == 2 || icol == 3 || (icol == 15 && str != _T("预订")))
			{
				m_Grid.SetItemFormat(irow,icol, DT_VCENTER|DT_CENTER);
			}
			else
			{
				m_Grid.SetItemFormat(irow,icol, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
			}

			if (icol == 0 || (icol == 15 && str == _T("预订")) )
			{
				m_Grid.SetCellType(irow,icol, RUNTIME_CLASS(CGridURLCell));
			}
		}
	}

// #if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
// m_Grid.AutoSize();
// #endif

	UpdateData(FALSE);
	
	Sleep(200);
	Invalidate(TRUE);
}


void CsvTicketMDlg::OnBnClickedBtnChangeStation()
{
	CString stext_from, stext_to;
	GetDlgItemText(IDC_FROM_STATION_NAME, stext_from);
	GetDlgItemText(IDC_TO_STATION_NAME, stext_to);

	m_from_station.SetItemText(stext_to);
	m_to_station.SetItemText(stext_from);

	UpdateData(FALSE);
}

LRESULT CsvTicketMDlg::OnGridCellClick(WPARAM wParam, LPARAM lParam)
{
	if ((unsigned int)lParam == 0)	// 车次
	{
		OnClickedStopStation(wParam, lParam);
	}
	else if ((unsigned int)lParam == 15)	// 预定
	{
		OnClickedSubmitOrder(wParam, lParam);
	}

	return 0;
}

void CsvTicketMDlg::OnClickedStopStation(int wp, int lp)
{
	gl_manage.query_stop_station(wp-1);
	CStopStationDlg stop_stat_dlg;
	stop_stat_dlg.stop_stat = gl_manage.get_stop_station();

	stop_stat_dlg.DoModal();

}


BOOL CsvTicketMDlg::PreTranslateMessage(MSG* pMsg)
{
	// 屏蔽 esc 退出窗口事件响应
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CsvTicketMDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(1);
		OnInitLogin();
	}

	CDialogEx::OnTimer(nIDEvent);
}

bool CsvTicketMDlg::OnInitLogin()
{
	CLoginDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString username = win32_A2U(gl_manage.get_username_cn().c_str());
		CString msg;
		msg.Format(_T("用户：[%s] 成功登录。"), username);
		svTicketRunLogPush(msg);

		return true;
	}
	
	return false;
}

void CsvTicketMDlg::OnStnClickedBtnSelect()
{
	CPassengerDlg passenger_dlg;
	if (passenger_dlg.DoModal() == IDOK)
	{
		m_passenger_list.DeleteAllItems();
		std::vector<passenger_datum> passenger_list = gl_manage.get_passengers().get_passenger_datum_list();

		std::vector<passenger_datum>::iterator iter = passenger_list.begin();
		int i = 0;
		for (iter; iter != passenger_list.end(); ++iter, ++i)
		{
			passenger_datum passenger = *iter;

			int col = m_passenger_list.InsertItem(i, win32_A2U(passenger.get_passenger_name().c_str()));
			m_passenger_list.SetItemText(col, 1, win32_A2U(passenger.get_passenger_id_no().c_str()));
			
			m_passenger_list.SetItemText(col, 2, win32_A2U(passenger.get_seat_type_name().c_str()));
			m_passenger_list.SetItemText(col, 3, win32_A2U(passenger.get_ticket_type_name().c_str()));
// 			if (passenger.get_ticket_type_name() == "1")
// 			{
// 				m_passenger_list.SetItemText(col, 3, _T("成人"));
// 			}
// 			else if (passenger.get_ticket_type() == "2")
// 			{
// 				m_passenger_list.SetItemText(col, 3, _T("儿童"));
// 			}
		}

		// 同时更新check_order 中的乘客列表
		m_checkorder_dlg->updatePassengerInfo();
	}

	UpdateData(FALSE);
}

// 预定
void CsvTicketMDlg::OnClickedSubmitOrder( int wp, int lp )
{
	ShowCheckOrderDialog();

	CString log_msg;
	while (!gl_manage.query_train_data(m_ticket_dto))
	{
		svTicketRunLogPush(_T("预定车票查询..."));
		Sleep(1000);
	}

	m_checkorder_dlg->m_train_info = gl_manage.get_train_data_list()[wp-1];
//	m_checkorder_dlg->m_train_info = (/*train_data_vec.size() == 0 || */gl_manage.get_train_data_list().size() <= (unsigned)wp) ?
//		gl_manage.get_train_data_surplus_list()[wp-1]:gl_manage.get_train_data_list()[wp-1];

	m_checkorder_dlg->updateTrainInfo();

	UpdateData(FALSE);

	if (!gl_manage.submit_order_request(m_checkorder_dlg->m_train_info))
	{
		svTicketRunLogPush(win32_A2U(gl_manage.get_error_buffer().c_str()));
	}
	;
}

void CsvTicketMDlg::ShowCheckOrderDialog( bool bshow /*= true*/ )
{
	static bool static_show_status = false;

	CRect mrect, orect;

	if (bshow)	// 显示checkorder界面
	{
		if (!static_show_status)
		{
			static_show_status = true;

			GetDlgItem(IDC_GRID)->GetWindowRect(&mrect);
			m_checkorder_dlg->GetWindowRect(&orect);

			ScreenToClient(mrect);
			mrect.bottom -= 158;
			mrect.right += 4;

			GetDlgItem(IDC_GRID)->MoveWindow(mrect);

			orect.left = mrect.left;
			orect.bottom = mrect.bottom + orect.Height();
			orect.top = mrect.bottom;
			orect.right = mrect.right;
			m_checkorder_dlg->MoveWindow(orect);

			m_checkorder_dlg->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_SUBMIT_ORDER)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (static_show_status)
		{
			static_show_status = false;

			GetDlgItem(IDC_GRID)->GetWindowRect(&mrect);
			m_checkorder_dlg->GetWindowRect(&orect);

			ScreenToClient(mrect);
			mrect.bottom += 158;

			GetDlgItem(IDC_GRID)->MoveWindow(mrect);
			m_checkorder_dlg->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_SUBMIT_ORDER)->ShowWindow(SW_HIDE);
		}
	}

	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CsvTicketMDlg::svTicketRunLogPush( CString str_msg )
{
	CString pre_str;
	pre_str += win32_A2U(svhttp::date_time().to_time_string(true).c_str());
	pre_str += _T("  ");
	pre_str += str_msg;
	pre_str += _T("\r\n");

	CRichEditCtrl *m_richedit_runlog = (CRichEditCtrl *)GetDlgItem(IDC_EDIT_RUNLOG);
	m_richedit_runlog->SetSel(-1,-1);
	m_richedit_runlog->ReplaceSel(pre_str);
	// 日志信息滚到最后一行。
	m_richedit_runlog->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	UpdateData(FALSE);
}

// 提交订单
void CsvTicketMDlg::OnBnClickedBtnSubmitOrder()
{
	CString log_msg;
	// todo: 更新乘客席位信息

	// 开始提交。
	// 验证码自动识别
	std::string pass_str = xdecaptcha::get_instance()->get_vcode_from_file(std::string("./data/pass.png"));

	while (!gl_manage.passenger_passcode_verify(pass_str))
	{
		log_msg.Format(_T("验证码：%s 识别错误."), win32_A2U(pass_str.c_str()));
		svTicketRunLogPush(log_msg);
		gl_manage.passenger_passcode_reflush();
		pass_str = xdecaptcha::get_instance()->get_vcode_from_file(std::string("./data/pass.png"));
	}
	log_msg.Format(_T("验证码：%s 识别成功."), win32_A2U(pass_str.c_str()));
	svTicketRunLogPush(log_msg);
}
