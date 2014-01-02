#include "StdAfx.h"
#include "EditColorDlg.h"
#include "BeyondPicker.h"


CEditColorDlg::CEditColorDlg( vector<Scheme> &Schemes )
	: m_hBrush( NULL )
	, m_hCursorHand( NULL )
	, m_uColor( 0 )
	, m_Schemes( Schemes )
{
	;
}


CEditColorDlg::~CEditColorDlg()
{
	if( m_hBrush )
		DeleteObject( m_hBrush );
	if( m_hCursorHand )
		DeleteObject( m_hCursorHand );
}


LRESULT
CEditColorDlg::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	m_cbSchemes = GetDlgItem( IDC_SCHEMES );

	// 将下拉列表部分的高度设大一些
	RECT rcWnd = { 0 };
	m_cbSchemes.GetWindowRect( &rcWnd );
	POINT pt = { rcWnd.left, rcWnd.top };
	ScreenToClient( &pt );
	m_cbSchemes.MoveWindow( pt.x, pt.y, rcWnd.right - rcWnd.left, 200 );

	// 将当前已经存在的配色方案列出
	vector<Scheme>::iterator it = m_Schemes.begin();
	while( m_Schemes.end() != it )
	{
		int nIndex = m_cbSchemes.InsertString( m_cbSchemes.GetCount(), it->m_strName.c_str() );
		m_cbSchemes.SetItemData( nIndex, (DWORD)it->m_hItem );
		if( it->m_bSelected )
			m_cbSchemes.SetCurSel( nIndex );
		it++;
	}

	m_hCursorHand = LoadCursor( NULL, MAKEINTRESOURCE( 32649 ) );  // IDC_HAND

	ShowColorInfo();
	if( 0 == m_strDesc.length() )  // 如果描述为空（新建），则默认用 HTML 格式作为其描述
	{
		TCHAR szColor[256] = { 0 };
		sprintf(
			szColor,
			"#%02X%02X%02X",
			GetRValue( m_uColor ),
			GetGValue( m_uColor ),
			GetBValue( m_uColor ) );
		m_strDesc = szColor;
	}
	SetDlgItemText( IDC_DESC, m_strDesc.c_str() );

	return( 0 );
}


LRESULT
CEditColorDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	EndDialog( IDCANCEL );
	return( 0 );
}


LRESULT
CEditColorDlg::OnSetCursor( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	RECT rcColorWnd = { 0 };
	CWindow ColorWnd = GetDlgItem( IDC_COLOR );
	ColorWnd.GetWindowRect( &rcColorWnd );

	DWORD dwPos = GetMessagePos();
	POINT pt = { LOWORD( dwPos ), HIWORD( dwPos ) };

	if( PtInRect( &rcColorWnd, pt ) )
		SetCursor( m_hCursorHand );
	else
		bHandled = FALSE;
	return( 0 );
}


LRESULT
CEditColorDlg::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	RECT rcColorWnd = { 0 };
	CWindow ColorWnd = GetDlgItem( IDC_COLOR );
	ColorWnd.GetWindowRect( &rcColorWnd );

	DWORD dwPos = GetMessagePos();
	POINT pt = { LOWORD( dwPos ), HIWORD( dwPos ) };

	if( !PtInRect( &rcColorWnd, pt ) )
		return( 0 );

	CHOOSECOLOR cc;
	cc.lStructSize    = sizeof( CHOOSECOLOR );
	cc.hwndOwner      = m_hWnd;
	cc.hInstance      = NULL;
	cc.rgbResult      = m_uColor;
	cc.lpCustColors   = g_CustomColors;
	cc.Flags          = CC_RGBINIT | CC_FULLOPEN;
	cc.lCustData      = 0;
	cc.lpfnHook       = NULL;
	cc.lpTemplateName = NULL;
	if( ChooseColor( &cc ) )
	{
		m_uColor = cc.rgbResult;
		ShowColorInfo();
	}

	return( 0 );
}


LRESULT
CEditColorDlg::OnCtlColorStatic( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	HWND hWnd = GetDlgItem( IDC_COLOR );
	if( (HWND)lParam == hWnd )
		return( (LRESULT)m_hBrush );

	bHandled = FALSE;
	return( 0 );
}


LRESULT
CEditColorDlg::OnOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	TCHAR szSchemeName[256] = { 0 };
	int nIndex = m_cbSchemes.GetCurSel();
	if( CB_ERR == nIndex )
	{
		m_cbSchemes.GetWindowText( szSchemeName, sizeof( szSchemeName ) );
		m_DestScheme.m_hItem = NULL;
	}
	else
	{
		m_cbSchemes.GetLBText( nIndex, szSchemeName );
		m_DestScheme.m_hItem = (HTREEITEM)m_cbSchemes.GetItemData( nIndex );
	}
	m_DestScheme.m_strName = szSchemeName;
	if( 0 == strlen( szSchemeName ) )
	{
		MessageBox( "Please select or enter scheme name!", GetAppName(), MB_OK | MB_ICONINFORMATION );
		m_cbSchemes.SetFocus();
		return( 0 );
	}

	char szDesc[256] = { 0 };
	CEdit Desc = GetDlgItem( IDC_DESC );
	Desc.GetWindowText( szDesc, sizeof( szDesc ) );
	if( 0 == strlen( szDesc ) )
	{
		MessageBox( "Please enter color description!", GetAppName(), MB_OK | MB_ICONINFORMATION );
		Desc.SetFocus();
		return( 0 );
	}

	m_strDesc = szDesc;
	EndDialog( IDOK );
	return( 0 );
}


LRESULT
CEditColorDlg::OnCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	EndDialog( IDCANCEL );
	return( 0 );
}


void
CEditColorDlg::ShowColorInfo( void )
{
	char szColor[256] = { 0 };
	sprintf(
		szColor,
		"#%02X%02X%02X; %d,%d,%d",
		GetRValue( m_uColor ),
		GetGValue( m_uColor ),
		GetBValue( m_uColor ),
		GetRValue( m_uColor ),
		GetGValue( m_uColor ),
		GetBValue( m_uColor ) );
	SetDlgItemText( IDC_COLOR_STRING, szColor );
	
	if( m_hBrush )
		DeleteObject( m_hBrush );
	m_hBrush = CreateSolidBrush( m_uColor );

	CWindow ColorWnd = GetDlgItem( IDC_COLOR );
	ColorWnd.RedrawWindow();
}