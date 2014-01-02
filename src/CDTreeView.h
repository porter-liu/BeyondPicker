#ifndef __CCDTREEVIEW_H__20051011__
#define __CCDTREEVIEW_H__20051011__


class CCDTreeView
	: public CWindowImpl< CCDTreeView, CTreeViewCtrl >
	, public CCustomDraw< CCDTreeView >
{
public:
    BEGIN_MSG_MAP( CCDTreeView )
		MESSAGE_HANDLER( WM_LBUTTONDBLCLK, OnLButtonDBClick )
		MESSAGE_HANDLER( WM_RBUTTONDOWN, OnRButtonDown )
//		REFLECTED_NOTIFY_CODE_HANDLER_EX(TVN_ITEMEXPANDING, OnItemExpanding)
        CHAIN_MSG_MAP_ALT( CCustomDraw<CCDTreeView>, 1 )
        DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
		
//	LRESULT OnItemExpanding ( NMHDR* phdr );

	typedef CWindowImpl< CCDTreeView, CTreeViewCtrl > inherited;
	struct ItemInfo
	{
		bool     m_bGroup;
		COLORREF m_uColor;
	};

	CCDTreeView();
	virtual ~CCDTreeView();

	HTREEITEM InsertGroup( const char *pName );
	HTREEITEM InsertColor( HTREEITEM hGroup, const char *pName, COLORREF uColor );
	BOOL DeleteItem( HTREEITEM hItem );
	BOOL DeleteAllItems( void );

	bool IsGroup( HTREEITEM hItem );

public:
	DWORD OnPrePaint( int idCtrl, LPNMCUSTOMDRAW lpNMCD );
	DWORD OnItemPrePaint( int idCtrl, LPNMCUSTOMDRAW lpNMCD );

private:
	void DrawItemBk( NMTVCUSTOMDRAW *pNMTVCD, NMCUSTOMDRAW *pNMCD );
	void DrawItem( NMTVCUSTOMDRAW *pNMTVCD, NMCUSTOMDRAW *pNMCD );

	LRESULT OnLButtonDBClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );
	LRESULT OnRButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled );

private:
	HBRUSH m_hBrushGroupN;
	HBRUSH m_hBrushGroupS;
	HPEN   m_hPenGroupN;
	HPEN   m_hPenGroupS;
	HBRUSH m_hBrushItemN;
	HBRUSH m_hBrushItemS;
	HPEN   m_hPenItemN;
	HPEN   m_hPenItemS;
};


#endif  // __CCDTREEVIEW_H__20051011__