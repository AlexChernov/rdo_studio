/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      application.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_APPLICATION_H_
#define _RDO_STUDIO_MFC_APPLICATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
#include <QtCore/qprocess.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "kernel/rdothread.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_method_manager.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
// --------------------------------------------------------------------------------

//#define PROCGUI_ENABLE

// --------------------------------------------------------------------------------
// -------------------- RDOStudioApp
// --------------------------------------------------------------------------------
class RDOStudioMainFrame;
class RDOThreadStudio;
class RDOThreadStudioGUI;
class RDOStudioPlugin;

namespace rdoTracer
{
class RDOTracer;
}

class RDOStudioApp: public CWinApp
{
friend class RDOStudioCommandLineInfo;
public:
	RDOStudioApp();
	virtual ~RDOStudioApp();

	PTR(RDOStudioMainFrame) m_pMainFrame;
	//! ��. �������� RDOKernelGUI
	//! ������� ����� ������ ����������, �.�. ������ ��� win32-gui, �� �� ������ �������
	PTR(RDOThread)          m_pStudioGUI;

	CREF(RPMethodManager) getMethodManager() const;
	REF(std::ofstream)    log             ();

	void           broadcastMessage(RDOThread::RDOTreadMessage message, PTR(void) pParam = NULL);
	void           insertReopenItem(CREF(tstring) item);

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
	void           autoCloseByPlugin(PTR(RDOStudioPlugin) plugin);
	rbool          isPluginAutoStart(PTR(RDOStudioPlugin) plugin) const;

	static tstring getFullExtName      ();
	tstring        getFullHelpFileName (tstring str = "RAO-help.qhc") const;
	static rbool   shortToLongPath     (CREF(tstring) shortPath, REF(tstring) longPath);
	tstring        chkHelpExist        (tstring fileName) const;
	void           chkAndRunQtAssistant();
	PTR(QProcess)  runQtAssistant      () const;
	void           callQtAssistant     (QByteArray ba);

private:
#ifdef RDO_MT
	// ������������ ��� �������� ����������������� ����������� �� ����������.
	// ��� ����, �� ���������� ��������� ������ ������ ����������, � ������� �����������
	// ������������ ����� ����������� ���������� ���������.
	PTR(RDOThreadStudio) m_pStudioMT;
#endif

	typedef  std::vector<tstring>  ReopenList;
	typedef  std::list<tstring>    PluginNameList;

	RPMethodManager                        m_methodManager;
	std::ofstream                          m_log;
	ReopenList                             m_reopenList;
	PTR(CMultiDocTemplate)                 m_editDocTemplate;
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
	PluginNameList                         m_pluginStartNameList;
	PluginNameList                         m_pluginExitNameList;
	QProcess*                              m_pAssistant;

	void setupFileAssociation();
	void updateReopenSubMenu () const;
	void loadReopen          ();
	void saveReopen          () const;

private:
	virtual BOOL Run                 ();
	virtual BOOL InitInstance        ();
	virtual int  ExitInstance        ();
	virtual BOOL PreTranslateMessage (PTR(MSG) pMsg);
	virtual BOOL OnIdle              (LONG lCount);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

	afx_msg void OnFileNew          ();
	afx_msg void OnWindowNew        ();
	afx_msg void OnFileOpen         ();
	afx_msg void OnFileSave         ();
	afx_msg void OnFileSaveAll      ();
	afx_msg void OnFileClose        ();
	afx_msg void OnFileSaveAs       ();
	afx_msg void OnModelBuild       ();
	afx_msg void OnModelRun         ();
	afx_msg void OnModelStop        ();
	afx_msg void OnAppAbout         ();
	afx_msg void OnProjectReopen    (UINT nID);
	afx_msg void OnUpdateFileSave   (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileSaveAll(PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileClose  (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileSaveAs (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateModelBuild (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateModelRun   (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateModelStop  (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileNew    (PTR(CCmdUI) pCmdUI);
	afx_msg void OnUpdateFileOpen   (PTR(CCmdUI) pCmdUI);
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------------
extern RDOStudioApp studioApp;

#endif // _RDO_STUDIO_MFC_APPLICATION_H_
