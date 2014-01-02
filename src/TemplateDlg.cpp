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
	m_BtnSave.EnableWindow( FALSE );  // 要在改变 Edit 内容后禁用保存按钮

	return( 0 );
}


LRESULT
CTemplateDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	EndDialog( 0 );
	return( 0 );
}


//
// 点击了保存模板按钮
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
		MessageBox( "名称和格式必须填写！", GetAppName(), MB_OK | MB_ICONINFORMATION );
		return( 0 );
	}

	if( !Test() )  // 测试失败的话不保存
		return( 0 );

	g_Templates[nIndex].m_strName   = szName;
	g_Templates[nIndex].m_strFormat = szFormat;
	m_BtnSave.EnableWindow( FALSE );  // 保存按钮要在保存后禁用
	return( 0 );
}


//
// 对当前模板格式语法进行测试
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
		sprintf( szInfo, "用 R=%d, G=%d, B=%d 测试的结果为：\n\n%s", 0, 128, 255, szTest );
		MessageBox( szInfo, GetAppName(), MB_OK | MB_ICONINFORMATION );
		return( true );
	}
	catch( ... )
	{
		MessageBox( "您的模板格式有语法错误！", GetAppName(), MB_OK | MB_ICONINFORMATION );
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
// 使用预制复制模板
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
// 改变了模板
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
// 复制模板的名称或格式发生了变化
//
LRESULT
CTemplateDlg::OnNameOrFormatChange( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled )
{
	if( EN_CHANGE != wNotifyCode )
		return( 0 );

	// 两者都填写了内容才能保存
	if( 0 == m_TmplName.GetWindowTextLength() || 0 == m_TmplFormat.GetWindowTextLength() )
		m_BtnSave.EnableWindow( FALSE );
	else
		m_BtnSave.EnableWindow( TRUE );

	// 只要格式串不为空就可测试
	if( m_TmplFormat.GetWindowTextLength() > 0 )
		m_BtnTest.EnableWindow( TRUE );
	else
		m_BtnTest.EnableWindow( FALSE );
	return( 0 );
}