#pragma once
#include "afxcmn.h"

#include "svTicketMDlg.h"
// CPassengerDlg 对话框

class CPassengerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassengerDlg)

public:
	CPassengerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPassengerDlg();

// 对话框数据
	enum { IDD = IDD_PASSENGER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnInitGridData();
	CString getPassengerIdStatus(contacts_datum& dto);
	CListCtrl m_passenger_list;
	
	afx_msg void OnNMDblclkPassengerList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickPassengerList(NMHDR *pNMHDR, LRESULT *pResult);

	bool ListCheckJudge(int iItem);	// listctrl checkbox选择判定<判定本次check事件是否应当响应>。
	afx_msg void OnBnClickedOk();
};
