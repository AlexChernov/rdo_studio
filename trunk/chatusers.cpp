#include "stdafx.h"
#include "chatusers.h"
#include "chatapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatUser
// ----------------------------------------------------------------------------
CChatUser::CChatUser():
	CObject(),
	userName( "" ),
	hostName( "" ),
	ip( "" ),
	statusMode( CSMT_Online )
{
}

CChatUser::CChatUser( const std::string& _username, const std::string& _hostname, const std::string& _ip, const CChatStatusModeType _statusMode ):
	CObject(),
	userName( _username ),
	hostName( _hostname ),
	ip( _ip ),
	statusMode( _statusMode )
{
}

CChatUser::~CChatUser()
{
}

std::string CChatUser::getUserName() const
{
	return userName;
}

void CChatUser::setUserName( const std::string& value )
{
	if ( userName != value ) {
		userName = value;
		if ( chatApp.pFrame ) {
			chatApp.pFrame->dock.tree.updateUserName( this );
		}
	}
}

std::string CChatUser::getHostName() const
{
	return hostName;
}

void CChatUser::setHostName( const std::string& value )
{
	hostName = value;
}

std::string CChatUser::getIP() const
{
	return ip;
}

void CChatUser::setIP( const std::string& value )
{
	ip = value;
}

CChatStatusModeType CChatUser::getStatusMode() const
{
	return statusMode;
}

void CChatUser::setStatusMode( const CChatStatusModeType value )
{
	if ( statusMode != value ) {
		statusMode = value;
		if ( chatApp.pFrame ) {
			chatApp.pFrame->dock.tree.updateUserStatus( this );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- CChatUsers
// ----------------------------------------------------------------------------
CChatUsers::CChatUsers(): CPtrArray()
{
}

CChatUsers::~CChatUsers()
{
	clear();
}

void CChatUsers::addUser( const std::string& username, const std::string& hostname, const std::string& ip, const CChatStatusModeType statusMode )
{
	if ( findUserByIP( ip ) == -1 ) {
		CChatUser* user = new CChatUser( username, hostname, ip, statusMode );
		Add( user );
		((CChatMainFrame*)AfxGetMainWnd())->dock.tree.addUser( user );
	}
}

void CChatUsers::deleteUser( const int listIndex )
{
	if ( listIndex != -1 && listIndex < GetSize() ) {
		CChatUser* user = (CChatUser*)GetAt( listIndex );
		if ( chatApp.pFrame ) {
			chatApp.pFrame->dock.tree.deleteUser( user );
		}
		delete user;
		RemoveAt( listIndex );
	}
}

void CChatUsers::deleteUser( const CChatUser* const user )
{
	int cnt = GetSize();
	for ( int i = 0; i < cnt; i++ ) {
		CChatUser* usr = (CChatUser*)GetAt(i);
		if ( usr == user ) {
			deleteUser( i );
			break;
		}
	}
}

int CChatUsers::findUserByHostName( const std::string& hostname )
{
	int cnt = GetSize();
	for ( int i = 0; i < cnt; i++ ) {
		CChatUser* user = (CChatUser*)GetAt(i);
		if ( user->getHostName() == hostname ) return i;
	}
	return -1;
}

int CChatUsers::findUserByIP( const std::string& ip )
{
	int cnt = GetSize();
	for ( int i = 0; i < cnt; i++ ) {
		CChatUser* user = (CChatUser*)GetAt(i);
		if ( user->getIP() == ip ) return i;
	}
	return -1;
}

CChatUser* CChatUsers::getUserByHostName( const std::string& hostname )
{
	int listIndex = findUserByHostName( hostname );
	if ( listIndex != -1 ) {
		return (CChatUser*)GetAt( listIndex );
	}
	return NULL;
}

CChatUser* CChatUsers::getUserByIP( const std::string& ip )
{
	int listIndex = findUserByIP( ip );
	if ( listIndex != -1 ) {
		return (CChatUser*)GetAt( listIndex );
	}
	return NULL;
}

void CChatUsers::clear( const CChatUser* const dont_delete_user )
{
	int cnt = GetSize();
	for ( int i = 0; i < GetSize(); i++ ) {
		CChatUser* usr = (CChatUser*)GetAt(i);
		if ( usr != dont_delete_user ) {
			deleteUser( i );
			i--;
		}
	}
}
