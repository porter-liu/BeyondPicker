#include "StdAfx.h"
#include "MainWnd.h"
#include "TemplateDlg.h"
#include "resource.h"


CMainWnd::CMainWnd()
	: m_bEnabled( false )
	, m_bDodgeCursor( true )
	, m_bZoomIn( true )
	, m_hIconRed( NULL )
	, m_hIconGreen( NULL )
{
	m_hIconRed = (HICON)LoadImage(
		_Module.GetResourceInstance(),
		MAKEINTRESOURCE( IDI_RED ),
		IMAGE_ICON,
		16, 16,
		LR_DEFAULTCOLOR );
	m_hIconGreen = (HICON)LoadImage(
		_Module.GetResourceInstance(),
		MAKEINTRESOURCE( IDI_GREEN ),
		IMAGE_ICON,
		16, 16,
		LR_DEFAULTCOLOR );
}


CMainWnd::~CMainWnd()
{
	if( m_hIconRed )
		DestroyIcon( m_hIconRed );
	if( m_hIconGreen )
		DestroyIcon( m_hIconGreen );
}


LRESULT
CMainWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	//
	// װ�븴��ģ�壬�����ݸ���ģ�彨�������˵�
	//
	string strName, strFormat;
	for( int i = 0; i < TEMPLATE_COUNT; i++ )
	{
		LoadTemplate( i, strName, strFormat );
		g_Templates[i].m_strName   = strName;
		g_Templates[i].m_strFormat = strFormat;
	}
	BuildTmplMenu();

	//
	// ������ģ��˵�ͬ�������˵�����ɫ������ݲ˵�
	//
	SyncCopyMenu();
	
	//
	// װ���Զ�����ɫ
	//
	LoadCustomColors();

	//
	// -== ToolBar ==-
	// WTL �� ToolBar::AddBitmap() ��������֧�� 256 ɫ���ϵ�λͼ��ֱ��ʹ�û���쳣
	//
	m_ToolBar.Create(
		m_hWnd,
		rcDefault,
		NULL,
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS );
	m_ToolBar.SetExtendedStyle( TBSTYLE_EX_DRAWDDARROWS );  // Ҫʹ�ÿ������Ĺ�������ť�ͱ������������չ���
	m_ToolBar.SetButtonStructSize( sizeof( TBBUTTON ) );    // ������κΰ�ť֮ǰһ��Ҫ���������ֵ
/*
	m_ToolBar.AddBitmap( 5, IDR_TOOLBAR );
/*/
	CImageList ImageList;
	ImageList.CreateFromImage(
		IDR_TOOLBAR,
		16,
		0,
		RGB( 255, 0, 255 ),
		IMAGE_BITMAP,
		LR_CREATEDIBSECTION );  // ����Ҫ�ľ��������������ֻ���°� MSDN �в��ܲ鵽��
	m_ToolBar.SetImageList( ImageList );
