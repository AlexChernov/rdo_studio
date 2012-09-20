/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/view.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qevent.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/model/view.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/src/output.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelView
// --------------------------------------------------------------------------------
static const UINT FINDINMODEL_MSG = ::RegisterWindowMessage( FINDMSGSTRING );

// ON_UPDATE_COMMAND_UI �������

//! @todo qt
//BEGIN_MESSAGE_MAP(RDOStudioModelView, RDOStudioEditBaseView)
//	ON_WM_CREATE()
//	ON_WM_SETFOCUS()
//	ON_WM_SIZE()
//	ON_COMMAND(ID_SEARCH_FIND_INMODEL, OnSearchFindInModel)
//	ON_REGISTERED_MESSAGE( FINDINMODEL_MSG, OnFindInModelMsg )
//	ON_UPDATE_COMMAND_UI( ID_COORD_STATUSBAR          , OnUpdateCoordStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_MODIFY_STATUSBAR         , OnUpdateModifyStatusBar )
//	ON_UPDATE_COMMAND_UI( ID_INSERTOVERWRITE_STATUSBAR, OnUpdateInsertOverwriteStatusBar )
//	ON_COMMAND(ID_FILE_PRINT, RDOStudioEditBaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, RDOStudioEditBaseView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, RDOStudioEditBaseView::OnFilePrintPreview)
//END_MESSAGE_MAP()

RDOStudioModelView::RDOStudioModelView(QWidget* pParent)
	: RDOStudioEditBaseView(pParent)
	, tab(NULL)
{}

RDOStudioModelView::~RDOStudioModelView()
{
	if ( tab ) { delete tab; tab = NULL; }
}

rbool RDOStudioModelView::init()
{
	tab = new RDOEditorTabCtrl(this);
	tab->Create( NULL, NULL, 0, CRect(0, 0, 100, 100), CWnd::FromHandle(winId()), 0 );
	return true;
}

void RDOStudioModelView::closeEvent(PTR(QCloseEvent) event)
{
	if (model->saveModified())
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

//! @todo qt
//BOOL RDOStudioModelView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
//{
//	if ( tab->getCurrentEdit()->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
//	return RDOStudioEditBaseView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//}
//
//void RDOStudioModelView::OnSetFocus(CWnd* pOldWnd)
//{
//	RDOStudioEditBaseView::OnSetFocus( pOldWnd );
//	tab->SetFocus();
//}
//

void RDOStudioModelView::resizeEvent(PTR(QResizeEvent) event)
{
	parent_type::resizeEvent(event);

	if (!tab)
		return;

	QSize size(event->size());
	tab->MoveWindow(0, 0, size.width(), size.height());
}

//void RDOStudioModelView::OnSize(UINT nType, int cx, int cy) 
//{
//	RDOStudioEditBaseView::OnSize(nType, cx, cy);
//	
//	CRect r;
//	GetClientRect( r );
//	tab->MoveWindow( r );
//}

REF(rdoEditor::RDOEditorTabCtrl) RDOStudioModelView::getTab()
{
	return *tab;
}

RDOEditorEdit* RDOStudioModelView::getEdit() const
{
	return tab->getCurrentEdit();
}

void RDOStudioModelView::OnSearchFindInModel() 
{
	CFindReplaceDialog* pDlg = new CFindReplaceDialog();
	pDlg->Create( true, getEdit()->getWordForFind().c_str(), NULL, FR_HIDEUPDOWN, &m_thisCWnd );
}

LRESULT RDOStudioModelView::OnFindInModelMsg( WPARAM /*wParam*/, LPARAM lParam )
{
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier( lParam );

	if ( !pDialog->IsTerminating() ) {
		studioApp.getIMainWnd()->getOutputDoc()->clearFind();
		studioApp.getIMainWnd()->getOutputDoc()->showFind();
		tstring findStr  = pDialog->GetFindString();
		rbool bMatchCase      = pDialog->MatchCase() ? true : false;
		rbool bMatchWholeWord = pDialog->MatchWholeWord() ? true : false;
		studioApp.getIMainWnd()->getOutputDoc()->getFind()->setKeyword( findStr, bMatchCase );
		studioApp.getIMainWnd()->getOutputDoc()->appendStringToFind( rdo::format( ID_FINDINMODEL_BEGINMSG, findStr.c_str() ) );
		int count = 0;
		for ( int i = 0; i < tab->getItemCount(); i++ ) {
			RDOEditorEdit* edit = tab->getItemEdit( i );
			int pos  = 0;
			int line = 0;
			while ( pos != -1 ) {
				pos = edit->findPos( findStr, line, bMatchCase, bMatchWholeWord );
				if ( pos != -1 ) {
					line = edit->getLineFromPosition( pos );
					studioApp.getIMainWnd()->getOutputDoc()->appendStringToFind( edit->getLine( line ), tab->indexToType( i ), line, pos - edit->getPositionFromLine( line ) );
					line++;
					count++;
				}
			}
		}
		pDialog->SendMessage( WM_CLOSE );
		tstring s;
		if ( count ) {
			s = rdo::format( ID_FINDINMODEL_ENDMSG_COUNT, count );
		} else {
			s = rdo::format( ID_FINDINMODEL_ENDMSG_NOTFOUND, findStr.c_str() );
		}
		studioApp.getIMainWnd()->getOutputDoc()->appendStringToFind( s );
	}
	return 0;
}

void RDOStudioModelView::OnUpdateCoordStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	RDOEditorEdit* edit = getEdit();
	if ( edit ) {
		pCmdUI->SetText( rdo::format( "%d: %d", edit->getCurrentColumnNumber() + 1, edit->getCurrentLineNumber() + 1 ).c_str() );
	} else {
		pCmdUI->SetText( "" );
	}
}

void RDOStudioModelView::OnUpdateModifyStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	RDOEditorEdit* edit = getEdit();
	if ( edit ) {
		if ( edit->isReadOnly() ) {
			pCmdUI->SetText( rdo::format( ID_STATUSBAR_READONLY ).c_str() );
		} else if ( edit->isModify() ) {
			pCmdUI->SetText( rdo::format( ID_STATUSBAR_MODIFIED ).c_str() );
		} else {
			pCmdUI->SetText( "" );
		}
	} else {
		pCmdUI->SetText( "" );
	}
}

void RDOStudioModelView::OnUpdateInsertOverwriteStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	RDOEditorEdit* edit = getEdit();
	if ( edit && edit->isOverwrite() ) {
		pCmdUI->SetText( rdo::format( ID_STATUSBAR_OVERWRITE ).c_str() );
	} else {
		pCmdUI->SetText( "" );
	}
}
