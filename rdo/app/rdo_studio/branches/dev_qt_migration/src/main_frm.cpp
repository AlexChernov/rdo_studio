/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      main_frm.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#include <QtCore/qprocess.h>
#include <QtGui/qmdisubwindow.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/help_context_i.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/options.h"
#include "app/rdo_studio_mfc/src/about.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/htmlhelp.h"
#include "app/rdo_studio_mfc/resource.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOToolBar
// --------------------------------------------------------------------------------
void RDOToolBar::init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID )
{
	CreateEx( parent, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOATING | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	LoadToolBar( tbResID );
	ModifyStyle( 0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT );
	SetWindowText( rdo::format( tbResID ).c_str() );

	disabledImage.Create( tbDisabledImageResID, 16, 0, 0xFF00FF );
	GetToolBarCtrl().SetDisabledImageList( &disabledImage );
}

// --------------------------------------------------------------------------------
// -------------------- RDOToolBarModel
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOToolBarModel, RDOToolBar)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void RDOToolBarModel::init( CWnd* parent, unsigned int tbResID, unsigned int tbDisabledImageResID )
{
	RDOToolBar::init( parent, tbResID, tbDisabledImageResID );
	SetButtonInfo( CommandToIndex(ID_MODEL_SPEED_SLIDER), ID_MODEL_SPEED_SLIDER, TBBS_SEPARATOR, 100 );
	CRect rc;
	GetItemRect( CommandToIndex(ID_MODEL_SPEED_SLIDER), rc );
	rc.top    += 1;
	rc.bottom -= 1;

#define VISTA_TRANSPARENT_FLAG 0x1000
	slider.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_HORZ | VISTA_TRANSPARENT_FLAG, rc, this, ID_MODEL_SPEED_SLIDER );
#undef VISTA_TRANSPARENT_FLAG

	slider.SetRange( 0, 100 );
	for ( int i = 1; i <= 101; i += 10 ) {
		slider.SetTic( (int)(log( (double)i ) / log101 * 100) );
	}
	slider.SetPos( 100 );
}

void RDOToolBarModel::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	UNUSED(nPos      );
	UNUSED(pScrollBar);

	if ( nSBCode == SB_THUMBTRACK ) {
		model->setSpeed( getSpeed() );
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioMainFrame
// --------------------------------------------------------------------------------
//! @todo qt
//BEGIN_MESSAGE_MAP(RDOStudioMainFrame, CMDIFrameWnd)
//	ON_WM_CREATE()
//	ON_COMMAND(ID_VIEW_TOOLBAR_FILE_TOOLBAR, OnViewFileToolbar)
//	ON_COMMAND(ID_VIEW_TOOLBAR_EDIT_TOOLBAR, OnViewEditToolbar)
//	ON_COMMAND(ID_VIEW_TOOLBAR_ZOOM_TOOLBAR, OnViewZoomToolbar)
//	ON_COMMAND(ID_VIEW_TOOLBAR_MODEL_TOOLBAR, OnViewModelToolbar)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_FILE_TOOLBAR, OnUpdateViewFileToolbar)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_EDIT_TOOLBAR, OnUpdateViewEditToolbar)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_ZOOM_TOOLBAR, OnUpdateViewZoomToolbar)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_MODEL_TOOLBAR, OnUpdateViewModelToolbar)
//	ON_WM_DESTROY()
//	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
//	ON_COMMAND(ID_HELP_KEYWORD, OnHelpKeyword)
//	ON_COMMAND(ID_MODEL_RUNTIME_MAXSPEED, OnModelRuntimeMaxSpeed)
//	ON_COMMAND(ID_MODEL_RUNTIME_JUMP, OnModelRuntimeJump)
//	ON_COMMAND(ID_MODEL_RUNTIME_SYNC, OnModelRuntimeSync)
//	ON_COMMAND(ID_MODEL_RUNTIME_PAUSE, OnModelRuntimePause)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_MAXSPEED, OnUpdateModelRuntimeMaxSpeed)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_JUMP, OnUpdateModelRuntimeJump)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_SYNC, OnUpdateModelRuntimeSync)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_RUNTIME_PAUSE, OnUpdateModelRuntimePause)
//	ON_COMMAND(ID_MODEL_SHOWRATE_INC, OnModelShowRateInc)
//	ON_COMMAND(ID_MODEL_SHOWRATE_INCFOUR, OnModelShowRateIncFour)
//	ON_COMMAND(ID_MODEL_SHOWRATE_DECFOUR, OnModelShowRateDecFour)
//	ON_COMMAND(ID_MODEL_SHOWRATE_DEC, OnModelShowRateDec)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INC, OnUpdateModelShowRateInc)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_INCFOUR, OnUpdateModelShowRateIncFour)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DECFOUR, OnUpdateModelShowRateDecFour)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWRATE_DEC, OnUpdateModelShowRateDec)
//	ON_COMMAND(ID_MODEL_FRAME_NEXT, OnModelFrameNext)
//	ON_COMMAND(ID_MODEL_FRAME_PREV, OnModelFramePrev)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_NEXT, OnUpdateModelFrameNext)
//	ON_UPDATE_COMMAND_UI(ID_MODEL_FRAME_PREV, OnUpdateModelFramePrev)
//	ON_WM_TIMER()
//	ON_WM_CLOSE()
//	ON_WM_SHOWWINDOW()
//	ON_WM_SIZE()
//	ON_WM_ENTERMENULOOP()
//	ON_WM_EXITMENULOOP()
//	ON_WM_ENTERIDLE()
//	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR           , OnUpdateCoordStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR          , OnUpdateModifyStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITE_STATUSBAR , OnUpdateInsertOverwriteStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODEL_TIME_STATUSBAR      , OnUpdateModelTimeStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODEL_RUNTYPE_STATUSBAR   , OnUpdateModelRunTypeStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODEL_SPEED_STATUSBAR     , OnUpdateModelSpeedStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODEL_SHOWRATE_STATUSBAR  , OnUpdateModelShowRateStatusBar )
//END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_COORD_STATUSBAR,
	ID_MODIFY_STATUSBAR,
	ID_INSERTOVERWRITE_STATUSBAR,
	ID_MODEL_TIME_STATUSBAR,
	ID_MODEL_RUNTYPE_STATUSBAR,
	ID_MODEL_SPEED_STATUSBAR,
	ID_MODEL_SHOWRATE_STATUSBAR,
	ID_PROGRESSSTATUSBAR
};

