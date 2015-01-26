
// svTicketMDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

#include "12306_help.h"
#include "GridCtrl_src/GridCtrl.h"
#include "autoComplete/StationEdit.h"

// CsvTicketMDlg �Ի���
class CsvTicketMDlg : public CDialogEx
{
// ����
public:
	CsvTicketMDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SVTICKETM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
