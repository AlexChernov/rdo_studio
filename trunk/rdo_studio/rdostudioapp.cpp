#include "stdafx.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudiomodel.h"
#include "rdostudioeditdoc.h"
#include "rdostudioeditview.h"
#include "resource.h"
#include "rdo_tracer/rdotracer.h"
#include "htmlhelp.h"

#include <rdokernel.h>
#include <rdorepository.h>		  

using namespace std;
using namespace rdoRepository;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOFileAssociationDlg
// ----------------------------------------------------------------------------
class RDOFileAssociationDlg: public CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual void OnCancel();

public:
	RDOFileAssociationDlg( CWnd* pParentWnd = NULL );
	virtual ~RDOFileAssociationDlg();

	int checkInFuture;
};

RDOFileAssociationDlg::RDOFileAssociationDlg( CWnd* pParentWnd ):
	CDialog( IDD_FILEASSOCIATION, pParentWnd ),
	checkInFuture( true )
{
}

RDOFileAssociationDlg::~RDOFileAssociationDlg()
{
}

void RDOFileAssociationDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );

	DDX_Check( pDX, IDC_FILEASSOCIATION_CHECK, checkInFuture );
}

void RDOFileAssociationDlg::OnCancel()
{
	CDialog::UpdateData( true );
	CDialog::OnCancel();
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioApp
// ----------------------------------------------------------------------------
RDOStudioApp studioApp;

BEGIN_MESSAGE_MAP(RDOStudioApp, CWinApp)
	//{{AFX_MSG_MAP(RDOStudioApp)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_MODEL_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	ON_COMMAND(ID_FILE_MODEL_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_MODEL_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_CLOSE, OnUpdateFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_MODEL_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_MODEL_BUILD, OnModelBuild)
	ON_COMMAND(ID_MODEL_RUN, OnModelRun)
	ON_COMMAND(ID_MODEL_STOP, OnModelStop)
	ON_UPDATE_COMMAND_UI(ID_MODEL_BUILD, OnUpdateModelBuild)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN, OnUpdateModelRun)
	ON_UPDATE_COMMAND_UI(ID_MODEL_STOP, OnUpdateModelStop)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE( ID_FILE_REOPEN_1, ID_FILE_REOPEN_10, OnProjectReopen )
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

RDOStudioApp::RDOStudioApp():
	CWinApp(),
	initInstance( false ),
	editDocTemplate( NULL ),
	fileAssociationSetup( false ),
	fileAssociationCheckInFuture( false ),
	openLastProject( false )
{
}

BOOL RDOStudioApp::InitInstance()
{
	CWinApp::InitInstance();
	
	if ( ::OleInitialize( NULL ) != S_OK )
		return FALSE;

#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	free( (void*)m_pszProfileName );
	m_pszProfileName = _tcsdup( _T("") );
	free( (void*)m_pszRegistryKey );
	m_pszRegistryKey = _tcsdup( _T("RAO-studio") );

	fileAssociationSetup         = GetProfileInt( "fileAssociation", "setup", false ) ? true : false;
	fileAssociationCheckInFuture = GetProfileInt( "fileAssociation", "checkInFuture", false ) ? true : false;
	openLastProject              = GetProfileInt( "general", "openLastProject", false ) ? true : false;

	editDocTemplate = new CMultiDocTemplate( IDR_EDIT_TYPE, RUNTIME_CLASS(RDOStudioEditDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioEditView) );
	AddDocTemplate( editDocTemplate );
	
	tracer = new rdoTracer::RDOTracer();

	AddDocTemplate( tracer->createDocTemplate() );

	mainFrame = new RDOStudioMainFrame;
	m_pMainWnd = mainFrame;
	if ( !mainFrame->LoadFrame( IDR_MAINFRAME ) ) return FALSE;

	loadReopen();
	updateReopenSubMenu();

	if ( getFileAssociationCheckInFuture() ) {
		setupFileAssociation();
	}

	mainFrame->ShowWindow(m_nCmdShow);
	mainFrame->UpdateWindow();

	string fileName( m_lpCmdLine );
	if ( !fileName.empty() ) {
		int pos = fileName.find_first_of( '"' );
		if ( pos == 0 ) {
			fileName.erase( 0, 1 );
		}
		pos = fileName.find_last_of( '"' );
		if ( pos == fileName.length() - 1 ) {
			fileName.erase( pos, 1 );
		}
		string longFileName;
		if ( shortToLongPath( fileName, longFileName ) ) {
			fileName = longFileName;
		}
		if ( model->openModel( fileName ) ) {
			insertReopenItem( kernel.getRepository()->getFullName() );
		} else {
			OnFileNew();
		}
	} else {
		OnFileNew();
	}

	initInstance = true;

	return TRUE;
}

