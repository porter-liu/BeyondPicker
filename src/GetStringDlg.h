#ifndef __GETSTRINGDLG_H__20051013__
#define __GETSTRINGDLG_H__20051013__


#include "resource.h"


class CGetStringDlg : public CDialogImpl< CGetStringDlg >
{
public:
	enum { IDD = IDD_GET_STRING };

	BEGIN_MSG_MAP( CGetStringDlg )
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		MESSAGE_HANDLER( WM_CLOSE, OnClose )
		COMMAND_ID_HANDLER( IDOK, OnOK )
		COMMAND_ID_HANDLER( IDCANCEL, OnCancel )
	END_MSG_MAP()

	CGetStringDlg( const char *pInfo, const char *pString = NULL );

	const char *String( void ) { return( m_strString.c_str() ); }
	void String( const char *pString ) { m_strString = pString; }

private:
	LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );

	LRESULT OnOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );

private:
	string m_strInfo;
	string m_strString;
};


#endif  // __GETSTRINGDLG_H__20051013__