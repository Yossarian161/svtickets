// CheckOrderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "svTicketM.h"
#include "CheckOrderDlg.h"
#include "afxdialogex.h"
#include "svTicketMDlg.h"


// CCheckOrderDlg 对话框

IMPLEMENT_DYNAMIC(CCheckOrderDlg, CDialogEx)

CCheckOrderDlg::CCheckOrderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckOrderDlg::IDD, pParent)
{
}

CCheckOrderDlg::~CCheckOrderDlg()
{
}

void CCheckOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ORDER_LIST, m_Grid);
}


BEGIN_MESSAGE_MAP(CCheckOrderDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCheckOrderDlg 消息处理程序


BOOL CCheckOrderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OnInitGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CCheckOrderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CCheckOrderDlg::PreTranslateMessage(MSG* pMsg)
{
	// 屏蔽 esc 退出窗口事件响应 && 以及enter键
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCheckOrderDlg::OnInitGrid()
{
	int cols_num = 7;
	m_Grid.SetTrackFocusCell(FALSE);
	m_Grid.SetFrameFocusCell(FALSE);

	m_Grid.SetGridBkColor(RGB(255,255,255));
	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.EnableSelection(FALSE);
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(222,232,242/*0xFF, 0xFF, 0xE0*/));
	
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(cols_num);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	m_Grid.SetScrollNoDisable(0, true);
	m_Grid.SetScrollNoDisable(1, true);

	m_Grid.SetFixedBkColor(RGB(200,220,240));

	CString TitleName[] = {
		_T("序号"),
		_T("票种"),
		_T("席别"),
		_T("姓名"),
		_T("证件类型"),
		_T("证件号码"),
		_T("手机号码")
	};
	int irow = 0;
	for (int idx = 0; idx < cols_num; ++idx)
	{
		m_Grid.SetRowHeight(irow, 20);
		m_Grid.SetColumnWidth(idx, 59);
		
		int nwidth = 0;
		switch (idx)
		{
		case 0:
			nwidth = 77;
			break;
		case 1:
		case 2:
		case 3:
			nwidth = 85;
			break;
		case 4:
			nwidth = 120;
			break;
		case 5:
			nwidth = 190;
			break;
		case 6:
			nwidth = 145;
			break;
		default:
			break;
		}
 
		m_Grid.SetColumnWidth(idx, nwidth);
 
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

bool CCheckOrderDlg::is_num(std::string str)
{
	std::stringstream sin(str);
	double d;
	char c;
	if(!(sin >> d))
		return false;
	if (sin >> c)
		return false;
	return true;
}

void CCheckOrderDlg::updateTrainInfo()
{
	// 显示车次详情<票价、余票信息等>
	std::ostringstream osstr;
	CString str;
	if (!(std::string(m_train_info.train_no)).empty())
	{
		gl_manage.query_ticket_price(m_train_info);
		ticket_price t_price = gl_manage.get_ticket_price();

		osstr << m_train_info.train_date << "  " << m_train_info.station_train_code << "次  "
			<< m_train_info.from_station_name << "(" << m_train_info.start_time << "开)  -->  "
			<< m_train_info.to_station_name << "(" << m_train_info.arrive_time << "到)\r\n"
			;

		std::vector<std::string> seat_info = m_train_info.get_train_seat_type_queue();
		std::vector<std::string>::iterator iter = seat_info.begin();
		for (iter; iter < seat_info.end(); ++iter)
		{
			osstr << seats_type::get_seat_type_name(*iter) << "(￥" << t_price.get_ticket_seat_price(*iter)
				<< ")" << m_train_info.get_seat_num(*iter) << (is_num(m_train_info.get_seat_num(*iter))?"张":"票") <<"  ";
		}

		str = win32_A2U(osstr.str().c_str());
		SetDlgItemText(IDC_TRAIN_INFO, str);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 显示订票乘客 席位等信息
	// updatePassengerInfo();
	UpdateData(FALSE);
}

void CCheckOrderDlg::updatePassengerInfo()
{
	UpdateData();
	// 显示订票乘客 席位等信息
	m_Grid.DeleteNonFixedRows();

	std::vector<passenger_datum> passenger_list = gl_manage.get_passengers().get_passenger_datum_list();
	for (unsigned int idx = 0; idx < passenger_list.size(); ++idx)
	{
		int irow = idx + 1;
		m_Grid.InsertRow(_T(""));
		m_Grid.SetRowHeight(irow, 21);
		for (int icol = 0; icol < m_Grid.GetColumnCount(); ++icol)
		{
			CString str;
			GV_ITEM Item;
			Item.mask = GVIF_TEXT;
			Item.row = irow;
			Item.col = icol;

			switch (icol)
			{
			case 0:
				str.Format(_T("第%d位"), idx+1);
				break;
			case 1:
				str = win32_A2U(passenger_list[idx].get_ticket_type_name().c_str());
				break;
			case 2:
				str = win32_A2U(passenger_list[idx].get_seat_type_name().c_str());
				break;
			case 3:
				str = win32_A2U(passenger_list[idx].get_passenger_name().c_str());
				break;
			case 4:
				str = win32_A2U(passenger_list[idx].get_passenger_id_type_name().c_str());
				break;
			case 5:
				str = win32_A2U(passenger_list[idx].get_passenger_id_no().c_str());
				break;
			case 6:
				str = win32_A2U(passenger_list[idx].get_mobile_no().c_str());
				break;
			default:
				break;
			}

			Item.strText =str;

			m_Grid.SetItem(&Item);

			m_Grid.SetItemState(irow, icol, m_Grid.GetItemState(irow, icol) | GVIS_READONLY);
			m_Grid.SetItemFormat(irow,icol, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_NOPREFIX);
		}
	}

	UpdateData(FALSE);
	Invalidate(TRUE);
}



