
// svTicketMDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

#include "12306_help.h"
#include "GridCtrl_src/GridCtrl.h"
#include "autoComplete/StationEdit.h"

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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnGridCellClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_Grid;
	void OnInitGridCtrl();
	afx_msg void OnBnClickedBtnQuery();
	CStationEdit m_from_station;
	CStationEdit m_to_station;

	ticket_manage gl_manage;
	CDateTimeCtrl m_train_date;
	afx_msg void OnBnClickedBtnChangeStation();

	void OnClickedStopStation(int wp, int lp);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
