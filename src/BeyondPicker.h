#ifndef __BEYONDPICKER_H__20050920__
#define __BEYONDPICKER_H__20050920__


#include "MainDlg.h"


#define TEMPLATE_COUNT    10  // 自定义复制模板的数量
#define CUSTOMCOLOR_COUNT 16  // 自定义颜色的数量


struct Template
{
	string m_strName;
	string m_strFormat;
};


extern vector<Template> g_Templates;  // 自定义模板数据(名称和格式字符串)
extern HMENU g_hMenuTemplate;         // 自定义模板菜单
extern HMENU g_hMenuGroup;            // 配色方案右键菜单
extern HMENU g_hMenuColor;            // 配色方案中颜色的右键菜单
extern COLORREF g_CustomColors[CUSTOMCOLOR_COUNT];  // 保存 16 种自定义颜色
extern BYTE g_crRed;    // 从屏幕上取到颜色中的红色值
extern BYTE g_crGreen;
extern BYTE g_crBlue;
extern CMainDlg g_MainUI;  // 主界面


const char *GetAppName( void );
const char *GetAppVer( void );
bool GetConfigFileName( string &strFileName );
void SetClipboardText( HWND hWnd, const char *pText );

void LoadDefaultTemplate( int nIndex, string &strName, string &strFormat );
void LoadTemplate( int nIndex, string &strName, string &strFormat );
void SaveTemplate( int nIndex, string &strName, string &strFormat );
void BuildTmplMenu( void );

void LoadCustomColors( void );
void SaveCustomColors( void );

HWND GetRootWindow( HWND hWnd );
bool IsWndTopMost( POINT *pPoint );
bool IsWndTopMost( HWND hWnd );
bool TopMostWndFromPoint( POINT *pPoint, bool bTopMost );
bool TopMostWndFromPoint( HWND hWnd, bool bTopMost );

HCURSOR LoadAniCursor( const char *pResourceType, UINT uID );


#endif  // __BEYONDPICKER_H__20050920__