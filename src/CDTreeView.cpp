#include "StdAfx.h"
#include "CDTreeView.h"


CCDTreeView::CCDTreeView()
	: m_hBrushGroupN( NULL )
	, m_hBrushGroupS( NULL )
	, m_hPenGroupN( NULL )
	, m_hPenGroupS( NULL )
	, m_hBrushItemN( NULL )
	, m_hBrushItemS( NULL )
	, m_hPenItemN( NULL )
	, m_hPenItemS( NULL )
{
	m_hBrushGroupN = CreateSolidBrush( 0xF6F6F6 );
	m_hBrushGroupS = CreateSolidBrush( 0xE8F3FF );
	m_hPenGroupN   = CreatePen( PS_SOLID, 1, 0xDDDDDD );
	m_hPenGroupS   = CreatePen( PS_SOLID, 1, 0x3399FF );
	m_hBrushItemN  = GetSysColorBrush( COLOR_WINDOW );
	m_hBrushItemS  = CreateSolidBrush( 0xFFEFE6 );
	m_hPenItemN    = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_WINDOW ) );
	m_hPenItemS    = CreatePen( PS_SOLID, 1, 0xFF6600 );
}


CCDTreeView::~CCDTreeView()
{
	if( m_hBrushGroupN )
		DeleteObject( m_hBrushGroupN );
	if( m_hBrushGroupS )
		DeleteObject( m_hBrushGroupS );
	if( m_hPenGroupN )
		DeleteObject( m_hPenGroupN );
	if( m_hPenGroupS )
		DeleteObject( m_hPenGroupS );
	if( m_hBrushItemN )
		DeleteObject( m_hBrushItemN );
	if( m_hBrushItemS )
		DeleteObject( m_hBrushItemS );
	if( m_hPenItemN )
		DeleteObject( m_hPenItemN );
	if( m_hPenItemS )
		DeleteObject( m_hPenItemS );
}


HTREEITEM
CCDTreeView::InsertGroup( const char *pName )
{
	ItemInfo *pItemInfo = new ItemInfo;
	if( pItemInfo )
	{
		pItemInfo->m_bGroup = true;

		HTREEITEM hItem = InsertItem( pName, NULL, TVI_LAST );
		if( hItem )
		{
			SetItemData( hItem, (DWORD)pItemInfo );
			return( hItem );
		}
	}

	if( pItemInfo )
		delete pItemInfo;
	return( NULL );
}


HTREEITEM
CCDTreeView::InsertColor( HTREEITEM hGroup, const char *pName, COLORREF uColor )
{
	ItemInfo *pItemInfo = new ItemInfo;
	if( hGroup && pItemInfo )
	{
		pItemInfo->m_bGroup = false;
		pItemInfo->m_uColor = uColor;

		HTREEITEM hItem = InsertItem( pName, hGroup, TVI_LAST );
		if( hItem )
		{
			SetItemData( hItem, (DWORD)pItemInfo );
			return( hItem );
		}
	}

	if( pItemInfo )
		delete pItemInfo;
	return( NULL );
}


BOOL
CCDTreeView::DeleteItem( HTREEITEM hItem )
{
	ItemInfo *pItemInfo = (ItemInfo *)GetItemData( hItem );
	if( pItemInfo)
		delete pItemInfo;

	HTREEITEM hChildItem = GetNextItem( hItem, TVGN_CHILD );
	while( hChildItem )
	{
		pItemInfo = (ItemInfo *)GetItemData( hChildItem );
		if( pItemInfo)
			delete pItemInfo;
		hChildItem = GetNextItem( hChildItem, TVGN_NEXT );
	}

	return( inherited::DeleteItem( hItem ) );
}


BOOL
CCDTreeView::DeleteAllItems( void )
{
	HTREEITEM hItem = GetNextItem( NULL, TVGN_ROOT );
	while( hItem )
	{
		ItemInfo *pItemInfo = (ItemInfo *)GetItemData( hItem );
		if( pItemInfo )
			delete pItemInfo;

		hItem = GetNextItem( hItem, TVGN_CHILD );
		if( NULL == hItem )
			hItem = GetNextItem( hItem, TVGN_NEXT );
	}

	return( inherited::DeleteAllItems() );
}


//
// 判断某 Item 是否 Group
//
bool
CCDTreeView::IsGroup( HTREEITEM hItem )
{
	ItemInfo *pItemInfo = (ItemInfo *)GetItemData( hItem );
	if( pItemInfo )
		return( pItemInfo->m_bGroup );
	else
		return( false );
}


//
//
//


DWORD
CCDTreeView::OnPrePaint( int idCtrl, LPNMCUSTOMDRAW lpNMCD )
{
	return( CDRF_NOTIFYITEMDRAW );  // 返回这个才导致 OnItemPrePaint 被调用
}


