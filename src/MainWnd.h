#ifndef __MAINWND_H__20050928__
#define __MAINWND_H__20050928__


#include "ColorSchemesDlg.h"


class CMainWnd : public CWindowImpl< CMainWnd >
{
public:
	DECLARE_WND_CLASS( _T(GetAppName()) )

	BEGIN_MSG_MAP( CMainWnd )
		MESSAGE_HANDLER( WM_CREATE, OnCreate )
		MESSAGE_HANDLER( WM_DESTROY, OnDestroy )
		MESSAGE_HANDLER( WM_DRAWITEM, OnDrawItem )
		MESSAGE_HANDLER( WM_TIMER, OnTimer )
		MESSAGE_HANDLER( WM_TIMER, OnTimer )
		MESSAGE_HANDLER( WM_INITMENUPOPUP, OnInitMenuPopup )
		COMMAND_ID_HANDLER( IDC_TEMPLATE, OnTemplate )
		COMMAND_ID_HANDLER( IDC_CUSTOM, OnCustom )
		COMMAND_ID_HANDLER( IDC_DODGE_CURSOR, OnDodgeCursor )
		COMMAND_ID_HANDLER( IDC_IS_ZOOM, OnZoomIn )
		COMMAND_ID_HANDLER( IDC_COLOR_SCHEMES, OnColorSchemes )
		COMMAND_ID_HANDLER( IDC_ADD_COLOR, OnAddColor )
		COMMAND_ID_HANDLER( IDC_ABOUT, OnAbout )
		COMMAND_ID_HANDLER( IDC_EXIT, OnExit )
		COMMAND_RANGE_HANDLER( WM_USER + 1, WM_USER + TEMPLATE_COUNT, OnApplyTemplate )
		NOTIFY_CODE_HANDLER( TBN_DROPDOWN, OnTBDropDown )
	END_MSG_MAP()

	CMainWnd();
	virtual ~CMainWnd();

private:
	LRESULT OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnDrawItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnInitMenuPopup( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );

	LRESULT OnApplyTemplate( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnTemplate( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnCustom( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnDodgeCursor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnZoomIn( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnColorSchemes( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnAddColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnAbout( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnExit( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
	{
		DestroyWindow();
		return( 0 );
	}

	LRESULT OnTBDropDown( int idCtrl, LPNMHDR pnmh, BOOL &bHandled );

private:
	bool UpdateStatus( bool bForceUpdate = false );
	void DodgeCursor( POINT *pPoint );
	void SyncCopyMenu( void );

private:
	CToolBarCtrl   m_ToolBar;
	CReBarCtrl     m_ReBar;
	CStatusBarCtrl m_StatusBar;

	bool           m_bEnabled;      // 取色模式是否开启
	bool           m_bDodgeCursor;  // 鼠标躲避是否开启
	bool           m_bZoomIn;       // 放大鼠标区域是否开启
	RECT           m_rcScreen[4];   // 把整个屏幕分成 4 块躲避区域
	HICON          m_hIconRed;
	HICON          m_hIconGreen;

	CColorSchemesDlg m_ColorSchemesDlg;
};


#endif  // __MAINWND_H__20050928__