//*/

	TBBUTTON tbbs[7] = { 0 };
	tbbs[0].iBitmap   = 0;
	tbbs[0].fsStyle   = TBSTYLE_DROPDOWN;
	tbbs[0].fsState   = TBSTATE_ENABLED;
	tbbs[0].idCommand = IDC_COPY;
	tbbs[0].iString   = 0;
	tbbs[1].iBitmap   = 2;
	tbbs[1].fsStyle   = TBSTYLE_BUTTON;
	tbbs[1].fsState   = TBSTATE_ENABLED;
	tbbs[1].idCommand = IDC_TEMPLATE;
	tbbs[2].iBitmap   = 1;
	tbbs[2].fsStyle   = TBSTYLE_BUTTON;
	tbbs[2].fsState   = TBSTATE_ENABLED;
	tbbs[2].idCommand = IDC_CUSTOM;
	tbbs[3].iBitmap   = 3;
	tbbs[3].fsStyle   = TBSTYLE_CHECK;
	tbbs[3].fsState   = TBSTATE_ENABLED;
	tbbs[3].idCommand = IDC_DODGE_CURSOR;
	tbbs[4].iBitmap   = 4;
	tbbs[4].fsStyle   = TBSTYLE_CHECK;
	tbbs[4].fsState   = TBSTATE_ENABLED;
	tbbs[4].idCommand = IDC_IS_ZOOM;
	tbbs[5].iBitmap   = 5;
	tbbs[5].fsStyle   = TBSTYLE_BUTTON;
	tbbs[5].fsState   = TBSTATE_ENABLED;
	tbbs[5].idCommand = IDC_COLOR_SCHEMES;
	tbbs[6].iBitmap   = 6;
	tbbs[6].fsStyle   = TBSTYLE_BUTTON;
	tbbs[6].fsState   = TBSTATE_ENABLED;
	tbbs[6].idCommand = IDC_ADD_COLOR;
	m_ToolBar.AddButtons( sizeof( tbbs ) / sizeof( TBBUTTON ), tbbs );
	m_ToolBar.AutoSize();  // ����갴ť�����

	CToolTipCtrl ToolTip = m_ToolBar.GetToolTips();
	RECT rcButton = { 0 };
	m_ToolBar.GetRect( IDC_COPY, &rcButton );
	ToolTip.AddTool( m_ToolBar, "Copy color string", &rcButton, IDC_COPY );
	m_ToolBar.GetRect( IDC_TEMPLATE, &rcButton );
	ToolTip.AddTool( m_ToolBar, "Set color string templates", &rcButton, IDC_TEMPLATE );
	m_ToolBar.GetRect( IDC_CUSTOM, &rcButton );
	ToolTip.AddTool( m_ToolBar, "Custom color", &rcButton, IDC_CUSTOM );
	m_ToolBar.GetRect( IDC_DODGE_CURSOR, &rcButton );
	ToolTip.AddTool( m_ToolBar, "Dodge cursor", &rcButton, IDC_DODGE_CURSOR );
	m_ToolBar.GetRect( IDC_IS_ZOOM, &rcButton );
	ToolTip.AddTool( m_ToolBar, "Zoom in cursor region", &rcButton, IDC_IS_ZOOM );
	m_ToolBar.GetRect( IDC_COLOR_SCHEMES, &rcButton );
	ToolTip.AddTool( m_ToolBar, "Color schemes", &rcButton, IDC_COLOR_SCHEMES );
	m_ToolBar.GetRect( IDC_ADD_COLOR, &rcButton );
	ToolTip.AddTool( m_ToolBar, "Add color", &rcButton, IDC_ADD_COLOR );

	//
	// ReBar
	//
	m_ReBar.Create(
		m_hWnd,
		rcDefault,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | CCS_NODIVIDER );
	REBARINFO rbi = { 0 };
	rbi.cbSize = sizeof( REBARBANDINFO );
	rbi.fMask  = 0;
	rbi.himl   = NULL;
	m_ReBar.SetBarInfo( &rbi );

	REBARBANDINFO rbbi = { 0 };
	rbbi.cbSize     = sizeof( REBARBANDINFO );
	rbbi.fMask      = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE;
	rbbi.fStyle     = RBBS_NOVERT | RBBS_CHILDEDGE;
	rbbi.hwndChild  = m_ToolBar;
	rbbi.cxMinChild = 0;
	rbbi.cyMinChild = 20;  // �������Ҫ�������þ���ʾ������
	m_ReBar.InsertBand( -1, &rbbi );

	//
	// StatusBar
	//
	m_StatusBar.Create( m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE );
	m_StatusBar.SetDlgCtrlID( 1234 );

	//
	// Main UI
	//
	RECT rcClient = { 0 };
	GetClientRect( &rcClient );
	int nWidthClient  = rcClient.right;   // ��ס���ڿͻ����ĸ߶ȺͿ��
	int nHeightClient = rcClient.bottom;
	AdjustWindowRectEx(
		&rcClient,
		GetWindowLong( GWL_STYLE ),
		TRUE,
		GetWindowLong( GWL_EXSTYLE ) );
	int nWidth  = rcClient.right - rcClient.left - nWidthClient;   // ��������ڿͻ����Ŀ������
	int nHeight = rcClient.bottom - rcClient.top - nHeightClient;  // ��������ڿͻ����ĸ߶�����

	// �߶�������Ӧ�ý� ToolBar �� StatusBar �ĸ߶ȼ���
	RECT rcReBar = { 0 };
	m_ReBar.GetWindowRect( &rcReBar );
	rcReBar.bottom -= 2;  // ########## ���ﲻ���Ѷ�Ϊ֮����Ҫ�ó���ȷ�Ķ�λ ##########
	RECT rcStatusBar = { 0 };
	m_StatusBar.GetWindowRect( &rcStatusBar );
	nHeight += ( rcReBar.bottom - rcReBar.top + rcStatusBar.bottom - rcStatusBar.top );

	// ���������ƶ����ʵ�λ��
	RECT rcMainUI = { 0 };
	g_MainUI.Create( m_hWnd );
	g_MainUI.GetClientRect( &rcMainUI );
	g_MainUI.MoveWindow( 0, rcReBar.bottom - rcReBar.top, rcMainUI.right, rcMainUI.bottom );
	g_MainUI.ShowWindow( SW_SHOW );

	// �����ڵ������պ÷��� ToolBar��StatusBar ��������Ĵ�С
	MoveWindow( 0, 0, rcMainUI.right + nWidth, rcMainUI.bottom + nHeight );
	CenterWindow();
	HICON hIcon = (HICON)::LoadImage(
		_Module.GetResourceInstance(),
		MAKEINTRESOURCE( IDR_MAINFRAME ),
		IMAGE_ICON,
		::GetSystemMetrics( SM_CXICON ),
		::GetSystemMetrics( SM_CYICON ),
		LR_DEFAULTCOLOR );
	SetIcon( hIcon, TRUE );
	HICON hIconSmall = (HICON)::LoadImage(
		_Module.GetResourceInstance(),
		MAKEINTRESOURCE( IDR_MAINFRAME ),
		IMAGE_ICON,
		::GetSystemMetrics( SM_CXSMICON ),
		::GetSystemMetrics( SM_CYSMICON ),
		LR_DEFAULTCOLOR );
	SetIcon( hIconSmall, FALSE );

	//
	// ��״̬���ƶ������ʵ�λ�ã������ø���Ԫ��ĳߴ������
	//
	GetClientRect( &rcClient );
	m_StatusBar.MoveWindow(
		0,
		rcReBar.bottom - rcReBar.top + rcMainUI.bottom - rcMainUI.top,
		rcClient.right - rcClient.left,
		rcStatusBar.bottom - rcStatusBar.top );

	int nWidths[2] = { rcClient.right - 80, -1 };
	m_StatusBar.SetParts( 2, nWidths );
	//m_StatusBar.SetText( 1, NULL, SBT_OWNERDRAW );  // �ѵ�Ԫ�����ó� OwnerDraw
	HICON hIconPosition = (HICON)LoadImage(
		_Module.GetResourceInstance(),
		MAKEINTRESOURCE( IDI_POSITION ),
		IMAGE_ICON,
		16, 16,
		LR_DEFAULTCOLOR );
	m_StatusBar.SetIcon( 1, hIconPosition );
	m_StatusBar.SetText( 1, "0, 0" );

	//
	// �趨������ʱ�� 4 ������
	//
	int nScreenWidth  = GetSystemMetrics( SM_CXFULLSCREEN );
	int nScreenHeight = GetSystemMetrics( SM_CYFULLSCREEN );
	SetRect( &m_rcScreen[0], 0, 0, nScreenWidth >> 1, nScreenHeight >> 1 );
	SetRect( &m_rcScreen[1], ( nScreenWidth >> 1 ) + 1, 0, nScreenWidth, nScreenHeight >> 1 );
	SetRect( &m_rcScreen[2], ( nScreenWidth >> 1 ) + 1, ( nScreenHeight >> 1 ) + 1, nScreenWidth, nScreenHeight );
	SetRect( &m_rcScreen[3], 0, ( nScreenHeight >> 1 ) + 1, nScreenWidth >> 1, nScreenHeight );

	//
	// ����
	//
