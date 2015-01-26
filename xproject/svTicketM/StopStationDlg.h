#pragma once

#include "12306_help.h"
#include "GridCtrl_src/GridCtrl.h"
// CStopStationDlg �Ի���

class CStopStationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStopStationDlg)

public:
	CStopStationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStopStationDlg();

// �Ի�������
	enum { IDD = IDD_STOP_STATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	stop_station stop_stat;
	CGridCtrl m_Grid;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void OnInitGridCtrl();
};
