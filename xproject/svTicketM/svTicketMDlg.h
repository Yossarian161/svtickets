
// svTicketMDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

#include "svticket.h"
#include "GridCtrl_src/GridCtrl.h"
#include "autoComplete/StationEdit.h"

#include "CheckOrderDlg.h"

extern ticket_manage gl_manage;

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

	// ��ʾ/���� ����ȷ�� ����
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
