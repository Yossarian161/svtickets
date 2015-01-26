
// svTicketMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "svTicketM.h"
#include "svTicketMDlg.h"
#include "afxdialogex.h"

#include "NewCellTypes/GridURLCell.h"

#include "StopStationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if _UNICODE
#define win32_A2U(T)	win32_utils::AnsiToUnicode(T)
#else
#define win32_A2U(T)	T
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
}

BEGIN_MESSAGE_MAP(CsvTicketMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_QUERY, &CsvTicketMDlg::OnBnClickedBtnQuery)
	ON_BN_CLICKED(IDC_BTN_CHANGE_STATION, &CsvTicketMDlg::OnBnClickedBtnChangeStation)
	ON_MESSAGE(WM_GRID_CELL_CLICK, &CsvTicketMDlg::OnGridCellClick)
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
	OnInitGridCtrl();

	m_train_date.SetFormat(_T("yyyy-MM-dd"));
	//
	gl_manage.login_init();
	gl_manage.get_station_name();

	m_from_station.Init();
	m_from_station.SetMode(_MODE_CURSOR_O_LIST_);

	m_from_station.AddSearchStrings(gl_manage.get_station_name_list());
	m_from_station.SetSelectOnly(true);

	m_to_station.Init();
	m_to_station.SetMode(_MODE_CURSOR_O_LIST_);

	m_to_station.AddSearchStrings(gl_manage.get_station_name_list());
	m_to_station.SetSelectOnly(true);

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

void CsvTicketMDlg::OnInitGridCtrl()
{
	int nCol = 16;

	m_Grid.SetFixedColumnSelection(TRUE);
//	m_Grid.SetFixedRowSelection(TRUE);
//	m_Grid.EnableColumnHide();
	m_Grid.EnableSelection(FALSE);
//	m_Grid.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);

	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
	//m_Grid.SetGridLineColor(RGB(0,255,0));
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
	UpdateData();
	bool m_surplus_query = true;	//启用余票查询

	CString stime;
	GetDlgItemText(IDC_TRAIN_DATE, stime);
	//AfxMessageBox(stime);

	// 清空grid历史数据
	m_Grid.DeleteNonFixedRows();

	//AfxMessageBox(m_from_station.GetItemValue());
	left_ticket_dto ticket_dto;
#if _UNICODE
	ticket_dto.set_from_station(win32_utils::UnicodeToAnsi(m_from_station.GetItemValue().GetBuffer(0)));
	ticket_dto.set_to_station(win32_utils::UnicodeToAnsi(m_to_station.GetItemValue().GetBuffer(0)));
	ticket_dto.set_train_date(win32_utils::UnicodeToAnsi(stime.GetBuffer(0)));
#else
	ticket_dto.set_from_station(m_from_station.GetItemValue().GetBuffer(0));
	ticket_dto.set_to_station(m_to_station.GetItemValue().GetBuffer(0));
	ticket_dto.set_train_date(stime.GetBuffer(0));
#endif

	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK_STUDENT))->GetCheck())
	{
		ticket_dto.set_purpose_codes(true);
	}
	else
		ticket_dto.set_purpose_codes();
	
	if (!m_surplus_query)
	{
		_manage.query_train_data(ticket_dto);
	}
	else
	{
		_manage.query_train_data_surplus(ticket_dto);
	}
	
	std::vector<train_data> data_list = m_surplus_query?
		_manage.get_train_data_surplus_list()
		:_manage.get_train_data_list();

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
	if ((unsigned int)lParam == 0)
	{
		OnClickedStopStation(wParam, lParam);
	}

	return 0;
}

void CsvTicketMDlg::OnClickedStopStation(int wp, int lp)
{
	_manage.query_stop_station(wp-1);
	CStopStationDlg stop_stat_dlg;
	stop_stat_dlg.stop_stat = _manage.get_stop_station();

	stop_stat_dlg.DoModal();

}


BOOL CsvTicketMDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
