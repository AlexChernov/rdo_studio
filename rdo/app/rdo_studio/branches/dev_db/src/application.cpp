/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qprocess.h>
#include <QtCore/qtextcodec.h>
#include <QtWidgets/qmessagebox.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/thread.h"
#include "app/rdo_studio/rdo_tracer/rdotracer.h"
#include "thirdparty/win_registry/registry.h"
#include "thirdparty/qt-solutions/qtwinmigrate/src/qmfcapp.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOFileAssociationDlg
// --------------------------------------------------------------------------------
class RDOFileAssociationDlg: public CDialog
{
protected:
	virtual void DoDataExchange(PTR(CDataExchange) pDX);
	virtual void OnCancel      ();

public:
	RDOFileAssociationDlg(PTR(CWnd) pParentWnd = NULL);
	virtual ~RDOFileAssociationDlg();

	int m_checkInFuture;
};

RDOFileAssociationDlg::RDOFileAssociationDlg(PTR(CWnd) pParentWnd)
	: CDialog(IDD_FILEASSOCIATION, pParentWnd)
	, m_checkInFuture(true)
{}

RDOFileAssociationDlg::~RDOFileAssociationDlg()
{}

void RDOFileAssociationDlg::DoDataExchange(PTR(CDataExchange) pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_FILEASSOCIATION_CHECK, m_checkInFuture);
}

void RDOFileAssociationDlg::OnCancel()
{
	CDialog::UpdateData(true);
	CDialog::OnCancel  ();
}

// -----------------------------------------------------------------
// -------------------- RDOStudioCommandLineInfo
// -----------------------------------------------------------------
class RDOStudioCommandLineInfo: public CCommandLineInfo
{
public:
	RDOStudioCommandLineInfo()
		: CCommandLineInfo()
	{}

protected:
	virtual void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);
};

