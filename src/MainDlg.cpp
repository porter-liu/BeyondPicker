#include "StdAfx.h"
#include "MainDlg.h"
#include "BeyondPicker.h"


CMainDlg::CMainDlg()
	: m_hBrush( NULL )
	, m_hMemDC( NULL )
	, m_hBmp( NULL )
	, m_hBmpOld( NULL )
	, m_bInDragMode( false )
	, m_ZoomWnd( this, 1 )  // ����������ʹ�ø�������Ϣӳ��� 1 ��С��
	, m_hIconTopmost( NULL )
	, m_hIconNull( NULL )
	, m_hCursorTopmost( NULL )
	, m_hCursorNoTopmost( NULL )
	, m_hCursorCanNotDo( NULL )
	, m_hCursorHand( NULL )
{
	m_hBrush = CreateSolidBrush( RGB( g_crRed, g_crGreen, g_crBlue ) );
}


CMainDlg::~CMainDlg()
{
	if( m_hBrush )
		DeleteObject( m_hBrush );
	if( m_hBmp )
		DeleteObject( m_hBmp );
	if( m_hMemDC )
	{
		SelectObject( m_hMemDC, m_hBmpOld );
		DeleteDC( m_hMemDC );
	}

	if( m_hIconNull )
		DestroyIcon( m_hIconNull );
	if( m_hIconTopmost )
		DestroyIcon( m_hIconTopmost );
	if( m_hCursorTopmost )
		DestroyCursor( m_hCursorTopmost );
	if( m_hCursorNoTopmost )
		DestroyCursor( m_hCursorNoTopmost );
	if( m_hCursorCanNotDo )
		DestroyCursor( m_hCursorCanNotDo );
	if( m_hCursorHand )
		DestroyCursor( m_hCursorHand );
}


LRESULT
CMainDlg::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	m_hIconNull        = LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDI_NULL ) );
	m_hIconTopmost     = LoadIcon( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDI_TOPMOST ) );
	m_hCursorTopmost   = LoadAniCursor( "ANI", IDR_TOPMOST );//LoadCursor( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDC_TOPMOST ) );
	m_hCursorNoTopmost = LoadAniCursor( "ANI", IDR_NOTOPMOST );//LoadCursor( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDC_NOTOPMOST ) );
	m_hCursorCanNotDo  = LoadCursor( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDC_CANNOTDO ) );
	m_hCursorHand      = LoadCursor( NULL, MAKEINTRESOURCE( 32649 ) );  // IDC_HAND

	m_TrackBar = GetDlgItem( IDC_MULTIPLE );
	m_TrackBar.SetRange( 2, 10 );  // �趨�Ŵ�����Χ

	m_TopMost = GetDlgItem( IDC_TOPMOST );
	m_TopMost.SetIcon( m_hIconTopmost );

	//
	// ������ͼʹ�õ��ڴ� DC ��λͼ
	//
	RECT rcZoomWnd = { 0 };
	m_ZoomWnd.SubclassWindow( GetDlgItem( IDC_ZOOM ) );  // ʹ�������ڱ������������ڷ�������
	m_ZoomWnd.GetClientRect( &rcZoomWnd );
	HDC hDC   = m_ZoomWnd.GetDC();
	m_hMemDC  = CreateCompatibleDC( hDC );
	m_hBmp    = CreateCompatibleBitmap( hDC, rcZoomWnd.right - rcZoomWnd.left, rcZoomWnd.bottom - rcZoomWnd.top );
	m_hBmpOld = (HBITMAP)SelectObject( m_hMemDC, m_hBmp );
	m_ZoomWnd.ReleaseDC( hDC );

	return TRUE;
}


//
// ͨ�����ز�ͬ��ɫ�� brush ����ʾ��ɫ��
//
LRESULT
CMainDlg::OnCtlColorStatic( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	HWND hWnd = GetDlgItem( IDC_COLOR );
	if( (HWND)lParam == hWnd )
		return( (LRESULT)m_hBrush );

	bHandled = FALSE;
	return( 0 );
}


LRESULT
CMainDlg::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	POINT pt = { 0 };
	GetCursorPos( &pt );

	RECT rcClient = { 0 };
	m_TopMost.GetWindowRect( &rcClient );

	if( PtInRect( &rcClient, pt ) )
	{
		m_bInDragMode = true;
		SetCapture();
		SetCursor( m_hCursorCanNotDo );
		m_TopMost.SetIcon( m_hIconNull );
	}

	return( 0 );
}