#ifndef _DEBUG
	SetWindowPos( HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );  // ʹ���ڳ��Ӷ���
#endif
	SetTimer( 0, 10 );  // ����������ʱ��
	UpdateStatus( true );
	g_MainUI.ShowColorInfo( RGB( g_crRed, g_crGreen, g_crBlue ) );  // ��ʼ����ɫ��Ϣ��ʾ
	m_ToolBar.CheckButton( IDC_DODGE_CURSOR, m_bDodgeCursor );  // Ĭ�����������
	m_ToolBar.CheckButton( IDC_IS_ZOOM, m_bZoomIn );            // Ĭ��ʹ��Ŀ������Ŵ�

	//
	// ������ɫ��������
	//
	m_ColorSchemesDlg.Create( NULL );

	return( 0 );
}


LRESULT
CMainWnd::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	//
	// �����˳�ʱ���渴��ģ��
	//
	for( int i = 0; i < g_Templates.size(); i++ )
		SaveTemplate( i, g_Templates[i].m_strName, g_Templates[i].m_strFormat );

	//
	// �����Զ�����ɫ
	//
	SaveCustomColors();

	//
	// ������ɫ��������
	//
	m_ColorSchemesDlg.DestroyWindow();

	PostQuitMessage( 0 );
	return( 0 );
}


