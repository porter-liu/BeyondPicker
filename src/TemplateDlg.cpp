#include "StdAfx.h"
#include "TemplateDlg.h"
#include "BeyondPicker.h"


CTemplateDlg::CTemplateDlg()
	: m_TmplSN( NULL )
{
	;
}


LRESULT
CTemplateDlg::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	m_TmplSN     = GetDlgItem( IDC_SN );
	m_BtnSave    = GetDlgItem( IDC_SAVE );
	m_BtnTest    = GetDlgItem( IDC_TEST );
	m_TmplName   = GetDlgItem( IDC_NAME );
	m_TmplFormat = GetDlgItem( IDC_FORMAT );

	m_BtnSave.EnableWindow( FALSE );

	char szSN[16];
	for( int i = 1; i <= TEMPLATE_COUNT; i++ )
	{
		sprintf( szSN, "NO. %d", i );
		m_TmplSN.AddString( szSN );
	}
	m_TmplSN.SetCurSel( 0 );
	m_TmplName.SetWindowText(   g_Templates[0].m_strName.c_str() );
	m_TmplFormat.SetWindowText( g_Templates[0].m_strFormat.c_str() );
	m_BtnSave.EnableWindow( FALSE );  // Ҫ�ڸı� Edit ���ݺ���ñ��水ť

	return( 0 );
}


LRESULT
CTemplateDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	EndDialog( 0 );
	return( 0 );
}


//
// ����˱���ģ�尴ť
//
LRESULT
CTemplateDlg::OnSave( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	int nIndex = m_TmplSN.GetCurSel();

	char szName[256], szFormat[256];
	m_TmplName.GetWindowText(   szName,   sizeof( szName ) );
	m_TmplFormat.GetWindowText( szFormat, sizeof( szFormat ) );
	if( 0 == strlen( szName ) || 0 == strlen( szFormat ) )
	{
		MessageBox( "���ƺ͸�ʽ������д��", GetAppName(), MB_OK | MB_ICONINFORMATION );
		return( 0 );
	}

	if( !Test() )  // ����ʧ�ܵĻ�������
		return( 0 );

	g_Templates[nIndex].m_strName   = szName;
	g_Templates[nIndex].m_strFormat = szFormat;
	m_BtnSave.EnableWindow( FALSE );  // ���水ťҪ�ڱ�������
	return( 0 );
}


//
// �Ե�ǰģ���ʽ�﷨���в���
//
bool
CTemplateDlg::Test( void )
{
	int nIndex = m_TmplSN.GetCurSel();

	char szFormat[256];
	m_TmplFormat.GetWindowText( szFormat, sizeof( szFormat ) );

	try
	{
		char szTest[256] = { 0 };
		sprintf( szTest, szFormat, 0, 128, 255 );

		char szInfo[256];
		sprintf( szInfo, "�� R=%d, G=%d, B=%d ���ԵĽ��Ϊ��\n\n%s", 0, 128, 255, szTest );
		MessageBox( szInfo, GetAppName(), MB_OK | MB_ICONINFORMATION );
		return( true );
	}
	catch( ... )
	{
		MessageBox( "����ģ���ʽ���﷨����", GetAppName(), MB_OK | MB_ICONINFORMATION );
		return( false );
	}
}


LRESULT
CTemplateDlg::OnTest( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	Test();
	return( 0 );
}


LRESULT
CTemplateDlg::OnDelete( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	int nIndex = m_TmplSN.GetCurSel();
	g_Templates[nIndex].m_strName   = "";
	g_Templates[nIndex].m_strFormat = "";

	m_TmplName.SetWindowText( NULL );
	m_TmplFormat.SetWindowText( NULL );
	return( 0 );
}


//
// ʹ��Ԥ�Ƹ���ģ��
//
LRESULT
CTemplateDlg::OnDefault( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	for( int i = 0; i < TEMPLATE_COUNT; i++ )
		LoadDefaultTemplate( i, g_Templates[i].m_strName, g_Templates[i].m_strFormat );

	m_TmplSN.SetCurSel( 0 );
	m_TmplName.SetWindowText(   g_Templates[0].m_strName.c_str() );
	m_TmplFormat.SetWindowText( g_Templates[0].m_strFormat.c_str() );
	m_BtnSave.EnableWindow( FALSE );
	return( 0 );
}


//
// �ı���ģ��
//
LRESULT
CTemplateDlg::OnTemplateChange( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( CBN_SELCHANGE != wNotifyCode )
		return( 0 );

	int nIndex = m_TmplSN.GetCurSel();

	m_TmplName.SetWindowText(   g_Templates[nIndex].m_strName.c_str() );
	m_TmplFormat.SetWindowText( g_Templates[nIndex].m_strFormat.c_str() );
	m_BtnSave.EnableWindow( FALSE );
	return( 0 );
}


//
// ����ģ������ƻ��ʽ�����˱仯
//
LRESULT
CTemplateDlg::OnNameOrFormatChange( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( EN_CHANGE != wNotifyCode )
		return( 0 );

	// ���߶���д�����ݲ��ܱ���
	if( 0 == m_TmplName.GetWindowTextLength() || 0 == m_TmplFormat.GetWindowTextLength() )
		m_BtnSave.EnableWindow( FALSE );
	else
		m_BtnSave.EnableWindow( TRUE );

	// ֻҪ��ʽ����Ϊ�վͿɲ���
	if( m_TmplFormat.GetWindowTextLength() > 0 )
		m_BtnTest.EnableWindow( TRUE );
	else
		m_BtnTest.EnableWindow( FALSE );
	return( 0 );
}