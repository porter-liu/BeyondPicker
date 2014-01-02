#include "StdAfx.h"
#include "ColorSchemesDlg.h"
#include "GetStringDlg.h"


CColorSchemesDlg::CColorSchemesDlg()
	: m_hItem( NULL )
	, m_hMenu( NULL )
{
	m_hMenu = CreatePopupMenu();
	AppendMenu( m_hMenu, MF_STRING, IDC_ADD_COLOR, "&Add color..." );
}


unsigned int
CColorSchemesDlg::GetAllSchemes( vector<Scheme> &Schemes )
{
	Schemes.clear();

	HTREEITEM hItem = m_TreeView.GetNextItem( NULL, TVGN_ROOT );
	while( hItem )
	{
		TCHAR szText[256] = { 0 };
		m_TreeView.GetItemText( hItem, szText, sizeof( szText ) );

		Scheme One;
		One.m_strName   = szText;
		One.m_hItem     = hItem;
		One.m_bSelected = ( hItem == m_hItem );
		Schemes.push_back( One );
		hItem = m_TreeView.GetNextItem( hItem, TVGN_NEXT );
	}

	return( Schemes.size() );
}


LRESULT
CColorSchemesDlg::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	HICON hIcon = (HICON)::LoadImage(
		_Module.GetResourceInstance(),
		MAKEINTRESOURCE( IDI_COLOR_SCHEMES ),
		IMAGE_ICON,
		::GetSystemMetrics( SM_CXICON ),
		::GetSystemMetrics( SM_CYICON ),
		LR_DEFAULTCOLOR );
	SetIcon( hIcon, TRUE );
	HICON hIconSmall = (HICON)::LoadImage(
		_Module.GetResourceInstance(),
		MAKEINTRESOURCE( IDI_COLOR_SCHEMES ),
		IMAGE_ICON,
		::GetSystemMetrics( SM_CXSMICON ),
		::GetSystemMetrics( SM_CYSMICON ),
		LR_DEFAULTCOLOR );
	SetIcon( hIconSmall, FALSE );

	m_TreeView.SubclassWindow( GetDlgItem( IDC_COLOR_SCHEMES ) );

	//
	// �������ļ���װ����ɫ����
	//
	TCHAR szName[256] = { 0 }, szValue[256] = { 0 };
	COLORREF uColor = 0;
	string strConfigFile;
	GetConfigFileName( strConfigFile );
	int nCountSchemes = GetPrivateProfileInt( "ColorSchemes", "Count", 0, strConfigFile.c_str() );
	for( int i = 0; i < nCountSchemes; i++ )
	{
		// ȡ��������
		sprintf( szName, "Scheme%d", i + 1 );
		GetPrivateProfileString( "ColorSchemes", szName, NULL, szValue, sizeof( szValue ), strConfigFile.c_str() );
		HTREEITEM hItem = m_TreeView.InsertGroup( szValue );
		// ȡ�˷����е���ɫ��
		sprintf( szName, "Scheme%dCount", i + 1 );
		int nCountColors = GetPrivateProfileInt( "ColorSchemes", szName, 0, strConfigFile.c_str() );
		for( int j = 0; j < nCountColors; j++ )
		{
			// ȡ��ɫֵ
			sprintf( szName, "Scheme%dColor%d", i + 1, j + 1 );
			uColor = GetPrivateProfileInt( "ColorSchemes", szName, 0, strConfigFile.c_str() );

			// ȡ��ɫ����
			sprintf( szName, "Scheme%dColor%dDesc", i + 1, j + 1 );
			GetPrivateProfileString( "ColorSchemes", szName, NULL, szValue, sizeof( szValue ), strConfigFile.c_str() );

			// ����
			m_TreeView.InsertColor( hItem, szValue, uColor );
		}
	}

#ifndef _DEBUG
	SetWindowPos( HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );  // ʹ���ڳ��Ӷ���
#endif
	return( 0 );
}


//
// ��ɫ���������ڳ��������ڼ�ʼ�մ���
//
LRESULT
CColorSchemesDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	ShowWindow( SW_HIDE );
	return( 0 );
}