rbool MainWindowBase::close_mode = false;

RDOStudioMainFrame::RDOStudioMainFrame()
	: m_updateTimerID(0)
{
	setupUi(this);
	mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);

	m_pSBCoord  = new QLabel(this);
	m_pSBModify = new QLabel(this);

	parent_type::statusBar()->addWidget(m_pSBCoord );
	parent_type::statusBar()->addWidget(m_pSBModify);

	QObject::connect(actFileNew,     SIGNAL(triggered(bool)), this, SLOT(onFileNew    ()));
	QObject::connect(actFileOpen,    SIGNAL(triggered(bool)), this, SLOT(onFileOpen   ()));
	QObject::connect(actFileClose,   SIGNAL(triggered(bool)), this, SLOT(onFileClose  ()));
	QObject::connect(actFileSave,    SIGNAL(triggered(bool)), this, SLOT(onFileSave   ()));
	QObject::connect(actFileSaveAs,  SIGNAL(triggered(bool)), this, SLOT(onFileSaveAs ()));
	QObject::connect(actFileSaveAll, SIGNAL(triggered(bool)), this, SLOT(onFileSaveAll()));

	QObject::connect(actModelBuild, SIGNAL(triggered(bool)), this, SLOT(onModelBuild()));
	QObject::connect(actModelRun,   SIGNAL(triggered(bool)), this, SLOT(onModelRun  ()));
	QObject::connect(actModelStop,  SIGNAL(triggered(bool)), this, SLOT(onModelStop ()));

	QObject::connect(actHelpContext, SIGNAL(triggered(bool)), this, SLOT(onHelpContext()));
	QObject::connect(actHelpAbout,   SIGNAL(triggered(bool)), this, SLOT(onHelpAbout  ()));
}

RDOStudioMainFrame::~RDOStudioMainFrame()
{}

