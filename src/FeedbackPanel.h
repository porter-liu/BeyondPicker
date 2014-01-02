#ifndef __FEEDBACKPANEL_H__20050907__
#define __FEEDBACKPANEL_H__20050907__


#include "resource.h"


class CFeedbackPanel : public CDialogImpl< CFeedbackPanel >
{
public:
	enum { IDD = IDD_FEEDBACK_PANEL };
	
	BEGIN_MSG_MAP( CFeedbackPanel )
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog )
		//COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		END_MSG_MAP()
		
		// Handler prototypes (uncomment arguments if needed):
		//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		LRESULT OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	
private:
	CHyperLink m_HyperLink;
};


#endif  // __FEEDBACKPANEL_H__20050907__