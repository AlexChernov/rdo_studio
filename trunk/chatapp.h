#ifndef CHATAPP_H
#define CHATAPP_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "chatudp.h"
#include "chatsound.h"
#include "chatsmile.h"
#include "chatstatusmode.h"
#include "chatuserlist.h"
#include "chatnetwork.h"

// ----------------------------------------------------------------------------
// ---------- CChatApp
// ----------------------------------------------------------------------------
class CChatMainFrame;

class CChatApp: public CWinApp
{
private:
	unsigned int port;
	std::string userName;
	std::string hostName;
	std::string ip;
	std::string broadcastIP;

	CFont font;

	bool initSocket();

public:
	CChatApp();

	CChatMainFrame*     mainFrame;
	CChatUdp            udp;
	CChatSoundList      sounds;
	CChatSmileList      smiles;
	CChatStatusModeList statusModes;
	CChatUserList       users;
	CChatNetwork        network;

	std::string getUserName() const    { return userName; }
	void setUserName( const std::string& value );
	std::string getHostName() const    { return hostName; }
	std::string getIP() const          { return ip; }
	unsigned int getPort() const       { return port; }
	std::string getBroadcastIP() const { return broadcastIP; }

	CChatStatusModeType getStatusMode();
	void setStatusMode( const CChatStatusModeType value, const bool automatically = false );

	void refreshUserList();

	CFont& getFont();

protected:
	//{{AFX_VIRTUAL(CChatApp)
	protected:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
extern CChatApp chatApp;

//{{AFX_INSERT_LOCATION}}

#endif // CHATAPP_H
