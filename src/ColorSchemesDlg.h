#ifndef __COLORSCHEMESDLG_H__20051012__
#define __COLORSCHEMESDLG_H__20051012__


#include "resource.h"
#include "CDTreeView.h"
#include "EditColorDlg.h"
#include "BeyondPicker.h"


class CColorSchemesDlg : public CDialogImpl< CColorSchemesDlg >
{
public:
	enum { IDD = IDD_COLOR_SCHEMES };

	BEGIN_MSG_MAP( CColorSchemesDlg )
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		MESSAGE_HANDLER( WM_CLOSE, OnClose )
		MESSAGE_HANDLER( WM_DESTROY, OnDestroy )
		COMMAND_ID_HANDLER( IDC_RENAME, OnRename )
		COMMAND_ID_HANDLER( IDC_DELETE, OnDelete )
		COMMAND_ID_HANDLER( IDC_SET_CURRENT_COLOR, OnSetCurrentColor )
		COMMAND_ID_HANDLER( IDC_MODIFY, OnModifyColor )
		COMMAND_ID_HANDLER( IDC_ADD_COLOR, OnAddColor )
		COMMAND_RANGE_HANDLER( WM_USER + 1, WM_USER + TEMPLATE_COUNT, OnApplyTemplate )
		NOTIFY_CODE_HANDLER( NM_RCLICK, OnTVRClick )
		NOTIFY_CODE_HANDLER( NM_DBLCLK, OnTVDBClick )
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CColorSchemesDlg();
	unsigned int GetAllSchemes( vector<Scheme> &Schemes );
	void AddColor( HWND hParentWnd );

private:
	LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );

	LRESULT OnRename( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnDelete( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnSetCurrentColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnModifyColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnAddColor( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );
	LRESULT OnApplyTemplate( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled );

	LRESULT OnTVRClick( int idCtrl, LPNMHDR pnmh, BOOL &bHandled );
	LRESULT OnTVDBClick( int idCtrl, LPNMHDR pnmh, BOOL &bHandled );

private:
	CCDTreeView m_TreeView;
	HTREEITEM   m_hItem;
	HMENU       m_hMenu;
};


#endif  // __COLORSCHEMESDLG_H__20051012__