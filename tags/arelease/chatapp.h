#ifndef CHATAPP_H
#define CHATAPP_H
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "chatmainfrm.h"
#include "chatudp.h"
#include "chatsound.h"
#include "chatstatusmode.h"
#include "chatusers.h"

// ----------------------------------------------------------------------------
// ---------- CChatApp
// ----------------------------------------------------------------------------
class CChatApp: public CWinApp
{
protected:
	unsigned int port;
	CString userName;
	CString hostName;
	CString ip;
	CString broadcastIP;

	CFont font;

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	bool initSocket();

	afx_msg void OnRefreshUsersList( UINT nID = 0 );
	afx_msg void OnStatusMode( UINT nID );
	afx_msg void OnUpdateStatusMode( CCmdUI* pCmdUI );
	afx_msg void OnStatusModeInfo( UINT nID );
	afx_msg void OnUpdateStatusModeInfo( CCmdUI* pCmdUI );
	afx_msg void OnToCryOut();
	afx_msg void OnOptions();

	DECLARE_MESSAGE_MAP()

public:
	CChatApp();

	CChatMainFrame* pFrame;
	CChatUdp udp;
	CChatSoundList sounds;
	CChatStatusModeList statusModes;
	CChatUsers users;

	CString getUserName() const    { return userName; }
	void setUserName( const CString& value );
	CString getHostName() const    { return hostName; }
	CString getIP() const          { return ip; }
	unsigned int getPort() const   { return port; }
	CString getBroadcastIP() const { return broadcastIP; }

	CChatStatusModeType getStatusMode();
	void setStatusMode( const CChatStatusModeType value, const bool automatically = false );

	CFont& getFont();
};

// ----------------------------------------------------------------------------
// ---------- CChatToCryOutDialog
// ----------------------------------------------------------------------------
class CChatToCryOutDialog: CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX );

public:
	CString message;

	CChatToCryOutDialog( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	virtual ~CChatToCryOutDialog();

	virtual int DoModal();
};

// ----------------------------------------------------------------------------
extern CChatApp chatApp;

#endif // CHATAPP_H