LRESULT
CMainDlg::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	if( !m_bInDragMode )  // ��δ�����϶�ģʽ
		return( 0 );

	POINT pt = { 0 };
	GetCursorPos( &pt );

	HWND hWnd = GetRootWindow( WindowFromPoint( pt ) );
	if( GetRootWindow( m_hWnd ) == hWnd )  // ����������Լ��Ĵ���
	{
		SetCursor( m_hCursorCanNotDo );
		return( 0 );
	}
	else if( IsWndTopMost( hWnd ) )
	{
		SetCursor( m_hCursorNoTopmost );
		return( 0 );
	}
	else
	{
		SetCursor( m_hCursorTopmost );
		return( 0 );
	}

	return( 0 );
}


LRESULT
CMainDlg::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	if( !m_bInDragMode )  // ��δ�����϶�ģʽ
	{
		RECT rcColorWnd = { 0 };
		CWindow ColorWnd = GetDlgItem( IDC_COLOR );
		ColorWnd.GetWindowRect( &rcColorWnd );
		
		DWORD dwPos = GetMessagePos();
		POINT pt = { LOWORD( dwPos ), HIWORD( dwPos ) };
		
		if( PtInRect( &rcColorWnd, pt ) )
		{
			CHOOSECOLOR cc;
			cc.lStructSize    = sizeof( CHOOSECOLOR );
			cc.hwndOwner      = m_hWnd;
			cc.hInstance      = NULL;
			cc.rgbResult      = RGB( g_crRed, g_crGreen, g_crBlue );
			cc.lpCustColors   = g_CustomColors;
			cc.Flags          = CC_RGBINIT | CC_FULLOPEN;
			cc.lCustData      = 0;
			cc.lpfnHook       = NULL;
			cc.lpTemplateName = NULL;
			if( ChooseColor( &cc ) )
			{
				g_MainUI.ShowColorInfo( cc.rgbResult );
			}
		}
		return( 0 );
	}

	m_bInDragMode = false;
	ReleaseCapture();  // ��������ָ�Ĭ�����ָ�룬�ͷ�����ָ����Զ��ָ�
	m_TopMost.SetIcon( m_hIconTopmost );

	POINT pt = { 0 };
	GetCursorPos( &pt );

	HWND hWnd = GetRootWindow( WindowFromPoint( pt ) );
	if( GetRootWindow( m_hWnd ) == hWnd )  // ����������Լ��Ĵ���
		return( 0 );

	bool bTopMost = IsWndTopMost( hWnd );
	TopMostWndFromPoint( hWnd, !bTopMost );
	TopMostWndFromPoint( GetRootWindow( m_hWnd ), true );  // �ٴν��Լ��ö������ⱻ��ס
	if( bTopMost )
		MessageBox( "The window is notopmost now", GetAppName(), MB_OK | MB_ICONINFORMATION );
	else
		MessageBox( "The window is topmost now", GetAppName(), MB_OK | MB_ICONINFORMATION );
	return( 0 );
}


LRESULT
CMainDlg::OnSetCursor( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	RECT rcColorWnd = { 0 }, rcTopmost = { 0 };
	::GetWindowRect( GetDlgItem( IDC_COLOR ), &rcColorWnd );
	::GetWindowRect( GetDlgItem( IDC_TOPMOST ), &rcTopmost );

	DWORD dwPos = GetMessagePos();
	POINT pt = { LOWORD( dwPos ), HIWORD( dwPos ) };

	if( PtInRect( &rcColorWnd, pt ) || PtInRect( &rcTopmost, pt ) )
		SetCursor( m_hCursorHand );
	else
		bHandled = FALSE;
	return( 0 );
}