void RDOStudioMainFrame::init()
{
	m_thisCWnd.Attach(winId());

	// ���-�� ������ ������� ������ � �����
	new RDOStudioModel();

	style_editor.init( "editor" );
	style_editor.load();

	style_build.init( "build" );
	style_build.window->wordWrap          = true;
	style_build.window->showHorzScrollBar = false;
	style_build.load();

	style_debug.init( "debug" );
	style_debug.window->wordWrap          = true;
	style_debug.window->showHorzScrollBar = false;
	style_debug.load();

	style_trace.init( "trace" );
	style_trace.load();

	style_results.init( "results" );
	style_results.load();

	style_find.init( "find" );
	style_find.load();

	style_frame.init( "frame" );
	style_frame.load();

	style_chart.init( "chart" );
	style_chart.load();

	m_pDockBuild   = new DockBuild  (this);
	m_pDockDebug   = new DockDebug  (this);
	m_pDockTrace   = new DockTrace  (this);
	m_pDockResults = new DockResults(this);
	m_pDockFind    = new DockFind   (this);

	QObject::connect(m_pDockBuild, SIGNAL(visibilityChanged(bool)), this, SLOT(onDockVisibleChanged(bool)));
	QObject::connect(m_pDockFind,  SIGNAL(visibilityChanged(bool)), this, SLOT(onDockVisibleChanged(bool)));

	updateAllStyles();
	addDockWidget(Qt::BottomDockWidgetArea, m_pDockBuild);
	tabifyDockWidget(m_pDockBuild, m_pDockDebug  );
	tabifyDockWidget(m_pDockBuild, m_pDockTrace  );
	tabifyDockWidget(m_pDockBuild, m_pDockResults);
	tabifyDockWidget(m_pDockBuild, m_pDockFind   );
	m_pDockDebug->raise();

	m_pDockTraceTree = new DockTraceTree(this);
	m_pDockFrame     = new DockFrame    (this);
	addDockWidget(Qt::LeftDockWidgetArea, m_pDockTraceTree);
	tabifyDockWidget(m_pDockTraceTree, m_pDockFrame);
	m_pDockTraceTree->raise();

	PTR(QMenu) pMenuDockView = new QMenu("����");
	ASSERT(pMenuDockView);
	menuView->insertMenu(menuViewToolbar->menuAction(), pMenuDockView);

	pMenuDockView->addAction(m_pDockBuild->toggleViewAction());
	pMenuDockView->addAction(m_pDockDebug->toggleViewAction());
	pMenuDockView->addAction(m_pDockTrace->toggleViewAction());
	pMenuDockView->addAction(m_pDockResults->toggleViewAction());
	pMenuDockView->addAction(m_pDockFind->toggleViewAction());
	pMenuDockView->addAction(m_pDockTraceTree->toggleViewAction());
	pMenuDockView->addAction(m_pDockFrame->toggleViewAction());

#ifdef PROCGUI_ENABLE
	m_pDockProcess = new DockProcess(this);
	tabifyDockWidget(m_pDockTraceTree, m_pDockProcess);
	pMenuDockView->addAction(m_pDockProcess->toggleViewAction());
#else
	m_pDockProcess = NULL;
#endif

	//! @todo qt
	//! �������� popup-menu ��� ���������� �� �����
	//popupMenu.CreatePopupMenu();

	//if (AfxGetMainWnd())
	//{
	//	CMenu* mainMenu = AfxGetMainWnd()->GetMenu();
	//	if (mainMenu)
	//	{
	//		rbool maximized = studioApp.getIMainWnd()->isMDIMaximazed();
	//		int delta = maximized ? 1 : 0;

	//		appendMenu( mainMenu->GetSubMenu( 1 + delta ), 4, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 1 + delta ), 8, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 1 + delta ), 10, &popupMenu );
	//		popupMenu.AppendMenu( MF_SEPARATOR );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 0, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 1, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 2, &popupMenu );
	//		popupMenu.AppendMenu( MF_SEPARATOR );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 7, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 8, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 9, &popupMenu );
	//		appendMenu( mainMenu->GetSubMenu( 2 + delta ), 10, &popupMenu );
	//	}
	//}
	//build->setPopupMenu( &popupMenu );
	//debug->setPopupMenu( &popupMenu );
	//results->setPopupMenu( &popupMenu );
	//find->setPopupMenu( &popupMenu );

	//! @todo qt
	//fileToolBar.init( c_wnd(), IDR_FILE_TOOLBAR, IDB_FILE_TOOLBAR_D );
	//editToolBar.init( c_wnd(), IDR_EDIT_TOOLBAR, IDB_EDIT_TOOLBAR_D );
	//zoomToolBar.init( c_wnd(), IDR_ZOOM_TOOLBAR, IDB_ZOOM_TOOLBAR_D );
	//modelToolBar.init( c_wnd(), IDR_MODEL_TOOLBAR, IDB_MODEL_TOOLBAR_D );

	//statusBar.Create( c_wnd() );
	//statusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) );
	//statusBar.SetPaneInfo( 0, ID_COORD_STATUSBAR           , SBPS_NORMAL , 70 );
	//statusBar.SetPaneInfo( 1, ID_MODIFY_STATUSBAR          , SBPS_NORMAL , 80 );
	//statusBar.SetPaneInfo( 2, ID_INSERTOVERWRITE_STATUSBAR , SBPS_NORMAL , 70 );
	//statusBar.SetPaneInfo( 3, ID_MODEL_TIME_STATUSBAR      , SBPS_NORMAL , 100 );
	//statusBar.SetPaneInfo( 4, ID_MODEL_RUNTYPE_STATUSBAR   , SBPS_NORMAL , 120 );
	//statusBar.SetPaneInfo( 5, ID_MODEL_SPEED_STATUSBAR     , SBPS_NORMAL , 90 );
	//statusBar.SetPaneInfo( 6, ID_MODEL_SHOWRATE_STATUSBAR  , SBPS_NORMAL , 140 );
	//statusBar.SetPaneInfo( 7, ID_PROGRESSSTATUSBAR         , SBPS_STRETCH, 70 );
	//statusBar.setProgressIndicator( ID_PROGRESSSTATUSBAR );

	//! @todo qt
	//fileToolBar.EnableDocking( CBRS_ALIGN_ANY );
	//editToolBar.EnableDocking( CBRS_ALIGN_ANY );
	//zoomToolBar.EnableDocking( CBRS_ALIGN_ANY );
	//modelToolBar.EnableDocking( CBRS_ALIGN_ANY );

	//modelToolBar.SetButtonStyle( 3, TBBS_CHECKBOX | TBBS_CHECKGROUP );
	//modelToolBar.SetButtonStyle( 4, TBBS_CHECKBOX | TBBS_CHECKGROUP );
	//modelToolBar.SetButtonStyle( 5, TBBS_CHECKBOX | TBBS_CHECKGROUP );

	tracer->registerClipboardFormat();

	PTR(IInit) pModelInit = dynamic_cast<PTR(IInit)>(model);
	ASSERT(pModelInit);
	pModelInit->init();
}