//
// �������������꣬�������̶��ɶ�ʱ������
//
LRESULT
CMainWnd::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	if( !UpdateStatus() )  // ���δ�ڹ���״̬��ִ���������
		return( 0 );

	POINT ptScreen = { 0 };
	HDC hDC = NULL;

	while( GetCursorPos( &ptScreen ) )
	{
		char szPosition[256];
		sprintf( szPosition, "%d, %d", ptScreen.x, ptScreen.y );
		m_StatusBar.SetText( 1, szPosition );

		hDC = ::GetDC( NULL );
		if( NULL == hDC )
			break;

		COLORREF crColor = GetPixel( hDC, ptScreen.x, ptScreen.y );  // ��ȡ�������λ����ɫ
		if( CLR_INVALID == crColor )
			break;

		g_MainUI.ShowColorInfo( crColor );      // ��ʾ��ɫ��Ϣ
		if( m_bDodgeCursor )
			DodgeCursor( &ptScreen );  // �����
		if( m_bZoomIn )
			g_MainUI.ZoomIn( hDC, &ptScreen );    // �Ŵ���ʾĿ������
		break;
	}

	if( hDC )
		ReleaseDC( hDC );
	return( 0 );
}


//
// �ڵ����˵���ʾ֮ǰ���������������á�����ܡ��͡��Ŵ󡱲˵����ѡ��״̬
//
LRESULT
CMainWnd::OnInitMenuPopup( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	HMENU hMenu     = (HMENU)wParam;
	bool  bSysMenu  = HIWORD( lParam ) ? true : false;  // �Ƿ�ϵͳ�˵�
	UINT  uPosition = LOWORD( lParam );                 // �˵�λ��

	// ֻ�������˵��� Tools �˵�
	if( !bSysMenu &&                // ������ϵͳ�˵�
		1 == uPosition &&           // �����ǵ� 2 ���Ӳ˵�
		hMenu != g_hMenuTemplate )  // �����Ǹ��Ʋ˵�
	{
		CheckMenuItem( hMenu, IDC_DODGE_CURSOR, MF_BYCOMMAND | ( m_bDodgeCursor ? MF_CHECKED : MF_UNCHECKED ) );
		CheckMenuItem( hMenu, IDC_IS_ZOOM,      MF_BYCOMMAND | ( m_bZoomIn ? MF_CHECKED : MF_UNCHECKED ) );
	}
	return( 0 );
}


