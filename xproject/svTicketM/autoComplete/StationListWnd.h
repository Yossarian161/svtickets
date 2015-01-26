#if !defined(AFX_ACWND_H__5CED9BF8_C1CB_4A74_B022_ABA25680CC42__INCLUDED_)
#define AFX_ACWND_H__5CED9BF8_C1CB_4A74_B022_ABA25680CC42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACWnd.h : Header-Datei
//
#if _UNICODE
#include "bencode.hpp"
#endif

#include<string>
#pragma warning(disable:4786)
#include <vector>
#include <map>

#ifndef _station_name_class
#define _station_name_class

class station_name
{
public:
	std::string stat_spell_logo;
	std::string stat_cn;
	std::string stat_code;
	std::string stat_spell_full;
	std::string stat_spell_logo2;
	std::string stat_id;
	
	inline bool parse_string(std::string stat_str)
	{
		if (stat_str.empty())
		{
			return false;
		}
		// bjb|北京北|VAP|beijingbei|bjb|0
		int x_pos, d_pos = 0;
		x_pos = stat_str.find("|");
		stat_spell_logo = stat_str.substr(0, x_pos);
		d_pos = stat_str.find("|", x_pos+1);
		stat_cn = stat_str.substr(x_pos+1, d_pos-x_pos-1);
		x_pos = stat_str.find("|", d_pos+1);
		stat_code = stat_str.substr(d_pos+1, x_pos-d_pos-1);
		d_pos = stat_str.find("|", x_pos+1);
		stat_spell_full = stat_str.substr(x_pos+1, d_pos-x_pos-1);
		x_pos = stat_str.find("|", d_pos+1);
		stat_spell_logo2 = stat_str.substr(d_pos+1, x_pos-d_pos-1);
		d_pos = stat_str.find("|", x_pos+1);
		stat_id = stat_str.substr(x_pos+1, d_pos-x_pos-1);
		
#if 0	
		// libiconv utf->bg2312 转换后，丢失字符处理
		if (_stat_code == "IBQ")
			_stat_cn = "北滘";
		if (_stat_code == "LEM")
			_stat_cn = "拉鲊";
		if (_stat_code == "KMQ")
			_stat_cn = "鲘门";
		if (_stat_code == "FWH")
			_stat_cn = "栟茶";
#endif

		return true;
	}
};

#endif
/*********************************************************************
*
* CStationListWnd
* Copyright (c) 200 by Andreas Kapust
* All rights reserved.
* info@akinstaller.de
*
*********************************************************************/

#include <afxtempl.h>		// CArray
#define ENAC_UPDATE        WM_USER + 1200
/////////////////////////////////////////////////////////////////////////////
// Fenster CStationListWnd
#define IDTimerInstall 10
class CStationListWnd : public CWnd
{
// Konstruktion
public:
	CStationListWnd();
	void Init(CWnd *pWnd);
	bool EnsureVisible(int item,bool m_bWait);
	bool SelectItem(int item);
	int FindString(int nStartAfter, LPCTSTR lpszString, bool m_bDisplayOnly = false);
	int FindStringExact( int nIndexStart, LPCTSTR lpszFind );
	int SelectString(LPCTSTR lpszString );
	bool GetText(int item, CString& m_Text);
	void AddSearchString(LPCTSTR lpszString)
	{
		m_SearchList.Add(lpszString);
	}

	//  [1/19/2015 Administrator]
	void AddSearchLists(std::vector<station_name> _list)
	{
		stat_name_list = _list;
		
		for (unsigned int idx = 0; idx < stat_name_list.size(); ++idx)
 		{
#if _UNICODE
 			m_SearchList.Add(win32_utils::AnsiToUnicode((stat_name_list[idx].stat_cn).c_str()));
			_stat_info_map[win32_utils::AnsiToUnicode(stat_name_list[idx].stat_cn.c_str())] = win32_utils::AnsiToUnicode(stat_name_list[idx].stat_code.c_str());
#else
			m_SearchList.Add((stat_name_list[idx].stat_cn).c_str());
			_stat_info_map[stat_name_list[idx].stat_cn.c_str()] = stat_name_list[idx].stat_code.c_str();
#endif
	 	}
	}

	void RemoveAll()
	{
		m_SearchList.RemoveAll();
		m_DisplayList.RemoveAll();
	}
	CString GetString();
	CString GetNextString(int m_iChar);

	void CopyList();
	void SortSearchList()
	{
		SortList(m_SearchList);
	}
// Attribute

	std::vector<station_name> stat_name_list;
	std::map<CString, CString> _stat_info_map;
	CString GetItemValue(CString& _str){ return _stat_info_map[_str];};

public:
	CListCtrl m_List;
	CString m_DisplayStr;
	TCHAR m_PrefixChar;
	long m_lMode;
// Operationen
public:
	CStringArray m_SearchList;
// 躡erschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions黚erschreibungen
	//{{AFX_VIRTUAL(CStationListWnd)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CStationListWnd();
	void DrawItem(CDC* pDC,long m_lItem,long width);

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CStationListWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
#if _MSC_VER > 1400
    afx_msg LRESULT OnNcHitTest(CPoint point);
#else
    afx_msg UINT OnNcHitTest(CPoint point);
#endif
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CStringArray m_DisplayList;
	CScrollBar m_VertBar, m_HoriBar;
	CRect m_LastSize, m_ParentRect;
	CFont *pFontDC;
	CFont fontDC, boldFontDC;
	CEdit *m_pEditParent;
	LOGFONT logfont;

	int m_nIDTimer;
	long m_lTopIndex,m_lCount,m_ItemHeight,m_VisibleItems,m_lSelItem;

	int HitTest(CPoint point);
	void SetScroller();
	void SetProp();
	long ScrollBarWidth();
	void InvalidateAndScroll();
	void SortList(CStringArray& m_List);
	static int CompareString(const void* p1, const void* p2);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f黦t unmittelbar vor der vorhergehenden Zeile zus鋞zliche Deklarationen ein.

#endif // AFX_ACWND_H__5CED9BF8_C1CB_4A74_B022_ABA25680CC42__INCLUDED_
