#include "StdAfx.h"
#include "CopyrightPanel.h"


LRESULT
CCopyrightPanel::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	m_HyperLink.SubclassWindow( GetDlgItem( IDC_CONTACT_US ) );
	m_HyperLink.SetHyperLink( "https://github.com/porter-liu" );
	//m_HyperLink.SetHyperLinkExtendedStyle( HLINK_NOTOOLTIP );
	return( 0 );
}