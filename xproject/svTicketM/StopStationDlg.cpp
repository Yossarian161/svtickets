// StopStationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "svTicketM.h"
#include "StopStationDlg.h"
#include "afxdialogex.h"

#include "bencode.hpp"

// CStopStationDlg �Ի���

IMPLEMENT_DYNAMIC(CStopStationDlg, CDialogEx)

CStopStationDlg::CStopStationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStopStationDlg::IDD, pParent)
{

}

CStopStationDlg::~CStopStationDlg()
{
}

void CStopStationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CStopStationDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CStopStationDlg ��Ϣ�������


BOOL CStopStationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	OnInitGridCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CStopStationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

#if _UNICODE
#define win32_A2U(T)	win32_utils::AnsiToUnicode(T)
#else
#define win32_A2U(T)	T
#endif

void CStopStationDlg::OnInitGridCtrl()
{
//	int nCol = 16;
	m_Grid.SetTrackFocusCell(FALSE);
	m_Grid.SetFrameFocusCell(FALSE);

	m_Grid.SetGridBkColor(RGB(255,255,255));
	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.EnableSelection(FALSE);
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(222,232,241));
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(5);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);

	m_Grid.SetScrollNoDisable(0, true);
	m_Grid.SetScrollNoDisable(1, true);

	CString TitleName[] = {
		_T("վ��"),
		_T("վ��"),
		_T("��վʱ��"),
		_T("����ʱ��"),
		_T("ͣ��ʱ��")
	};
	int irow = 0;
	for (int idx = 0; idx < 5; ++idx)
	{
		m_Grid.SetRowHeight(irow, 25);	
		m_Grid.SetColumnWidth(idx, 59);
		if (idx == 0)
		{
			m_Grid.SetColumnWidth(idx, 43);
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
	
	std::vector<station_info> stop_stat_list = stop_stat.get_stop_station_list();

	std::ostringstream osstr1;
	osstr1 << stop_stat.get_station_train_code() << "��   " 
		<< stop_stat.get_start_station_name() << "  -->  "
		<< stop_stat.get_end_station_name() << "   "
		<< stop_stat.get_train_class_name() << "   "
		<< ((stop_stat.get_service_type()=="0")?"�޿յ�":"�пյ�");

	SetWindowText(win32_A2U(osstr1.str().c_str()));
	//SetWindowText(win32_A2U((stop_stat.get_station_train_code()+"��").c_str()));

	std::ostringstream osstr;
	osstr << "   " << stop_stat.get_station_train_code() << "��      " 
		<< stop_stat.get_start_station_name() << "     -->     "
		<< stop_stat.get_end_station_name() << "       "
		<< stop_stat.get_train_class_name() << "      "
		<< ((stop_stat.get_service_type()=="0")?"�޿յ�":"�пյ�");

	SetDlgItemText(IDC_STATION_INFO, win32_A2U(osstr.str().c_str()));

	for (unsigned int idx = 0; idx < stop_stat_list.size(); ++idx)
	{
		int irow = idx + 1;
		m_Grid.InsertRow(_T(""));
		m_Grid.SetRowHeight(irow, 30);
		for (int icol = 0; icol < 5; ++icol)
		{
			CString str;
			GV_ITEM Item;
			Item.mask = GVIF_TEXT;
			Item.row = irow;
			Item.col = icol;

			switch (icol)
			{
			case 0:
				str = win32_A2U(stop_stat_list[idx].station_no.c_str());
				break;
			case 1:
				str = win32_A2U(stop_stat_list[idx].stat_name.c_str());
				break;
			case 2:
				str = win32_A2U(stop_stat_list[idx].arrive_time.c_str());
				break;
			case 3:
				str = win32_A2U(stop_stat_list[idx].start_time.c_str());
				break;
			case 4:
				str = win32_A2U(stop_stat_list[idx].stopover_time.c_str());
				break;
			default:
				break;
			}

			Item.strText =str;

 			if (!stop_stat_list[idx].isEnabled)
 			{
 				COLORREF clr = RGB(rand()%128 + 128, rand()%128 + 128, rand()%128 + 128);
 				Item.crBkClr = clr;             // or - m_Grid.SetItemBkColour(row, col, clr);
 				Item.crFgClr = clr;    // or - m_Grid.SetItemFgColour(row, col, RGB(255,0,0));				    
 				Item.mask    |= (GVIF_BKCLR|GVIF_FGCLR);
 			}

			m_Grid.SetItem(&Item);

			m_Grid.SetItemState(irow, icol, m_Grid.GetItemState(irow, icol) | GVIS_READONLY);
			m_Grid.SetItemFormat(irow,icol, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_NOPREFIX);
		}
	}

// #if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
// 	m_Grid.AutoSize(GVS_DEFAULT);
// #endif

	UpdateData(FALSE);
}
