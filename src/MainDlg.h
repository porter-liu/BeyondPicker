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

	BEGIN_MSG_MAP_EX( CMainDlg )  // Ϊ��֧���������ڵ���Ϣӳ�䣬���ﲻ���� BEGIN_MSG_MAP()
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
	bool          m_bInDragMode;  // �Ƿ����϶�ģʽ��
	HBRUSH        m_hBrush;
	HDC           m_hMemDC;
	HBITMAP       m_hBmp;
	HBITMAP       m_hBmpOld;
	CContainedWindowT<CStatic> m_ZoomWnd;

	HICON         m_hIconTopmost;
	HICON         m_hIconNull;
	HCURSOR       m_hCursorTopmost;    // ָʾ����ʹ���� Topmost �Ĺ��
	HCURSOR       m_hCursorNoTopmost;  // ָʾ����ʹ���� NoTopmost �Ĺ��
	HCURSOR       m_hCursorCanNotDo;   // ָʾ���ܲ����Ĺ��
	HCURSOR       m_hCursorHand;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif  // __MAINDLG_H__20050828__