void RDOStudioMainFrame::setVisible(rbool visible)
{
	parent_type::setVisible(visible);
}

void RDOStudioMainFrame::closeEvent(QCloseEvent* event)
{
	close_mode = true;
	if (model && !model->saveModified())
	{
		event->ignore();
	}
	close_mode = false;

	if (event->isAccepted())
	{
		update_stop();
		style_editor.save();
		style_build.save();
		style_debug.save();
		style_trace.save();
		style_results.save();
		style_find.save();
		style_frame.save();
		style_chart.save();
	}
}

void RDOStudioMainFrame::onFileNew()
{
	//! @todo qt
	//RDOStudioModelNew dlg;
	//if (dlg.DoModal() == IDOK)
	//{
	//	model->newModel(dlg.getModelName(), dlg.getModelPath() + dlg.getModelName(), dlg.getModelTemplate());
	//}
}

void RDOStudioMainFrame::onFileOpen()
{
	model->openModel();
}

void RDOStudioMainFrame::onFileClose()
{
	model->closeModel();
}

void RDOStudioMainFrame::onFileSave()
{
	model->saveModel();
}

void RDOStudioMainFrame::onFileSaveAs()
{
	model->saveAsModel();
}

void RDOStudioMainFrame::onFileSaveAll()
{
	model->saveModel();
}

void RDOStudioMainFrame::onModelBuild()
{
	model->buildModel();
}

void RDOStudioMainFrame::onModelRun()
{
	model->runModel();
}

void RDOStudioMainFrame::onModelStop()
{
	model->stopModel();
}