void RDOStudioCommandLineInfo::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
	if (bFlag)
	{
		if (CString(lpszParam).CompareNoCase(_T("autorun")) == 0)
		{
			studioApp.m_autoRun = true;
		}
		else if (CString(lpszParam).CompareNoCase(_T("autoexit")) == 0)
		{
			studioApp.m_autoExitByModel = true;
		}
		else if (CString(lpszParam).CompareNoCase(_T("dont_close_if_error")) == 0)
		{
			studioApp.m_dontCloseIfError = true;
		}
	}
	else
	{
		if (studioApp.m_openModelName.empty())
		{
			studioApp.m_openModelName = lpszParam;
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
RDOStudioApp studioApp;

#ifdef _DEBUG
void g_messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	if (msg.contains("requested for null window or window without handle"))
		return;

	if (msg.contains("QBackingStore::flush() called with non-exposed window, behavior is undefined"))
		return;

	if (msg.contains("Cannot create accessible interface for object"))
		return;

	qInstallMessageHandler(NULL);

	QString message = QString("%1\n\nfile: %2: %3\nat function: %4")
		.arg(msg)
		.arg(context.file)
		.arg(context.line)
		.arg(context.function);

	switch (type)
	{
	case QtDebugMsg:
		TRACE1("Debug: %s\n", msg.toLocal8Bit().constData());
		break;

	case QtWarningMsg:
		TRACE1("Warning: %s\n", message.toLocal8Bit().constData());
		QMessageBox::warning(studioApp.getMainWnd(), "QtWarning", message);
		break;

	case QtCriticalMsg:
		QMessageBox::critical(studioApp.getMainWnd(), "QtCritical", message);
		break;

	case QtFatalMsg:
		QMessageBox::critical(studioApp.getMainWnd(), "QtFatal", message);
		break;
	}

	qInstallMessageHandler(g_messageOutput);
}
#endif

RDOStudioApp::RDOStudioApp()
	: CWinApp()
	, m_pStudioGUI                  (NULL  )
#ifdef RDO_MT
	, m_pStudioMT                   (NULL  )
#endif
	, m_fileAssociationSetup        (false )
	, m_fileAssociationCheckInFuture(false )
	, m_openLastProject             (false )
	, m_lastProjectName             (_T(""))
	, m_showCaptionFullName         (false )
	, m_autoRun                     (false )
	, m_autoExitByModel             (false )
	, m_dontCloseIfError            (false )
	, m_exitCode                    (rdo::simulation::report::EC_OK)
	, m_openModelName               (_T(""))
	, m_pMainFrame                  (NULL  )
{
#ifdef _DEBUG
	qInstallMessageHandler(g_messageOutput);
#endif

	setlocale(LC_ALL,     _T("rus"));
	setlocale(LC_NUMERIC, _T("eng"));

	m_log.open(_T("log.txt"));
}

RDOStudioApp::~RDOStudioApp()
{}

BOOL RDOStudioApp::Run()
{
	int result = QMfcApp::run(this);
	delete qApp;
	return result;
}

BOOL RDOStudioApp::InitInstance()
{
	CWinApp::InitInstance();

	if (::OleInitialize(NULL) != S_OK)
		return FALSE;

	free((PTR(void))m_pszProfileName);
	m_pszProfileName = _tcsdup(_T(""));
	free((PTR(void))m_pszRegistryKey);
	m_pszRegistryKey = _tcsdup(_T("RAO-studio"));

	m_fileAssociationSetup         = GetProfileInt   (_T("fileAssociation"), _T("setup"),                true) ? true : false;
	m_fileAssociationCheckInFuture = GetProfileInt   (_T("fileAssociation"), _T("checkInFuture"),        true) ? true : false;
	m_openLastProject              = GetProfileInt   (_T("general"),         _T("openLastProject"),      true) ? true : false;
	m_lastProjectName              = GetProfileString(_T("general"),         _T("lastProject"),        _T(""));
	m_showCaptionFullName          = GetProfileInt   (_T("general"),         _T("showCaptionFullName"), false) ? true : false;

	// ���-�� ������ ������� ������ � �����
	RDOKernel::init();
#ifdef RDO_MT
	m_pStudioGUI = new RDOThreadStudioGUI();
#else
	m_pStudioGUI = kernel;
#endif
	new rdo::service::simulation::RDOThreadSimulator();
	new rdo::service::simulation::RDOThreadCodeComp();
	new rdo::repository::RDOThreadRepository();

//#define CORBA_ENABLE
#ifdef CORBA_ENABLE
	new rdoCorba::RDOThreadCorba();
#endif

#ifdef RDO_MT
	m_pStudioMT = new RDOThreadStudio();
#endif
//	new RDOThreadStudio1();
//	new RDOThreadStudio2();

	g_pTracer = new rdo::gui::tracer::Tracer();

	QMfcApp::instance(this);
	qApp->setQuitOnLastWindowClosed(true);

	m_pEditorEditStyle = rdo::Factory<rdoEditor::RDOEditorEditStyle>::create();

	// ������ ��������� ������ ������
	m_pMainFrame = new RDOStudioMainFrame();
	m_pMainFrame->init();
	m_pMainWnd = m_pMainFrame->c_wnd();
	m_pMainFrame->show();

#ifdef RDO_MT
	kernel->thread_studio = m_pStudioGUI;
#else
	kernel->thread_studio = model;
#endif

	if (getFileAssociationCheckInFuture())
	{
		setupFileAssociation();
	}

	RDOStudioCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	rbool newModel  = true;
	rbool autoModel = false;
	if (!m_openModelName.empty())
	{
		if (!rdo::extractFilePath(m_openModelName).empty())
		{
			tstring longFileName;
			if (shortToLongPath(m_openModelName, longFileName))
			{
				m_openModelName = longFileName;
			}
			if (model->openModel(m_openModelName))
			{
				autoModel = true;
			}
		}
		else
		{
			m_openModelName = rdo::extractFilePath(RDOStudioApp::getFullExtName()) + m_openModelName;
			if (rdo::File::exist(m_openModelName) && model->openModel(m_openModelName))
			{
				m_autoRun          = true;
				m_autoExitByModel  = true;
				m_dontCloseIfError = true;
				autoModel          = true;
			}
			else
			{
				m_exitCode = rdo::simulation::report::EC_ModelNotFound;
				return false;
			}
		}
	}
	else
	{
		if (getOpenLastProject() && !getLastProjectName().empty() && rdo::File::exist(getLastProjectName()))
		{
			model->openModel(getLastProjectName());
			newModel = false;
		}
	}

	if (autoModel)
	{
		newModel = false;
	}
	else
	{
		m_autoRun          = false;
		m_autoExitByModel  = false;
		m_dontCloseIfError = false;
	}

	if (m_autoRun)
	{
		model->runModel();
	}

	return TRUE;
}

int RDOStudioApp::ExitInstance()
{
	m_pMainFrame = NULL;

	if (m_exitCode != rdo::simulation::report::EC_ModelNotFound)
	{
		m_exitCode = model->getExitCode();
	}
#ifdef RDO_MT
	if (m_pStudioGUI)
	{
		m_pStudioGUI->sendMessage(m_pStudioGUI, RDOThread::RT_THREAD_CLOSE);
		delete static_cast<PTR(RDOThreadStudioGUI)>(m_pStudioGUI);
		m_pStudioGUI = NULL;
	}
#endif

	// ������ ������ � ��������� ��� �����
	RDOKernel::close();

	if (m_autoExitByModel)
	{
		CWinApp::ExitInstance();
		return m_exitCode;
	}
	else
	{
		return CWinApp::ExitInstance();
	}
}

PTR(RDOStudioMainFrame) RDOStudioApp::getMainWndUI()
{
	return m_pMainFrame;
}

PTR(QMainWindow) RDOStudioApp::getMainWnd()
{
	return m_pMainFrame;
}

PTR(MainWindowBase) RDOStudioApp::getStyle()
{
	return m_pMainFrame;
}

PTR(MainWindowBase) RDOStudioApp::getIMainWnd()
{
	return m_pMainFrame;
}

REF(std::ofstream) RDOStudioApp::log()
{
	return m_log;
}

rbool RDOStudioApp::shortToLongPath(CREF(tstring) shortPath, REF(tstring) longPath)
{
	USES_CONVERSION;

	LPSHELLFOLDER psfDesktop    = NULL;
	ULONG         chEaten       = 0;
	LPITEMIDLIST  pidlShellItem = NULL;
	WCHAR         szLongPath[_MAX_PATH] = { 0 };

	// Get the Desktop's shell folder interface
	HRESULT hr = ::SHGetDesktopFolder(&psfDesktop);

	LPWSTR  lpwShortPath = A2W(shortPath.c_str());

	// Request an ID list (relative to the desktop) for the short pathname 
	hr = psfDesktop->ParseDisplayName(NULL, NULL, lpwShortPath, &chEaten, &pidlShellItem, NULL);
	psfDesktop->Release(); // Release the desktop's IShellFolder

	if (FAILED(hr))
	{
		// If we couldn't get an ID list for short pathname, it must not exist.
		longPath.empty();
		return false;
	}
	else
	{
		// We did get an ID list, convert it to a long pathname
		::SHGetPathFromIDListW(pidlShellItem, szLongPath);
		// Free the ID list allocated by ParseDisplayName
		LPMALLOC pMalloc = NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidlShellItem);
		pMalloc->Release();
		longPath = W2A(szLongPath);
		return true;
	}
}