LRESULT
CMainWnd::OnApplyTemplate( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	char szColor[256];
	try
	{
		sprintf( szColor, g_Templates[wID - WM_USER - 1].m_strFormat.c_str(), g_crRed, g_crGreen, g_crBlue );
		SetClipboardText( m_hWnd, szColor );
	}
	catch( ... )
	{
		MessageBox( "ģ���趨����", GetAppName(), MB_OK | MB_ICONINFORMATION );
	}

	return( 0 );
}


LRESULT
CMainWnd::OnTemplate( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	CTemplateDlg Dlg;
	Dlg.DoModal();

	//
	// ����ģ����ܻᱻ�޸ģ�����Ҫ�ؽ�����ģ��˵�������ͬ����������Ҫʹ�ø���
	// ģ��˵��Ĳ˵���
	//
	BuildTmplMenu();
	SyncCopyMenu();

	return( 0 );
}


//
// ��Ӧ�Զ�����ɫ��ť�ĵ���¼�
//
LRESULT
CMainWnd::OnCustom( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	COLORREF crColor = RGB( g_crRed, g_crGreen, g_crBlue );
	CHOOSECOLOR cc;
	cc.lStructSize    = sizeof( CHOOSECOLOR );
	cc.hwndOwner      = m_hWnd;
	cc.hInstance      = NULL;
	cc.rgbResult      = crColor;
	cc.lpCustColors   = g_CustomColors;
	cc.Flags          = CC_RGBINIT | CC_FULLOPEN;
	cc.lCustData      = 0;
	cc.lpfnHook       = NULL;
	cc.lpTemplateName = NULL;
	if( ChooseColor( &cc ) )
	{
		g_MainUI.ShowColorInfo( cc.rgbResult );
	}
	
	return( 0 );
}


LRESULT
CMainWnd::OnDodgeCursor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	m_bDodgeCursor = !m_bDodgeCursor;
	m_ToolBar.CheckButton( IDC_DODGE_CURSOR, m_bDodgeCursor );
	return( 0 );
}


LRESULT
CMainWnd::OnZoomIn( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	m_bZoomIn = !m_bZoomIn;
	m_ToolBar.CheckButton( IDC_IS_ZOOM, m_bZoomIn );
	return( 0 );
}


LRESULT
CMainWnd::OnColorSchemes( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	m_ColorSchemesDlg.ShowWindow( SW_SHOW );
	return( 0 );
}


LRESULT
CMainWnd::OnAddColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	m_ColorSchemesDlg.AddColor( m_hWnd );
	return( 0 );
}


LRESULT
CMainWnd::OnAbout( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	CAboutDlg Dlg;
	Dlg.DoModal();
	return( 0 );
}


//
// ��Ӧ�������ϣ�����ʽ��ť���Ұ�߱����µ���Ϣ
//
LRESULT
CMainWnd::OnTBDropDown( int idCtrl, LPNMHDR pnmh, BOOL &bHandled )
{
	NMTOOLBAR *pNMTB = (NMTOOLBAR *)pnmh;
	RECT rcButton = { 0 };
	m_ToolBar.GetRect( pNMTB->iItem, &rcButton );
	ClientToScreen( &rcButton );

	TrackPopupMenu(
		g_hMenuTemplate,
		TPM_LEFTALIGN | TPM_TOPALIGN,
		rcButton.left - 1,
		rcButton.bottom + 2,
		0,
		m_hWnd,
		NULL );
	return( TBDDRET_DEFAULT );
}


//
// ����ʵ�� OwnerDraw StatusBar �Ĵ��룬Ŀǰ����
//
LRESULT
CMainWnd::OnDrawItem( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
/*	DRAWITEMSTRUCT *pDIS = ( DRAWITEMSTRUCT *)lParam;
	UINT uCtrlID = (UINT)wParam;  // ������������������ĸ��ؼ���pDIS->itemID ���������ǿؼ��е��ĸ� item

	char szCoordinate[256];
	POINT pt;
	GetCursorPos( &pt );
	sprintf( szCoordinate, "%d, %d", pt.x, pt.y );
	SetBkMode( pDIS->hDC, TRANSPARENT );
	DrawText( pDIS->hDC, szCoordinate, strlen( szCoordinate ), &pDIS->rcItem, DT_VCENTER | DT_SINGLELINE );*/
	return( 0 );
}