void RDOStudioMainFrame::OnViewFileToolbar()
{
	//! @todo qt
	//ShowControlBar( &fileToolBar, !(fileToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewEditToolbar()
{
	//! @todo qt
	//ShowControlBar( &editToolBar, !(editToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewZoomToolbar()
{
	//! @todo qt
	//ShowControlBar( &zoomToolBar, !(zoomToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnViewModelToolbar()
{
	//! @todo qt
	//ShowControlBar( &modelToolBar, !(modelToolBar.GetStyle() & WS_VISIBLE), false );
}

void RDOStudioMainFrame::OnUpdateViewFileToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( fileToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewEditToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( editToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewZoomToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( zoomToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateViewModelToolbar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( modelToolBar.GetStyle() & WS_VISIBLE );
}

void RDOStudioMainFrame::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( "" );
}

void RDOStudioMainFrame::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( "" );
}

void RDOStudioMainFrame::OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( "" );
}

void RDOStudioMainFrame::OnUpdateModelTimeStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( rdo::format( ID_STATUSBAR_MODEL_TIME, model->getTimeNow() ).c_str() );
}

void RDOStudioMainFrame::OnUpdateModelRunTypeStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	tstring s = "";
	if ( model->isRunning() ) {
		switch ( model->getRuntimeMode() ) {
			case rdo::runtime::RTM_MaxSpeed  : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_MAXSPEED ); break;
			case rdo::runtime::RTM_Jump      : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_JUMP ); break;
			case rdo::runtime::RTM_Sync      : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_SYNC ); break;
			case rdo::runtime::RTM_Pause     : s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_PAUSE ); break;
			case rdo::runtime::RTM_BreakPoint: s = rdo::format( ID_STATUSBAR_MODEL_RUNTIME_BREAKPOINT, model->getLastBreakPointName().c_str() ); break;
		}
	}
	pCmdUI->SetText( s.c_str() );
}

void RDOStudioMainFrame::OnUpdateModelSpeedStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	if ( model->getRuntimeMode() != rdo::runtime::RTM_MaxSpeed || !model->isRunning() ) {
		pCmdUI->SetText( rdo::format( IDS_MODEL_SPEED, static_cast<int>(model->getSpeed() * 100) ).c_str() );
	} else {
		pCmdUI->SetText( "" );
	}
}

void RDOStudioMainFrame::OnUpdateModelShowRateStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	if ( model->isRunning() ) {
		switch ( model->getRuntimeMode() ) {
			case rdo::runtime::RTM_MaxSpeed:
			case rdo::runtime::RTM_Jump: {
				pCmdUI->SetText( rdo::format( IDS_MODEL_SHOWRATE_S, rdo::format( IDS_INFINITI ).c_str() ).c_str()  );
				break;
			}
			case rdo::runtime::RTM_Pause     :
			case rdo::runtime::RTM_BreakPoint: {
				pCmdUI->SetText( rdo::format( IDS_MODEL_SHOWRATE_S, "0.0" ).c_str() );
				break;
			}
			case rdo::runtime::RTM_Sync: {
				tstring s;
				double showRate = model->getShowRate();
				if ( showRate < 1e-10 || showRate > 1e10 ) {
					s = rdo::format( IDS_MODEL_SHOWRATE_E, showRate );
				} else if ( showRate >= 1 ) {
					s = rdo::format( IDS_MODEL_SHOWRATE_FMOREONE, showRate );
				} else {
					s = rdo::format( IDS_MODEL_SHOWRATE_FLESSONE, showRate );
				}
				pCmdUI->SetText( s.c_str() );
				break;
			}
		}
	} else {
		pCmdUI->SetText( "" );
	}
}

void RDOStudioMainFrame::OnViewOptions()
{
	RDOStudioOptions dlg;
	dlg.DoModal();
}

void RDOStudioMainFrame::updateAllStyles()
{
	model->updateStyleOfAllModel();
	getDockBuild  ().getContext().setEditorStyle(&style_build  );
	getDockDebug  ().getContext().setEditorStyle(&style_debug  );
	getDockTrace  ().getContext().setStyle      (&style_trace  );
	getDockResults().getContext().setEditorStyle(&style_results);
	getDockFind   ().getContext().setEditorStyle(&style_find   );
	tracer->updateChartsStyles();
}

void RDOStudioMainFrame::beginProgress( const int lower, const int upper, const int step )
{
	//! @todo qt
	//statusBar.setRange( lower, upper );
	//statusBar.setStep( step );
	//statusBar.setPos( lower );
	//statusBar.setProgressVisible( true );
}

void RDOStudioMainFrame::getProgressRange(int& lower, int& upper) const
{
	//! @todo qt
	//statusBar.getRange( lower, upper );
};

void RDOStudioMainFrame::setProgress(const int pos)
{
	//! @todo qt
	//statusBar.setPos( pos );
};