tstring RDOStudioApp::getFullExtName()
{
	tstring fileName = _T("");
	TCHAR szExeName[ MAX_PATH + 1 ];
	if (::GetModuleFileName(NULL, szExeName, MAX_PATH))
	{
		fileName = szExeName;
	}
	return fileName;
}

tstring RDOStudioApp::getFullHelpFileName(tstring str) const
{
	tstring strTemp = chkHelpExist(str);
	if (                       strTemp.size() < 3) return _T("");
	if (chkHelpExist ("assistant.exe").size() < 3) return _T("");

	return strTemp;
}

tstring RDOStudioApp::chkHelpExist(tstring fileName) const
{
	fileName.insert(0, rdo::extractFilePath(RDOStudioApp::getFullExtName()));
	if (!rdo::File::exist(fileName))
	{
		::MessageBox(NULL, rdo::format(ID_MSG_NO_HELP_FILE, fileName.c_str()).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK);
		return _T("");
	}
	return fileName;
}

void RDOStudioApp::chkAndRunQtAssistant()
{
	if (!m_pAssistant)
	{
		m_pAssistant = runQtAssistant();
	}
	else if (m_pAssistant->state() == m_pAssistant->Running)
		return;
	else
		m_pAssistant = runQtAssistant();
}

PTR(QProcess) RDOStudioApp::runQtAssistant() const
{
	PTR(QProcess) pProcess = new QProcess;
	QStringList args;
	args << QString("-collectionFile")
		<< QString(getFullHelpFileName().c_str())
		<< QString("-enableRemoteControl")
		<< QString("-quiet");
	pProcess->start(QString("assistant"), args);
	return pProcess;
}

