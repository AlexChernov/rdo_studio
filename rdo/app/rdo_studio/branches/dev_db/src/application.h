/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_APPLICATION_H_
#define _RDO_STUDIO_APPLICATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
#include <QtCore/qprocess.h>
#include <QtWidgets/qmainwindow.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/main_windows_base.h"
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
class RDOStudioMainFrame;
class RDOThreadStudio;
class RDOThreadStudioGUI;

namespace rdo { namespace gui { namespace tracer {
class Tracer;
}}}

class RDOStudioApp: public CWinApp
{
friend class RDOStudioCommandLineInfo;
public:
	RDOStudioApp();
	virtual ~RDOStudioApp();

	PTR(RDOStudioMainFrame) getMainWndUI();
	PTR(QMainWindow)        getMainWnd ();
	PTR(MainWindowBase)     getIMainWnd();
	PTR(MainWindowBase)     getStyle   ();

	//! ��. �������� RDOKernelGUI
	//! ������� ����� ������ ����������, �.�. ������ ��� win32-gui, �� �� ������ �������
	PTR(RDOThread)          m_pStudioGUI;

	REF(std::ofstream)      log();

	void           broadcastMessage(RDOThread::RDOTreadMessage message, PTR(void) pParam = NULL);

	rbool          getFileAssociationSetup() const;
	void           setFileAssociationSetup(rbool value);

	rbool          getFileAssociationCheckInFuture() const;
	void           setFileAssociationCheckInFuture(rbool value);

	rbool          getOpenLastProject() const;
	void           setOpenLastProject(rbool value);

	CREF(tstring)  getLastProjectName() const;
	void           setLastProjectName(CREF(tstring) projectName);

	rbool          getShowCaptionFullName() const;
	void           setShowCaptionFullName(rbool value);

	void           autoCloseByModel ();

	static tstring getFullExtName      ();
	tstring        getFullHelpFileName (tstring str = "RAO-help.qhc") const;
	static rbool   shortToLongPath     (CREF(tstring) shortPath, REF(tstring) longPath);
	tstring        chkHelpExist        (tstring fileName) const;
	void           chkAndRunQtAssistant();
	PTR(QProcess)  runQtAssistant      () const;
	void           callQtAssistant     (CREF(QByteArray) ba);

	CREF(rdoEditor::LPRDOEditorEditStyle) getEditorEditStyle() const;

private:
#ifdef RDO_MT
	// ������������ ��� �������� ����������������� ����������� �� ����������.
	// ��� ����, �� ���������� ��������� ������ ������ ����������, � ������� �����������
	// ������������ ����� ����������� ���������� ���������.
	PTR(RDOThreadStudio) m_pStudioMT;
#endif

	std::ofstream                          m_log;
	rbool                                  m_fileAssociationSetup;
	rbool                                  m_fileAssociationCheckInFuture;
	rbool                                  m_openLastProject;
	tstring                                m_lastProjectName;
	rbool                                  m_showCaptionFullName;
	rbool                                  m_autoRun;
	rbool                                  m_autoExitByModel;
	rbool                                  m_dontCloseIfError;
	rdo::simulation::report::RDOExitCode  m_exitCode;
	tstring                                m_openModelName;
	QProcess*                              m_pAssistant;
	PTR(RDOStudioMainFrame)                m_pMainFrame;
	rdoEditor::LPRDOEditorEditStyle        m_pEditorEditStyle;

	void setupFileAssociation();

private:
	virtual BOOL Run                 ();
	virtual BOOL InitInstance        ();
	virtual int  ExitInstance        ();
	virtual BOOL PreTranslateMessage (PTR(MSG) pMsg);
	virtual BOOL OnIdle              (LONG lCount);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
};

// --------------------------------------------------------------------------------
extern RDOStudioApp studioApp;

#endif // _RDO_STUDIO_APPLICATION_H_