int RDOStudioMainFrame::getProgress() const
{
	return 0;
	//! @todo qt
	//return statusBar.getPos();
};

void RDOStudioMainFrame::offsetProgress(const int offset)
{
	//! @todo qt
	//statusBar.offsetPos( offset );
};

void RDOStudioMainFrame::stepProgress()
{
	//! @todo qt
	//statusBar.stepIt();
};

void RDOStudioMainFrame::endProgress()
{
	//! @todo qt
	//statusBar.setProgressVisible( false );
}

void RDOStudioMainFrame::OnHelpKeyword()
{
	QByteArray ba;
	ba.append("setSource qthelp://language/doc/rdo_studio_rus/html/about.htm\n");
	studioApp.callQtAssistant(ba);
}

void RDOStudioMainFrame::onHelpContext()
{
	PTR(IHelpContext) pHelpContext = dynamic_cast<PTR(IHelpContext)>(focusWidget());
	if (pHelpContext)
	{
		pHelpContext->onHelpContext();
		return;
	}
}

void RDOStudioMainFrame::onHelpAbout()
{
	About dlg(this);
	dlg.exec();
}

void RDOStudioMainFrame::OnModelRuntimeMaxSpeed()
{
	model->setRuntimeMode( rdo::runtime::RTM_MaxSpeed );
}

void RDOStudioMainFrame::OnModelRuntimeJump()
{
	model->setRuntimeMode( rdo::runtime::RTM_Jump );
}

void RDOStudioMainFrame::OnModelRuntimeSync()
{
	model->setRuntimeMode( rdo::runtime::RTM_Sync );
}

void RDOStudioMainFrame::OnModelRuntimePause()
{
	model->setRuntimeMode( rdo::runtime::RTM_Pause );
}

void RDOStudioMainFrame::OnUpdateModelRuntimeMaxSpeed( CCmdUI* pCmdUI )
{
	rbool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && model->getRuntimeMode() == rdo::runtime::RTM_MaxSpeed ? 1 : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRuntimeJump( CCmdUI* pCmdUI )
{
	rbool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && model->getRuntimeMode() == rdo::runtime::RTM_Jump ? 1 : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRuntimeSync( CCmdUI* pCmdUI )
{
	rbool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && model->getRuntimeMode() == rdo::runtime::RTM_Sync ? 1 : 0 );
}

void RDOStudioMainFrame::OnUpdateModelRuntimePause( CCmdUI* pCmdUI )
{
	rbool runing = model->isRunning();
	pCmdUI->Enable( runing );
	pCmdUI->SetCheck( runing && (model->getRuntimeMode() == rdo::runtime::RTM_Pause || model->getRuntimeMode() == rdo::runtime::RTM_BreakPoint) ? 1 : 0 );
}

void RDOStudioMainFrame::OnModelShowRateInc()
{
	model->setShowRate( model->getShowRate() * 1.5 );
}

void RDOStudioMainFrame::OnModelShowRateIncFour()
{
	model->setShowRate( model->getShowRate() * 4 );
}

void RDOStudioMainFrame::OnModelShowRateDecFour()
{
	model->setShowRate( model->getShowRate() / 4 );
}

void RDOStudioMainFrame::OnModelShowRateDec()
{
	model->setShowRate( model->getShowRate() / 1.5 );
}

void RDOStudioMainFrame::OnUpdateModelShowRateInc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdo::runtime::RTM_Sync && model->getShowRate() * 1.5 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateIncFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdo::runtime::RTM_Sync && model->getShowRate() * 4 <= DBL_MAX );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDecFour(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdo::runtime::RTM_Sync && model->getShowRate() / 4 >= DBL_MIN );
}

void RDOStudioMainFrame::OnUpdateModelShowRateDec(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->isRunning() && model->getRuntimeMode() == rdo::runtime::RTM_Sync && model->getShowRate() / 1.5 >= DBL_MIN );
}

void RDOStudioMainFrame::OnModelFrameNext()
{
	model->showNextFrame();
}

void RDOStudioMainFrame::OnModelFramePrev()
{
	model->showPrevFrame();
}

void RDOStudioMainFrame::OnUpdateModelFrameNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->canShowNextFrame() );
}

void RDOStudioMainFrame::OnUpdateModelFramePrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( model->canShowPrevFrame() );
}

