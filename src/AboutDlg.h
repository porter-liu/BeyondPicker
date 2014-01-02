// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__F067184E_21DB_4881_A3F1_945E8294C709__INCLUDED_)
#define AFX_ABOUTDLG_H__F067184E_21DB_4881_A3F1_945E8294C709__INCLUDED_


#include "resource.h"
#include "AboutPanel.h"
#include "DonatePanel.h"
#include "CopyrightPanel.h"
#include "FeedbackPanel.h"


static const char *g_pAboutText     = "关于";
static const char *g_pDonateText    = "捐赠";
static const char *g_pCopyrightText = "版权声明";
static const char *g_pFeedbackText  = "建议与反馈";


class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		NOTIFY_CODE_HANDLER( TCN_SELCHANGE , OnTabSelChange )
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow( GetParent() );

		RECT rcClient = { 0 };
		GetClientRect( &rcClient );
		rcClient.top = 5;
		rcClient.left += 5;
		rcClient.right -= 5;
		rcClient.bottom = 30;
		m_TabCtrl.Create( m_hWnd, rcClient, NULL, WS_CHILD | WS_VISIBLE | TCS_BUTTONS | TCS_FLATBUTTONS | TCS_HOTTRACK );
		m_TabCtrl.SetFont( (HFONT)GetStockObject( DEFAULT_GUI_FONT ) );

		//
		// 下面创建各 Panel 并设置其大小及位置
		//
		GetClientRect( &rcClient );
		rcClient.top = 30;
		TCITEM Item = { 0 };
		Item.mask = TCIF_TEXT;

		//
		// 关于
		//
		Item.pszText = (char *)g_pAboutText;
		Item.cchTextMax = strlen( g_pAboutText );
		m_TabCtrl.InsertItem( m_TabCtrl.GetItemCount(), &Item );

		m_Panels.push_back( m_AboutPanel.Create( m_hWnd ) );
		m_AboutPanel.MoveWindow( &rcClient );

		//
		// 捐赠
		//
		Item.pszText = (char *)g_pDonateText;
		Item.cchTextMax = strlen( g_pDonateText );
		m_TabCtrl.InsertItem( m_TabCtrl.GetItemCount(), &Item );

		m_Panels.push_back( m_DonatePanel.Create( m_hWnd ) );
		m_DonatePanel.MoveWindow( &rcClient );

		//
		// 版权声明
		//
		Item.pszText = (char *)g_pCopyrightText;
		Item.cchTextMax = strlen( g_pCopyrightText );
		m_TabCtrl.InsertItem( m_TabCtrl.GetItemCount(), &Item );

		m_Panels.push_back( m_CopyrightPanel.Create( m_hWnd ) );
		m_CopyrightPanel.MoveWindow( &rcClient );

		//
		// 建议与反馈
		//
		Item.pszText = (char *)g_pFeedbackText;
		Item.cchTextMax = strlen( g_pFeedbackText );
		m_TabCtrl.InsertItem( m_TabCtrl.GetItemCount(), &Item );

		m_Panels.push_back( m_FeedbackPanel.Create( m_hWnd ) );
		m_FeedbackPanel.MoveWindow( &rcClient );

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnTabSelChange(int idCtrl, LPNMHDR pnmh, BOOL &bHandled )
	{
		for( int i = 0; i < m_Panels.size(); i++ )
			::ShowWindow( m_Panels[i], SW_HIDE );

		::ShowWindow( m_Panels[m_TabCtrl.GetCurSel()], SW_SHOW );
		return( 0 );
	}

private:
	CTabCtrl        m_TabCtrl;
	CAboutPanel     m_AboutPanel;
	CDonatePanel    m_DonatePanel;
	CCopyrightPanel m_CopyrightPanel;
	CFeedbackPanel  m_FeedbackPanel;
	vector<HWND>    m_Panels;
};

#endif // !defined(AFX_ABOUTDLG_H__F067184E_21DB_4881_A3F1_945E8294C709__INCLUDED_)
