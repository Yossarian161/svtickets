// PassengerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "svTicketM.h"
#include "PassengerDlg.h"
#include "afxdialogex.h"

// CPassengerDlg �Ի���

IMPLEMENT_DYNAMIC(CPassengerDlg, CDialogEx)

CPassengerDlg::CPassengerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassengerDlg::IDD, pParent)
{

}

CPassengerDlg::~CPassengerDlg()
{
}

void CPassengerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PASSENGER_LIST, m_passenger_list);
}


BEGIN_MESSAGE_MAP(CPassengerDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_PASSENGER_LIST, &CPassengerDlg::OnNMDblclkPassengerList)
	ON_NOTIFY(NM_CLICK, IDC_PASSENGER_LIST, &CPassengerDlg::OnNMClickPassengerList)
	ON_BN_CLICKED(IDOK, &CPassengerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassengerDlg ��Ϣ�������


BOOL CPassengerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// itemǰ����checkbox�ؼ�  LVS_EX_CHECKBOXES
	m_passenger_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

	m_passenger_list.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60);
	m_passenger_list.InsertColumn(1, _T("����"), LVCFMT_LEFT, 70);
	m_passenger_list.InsertColumn(2, _T("֤������"), LVCFMT_LEFT, 100);
	m_passenger_list.InsertColumn(3, _T("֤������"), LVCFMT_LEFT, 150);
	m_passenger_list.InsertColumn(4, _T("�ֻ�/�绰"), LVCFMT_LEFT, 100);
	m_passenger_list.InsertColumn(5, _T("�ÿ�����"), LVCFMT_LEFT, 70);
	m_passenger_list.InsertColumn(6, _T("����״̬"), LVCFMT_LEFT, 70);

	m_passenger_list.SetTextBkColor(RGB(220,235,245));

	OnInitGridData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CPassengerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CPassengerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);
}

void CPassengerDlg::OnInitGridData()
{
	if ((gl_manage.get_passengers().get_contacts_datum_list()).size() == 0)
	{
		gl_manage.query_passengers();
	}
	
	std::vector<contacts_datum> contacts_list = gl_manage.get_passengers().get_contacts_datum_list();

	std::vector<contacts_datum>::iterator iter = contacts_list.begin();
	int i = 0;
	for (iter; iter != contacts_list.end(); ++iter, ++i)
	{
		contacts_datum contacts = *iter;

		CString sID;
		sID.Format(_T("%d"), i+1);
		int col = m_passenger_list.InsertItem(i, sID);
		m_passenger_list.SetItemText(col, 1, win32_A2U(contacts.passenger_name.c_str()));
		m_passenger_list.SetItemText(col, 2, win32_A2U(contacts.passenger_id_type_name.c_str()));
		m_passenger_list.SetItemText(col, 3, win32_A2U(contacts.passenger_id_no.c_str()));
		m_passenger_list.SetItemText(col, 4, win32_A2U(contacts.mobile_no.c_str()));
		m_passenger_list.SetItemText(col, 5, win32_A2U(contacts.passenger_type_name.c_str()));
		m_passenger_list.SetItemText(col, 6, getPassengerIdStatus(contacts));

		if (gl_manage.get_passengers().passenger_selected_judge(contacts))
		{
			m_passenger_list.SetCheck(col);
		}
	}
}

CString CPassengerDlg::getPassengerIdStatus(contacts_datum& contacts)
{
	CString id_status_str;	// ��ݺ���״̬ 
	if (contacts.passenger_id_type_code == "2")
	{
		// ����վ����֧��һ���������֤�������Ϊ�����������֤.
		id_status_str = _T("δͨ��");
	}
	else
	{
		if (contacts.total_times == "95" || contacts.total_times == "97")
		{
			id_status_str = _T("��ͨ��");
		}
		else
		{
			if (contacts.total_times == "93" || contacts.total_times == "99")
			{
				if (contacts.passenger_id_type_code == "1")
				{
					id_status_str = _T("��ͨ��");
				}
				else
				{
					id_status_str = _T("Ԥͨ��");
				}
			}
			else
			{
				if (contacts.total_times == "94" || contacts.total_times == "96")
				{
					id_status_str = _T("δͨ��");
				}
				else
				{
					if (contacts.total_times == "92" || contacts.total_times == "98")
					{
						if (contacts.passenger_id_type_code == "B" || contacts.passenger_id_type_code == "G" 
							|| contacts.passenger_id_type_code == "C" )
						{
							id_status_str = _T("�뱨��");
						}
						else
						{
							id_status_str = _T("������");
						}
					}
					else
					{
						id_status_str = _T("�뱨��");
					}
				}
			}
		}
	}

	return id_status_str;
}



void CPassengerDlg::OnNMDblclkPassengerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (!ListCheckJudge(pNMItemActivate->iItem))
	{
		AfxMessageBox(_T("���ֻ��ѡ��5λ�˿�"));
	}
	else
	{
		if (!m_passenger_list.GetCheck(pNMItemActivate->iItem))
		{
			gl_manage.get_passengers().passenger_datum_selected_push(pNMItemActivate->iItem);
		}
		else
		{
			gl_manage.get_passengers().passenger_datum_selected_pop(pNMItemActivate->iItem);
		}

		m_passenger_list.SetCheck(pNMItemActivate->iItem, !m_passenger_list.GetCheck(pNMItemActivate->iItem));
	}
	
	*pResult = 0;
}


void CPassengerDlg::OnNMClickPassengerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);


	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  

	m_passenger_list.ScreenToClient(&point);  

	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	lvinfo.flags = LVHT_ABOVE;  

	UINT nFlag;  
	int nItem = m_passenger_list.HitTest(point, &nFlag);  
	//�ж��Ƿ����checkbox��  
	if(nFlag == LVHT_ONITEMSTATEICON)  
	{
// 		TRACE("You click CheckBox,Item will be Clicked!");
// 		TRACE("status: %d-%d-%d \n", pNMItemActivate->iItem, nItem,
// 			(int)m_passenger_list.GetCheck(pNMItemActivate->iItem));
// 		// ��ȡ��checkbox״̬Ϊ���ǰ��״̬

		if (!ListCheckJudge(nItem))
		{
			AfxMessageBox(_T("���ֻ��ѡ��5λ�˿�"));
			*pResult = 1;
			return ;
		}
		else
		{
			if (!m_passenger_list.GetCheck(pNMItemActivate->iItem))
			{
				gl_manage.get_passengers().passenger_datum_selected_push(pNMItemActivate->iItem);
			}
			else
			{
				gl_manage.get_passengers().passenger_datum_selected_pop(pNMItemActivate->iItem);
			}
		}
		
	}

	*pResult = 0;
}

bool CPassengerDlg::ListCheckJudge( int iItem )
{
	int check_num = gl_manage.get_passengers().get_passenger_datum_list().size();

	// getcheck ��ȡ���ǵ�ǰcheck�¼���Ӧǰ��״̬
	if (m_passenger_list.GetCheck(iItem))
	{
		--check_num;
	}
	else
	{
		++check_num;
	}

	if (check_num > 5)
	{
		return false;
	}

	return true;
}


void CPassengerDlg::OnBnClickedOk()
{
// 	passenger_dto& _manager = gl_manage.get_passengers();
// 
// 	_manager.passenger_datum_selected_clear();
// 
// 	for (int idx = 0; idx < m_passenger_list.GetItemCount(); ++idx)
// 	{
// 		if (m_passenger_list.GetCheck(idx))
// 		{
// 			_manager.passenger_datum_selected_push(idx);
// 		}
// 	}

	OnOK();
}