bool RDOStudioApp::shortToLongPath( const std::string& shortPath, std::string& longPath )
{
	USES_CONVERSION;

	LPSHELLFOLDER psfDesktop = NULL;
	ULONG         chEaten = 0;
	LPITEMIDLIST  pidlShellItem = NULL;
	WCHAR         szLongPath[_MAX_PATH] = { 0 };
	BOOL          bResult = TRUE;

	// Get the Desktop's shell folder interface
	HRESULT hr = ::SHGetDesktopFolder( &psfDesktop );

	LPWSTR  lpwShortPath = A2W( shortPath.c_str() );

	// Request an ID list (relative to the desktop) for the short pathname 
	hr = psfDesktop->ParseDisplayName( NULL, NULL, lpwShortPath, &chEaten, &pidlShellItem, NULL );
	psfDesktop->Release(); // Release the desktop's IShellFolder

	if ( FAILED( hr ) ) {
		// If we couldn't get an ID list for short pathname, it must not exist.
		longPath.empty();
		return false;
	} else { 
		// We did get an ID list, convert it to a long pathname
		::SHGetPathFromIDListW( pidlShellItem, szLongPath );
		// Free the ID list allocated by ParseDisplayName
		LPMALLOC pMalloc = NULL;
		::SHGetMalloc( &pMalloc );
		pMalloc->Free( pidlShellItem );
		pMalloc->Release();
		longPath = W2A( szLongPath );
		return true;
	}
}

int RDOStudioApp::ExitInstance()
{
	HtmlHelp( NULL, NULL, HH_CLOSE_ALL, 0 );

	if( tracer ) delete tracer;

	return CWinApp::ExitInstance();
}

void RDOStudioApp::OnFileNew() 
{
	model->newModel( initInstance );
}

void RDOStudioApp::OnFileOpen() 
{
	if ( model->openModel() ) {
		insertReopenItem( kernel.getRepository()->getFullName() );
	}
}

void RDOStudioApp::OnFileClose() 
{
	model->closeModel();
}

void RDOStudioApp::OnFileSave() 
{
	model->saveModel();
}

void RDOStudioApp::OnFileSaveAs() 
{
	model->saveAsModel();
}

void RDOStudioApp::OnFileSaveAll() 
{
	model->saveModel();
}

void RDOStudioApp::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->getModelDoc() ? true : false );
}

void RDOStudioApp::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	model->updateModify();

	POSITION pos = editDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioEditDoc* doc = static_cast<RDOStudioEditDoc*>(editDocTemplate->GetNextDoc( pos ));
		if ( doc ) {
			doc->updateModify();
		}
	}
	bool flag = false;
	CMDIChildWnd* mdi = mainFrame->MDIGetActive();
	if ( mdi ) {
		CDocument* doc = mdi->GetActiveDocument();
		if ( doc ) {
			flag = doc->IsModified() ? true : false;
		}
	}
	pCmdUI->Enable( flag );
}

void RDOStudioApp::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model->getModelDoc() ? true : false );
}

void RDOStudioApp::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	bool flag = model->isModify();
	if ( !flag ) {
		POSITION pos = editDocTemplate->GetFirstDocPosition();
		while ( pos ) {
			RDOStudioEditDoc* doc = static_cast<RDOStudioEditDoc*>(editDocTemplate->GetNextDoc( pos ));
			if ( doc ) {
				doc->updateModify();
				flag = doc->IsModified() ? true : false;
				if ( flag ) break;
			}
		}
	}
	pCmdUI->Enable( flag );
}