void RDOStudioMainFrame::update_start()
{
	m_updateTimerID = startTimer(1000 / 30);
}

void RDOStudioMainFrame::update_stop()
{
	if (m_updateTimerID)
	{
		killTimer(m_updateTimerID);
		m_updateTimerID = 0;
	}
}

void RDOStudioMainFrame::timerEvent(QTimerEvent* event)
{
	parent_type::timerEvent(event);

	if (event->timerId() == m_updateTimerID)
	{
		update_stop();
		model->update();
		update_start();
	}
}

void RDOStudioMainFrame::showEvent(QShowEvent*)
{
	if (plugins)
	{
		plugins->saveMainFrameState(SW_SHOW);
	}
}

void RDOStudioMainFrame::hideEvent(QHideEvent*)
{
	if (plugins)
	{
		plugins->saveMainFrameState(SW_HIDE);
	}
}

//! @todo qt
//void RDOStudioMainFrame::OnEnterMenuLoop( BOOL bIsTrackPopupMenu )
//{
//	model->setGUIPause();
//}
//
//void RDOStudioMainFrame::OnExitMenuLoop( BOOL bIsTrackPopupMenu )
//{
//	model->setGUIContinue();
//}

void RDOStudioMainFrame::addSubWindow(QWidget* pWidget)
{
	ASSERT(pWidget);

	QList<QMdiSubWindow*> frameList = mdiArea->subWindowList();

	rbool maximized = frameList.empty();
	if (!maximized)
	{
		maximized = frameList.front()->isMaximized();
	}

	QMdiSubWindow* pFrame = mdiArea->addSubWindow(pWidget);

	IInit* pInitWidget = dynamic_cast<IInit*>(pWidget);
	if (pInitWidget)
	{
		pInitWidget->init();
	}
	pWidget->show();

	static const float sizeScale = 0.9f;
	QSize size(mdiArea->size());
	size.setWidth (float(size.width ()) * sizeScale);
	size.setHeight(float(size.height()) * sizeScale);
	pFrame->move  (frameList.size() * 4, frameList.size() * 4);
	pFrame->resize(size);

	if (maximized)
	{
		pFrame->showMaximized();
	}
}

void RDOStudioMainFrame::activateSubWindow(QWidget* pWidget)
{
	QMdiSubWindow* pSubWindow = dynamic_cast<QMdiSubWindow*>(pWidget);
	if (!pSubWindow)
		return;

	mdiArea->setActiveSubWindow(pSubWindow);
}

void RDOStudioMainFrame::connectOnActivateSubWindow(QObject* pObject)
{
	ASSERT(mdiArea);
	ASSERT(pObject);

	QObject::connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), pObject, SLOT(onSubWindowActivated(QMdiSubWindow*)));
}

template <RDOStudioMainFrame::StatusBar N>
void RDOStudioMainFrame::updateStatusBar(CREF(QString) message)
{
	updateStatusBar(StatusBarType<N>(), message);
}

template <RDOStudioMainFrame::StatusBar N>
void RDOStudioMainFrame::updateStatusBar(StatusBarType<N> statusBar, CREF(QString) message)
{
	PTR(QLabel) pLabel = getStatusBarLabel(statusBar);
	ASSERT(pLabel);
	pLabel->setText(message);
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_COORD>(StatusBarType<SB_COORD>)
{
	return m_pSBCoord;
}

template <>
PTR(QLabel) RDOStudioMainFrame::getStatusBarLabel<RDOStudioMainFrame::SB_MODIFY>(StatusBarType<SB_MODIFY>)
{
	return m_pSBModify;
}

void RDOStudioMainFrame::onDockVisibleChanged(rbool visible)
{
	if (!visible)
		return;

	QDockWidget* pDock = dynamic_cast<QDockWidget*>(sender());
	ASSERT(pDock);

	rdoEditCtrl::RDOLogEdit* pLog = NULL;

	if (pDock == &getDockBuild())
	{
		pLog = &getDockBuild().getContext();
	}
	else if (pDock == &getDockFind())
	{
		pLog = &getDockFind().getContext();
	}

	if (!pLog)
		return;

	rdoEditor::RDOEditorTabCtrl* pEditorTab = model->getTab();
	if (pEditorTab)
	{
		for (int i = 0; i < pEditorTab->count(); ++i)
		{
			pEditorTab->getItemEdit(i)->setLog(*pLog);
		}
	}
}
