#if !defined(AFX_StationEdit_H__56D21C13_ECEA_41DF_AADF_55980E861AC2__INCLUDED_)
#define AFX_StationEdit_H__56D21C13_ECEA_41DF_AADF_55980E861AC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StationEdit.h : Header-Datei
//

/*********************************************************************
*
* CStationEdit
* Copyright (c) 2003 by Andreas Kapust
* All rights reserved.
* info@akinstaller.de
*
*********************************************************************/


#define _MODE_ONLY_FILES		(1L << 16)
#define _MODE_ONLY_DIRS			(1L << 17)

#define _MODE_STANDARD_			(1L << 0)
#define _MODE_SEPARATION_		(1L << 1)
#define _MODE_FILESYSTEM_		(1L << 2)
#define _MODE_FS_START_DIR_		(1L << 3)
#define _MODE_CURSOR_O_LIST_	(1L << 4)
#define _MODE_FIND_ALL_			(1L << 5)

// #define _MODE_FS_ONLY_FILE_	(_MODE_FILESYSTEM_|_MODE_ONLY_FILES)
// #define _MODE_FS_ONLY_DIR_	(_MODE_FILESYSTEM_|_MODE_ONLY_DIRS)
// #define _MODE_SD_ONLY_FILE_	(_MODE_FS_START_DIR_|_MODE_ONLY_FILES)
// #define _MODE_SD_ONLY_DIR_	(_MODE_FS_START_DIR_|_MODE_ONLY_DIRS)  //Fix 1.2

/////////////////////////////////////////////////////////////////////////////
// Fenster CStationEdit
#include "StationListWnd.h"

class CStationEdit : public CWnd //CEdit
{
// Konstruktion
public:
	CStationEdit();
	void SetMode(int iMode=_MODE_STANDARD_);
	void SetSeparator(LPCTSTR lpszString,TCHAR lpszPrefixChar = 0)
	{
		m_SeparationStr = lpszString;
		m_Liste.m_PrefixChar = m_PrefixChar = lpszPrefixChar;
		SetMode(_MODE_SEPARATION_);
	}

	// CComboBox
	//int AddString( LPCTSTR lpszString);
	int GetLBText( int nIndex, LPTSTR lpszText );
	void GetLBText( int nIndex, CString& rString );
	int SetDroppedWidth(UINT nWidth);
	int FindString( int nStartAfter, LPCTSTR lpszString );
	int SelectString( int nStartAfter, LPCTSTR lpszString );
	void ShowDropDown(BOOL bShowIt = TRUE );
	void ResetContent();
	int GetCurSel();
	// Attribute
public:
	void Init();
	//void AddSearchString(LPCTSTR lpszString);
	void AddSearchStrings(std::vector<station_name>& _vec);
	void RemoveSearchAll();
	void SetStartDirectory(LPCTSTR lpszString);

	// add jianyaoy@gmail.com
	void SetSelectOnly(bool val = false){ _select_only = val; };
	bool _select_only;
	CString GetItemValue()
	{
		CString editText;
		GetWindowText(editText);
		return m_Liste.GetItemValue(editText); 
	}
	void SetItemText(CString val)
	{
		SetWindowText(val);
		OnKillfocus();
	}

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CStationEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CStationEdit();
	CStationListWnd m_Liste;
	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	CString m_EditText, m_SeparationStr,m_LastDirectory;
	TCHAR m_PrefixChar;
	int m_iMode;
	//{{AFX_MSG(CStationEdit)
	afx_msg void OnKillfocus();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	afx_msg void OnCloseList();
	//}}AFX_MSG
	afx_msg LONG OnUpdateFromList(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()


	int FindSepLeftPos(int pos, bool FindSepLeftPos = false);
	int FindSepLeftPos2(int pos);
	int FindSepRightPos(int pos);
	bool HandleKey(UINT nChar, bool m_bFromChild);

	bool m_CursorMode;
	int m_iType;
	CEdit *m_pEdit;

//	char m_szDrive[_MAX_DRIVE], m_szDir[_MAX_DIR],m_szFname[_MAX_FNAME], m_szExt[_MAX_EXT];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_StationEdit_H__56D21C13_ECEA_41DF_AADF_55980E861AC2__INCLUDED_