LRESULT
CColorSchemesDlg::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	//
	// ����ɫ�������浽�����ļ���
	//
	TCHAR szName[256] = { 0 }, szValue[256] = { 0 };
	COLORREF uColor = 0;
	int nCountSchemes = 0, nCountColors = 0;

	string strConfigFile;
	GetConfigFileName( strConfigFile );
	WritePrivateProfileString( "ColorSchemes", NULL, NULL, strConfigFile.c_str() );

	HTREEITEM hItem = m_TreeView.GetNextItem( NULL, TVGN_ROOT );
	while( hItem )
	{
		nCountSchemes++;

		// ��������
		m_TreeView.GetItemText( hItem, szValue, sizeof( szValue ) );
		sprintf( szName, "Scheme%d", nCountSchemes );
		WritePrivateProfileString( "ColorSchemes", szName, szValue, strConfigFile.c_str() );

		nCountColors = 0;  // ��ɫ�����¼���
		HTREEITEM hChildItem = m_TreeView.GetNextItem( hItem, TVGN_CHILD );
		while( hChildItem )
		{
			nCountColors++;

			// ��ɫֵ
			CCDTreeView::ItemInfo *pItemInfo = (CCDTreeView::ItemInfo *)m_TreeView.GetItemData( hChildItem );
			if( pItemInfo )
				uColor = pItemInfo->m_uColor;
			sprintf( szName, "Scheme%dColor%d", nCountSchemes, nCountColors );
			sprintf( szValue, "%d", uColor );
			WritePrivateProfileString( "ColorSchemes", szName, szValue, strConfigFile.c_str() );

			// ��ɫ����
			sprintf( szName, "Scheme%dColor%dDesc", nCountSchemes, nCountColors );
			m_TreeView.GetItemText( hChildItem, szValue, sizeof( szValue ) );
			WritePrivateProfileString( "ColorSchemes", szName, szValue, strConfigFile.c_str() );

			hChildItem = m_TreeView.GetNextItem( hChildItem, TVGN_NEXT );
		}
		// �˷����е���ɫ��
		sprintf( szName, "Scheme%dCount", nCountSchemes );
		sprintf( szValue, "%d", nCountColors );
		WritePrivateProfileString( "ColorSchemes", szName, szValue, strConfigFile.c_str() );

		hItem = m_TreeView.GetNextItem( hItem, TVGN_NEXT );
	}
	//��ɫ��������
	sprintf( szValue, "%d", nCountSchemes );
	WritePrivateProfileString( "ColorSchemes", "Count", szValue, strConfigFile.c_str() );

	return( 0 );
}


LRESULT
CColorSchemesDlg::OnRename( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( m_hItem )
	{
		char szName[256] = { 0 };
		m_TreeView.GetItemText( m_hItem, szName, sizeof( szName ) );
		CGetStringDlg Dlg( "Please input new scheme name", szName );
		if( IDOK == Dlg.DoModal( m_hWnd ) )
			m_TreeView.SetItemText( m_hItem, Dlg.String() );
	}

	return( 0 );
}


LRESULT
CColorSchemesDlg::OnDelete( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( m_hItem )
	{
		const char *pInfo = NULL;
		if( m_TreeView.IsGroup( m_hItem ) )
			pInfo = "���ɾ����ɫ��������˷����е�������ɫ������ɾ����\n\nȷ��Ҫɾ������ɫ������";
		else
			pInfo = "ȷ��Ҫɾ������ɫ��";
		if( IDNO == MessageBox( pInfo, GetAppName(), MB_YESNO | MB_ICONQUESTION ) )
			return( 0 );

		m_TreeView.DeleteItem( m_hItem );
	}
	return( 0 );
}


LRESULT
CColorSchemesDlg::OnSetCurrentColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( m_hItem )
	{
		CCDTreeView::ItemInfo *pItemInfo = (CCDTreeView::ItemInfo *)m_TreeView.GetItemData( m_hItem );
		if( pItemInfo )
		{
			g_MainUI.ShowColorInfo( pItemInfo->m_uColor );
		}
	}
	return( 0 );
}


LRESULT
CColorSchemesDlg::OnModifyColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( m_hItem )
	{
		CCDTreeView::ItemInfo *pItemInfo = (CCDTreeView::ItemInfo *)m_TreeView.GetItemData( m_hItem );
		if( pItemInfo )
		{
			vector<Scheme> Schemes;
			GetAllSchemes( Schemes );

			// ѡ�и��ڵ㣨��ɫ������
			HTREEITEM hParentItem = m_TreeView.GetNextItem( m_hItem, TVGN_PARENT );
			vector< Scheme >::iterator it = Schemes.begin();
			while( Schemes.end() != it )
			{
				if( it->m_hItem == hParentItem )
					it->m_bSelected = true;
				else
					it->m_bSelected = false;
				it++;
			}

			char szDesc[256] = { 0 };
			m_TreeView.GetItemText( m_hItem, szDesc, sizeof( szDesc ) );

			CEditColorDlg Dlg( Schemes );
			Dlg.Color( pItemInfo->m_uColor );
			Dlg.Desc( szDesc );
			if( IDOK == Dlg.DoModal( m_hWnd ) )
			{
				Scheme *pScheme = Dlg.GetDestScheme();
				if( pScheme->m_hItem == m_TreeView.GetNextItem( m_hItem, TVGN_PARENT ) )  // û�иı���ɫ�����ķ���
				{
					pItemInfo->m_uColor = Dlg.Color();
					m_TreeView.SetItemText( m_hItem, Dlg.Desc() );
				}
				else  // �ı�����ɫ�����ķ���
				{
					HTREEITEM hParentItem = NULL;
					if( NULL == pScheme->m_hItem )  // ��Ҫ�����½��ķ�����
					{
						hParentItem = m_TreeView.InsertGroup( pScheme->m_strName.c_str() );
					}
					else  // ��Ҫ��������������
					{
						m_TreeView.DeleteItem( m_hItem );  // �Ӿ�λ��ɾ��
						hParentItem = pScheme->m_hItem;
						m_TreeView.InsertColor( hParentItem, Dlg.Desc(), Dlg.Color() );
						m_hItem = NULL;
					}
				}
			}
		}
	}
	return( 0 );
}