void RDOStudioApp::OnProjectReopen( UINT nID )
{
	int i = 0;
	switch( nID ) {
		case ID_FILE_REOPEN_1 : i = 0; break;
		case ID_FILE_REOPEN_2 : i = 1; break;
		case ID_FILE_REOPEN_3 : i = 2; break;
		case ID_FILE_REOPEN_4 : i = 3; break;
		case ID_FILE_REOPEN_5 : i = 4; break;
		case ID_FILE_REOPEN_6 : i = 5; break;
		case ID_FILE_REOPEN_7 : i = 6; break;
		case ID_FILE_REOPEN_8 : i = 7; break;
		case ID_FILE_REOPEN_9 : i = 8; break;
		case ID_FILE_REOPEN_10: i = 9; break;
	}
	if ( model->openModel( reopenList[i] ) ) {
		insertReopenItem( kernel.getRepository()->getFullName() );
	} else if ( model->isPrevModelClosed() ) {
		string item = reopenList[i];
		for ( vector< string >::iterator it = reopenList.begin(); it != reopenList.end(); it++ ) {
			if ( *it == item ) {
				reopenList.erase( it );
				break;
			}
		}
		updateReopenSubMenu();
	}
}

void RDOStudioApp::insertReopenItem( const string& item )
{
	if ( !item.empty() ) {

		for ( vector< string >::iterator it = reopenList.begin(); it != reopenList.end(); it++ ) {
			if ( *it == item ) {
				reopenList.erase( it );
				break;
			}
		}

		reopenList.insert( reopenList.begin(), item );

		while ( reopenList.size() > 10 ) {
			vector< string >::iterator it = reopenList.end();
			it--;
			reopenList.erase( it );
		}

		updateReopenSubMenu();
	}
}

void RDOStudioApp::updateReopenSubMenu() const
{
	BOOL maximized;
	mainFrame->MDIGetActive( &maximized );
	int delta = maximized ? 1 : 0;

	CMenu* reopenMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu( delta )->GetSubMenu( 2 );

	while ( reopenMenu->GetMenuItemCount() ) {
		reopenMenu->DeleteMenu( 0, MF_BYPOSITION );
	}

	if ( !reopenList.empty() ) {
		AfxGetMainWnd()->GetMenu()->GetSubMenu( delta )->EnableMenuItem( 2, MF_BYPOSITION | MF_ENABLED );
		for ( vector< string >::size_type i = 0; i < reopenList.size(); i++ ) {
			if ( i == 4 ) reopenMenu->AppendMenu( MF_SEPARATOR );
			int id = ID_FILE_MRU_FILE1;
			switch( i ) {
				case 0: id = ID_FILE_REOPEN_1; break;
				case 1: id = ID_FILE_REOPEN_2; break;
				case 2: id = ID_FILE_REOPEN_3; break;
				case 3: id = ID_FILE_REOPEN_4; break;
				case 4: id = ID_FILE_REOPEN_5; break;
				case 5: id = ID_FILE_REOPEN_6; break;
				case 6: id = ID_FILE_REOPEN_7; break;
				case 7: id = ID_FILE_REOPEN_8; break;
				case 8: id = ID_FILE_REOPEN_9; break;
				case 9: id = ID_FILE_REOPEN_10; break;
			}
			reopenMenu->AppendMenu( MF_STRING, id, format( "%d. %s", i+1, reopenList[i].c_str() ).c_str() );
		}
	} else {
		AfxGetMainWnd()->GetMenu()->GetSubMenu( delta )->EnableMenuItem( 2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED );
	}

	saveReopen();
}

void RDOStudioApp::loadReopen()
{
	reopenList.clear();
	for ( int i = 0; i < 10; i++ ) {
		string sec;
		if ( i+1 < 10 ) {
			sec = format( "0%d", i+1 );
		} else {
			sec = format( "%d", i+1 );
		}
		TRY {
			string s = AfxGetApp()->GetProfileString( "reopen", sec.c_str(), "" );
			if ( !s.empty() ) {
				reopenList.insert( reopenList.end(), s );
			}
		} CATCH( CException, e ) {
		}
		END_CATCH
	}
}

void RDOStudioApp::saveReopen() const
{
	for ( vector< string >::size_type i = 0; i < 10; i++ ) {
		string sec;
		if ( i+1 < 10 ) {
			sec = format( "0%d", i+1 );
		} else {
			sec = format( "%d", i+1 );
		}
		string s;
		if ( i < reopenList.size() ) {
			s = reopenList[i];
		} else {
			s = "";
		}
		TRY {
			AfxGetApp()->WriteProfileString( "reopen", sec.c_str(), s.c_str() );
		} CATCH( CException, e ) {
		}
		END_CATCH
	}
}

void RDOStudioApp::OnModelBuild() 
{
	model->buildModel();
}

