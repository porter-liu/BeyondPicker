#ifndef __EDITCOLORDLG_H__20051013__
#define __EDITCOLORDLG_H__20051013__


#include "resource.h"


struct Scheme
{
	string    m_strName;    // 名称
	HTREEITEM m_hItem;      // 对应的 TreeView Item
	bool      m_bSelected;  // 当前是否选中
};


class CEditColorDlg : public CDialogImpl< CEditColorDlg >
{
public:
	enum { IDD = IDD_EDIT_COLOR };

	BEGIN_MSG_MAP( CEditColorDlg )
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		MESSAGE_HANDLER( WM_CLOSE, OnClose )
		MESSAGE_HANDLER( WM_SETCURSOR, OnSetCursor )
		MESSAGE_HANDLER( WM_LBUTTONUP, OnLButtonUp )
		MESSAGE_HANDLER( WM_CTLCOLORSTATIC, OnCtlColorStatic )
		COMMAND_ID_HANDLER( IDOK, OnOK )
		COMMAND_ID_HANDLER( IDCANCEL, OnCancel )
	END_MSG_MAP()

	CEditColorDlg( vector<Scheme> &Schemes );
	~CEditColorDlg();

	COLORREF Color( void ) { return( m_uColor ); }
	const char *Desc( void ) { return( m_strDesc.c_str() ); }
	void Color( COLORREF uColor ) { m_uColor = uColor; }
	void Desc( const char *pDesc ) { m_strDesc.assign( pDesc ); }
	void SetSchemes( vector<Scheme> &Schemes );
	Scheme *GetDestScheme( void ) { return( &m_DestScheme ); }

private:
	LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnSetCursor( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnCtlColorStatic( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );

	LRESULT OnOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );

private:
	void ShowColorInfo( void );

private:
	COLORREF m_uColor;
	string   m_strDesc;

	HBRUSH   m_hBrush;
	HCURSOR  m_hCursorHand;

	CComboBox      m_cbSchemes;
	vector<Scheme> m_Schemes;
	Scheme         m_DestScheme;
};


#endif  // __EDITCOLORDLG_H__20051013__