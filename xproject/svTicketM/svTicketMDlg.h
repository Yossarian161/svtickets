
// svTicketMDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

#include "svticket.h"
#include "GridCtrl_src/GridCtrl.h"
#include "autoComplete/StationEdit.h"

#include "CheckOrderDlg.h"

extern ticket_manage gl_manage;

// CsvTicketMDlg 对话框
class CsvTicketMDlg : public CDialogEx
{
// 构造
public:
	CsvTicketMDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SVTICKETM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnGridCellClick(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	void OnInitControl();
	void OnInitGridCtrl();
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnBnClickedBtnChangeStation();
	afx_msg void OnStnClickedBtnSelect();

	bool OnInitLogin();

	void OnClickedStopStation(int wp, int lp);
	void OnClickedSubmitOrder(int wp, int lp);

	// 显示/隐藏 订单确认 画面
	void ShowCheckOrderDialog(bool bshow = true);
	// 
	void svTicketRunLogPush(CString str_msg);
public:
	CGridCtrl m_Grid;
	
	CStationEdit m_from_station;
	CStationEdit m_to_station;
	CDateTimeCtrl m_train_date;

	left_ticket_dto m_ticket_dto;

	CListCtrl m_passenger_list;

	CCheckOrderDlg* m_checkorder_dlg;
	afx_msg void OnBnClickedBtnSubmitOrder();
};
