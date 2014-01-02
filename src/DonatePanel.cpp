#include "StdAfx.h"
#include "DonatePanel.h"
#include "BeyondPicker.h"


LRESULT
CDonatePanel::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	char szURL[1024] = { 0 };
	m_HyperLink.SubclassWindow( GetDlgItem( IDC_DONATE ) );
	sprintf(
		szURL,
		"https://github.com/porter-liu",
		GetAppName(),
		GetAppVer() );
	m_HyperLink.SetHyperLink( szURL );
	m_HyperLink.SetHyperLinkExtendedStyle( HLINK_NOTOOLTIP );
	return( 0 );
}