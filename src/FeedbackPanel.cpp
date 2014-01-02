#include "StdAfx.h"
#include "FeedbackPanel.h"
#include "BeyondPicker.h"


LRESULT
CFeedbackPanel::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	char szURL[1024] = { 0 };
	m_HyperLink.SubclassWindow( GetDlgItem( IDC_TELLUS ) );
	sprintf(
		szURL,
		"https://github.com/porter-liu",
		GetAppName(),
		GetAppName(),
		GetAppVer() );
	m_HyperLink.SetHyperLink( szURL );
	m_HyperLink.SetHyperLinkExtendedStyle( HLINK_NOTOOLTIP );
	return( 0 );
}