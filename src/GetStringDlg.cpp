#include "StdAfx.h"
#include "GetStringDlg.h"
#include "BeyondPicker.h"


CGetStringDlg::CGetStringDlg( const char *pInfo, const char *pString )
	: m_strInfo( pInfo )
{
	if( pString )
		m_strString = pString;
}


LRESULT
CGetStringDlg::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	SetDlgItemText( IDC_TEXT,   m_strInfo.c_str() );
	SetDlgItemText( IDC_STRING, m_strString.c_str() );
	return( 0 );
}


LRESULT
CGetStringDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	EndDialog( IDCANCEL );
	return( 0 );
}


LRESULT
CGetStringDlg::OnOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	char szString[256] = { 0 };
	CEdit Name = GetDlgItem( IDC_STRING );
	Name.GetWindowText( szString, sizeof( szString ) );
	if( 0 == strlen( szString ) )
	{
		MessageBox( "Please enter scheme name!", GetAppName(), MB_OK | MB_ICONINFORMATION );
		Name.SetFocus();
		return( 0 );
	}

	m_strString = szString;
	EndDialog( IDOK );
	return( 0 );
}


LRESULT
CGetStringDlg::OnCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	EndDialog( IDCANCEL );
	return( 0 );
}