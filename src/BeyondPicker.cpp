#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainWnd.h"
#include "BeyondPicker.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	g_hMenuGroup = LoadMenu( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MENU_GROUP ) );
	g_hMenuGroup = GetSubMenu( g_hMenuGroup, 0 );
	g_hMenuColor = LoadMenu( _Module.GetResourceInstance(), MAKEINTRESOURCE( IDR_MENU_COLOR ) );
	g_hMenuColor = GetSubMenu( g_hMenuColor, 0 );

	CMainWnd wndMain;
	char szTitle[256] = { 0 };
	sprintf( szTitle, "%s %s", GetAppName(), GetAppVer() );
	if( NULL == wndMain.Create( NULL, CWindow::rcDefault, szTitle, WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ) )
	{
		ATLTRACE( _T("Main window creation failed!\n") );
		return( 0 );
	}
	wndMain.ShowWindow( nCmdShow );

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}


//
// -----------------------------------------------------------------------------
//


vector<Template> g_Templates( TEMPLATE_COUNT );
HMENU g_hMenuTemplate = NULL;
HMENU g_hMenuGroup    = NULL;
HMENU g_hMenuColor    = NULL;
COLORREF g_CustomColors[16] = { 0 };
BYTE g_crRed   = 0;
BYTE g_crGreen = 0;
BYTE g_crBlue  = 0;
CMainDlg g_MainUI;


const char *
GetAppName( void )
{
	return( "BeyondPicker" );
}


const char *
GetAppVer( void )
{
	return( "1.8.0" );
}


bool
GetAppFullPath( string &strPath )
{
	char szPath[1024] = { 0 };
	if( GetModuleFileName( NULL, szPath, sizeof( szPath ) ) )
	{
		char *p = strrchr( szPath, '\\' );
		if( p )
			*p = '\0';
		strPath = szPath;
		return( true );
	}
	else
	{
		return( false );
	}
}


bool
GetConfigFileName( string &strFileName )
{
	if( GetAppFullPath( strFileName ) )
	{
		strFileName += "\\Config.ini";
		return( true );
	}
	else
	{
		return( false );
	}
}


//
// ���ƶ����ı����������
//
void
SetClipboardText(
	HWND        hWnd,
	const char *pText )
{
	HGLOBAL hGlobal = GlobalAlloc( GHND|GMEM_SHARE, strlen( pText ) + 1 );
	char *pGlobal = (char *)GlobalLock( hGlobal );
	strcpy( pGlobal, pText );
	GlobalUnlock( hGlobal );
	
	OpenClipboard( hWnd );
	EmptyClipboard();
	SetClipboardData( CF_TEXT, hGlobal );
	CloseClipboard();
}


//
// ����ָ����Ԥ��ģ��
//
void
LoadDefaultTemplate( int nIndex, string &strName, string &strFormat )
{
	switch( nIndex )
	{
	case 0:
		strName   = "HTML";
		strFormat = "#%02X%02X%02X";
		break;
	case 1:
		strName   = "VC - RGB Macro";
		strFormat = "RGB( %d, %d, %d )";
		break;
	case 2:
		strName   = "VC - DWORD";
		strFormat = "0x00%02X%02X%02X";
		break;
	case 3:
		strName   = "Delphi";
		strFormat = "$00%02X%02X%02X";
		break;
	case 4:
		strName   = "VB";
		strFormat = "0&00%02X%02X%02X";
		break;
	case 5:
		strName   = "PhotoShop";
		strFormat = "%02X%02X%02X";
		break;
	default:
		strName   = "";
		strFormat = "";
		break;
	}
}


//
// �������ļ���װ��ָ����ģ�壬������ò����ھ�ʹ��Ԥ��ģ��
//
void
LoadTemplate( int nIndex, string &strName, string &strFormat )
{
	string strConfigFile;
	GetConfigFileName( strConfigFile );

	char szName[256], szValue[256];

	memset( szValue, 0, sizeof( szValue ) );
	sprintf( szName, "Name%d", nIndex + 1 );
	GetPrivateProfileString( "Templates", szName, GetAppName(), szValue, sizeof( szValue ), strConfigFile.c_str() );
	if( 0 == stricmp( szValue, GetAppName() ) )
	{
		LoadDefaultTemplate( nIndex, strName, strFormat );
		return;
	}
	strName = szValue;

	memset( szValue, 0, sizeof( szValue ) );
	sprintf( szName, "Format%d", nIndex + 1 );
	GetPrivateProfileString( "Templates", szName, NULL, szValue, sizeof( szValue ), strConfigFile.c_str() );
	strFormat = szValue;
}