DWORD
CCDTreeView::OnItemPrePaint( int idCtrl, LPNMCUSTOMDRAW lpNMCD )
{
	NMTVCUSTOMDRAW *pNMTVCD = (NMTVCUSTOMDRAW *)lpNMCD;
	NMCUSTOMDRAW   *pNMCD   = &pNMTVCD->nmcd;

	DrawItemBk( pNMTVCD, pNMCD );
	DrawItem( pNMTVCD, pNMCD );

	return( CDRF_SKIPDEFAULT );  // 不进行默认的绘制，我们自己画；要进行默认绘制则要返回 CDRF_DODEFAULT
}


void
CCDTreeView::DrawItemBk(
	NMTVCUSTOMDRAW *pNMTVCD,
	NMCUSTOMDRAW   *pNMCD )
{
	CDCT<false> dc = pNMCD->hdc;
	
	HBRUSH hBrushOld = NULL;
	HPEN   hPenOld   = NULL;

	ItemInfo *pItemInfo = (ItemInfo *)GetItemData( (HTREEITEM)pNMCD->dwItemSpec );
	if( NULL == pItemInfo )
		return;

	if( pItemInfo->m_bGroup )
	{
		if( pNMCD->uItemState & CDIS_SELECTED )
		{
			hBrushOld = dc.SelectBrush( m_hBrushGroupS );
			hPenOld = dc.SelectPen( m_hPenGroupS );
		}
		else
		{
			hBrushOld = dc.SelectBrush( m_hBrushGroupN );
			hPenOld = dc.SelectPen( m_hPenGroupN );
		}
		POINT pt = { 5, 5 };
		dc.RoundRect( &pNMCD->rc, pt );
		dc.SelectPen( hPenOld );
		dc.SelectBrush( hBrushOld );
	}
	else
	{
		RECT rcItem( pNMCD->rc );
		rcItem.right = pNMTVCD->iLevel * GetIndent();
		InflateRect( &rcItem, -1, -1 );
		HBRUSH hBrushColor = CreateSolidBrush( pItemInfo->m_uColor );
		hBrushOld = dc.SelectBrush( hBrushColor );
		POINT pt = { 4, 4 };
		dc.RoundRect( &rcItem, pt );
		dc.SelectBrush( hBrushOld );
		DeleteObject( hBrushColor );

		pNMCD->rc.left += pNMTVCD->iLevel * GetIndent();
		if( pNMCD->uItemState & CDIS_SELECTED )
		{
			hBrushOld = dc.SelectBrush( m_hBrushItemS );
			hPenOld = dc.SelectPen( m_hPenItemS );
		}
		else
		{
			hBrushOld = dc.SelectBrush( m_hBrushItemN );
			hPenOld = dc.SelectPen( m_hPenItemN );
		}
		dc.RoundRect( &pNMCD->rc, pt );
		dc.SelectPen( hPenOld );
		dc.SelectBrush( hBrushOld );
	}
}


void
CCDTreeView::DrawItem(
	NMTVCUSTOMDRAW *pNMTVCD,
	NMCUSTOMDRAW   *pNMCD )
{
	CDCT<false> dc = pNMCD->hdc;
	dc.SetBkMode( TRANSPARENT );

	char szText[256];
	GetItemText( (HTREEITEM)pNMCD->dwItemSpec, szText, sizeof( szText ) );
	
	/*if( pNMCD->uItemState & CDIS_SELECTED )
		dc.SetTextColor( RGB( 255, 255, 255 ) );
	else*/
		dc.SetTextColor( RGB( 0, 0, 0 ) );
	pNMCD->rc.left  += 4;
	pNMCD->rc.right -= 4;
	dc.DrawText( szText, strlen( szText ), &pNMCD->rc, DT_VCENTER | DT_SINGLELINE );
}


//
// 使得父窗口在鼠标双击了 Item 的非文字区域后仍然可以收到 NM_DBLCLK 通知消息
//
LRESULT
CCDTreeView::OnLButtonDBClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	TVHITTESTINFO tvhti = { 0 };
	tvhti.pt.x = LOWORD( lParam );
	tvhti.pt.y = HIWORD( lParam );
	HitTest( &tvhti );
	if( tvhti.hItem && !( tvhti.flags & TVHT_ONITEM ) )  // 只在点击了非文字区域时才进行处理
	{
		ItemInfo *pItemInfo = (ItemInfo *)GetItemData( tvhti.hItem );
		if( pItemInfo )
		{
			NMHDR nmhdr = { 0 };
			nmhdr.hwndFrom = m_hWnd;
			nmhdr.idFrom   = GetWindowLong( GWL_ID );
			nmhdr.code     = NM_DBLCLK;
			::SendMessage( GetParent(), WM_NOTIFY, GetWindowLong( GWL_ID ), (LPARAM)&nmhdr );
		}
	}

	bHandled = FALSE;
	return( 0 );
}


//
// 使得鼠标右击会选中 Item
//
LRESULT
CCDTreeView::OnRButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	TVHITTESTINFO tvhti = { 0 };
	tvhti.pt.x = LOWORD( lParam );
	tvhti.pt.y = HIWORD( lParam );
	HitTest( &tvhti );
	if( tvhti.hItem )
		SelectItem( tvhti.hItem );

	bHandled = FALSE;
	return( 0 );
}