void RDOStudioApp::OnModelRun() 
{
	model->runModel();
}

void RDOStudioApp::OnModelStop() 
{
	model->stopModel();
}

void RDOStudioApp::OnUpdateModelBuild(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && model->getModelDoc() && !model->isRunning() );
}

void RDOStudioApp::OnUpdateModelRun(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && model->getModelDoc() && !model->isRunning() );
}

void RDOStudioApp::OnUpdateModelStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( model && model->getModelDoc() && model->isRunning() );
}

string RDOStudioApp::getFullFileName()
{
	string fileName = "";
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		fileName = szExeName;
	}
	return fileName;
}

string RDOStudioApp::extractFilePath( const string& fileName )
{
	string s;
	string::size_type pos = fileName.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = fileName.find_last_of( '/' );
	}
	if ( pos != string::npos && pos < fileName.length() - 1 ) {
		s.assign( fileName.begin(), pos + 1 );
		static char szDelims[] = " \t\n\r";
		s.erase( 0, s.find_first_not_of( szDelims ) );
		s.erase( s.find_last_not_of( szDelims ) + 1, string::npos );
	} else {
		s = fileName;
	}
	pos = s.find_last_of( '\\' );
	if ( pos == string::npos ) {
		pos = s.find_last_of( '/' );
	}
	if ( pos != s.length() - 1 && s.length() ) {
		s += "/";
	}
	return s;
}