void
CColorSchemesDlg::AddColor( HWND hParentWnd )
{
	vector<Scheme> Schemes;
	GetAllSchemes( Schemes );

	CEditColorDlg Dlg( Schemes );
	Dlg.Color( RGB( g_crRed, g_crGreen, g_crBlue ) );
	if( IDOK == Dlg.DoModal( hParentWnd ) )
	{
		Scheme *pScheme = Dlg.GetDestScheme();
		HTREEITEM hItem = pScheme->m_hItem;
		if( NULL == hItem )  // ��Ҫ�½�һ������
			hItem = m_TreeView.InsertGroup( pScheme->m_strName.c_str() );
		m_TreeView.InsertColor( hItem, Dlg.Desc(), Dlg.Color() );
	}
}


LRESULT
CColorSchemesDlg::OnAddColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	AddColor( m_hWnd );
	return( 0 );
}


LRESULT
CColorSchemesDlg::OnApplyTemplate( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( m_hItem )
	{
		CCDTreeView::ItemInfo *pItemInfo = (CCDTreeView::ItemInfo *)m_TreeView.GetItemData( m_hItem );
		if( pItemInfo )
		{
			char szColor[256];
			try
			{
				sprintf(
					szColor,
					g_Templates[wID - WM_USER - 1].m_strFormat.c_str(),
					GetRValue( pItemInfo->m_uColor ),
					GetGValue( pItemInfo->m_uColor ),
					GetBValue( pItemInfo->m_uColor ) );
				SetClipboardText( m_hWnd, szColor );
			}
			catch( ... )
			{
				MessageBox( "ģ���趨����", GetAppName(), MB_OK | MB_ICONINFORMATION );
			}
		}
	}
	
	return( 0 );
}


LRESULT
CColorSchemesDlg::OnTVRClick( int idCtrl, LPNMHDR pnmh, BOOL &bHandled )
{
	DWORD dwPos = GetMessagePos();
	TVHITTESTINFO tvhti = { 0 };
	tvhti.pt.x = LOWORD( dwPos );
	tvhti.pt.y = HIWORD( dwPos );
	m_TreeView.ScreenToClient( &tvhti.pt );

	HMENU hMenu = NULL;
	m_TreeView.HitTest( &tvhti );
	if( NULL == tvhti.hItem )
	{
		hMenu = m_hMenu;
	}
	else
	{
		m_hItem = tvhti.hItem;
		if( m_TreeView.IsGroup( tvhti.hItem ) )
			hMenu = g_hMenuGroup;
		else
			hMenu = g_hMenuColor;
	}

	TrackPopupMenu(
		hMenu,
		TPM_TOPALIGN | TPM_LEFTALIGN,
		LOWORD( dwPos ),
		HIWORD( dwPos ),
		0,
		m_hWnd,
		NULL );
	return( 0 );
}


//
// ֻҪ˫�� Item ���ξͿ����յ����֪ͨ��Ϣ
//
LRESULT
CColorSchemesDlg::OnTVDBClick( int idCtrl, LPNMHDR pnmh, BOOL &bHandled )
{
	DWORD dwPos = GetMessagePos();
	TVHITTESTINFO tvhti = { 0 };
	tvhti.pt.x = LOWORD( dwPos );
	tvhti.pt.y = HIWORD( dwPos );
	m_TreeView.ScreenToClient( &tvhti.pt );

	m_TreeView.HitTest( &tvhti );
	if( tvhti.hItem )
	{
		CCDTreeView::ItemInfo *pItemInfo = (CCDTreeView::ItemInfo *)m_TreeView.GetItemData( tvhti.hItem );
		if( pItemInfo )
		{
			if( pItemInfo->m_bGroup )  // ˫�������κεط�������չ��/�۵�
			{
				if( !( tvhti.flags & TVHT_ONITEM ) )
					m_TreeView.Expand( tvhti.hItem, TVE_TOGGLE );
			}
			else  // ˫����ɫ���κ����򶼿��Խ����޸Ľ���
			{
				m_hItem = tvhti.hItem;

				// ���ﲻ��ֱ�ӵ���Ϣ���� OnModifyColor()���ᵼ�� CommCtrl.dll
				// �쳣������������ĶԻ�����̺�ʱ̫�ã�PostMessage() ��û����
				//OnModifyColor( 0, 0, NULL, bHandled );
				PostMessage( WM_COMMAND, MAKELONG( IDC_MODIFY, 0 ), NULL );
			}
		}
	}
	return( 0 );
}