void RDOStudioApp::callQtAssistant(CREF(QByteArray) ba)
{
	chkAndRunQtAssistant();
	if (m_pAssistant->state() != m_pAssistant->Running)
		return;

	m_pAssistant->write(ba);
}

rbool RDOStudioApp::getFileAssociationSetup() const
{
	return m_fileAssociationSetup;
}

void RDOStudioApp::setFileAssociationSetup(rbool value)
{
	m_fileAssociationSetup = value;
	WriteProfileInt(_T("fileAssociation"), _T("setup"), m_fileAssociationSetup);
	if (m_fileAssociationSetup)
	{
		setupFileAssociation();
	}
}

rbool RDOStudioApp::getFileAssociationCheckInFuture() const
{
	return m_fileAssociationCheckInFuture;
}

void RDOStudioApp::setFileAssociationCheckInFuture(rbool value)
{
	if (m_fileAssociationCheckInFuture != value)
	{
		m_fileAssociationCheckInFuture = value;
		WriteProfileInt(_T("fileAssociation"), _T("checkInFuture"), m_fileAssociationCheckInFuture);
	}
}

rbool RDOStudioApp::getOpenLastProject() const
{
	return m_openLastProject;
}

void RDOStudioApp::setOpenLastProject(rbool value)
{
	if (m_openLastProject != value)
	{
		m_openLastProject = value;
		WriteProfileInt(_T("general"), _T("openLastProject"), m_openLastProject);
	}
}

CREF(tstring) RDOStudioApp::getLastProjectName() const
{
	return m_lastProjectName;
}

void RDOStudioApp::setLastProjectName(CREF(tstring) projectName)
{
	m_pMainFrame->insertMenuFileReopenItem(projectName);
	if (m_lastProjectName != projectName)
	{
		m_lastProjectName = projectName;
		WriteProfileString(_T("general"), _T("lastProject"), getOpenLastProject() ? m_lastProjectName.c_str() : _T(""));
	}
}

rbool RDOStudioApp::getShowCaptionFullName() const
{
	return m_showCaptionFullName;
}

void RDOStudioApp::setShowCaptionFullName(rbool value)
{
	if (m_showCaptionFullName != value)
	{
		m_showCaptionFullName = value;
		model->setName(model->getName());
		WriteProfileInt(_T("general"), _T("showCaptionFullName"), m_showCaptionFullName);
	}
}

