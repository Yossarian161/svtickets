#pragma once
#include "gridctrl_src\gridctrl.h"
#include "svticket.h"

// CCheckOrderDlg 对话框

class CCheckOrderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckOrderDlg)

public:
	CCheckOrderDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckOrderDlg();

// 对话框数据
	enum { IDD = IDD_CHECK_ORDER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_Grid;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void OnInitGrid();
	void updateTrainInfo();
	void updatePassengerInfo();
	void applyPassengerInfo();

	int get_passenger_count();

	train_data m_train_info;
	// combox 下拉列表。。
	CStringArray m_seat_array;
	CStringArray m_ticket_array;

	bool is_num(std::string str);
};
