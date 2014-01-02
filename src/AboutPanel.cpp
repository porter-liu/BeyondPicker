#include "StdAfx.h"
#include "AboutPanel.h"
#include "BeyondPicker.h"


LRESULT
CAboutPanel::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	char szNameVer[256] = { 0 };
	sprintf( szNameVer, "%s %s", GetAppName(), GetAppVer() );
	SetDlgItemText( IDC_NAME_VER, szNameVer );

	char szURL[1024] = { 0 };
	m_HyperLink.SubclassWindow( GetDlgItem( IDC_HYPERLINK ) );
	sprintf( szURL, "https://github.com/porter-liu" );
	m_HyperLink.SetHyperLink( szURL );
	m_HyperLink.SetHyperLinkExtendedStyle( HLINK_NOTOOLTIP );
	return( 0 );
}