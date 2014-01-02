#ifndef __TEMPLATE_H__20050921__
#define __TEMPLATE_H__20050921__


#include "resource.h"


class CTemplateDlg : public CDialogImpl< CTemplateDlg >
{
public:
	enum { IDD = IDD_TEMPLATE };
	
	BEGIN_MSG_MAP( CTemplateDlg )
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		MESSAGE_HANDLER( WM_CLOSE, OnClose )
		COMMAND_ID_HANDLER( IDC_SAVE, OnSave )
		COMMAND_ID_HANDLER( IDC_TEST, OnTest )
		COMMAND_ID_HANDLER( IDC_DELETE, OnDelete )
		COMMAND_ID_HANDLER( IDC_DEFAULT, OnDefault )
		COMMAND_ID_HANDLER( IDC_SN, OnTemplateChange )
		COMMAND_ID_HANDLER( IDC_NAME, OnNameOrFormatChange )
		COMMAND_ID_HANDLER( IDC_FORMAT, OnNameOrFormatChange )
		//COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	CTemplateDlg();

private:
	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnSave( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnTest( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnDelete( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnDefault( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnTemplateChange( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnNameOrFormatChange( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );

	bool Test( void );

private:
	CComboBox m_TmplSN;
	CEdit     m_TmplName;
	CEdit     m_TmplFormat;
	CButton   m_BtnSave;
	CButton   m_BtnTest;
};


#endif  // __TEMPLATE_H__20050921__