void RDOStudioApp::setupFileAssociation()
{
	tstring strFileTypeId    = _T("RAO.Project");
	tstring strFileTypeName  = _T("RAO Project");
	tstring strParam         = _T(" \"%1\"");
	tstring strPathName      = RDOStudioApp::getFullExtName();
	tstring strRAOExt        = _T(".rdox");

	tstring strFileTypeIdOld = _T("RAO.FileInfo");
	tstring strRAOExtOld     = _T(".smr");

	rbool win2000 = false;
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (::GetVersionEx(&osvi))
	{
		win2000 = osvi.dwMajorVersion >= 5;
	}

	HKEY hCurUserSoftClasses = 0;
	HKEY hKey;
	DWORD result;
	if (win2000)
	{
		if (::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hCurUserSoftClasses) != ERROR_SUCCESS)
		{
			hCurUserSoftClasses = 0;
		}
	}
	else
	{
		hCurUserSoftClasses = HKEY_CLASSES_ROOT;
	}
	if (hCurUserSoftClasses)
	{
		if (::RegCreateKeyEx(hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &result) == ERROR_SUCCESS)
		{
			rbool mustBeRegistered = true;
			DWORD size;
			if (::RegQueryValueEx(hKey, _T(""), NULL, NULL, NULL, &size) == ERROR_SUCCESS)
			{
				if (size > 0)
				{
					HKEY hKeyPath;
					if (::RegOpenKeyEx(hKey, _T("shell\\open\\command"), 0, KEY_READ, &hKeyPath) == ERROR_SUCCESS)
					{
						CString s;
						DWORD length = MAX_PATH + strParam.length() * sizeof(TCHAR) + 1;
						::RegQueryValueEx(hKeyPath, _T(""), NULL, NULL, (LPBYTE)s.GetBuffer(length), &length);
						s.ReleaseBuffer();
						::RegCloseKey(hKeyPath);

						int pos = s.Find(strParam.c_str());
						if (pos != -1)
						{
							s.Delete(pos, strParam.length());
							if (s != strPathName.c_str())
							{
								if (!getFileAssociationSetup())
								{
									RDOFileAssociationDlg dlg;
									mustBeRegistered = dlg.DoModal() == IDOK;
									setFileAssociationCheckInFuture(dlg.m_checkInFuture ? true : false);
								}
							}
							else
							{
								mustBeRegistered = false;
							}
						}
					}
				}
			}
			if (mustBeRegistered)
			{
				DeleteRegistryKey(hCurUserSoftClasses, strFileTypeIdOld.c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strFileTypeIdOld + _T("\\DefaultIcon")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strFileTypeIdOld + _T("\\shell\\open\\command")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, tstring(strRAOExtOld + _T("\\ShellNew")).c_str());
				DeleteRegistryKey(hCurUserSoftClasses, strRAOExtOld.c_str());

				HKEY hKey_tmp;
				if (::RegCreateKeyEx(hCurUserSoftClasses, strFileTypeId.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strFileTypeName;
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, tstring(strFileTypeId + _T("\\DefaultIcon")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strPathName + _T(",0");
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, tstring(strFileTypeId + _T("\\shell\\open\\command")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strPathName + strParam;
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, strRAOExt.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = strFileTypeId;
					::RegSetValueEx(hKey_tmp, _T(""), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
				if (::RegCreateKeyEx(hCurUserSoftClasses, tstring(strRAOExt + _T("\\ShellNew")).c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey_tmp, &result) == ERROR_SUCCESS)
				{
					tstring s = _T("");
					::RegSetValueEx(hKey_tmp, _T("NullFile"), 0, REG_SZ, (LPBYTE)s.c_str(), s.length());
					::RegCloseKey(hKey_tmp);
				}
			}
			::RegCloseKey(hKey);
		}
		if (hCurUserSoftClasses != HKEY_CLASSES_ROOT)
		{
			::RegCloseKey(hCurUserSoftClasses);
		}
	}
}

void RDOStudioApp::autoCloseByModel()
{
	if (m_autoExitByModel)
	{
		if (!m_dontCloseIfError || !model || (m_dontCloseIfError && (model->getExitCode() == rdo::simulation::report::EC_OK || model->getExitCode() == rdo::simulation::report::EC_NoMoreEvents)))
		{
			m_pMainFrame->close();
		}
	}
}

BOOL RDOStudioApp::PreTranslateMessage(PTR(MSG) pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//! @todo qt
		//PTR(CMDIChildWnd) pChild = m_pMainFrame->MDIGetActive();
		//if (pChild)
		//{
		//	PTR(CView) pView = pChild->GetActiveView();
		//	if (dynamic_cast<PTR(RDOStudioFrameView)>(pView) && pView == pChild->GetFocus())
		//	{
		//		pView->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		//		return true;
		//	}
		//}
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL RDOStudioApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

void RDOStudioApp::broadcastMessage(RDOThread::RDOTreadMessage message, PTR(void) pParam)
{
#ifdef RDO_MT
	PTR(CEvent) pEvent = m_pStudioMT->manualMessageFrom(message, pParam);
	while (::WaitForSingleObject(pEvent->m_hObject, 0) == WAIT_TIMEOUT) {
		static_cast<PTR(RDOThreadStudioGUI)>(m_pStudioGUI)->processMessages();
		if (m_pMainFrame) {
			m_pMainFrame->UpdateWindow();
		} else {
			break;
		}
	}
	delete pEvent;
#else
	m_pStudioGUI->broadcastMessage(message, pParam);
#endif
}

BOOL RDOStudioApp::OnIdle(LONG lCount)
{
#ifdef RDO_MT
	static_cast<PTR(RDOThreadStudioGUI)>(m_pStudioGUI)->processMessages();
	CWinApp::OnIdle(lCount);
	return true;
#else
	kernel->idle();
	if (lCount > 10000)
	{
		return CWinApp::OnIdle(lCount);
	}
	else
	{
		CWinApp::OnIdle(lCount);
		return true;
	}
#endif
}

CREF(rdoEditor::LPRDOEditorEditStyle) RDOStudioApp::getEditorEditStyle() const
{
	ASSERT(m_pEditorEditStyle);
	return m_pEditorEditStyle;
}