//
// ��ʾ������ɫ����Ϣ
//
bool
CMainDlg::ShowColorInfo( COLORREF crColor )
{
	if( RGB( g_crRed, g_crGreen, g_crBlue ) == crColor )
		return( true );

	g_crRed   = GetRValue( crColor );
	g_crGreen = GetGValue( crColor );
	g_crBlue  = GetBValue( crColor );

	char szHTML[256];
	sprintf( szHTML, "#%02X%02X%02X", g_crRed, g_crGreen, g_crBlue );
	SetDlgItemText( IDC_HTML, szHTML );

	sprintf( szHTML, "%d", g_crRed  );
	SetDlgItemText( IDC_RED, szHTML );
	sprintf( szHTML, "%d", g_crGreen );
	SetDlgItemText( IDC_GREEN, szHTML );
	sprintf( szHTML, "%d", g_crBlue );
	SetDlgItemText( IDC_BLUE, szHTML );

	if( m_hBrush )
	{
		DeleteObject( m_hBrush );
		m_hBrush = NULL;
	}
	m_hBrush  = CreateSolidBrush( crColor );
	::InvalidateRect( GetDlgItem( IDC_COLOR ), NULL, TRUE );
	return( m_hBrush ? true : false );
}


//
// ��������� CMainDlg �� WM_PAINT ��Ϣ��Ӧ�����������������ڵ� WM_PAINT ��Ϣ��
// Ӧ���������βδ����� DC Ӧ���� WTL ��һ�� Bug����Ϊ WM_PAINT �� wParam ������
// DC��������Ӧʱ��Ȼ��Ҫ���� BeginPaint() ȥ��ȡ DC��
//
LRESULT
CMainDlg::OnPaint( HDC hUselessDC )
{
	PAINTSTRUCT ps;
	CWindow ZoomWnd = GetDlgItem( IDC_ZOOM );
	HDC hDC = ZoomWnd.BeginPaint( &ps );
	ShowZoomBmp();
	ZoomWnd.EndPaint( &ps );
	return( 0 );
}


//
// �������������ҷŴ��˵�Ŀ������λͼ��ʾ�ڷŴ󴰿���
//
void
CMainDlg::ShowZoomBmp( void )
{
	RECT rcZoom = { 0 };
	CWindow ZoomWnd = GetDlgItem( IDC_ZOOM );
	ZoomWnd.GetClientRect( &rcZoom );

	HDC hDC = ZoomWnd.GetDC();
	BitBlt( hDC, 0, 0, rcZoom.right - rcZoom.left, rcZoom.bottom - rcZoom.top, m_hMemDC, 0, 0, SRCCOPY );
	ZoomWnd.ReleaseDC( hDC );
}


//
// �Ŵ���Ļ�ƶ�λ�õ�һ������
//
void
CMainDlg::ZoomIn( HDC hScreenDC, POINT *pPoint )
{
	CWindow ZoomWnd = GetDlgItem( IDC_ZOOM );
	RECT rcZoom = { 0 };
	ZoomWnd.GetClientRect( &rcZoom );

	int nMultiple   = m_TrackBar.GetPos();  // �õ������趨�ķŴ���
	int nZoomWidth  = rcZoom.right  - rcZoom.left;
	int nZoomHeight = rcZoom.bottom - rcZoom.top;
	int nSrcWidth   = nZoomWidth  / nMultiple;
	int nSrcHeight  = nZoomHeight / nMultiple;

	PatBlt( m_hMemDC, 0, 0, nZoomWidth, nZoomHeight, BLACKNESS );
	StretchBlt(
		m_hMemDC,
		0,
		0,
		nZoomWidth,
		nZoomHeight,
		hScreenDC,
		pPoint->x - ( nSrcWidth  >> 1 ),
		pPoint->y - ( nSrcHeight >> 1 ),
		nSrcWidth,
		nSrcHeight,
		SRCCOPY );

	// �þ���Ȧ��ȡɫλ��
	POINT Points[4];
	Points[0].x = ( nSrcWidth  >> 1 ) * nMultiple - 1;
	Points[0].y = ( nSrcHeight >> 1 ) * nMultiple - 1;
	Points[1].x = Points[0].x + nMultiple + 2;
	Points[1].y = Points[0].y;
	Points[2].x = Points[0].x + nMultiple + 2;
	Points[2].y = Points[0].y + nMultiple + 2;
	Points[3].x = Points[0].x;
	Points[3].y = Points[0].y + nMultiple + 2;
	SetROP2( m_hMemDC, R2_NOTXORPEN );
	Polygon( m_hMemDC, Points, 4 );

	ShowZoomBmp();
}