//
// -----------------------------------------------------------------------------
//


//
// ���� Scroll Lock ����״̬���� UI ��ʾ״̬
//
bool
CMainWnd::UpdateStatus( bool bForceUpdate )
{
	bool bEnabled = GetKeyState( VK_SCROLL ) && 0x01;
	if( bEnabled != m_bEnabled || bForceUpdate )
	{
		if( bEnabled )
		{
			m_StatusBar.SetIcon( 0, m_hIconGreen );
			m_StatusBar.SetText( 0, _T("Press \"Scroll Lock\" key to pause") );
		}
		else
		{
			m_StatusBar.SetIcon( 0, m_hIconRed );
			m_StatusBar.SetText( 0, _T("Press \"Scroll Lock\" key to start") );
		}
	}
	m_bEnabled = bEnabled;
	return( m_bEnabled );
}


//
// �����
//
void
CMainWnd::DodgeCursor( POINT *pPoint )
{
	RECT rcWindow = { 0 };
	GetWindowRect( &rcWindow );
	InflateRect( &rcWindow, 20, 20 );
	if( PtInRect( &rcWindow, *pPoint ) )
	{
		for( int i = 0; i < sizeof( m_rcScreen ) / sizeof( RECT ); i++ )
		{
			if( PtInRect( &m_rcScreen[i], *pPoint ) )
			{
				int nDstRect = 0;
				switch( i )
				{
				case 0:
					nDstRect = 2;
					break;
				case 1:
					nDstRect = 3;
					break;
				case 2:
					nDstRect = 0;
					break;
				case 3:
					nDstRect = 1;
					break;
				}
				InflateRect( &rcWindow, -20, -20 );
				int nWidthRegion  = m_rcScreen[nDstRect].right  - m_rcScreen[nDstRect].left;
				int nHeightRegion = m_rcScreen[nDstRect].bottom - m_rcScreen[nDstRect].top;
				int nWidthWindow  = rcWindow.right  - rcWindow.left;
				int nHeightWindow = rcWindow.bottom - rcWindow.top;
				MoveWindow(
					m_rcScreen[nDstRect].left + ( nWidthRegion  - nWidthWindow )  / 2,
					m_rcScreen[nDstRect].top  + ( nHeightRegion - nHeightWindow ) / 2,
					nWidthWindow,
					nHeightWindow,
					TRUE );
				break;
			}
		}
	}
}


//
// ������ģ��˵�ͬ�������˵�����ɫ������ݲ˵�
//
void
CMainWnd::SyncCopyMenu( void )
{
	//
	// ͬ���������еĸ��Ʋ˵�
	//
	HMENU hMainMenu = GetMenu();
	if( hMainMenu )
		DestroyMenu( hMainMenu );
	hMainMenu = LoadMenu( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MAINMENU ) );
	HMENU hMenu = GetSubMenu( hMainMenu, 1 );
	char szText[256] = { 0 };
	GetMenuString( hMenu, IDC_COPY, szText, sizeof( szText ), MF_BYCOMMAND );
	ModifyMenu( hMenu, IDC_COPY, MF_BYCOMMAND | MF_POPUP, (UINT)g_hMenuTemplate, szText );
	SetMenu( hMainMenu );
	DestroyMenu( hMainMenu );

	//
	// ͬ����ɫ���������е���ɫ��ݲ˵�
	//
	if( g_hMenuColor )
		DestroyMenu( g_hMenuColor );
	g_hMenuColor = LoadMenu( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MENU_COLOR ) );
	g_hMenuColor = GetSubMenu( g_hMenuColor, 0 );
	GetMenuString( g_hMenuColor, IDC_COPY, szText, sizeof( szText ), MF_BYCOMMAND );
	ModifyMenu( g_hMenuColor, IDC_COPY, MF_BYCOMMAND | MF_POPUP, (UINT)g_hMenuTemplate, szText );
}