//
// ��ģ�屣�浽�����ļ���
//
void
SaveTemplate( int nIndex, string &strName, string &strFormat )
{
	string strConfigFile;
	GetConfigFileName( strConfigFile );

	char szName[256];
	
	sprintf( szName, "Name%d", nIndex + 1 );
	WritePrivateProfileString( "Templates", szName, strName.c_str(), strConfigFile.c_str() );
	
	sprintf( szName, "Format%d", nIndex + 1 );
	WritePrivateProfileString( "Templates", szName, strFormat.c_str(), strConfigFile.c_str() );
}


//
// �������ļ���װ���Զ�����ɫ
//
void
LoadCustomColors( void )
{
	string strConfigFile;
	GetConfigFileName( strConfigFile );

	char szName[256];
	for( int i = 0; i < CUSTOMCOLOR_COUNT; i++ )
	{
		sprintf( szName, "Color%d", i + 1 );
		g_CustomColors[i] = GetPrivateProfileInt( "CustomColors", szName, 0, strConfigFile.c_str() );
	}
}


//
// ���Զ�����ɫ���浽�����ļ���
//
void
SaveCustomColors( void )
{
	string strConfigFile;
	GetConfigFileName( strConfigFile );

	char szName[256], szColor[16];
	for( int i = 0; i < CUSTOMCOLOR_COUNT; i++ )
	{
		sprintf( szName, "Color%d", i + 1 );
		sprintf( szColor, "%d", g_CustomColors[i] );
		WritePrivateProfileString( "CustomColors", szName, szColor, strConfigFile.c_str() );
	}
}


//
// ���ݸ���ģ�崴�����Ʋ˵�
//
void
BuildTmplMenu( void )
{
	if( g_hMenuTemplate )
	{
		DestroyMenu( g_hMenuTemplate );
		g_hMenuTemplate = NULL;
	}
	
	g_hMenuTemplate = CreatePopupMenu();
	if( g_hMenuTemplate )
	{
		char szName[256];
		for( int i = 0; i < TEMPLATE_COUNT; i++ )
		{
			if( g_Templates[i].m_strName.length() )
			{
				AppendMenu( g_hMenuTemplate, MF_STRING, WM_USER + i + 1, g_Templates[i].m_strName.c_str() );
			}
			else
			{
				sprintf( szName, "NO. %d", i + 1 );
				AppendMenu( g_hMenuTemplate, MF_STRING, 0, szName );
			}
		}
	}
}


//
// �õ�ָ�����ڵĶ��㴰��
//
HWND
GetRootWindow( HWND hWnd )
{
	if( NULL == hWnd )
		return( NULL );

	HWND hWndParent = hWnd;
	do
	{
		hWnd = hWndParent;
		hWndParent = GetParent( hWnd );
	}
	while( NULL != hWndParent );

	return( hWnd );
}


//
// �жϴ����Ƿ񳣾Ӷ���
//
bool
IsWndTopMost( POINT *pPoint )
{
	HWND hWnd = WindowFromPoint( *pPoint );
	if( hWnd )
	{
		HWND hWndAncestor = GetRootWindow( hWnd );
		if( hWndAncestor )
			return( IsWndTopMost( hWndAncestor ) );
	}
	
	return( false );
}


bool
IsWndTopMost( HWND hWnd )
{
	LONG lStyle = GetWindowLong( hWnd, GWL_EXSTYLE );
	if( WS_EX_TOPMOST & lStyle )
		return( true );
	else
		return( false );
}


//
// ʹ���ڳ��Ӷ���/�˾Ӷ���
//
bool
TopMostWndFromPoint( POINT *pPoint, bool bTopMost )
{
	HWND hWnd = WindowFromPoint( *pPoint );
	if( hWnd )
	{
		HWND hWndAncestor = GetRootWindow( hWnd );
		if( hWndAncestor )
			return( TopMostWndFromPoint( hWndAncestor, bTopMost ) );
	}

	return( false );
}


bool
TopMostWndFromPoint( HWND hWnd, bool bTopMost )
{
	if( SetWindowPos( hWnd, bTopMost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE ) )
		return( true );
	else
		return( false );
}


//
// ����Դ�м��ض������
// ������������ 256 ɫ�ģ�CreateIconFromResource() �ͻ�ʧ�ܷ��� NULL���� GetLastError() ���� 0
//
HCURSOR
LoadAniCursor(
	const char *pResourceType,  // ��Դ����
	UINT        uID )           // ��Դ ID
{
	HINSTANCE hInst = _Module.GetResourceInstance();
	HRSRC hRes = FindResource( hInst, MAKEINTRESOURCE( uID ), pResourceType );
	if( hRes )
	{
		DWORD   dwSize = SizeofResource( hInst, hRes );
		HGLOBAL hGlob  = LoadResource( hInst, hRes );
		if( hGlob )
		{
			LPBYTE pBytes = (LPBYTE)LockResource( hGlob ); 
			if( pBytes )
				return( (HCURSOR)CreateIconFromResource( pBytes, dwSize, FALSE, 0x00030000 ) );
		}
	}

	return( NULL );
}