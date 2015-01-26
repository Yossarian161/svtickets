#pragma once

#include "12306_help.h"
#include "GridCtrl_src/GridCtrl.h"
// CStopStationDlg 对话框

class CStopStationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStopStationDlg)

public:
	CStopStationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStopStationDlg();

// 对话框数据
	enum { IDD = IDD_STOP_STATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	stop_station stop_stat;
	CGridCtrl m_Grid;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void OnInitGridCtrl();
};