bool RDOStudioApp::isFileExists( const string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

string RDOStudioApp::getFullHelpFileName( string str )
{
	str.insert( 0, extractFilePath( getFullFileName() ) );
	
	if ( !isFileExists( str ) ) {
		::MessageBox( NULL, format( ID_MSG_NO_HELP_FILE, str.c_str() ).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK );
		return "";
	}

	return str;
}

void RDOStudioApp::setFileAssociationSetup( const bool value )
{
	fileAssociationSetup = value;
	WriteProfileInt( "fileAssociation", "setup", fileAssociationSetup );
	if ( fileAssociationSetup ) {
		setupFileAssociation();
	}
}

void RDOStudioApp::setFileAssociationCheckInFuture( const bool value )
{
	if ( fileAssociationCheckInFuture != value ) {
		fileAssociationCheckInFuture = value;
		WriteProfileInt( "fileAssociation", "checkInFuture", fileAssociationCheckInFuture );
	}
}

void RDOStudioApp::setOpenLastProject( const bool value )
{
	if ( openLastProject != value ) {
		openLastProject = value;
		WriteProfileInt( "general", "openLastProject", openLastProject );
	}
}

void RDOStudioApp::setupFileAssociation()
{
	string strFileTypeId   = _T("RAO.FileInfo");
	string strFileTypeName = _T("RAO FileInfo");
	string strParam        = _T(" \"%1\"");
	string strPathName     = getFullFileName();
	string strRAOExt       = _T(".smr");

	bool win2000 = false;
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if ( ::GetVersionEx( &osvi ) ) {
		win2000 = osvi.dwMajorVersion >= 5;
	}

	HKEY hCurUserSoftClasses = 0;
	HKEY hKey;
	DWORD result;
	if ( win2000 ) {
		if ( ::RegOpenKeyEx( HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hCurUserSoftClasses ) != ERROR_SUCCESS ) {
			hCurUserSoftClasses = 0;
		}
	} else {
		hCurUserSoftClasses = HKEY_CLASSES_ROOT;
	}
	if ( hCurUserSoftClasses ) {
		if ( ::RegCreateKeyEx( hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &result ) == ERROR_SUCCESS ) {
			bool mustBeRegistered = true;
			DWORD size;
			if ( ::RegQueryValueEx( hKey, _T(""), NULL, NULL, NULL, &size ) == ERROR_SUCCESS ) {
				if ( size > 0 ) {
					HKEY hKeyPath;
					if ( ::RegOpenKeyEx( hKey, _T("shell\\open\\command"), 0, KEY_READ, &hKeyPath ) == ERROR_SUCCESS ) {
						CString s;
						DWORD length = MAX_PATH + strParam.length() * sizeof(TCHAR) + 1;
						::RegQueryValueEx( hKeyPath, _T(""), NULL, NULL, (LPBYTE)s.GetBuffer( length ), &length );
						s.ReleaseBuffer();
						::RegCloseKey( hKeyPath );

						int pos = s.Find( strParam.c_str() );
						if ( pos != -1 ) {
							s.Delete( pos, strParam.length() );
							if ( s != strPathName.c_str() ) {
								if ( !getFileAssociationSetup() ) {
									RDOFileAssociationDlg dlg;
									mustBeRegistered = dlg.DoModal() == IDOK;
									setFileAssociationCheckInFuture( dlg.checkInFuture ? true : false );
								}
							} else {
								mustBeRegistered = false;
							}
						}
					}
				}
			}
			if ( mustBeRegistered ) {
				HKEY hKey_tmp;
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strFileTypeName;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, string(strFileTypeId + _T("\\DefaultIcon")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strPathName + _T(",0");
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, string(strFileTypeId + _T("\\shell\\open\\command")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strPathName + strParam;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strRAOExt.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strFileTypeId;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, strRAOExt.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = strFileTypeId;
					::RegSetValueEx( hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
				if ( ::RegCreateKeyEx( hCurUserSoftClasses, string(strRAOExt + _T("\\ShellNew")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result ) == ERROR_SUCCESS ) {
					string s = "";
					::RegSetValueEx( hKey_tmp, _T("NullFile"), 0, REG_SZ, (LPBYTE)s.c_str(), s.length() );
					::RegCloseKey( hKey_tmp );
				}
			}
			::RegCloseKey( hKey );
		}
		if ( hCurUserSoftClasses != HKEY_CLASSES_ROOT ) {
			::RegCloseKey( hCurUserSoftClasses );
		}
	}
}

void RDOStudioApp::OnAppAbout()
{
	RDOAboutDlg dlg;
	dlg.DoModal();
}

// ----------------------------------------------------------------------------
// ---------- RDOAboutDlg
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOAboutDlg, CDialog )
	//{{AFX_MSG_MAP(RDOAboutDlg)
	ON_BN_CLICKED(IDC_ABOUT_EMAIL, OnAboutEmail)
	ON_BN_CLICKED(IDC_ABOUT_WEB, OnAboutWeb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOAboutDlg::RDOAboutDlg():
	CDialog( IDD )
{
	//{{AFX_DATA_INIT(RDOAboutDlg)
	m_caption = _T("");
	//}}AFX_DATA_INIT
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		DWORD dwHnd;
		DWORD size = ::GetFileVersionInfoSize( szExeName, &dwHnd );
		if ( size ) {
			void* pBuffer = malloc( size );
			if ( pBuffer != NULL ) {
				if ( ::GetFileVersionInfo( szExeName, dwHnd, size, pBuffer ) ) {
					DWORD* pTranslation;
					UINT   length;
					if ( ::VerQueryValue( pBuffer, _T("\\VarFileInfo\\Translation"), (void**)&pTranslation, &length ) ) {
						DWORD translation = *pTranslation;
						char key[2000];
						wsprintf( key, _T("\\StringFileInfo\\%04x%04x\\ProductName"), LOWORD( translation ), HIWORD( translation ) );
						char* productName;
						if ( ::VerQueryValue( pBuffer, key, (void**)&productName, &length ) ) {
							VS_FIXEDFILEINFO* fixedInfo;
							if ( ::VerQueryValue( pBuffer, _T("\\"), (void**)&fixedInfo, &length ) ) {
								CString s;
								s.Format( "%s     version %u.%u (build %u)", productName, HIWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionLS ) );
								m_caption = s;
							}
						}
					}
				}
				free( pBuffer );
			}
		}
	}
}

RDOAboutDlg::~RDOAboutDlg()
{
}

void RDOAboutDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RDOAboutDlg)
	DDX_Control(pDX, IDC_ABOUT_WEB, m_web);
	DDX_Control(pDX, IDC_ABOUT_EMAIL, m_email);
	DDX_Text(pDX, IDC_ABOUT_CAPTION, m_caption);
	//}}AFX_DATA_MAP
}

void RDOAboutDlg::OnAboutEmail() 
{
	CString s;
	m_email.GetWindowText( s );
	::ShellExecute( m_hWnd, "open", "mailto:" + s, 0, 0, SW_SHOWNORMAL );
}

void RDOAboutDlg::OnAboutWeb() 
{
	CString s;
	m_web.GetWindowText( s );
	::ShellExecute( m_hWnd, "open", s, 0, 0, SW_SHOWNORMAL );
}
