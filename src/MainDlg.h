// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MAINDLG_H__20050828__
#define __MAINDLG_H__20050828__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "AboutDlg.h"
#include "resource.h"


class CMainDlg : public CDialogImpl< CMainDlg >
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP_EX( CMainDlg )  // 为了支持容器窗口的消息映射，这里不能用 BEGIN_MSG_MAP()
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		MESSAGE_HANDLER( WM_LBUTTONDOWN, OnLButtonDown )
		MESSAGE_HANDLER( WM_MOUSEMOVE, OnMouseMove )
		MESSAGE_HANDLER( WM_LBUTTONUP, OnLButtonUp )
		MESSAGE_HANDLER( WM_SETCURSOR, OnSetCursor )
		MESSAGE_HANDLER( WM_CTLCOLORSTATIC, OnCtlColorStatic )
	ALT_MSG_MAP( 1 )
		MSG_WM_PAINT( OnPaint )
	END_MSG_MAP()

	CMainDlg();
	virtual ~CMainDlg();

	void ZoomIn( HDC hScreenDC, POINT *pPoint );
	bool ShowColorInfo( COLORREF crColor );

private:
	LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnSetCursor( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnCtlColorStatic( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnPaint( HDC hDC );

	void ShowZoomBmp( void );

private:
	CTrackBarCtrl m_TrackBar;
	CStatic       m_TopMost;
	bool          m_bInDragMode;  // 是否在拖动模式中
	HBRUSH        m_hBrush;
	HDC           m_hMemDC;
	HBITMAP       m_hBmp;
	HBITMAP       m_hBmpOld;
	CContainedWindowT<CStatic> m_ZoomWnd;

	HICON         m_hIconTopmost;
	HICON         m_hIconNull;
	HCURSOR       m_hCursorTopmost;    // 指示可以使窗口 Topmost 的光标
	HCURSOR       m_hCursorNoTopmost;  // 指示可以使窗口 NoTopmost 的光标
	HCURSOR       m_hCursorCanNotDo;   // 指示不能操作的光标
	HCURSOR       m_hCursorHand;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif  // __MAINDLG_H__20050828__
