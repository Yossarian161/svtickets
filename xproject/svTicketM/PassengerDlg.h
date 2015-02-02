#pragma once
#include "afxcmn.h"

#include "svTicketMDlg.h"
// CPassengerDlg �Ի���

class CPassengerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPassengerDlg)

public:
	CPassengerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPassengerDlg();

// �Ի�������
	enum { IDD = IDD_PASSENGER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	bool ListCheckJudge(int iItem);	// listctrl checkboxѡ���ж�<�ж�����check�¼��Ƿ�Ӧ����Ӧ>��
	afx_msg void OnBnClickedOk();
};
