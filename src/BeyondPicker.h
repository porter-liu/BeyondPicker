#ifndef __BEYONDPICKER_H__20050920__
#define __BEYONDPICKER_H__20050920__


#include "MainDlg.h"


#define TEMPLATE_COUNT    10  // �Զ��帴��ģ�������
#define CUSTOMCOLOR_COUNT 16  // �Զ�����ɫ������


struct Template
{
	string m_strName;
	string m_strFormat;
};


extern vector<Template> g_Templates;  // �Զ���ģ������(���ƺ͸�ʽ�ַ���)
extern HMENU g_hMenuTemplate;         // �Զ���ģ��˵�
extern HMENU g_hMenuGroup;            // ��ɫ�����Ҽ��˵�
extern HMENU g_hMenuColor;            // ��ɫ��������ɫ���Ҽ��˵�
extern COLORREF g_CustomColors[CUSTOMCOLOR_COUNT];  // ���� 16 ���Զ�����ɫ
extern BYTE g_crRed;    // ����Ļ��ȡ����ɫ�еĺ�ɫֵ
extern BYTE g_crGreen;
extern BYTE g_crBlue;
extern CMainDlg g_MainUI;  // ������


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