#include "stdafx.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "resource.h"
#include "rdo_edit/rdoeditortabctrl.h"

#include <rdokernel.h>
#include <rdorepository.h>
#include <rdosimwin.h>

using namespace std;
using namespace rdoEditor;
using namespace rdoRepository;
using namespace RDOSimulatorNS;

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
RDOStudioModel* model = NULL;

RDOStudioModel::RDOStudioModel():
	name( "" ),
	useTemplate( false ),
	closeWithDocDelete( true ),
	canNotCloseByModel( false ),
	running( false )
{
	model = this;
	kernel.setNotifyReflect( RDOKernel::newModel, newModelNotify );
	kernel.setNotifyReflect( RDOKernel::openModel, openModelNotify );
	kernel.setNotifyReflect( RDOKernel::saveModel, saveModelNotify );
	kernel.setNotifyReflect( RDOKernel::canCloseModel, canCloseModelNotify );
	kernel.setNotifyReflect( RDOKernel::closeModel, closeModelNotify );
	kernel.setNotifyReflect( RDOKernel::canNotCloseModel, canNotCloseModelNotify );

	kernel.setNotifyReflect( RDOKernel::modelStarted, runModelNotify );
	kernel.setNotifyReflect( RDOKernel::endExecuteModel, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::modelStopped, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::parseError, parseErrorModelNotify );
	kernel.setNotifyReflect( RDOKernel::buildString, buildNotify );
	kernel.setNotifyReflect( RDOKernel::debugString, debugNotify );
}

RDOStudioModel::~RDOStudioModel()
{
	model = NULL;
}

void RDOStudioModel::newModel( const bool _useTemplate )
{
	useTemplate = _useTemplate;
	studioApp.mainFrame->output.clearBuild();
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.clearResults();
	studioApp.mainFrame->output.clearFind();
	kernel.getRepository()->newModel();
}

bool RDOStudioModel::openModel( const string& modelName ) const
{
	studioApp.mainFrame->output.clearBuild();
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.clearResults();
	studioApp.mainFrame->output.clearFind();
	return kernel.getRepository()->openModel( modelName );
}

bool RDOStudioModel::saveModel() const
{
	return kernel.getRepository()->saveModel();
}

void RDOStudioModel::saveAsModel() const
{
	kernel.getRepository()->saveAsModel();
}

void RDOStudioModel::closeModel() const
{
	studioApp.mainFrame->output.clearBuild();
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.clearResults();
	studioApp.mainFrame->output.clearFind();
	kernel.getRepository()->closeModel();
}

void RDOStudioModel::runModel() const
{
	if ( saveModel() ) {
		studioApp.mainFrame->output.clearBuild();
		studioApp.mainFrame->output.clearDebug();
		studioApp.mainFrame->output.clearResults();
		studioApp.mainFrame->output.showBuild();
		kernel.getSimulator()->runModel();
	}
}

void RDOStudioModel::stopModel() const
{
	kernel.getSimulator()->stopModel();
}

void RDOStudioModel::newModelNotify()
{
	model->newModelFromRepository();
}

void RDOStudioModel::openModelNotify()
{
	model->openModelFromRepository();
}

void RDOStudioModel::saveModelNotify()
{
	model->saveModelToRepository();
}

bool RDOStudioModel::canCloseModelNotify()
{
	return model->canCloseModel();
}

void RDOStudioModel::closeModelNotify()
{
	model->closeModelFromRepository();
}

void RDOStudioModel::canNotCloseModelNotify()
{
	model->canNotCloseModelFromRepository();
}

void RDOStudioModel::runModelNotify()
{
	studioApp.mainFrame->output.clearDebug();
	studioApp.mainFrame->output.showDebug();
	model->running = true;
}

void RDOStudioModel::stopModelNotify()
{
	model->running = false;
}

void RDOStudioModel::parseErrorModelNotify()
{
	vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	for ( vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
		rdoModelObjects::RDOFileType fileType = rdoModelObjects::PAT;
		switch ( it->file ) {
			case PAT_FILE : fileType = rdoModelObjects::PAT; break;
			case RTP_FILE : fileType = rdoModelObjects::RTP; break;
			case RSS_FILE : fileType = rdoModelObjects::RSS; break;
			case OPR_FILE : fileType = rdoModelObjects::OPR; break;
			case FRM_FILE : fileType = rdoModelObjects::FRM; break;
			case FUN_FILE : fileType = rdoModelObjects::FUN; break;
			case DPT_FILE : fileType = rdoModelObjects::DPT; break;
			case SMR1_FILE: fileType = rdoModelObjects::SMR; break;
			case SMR2_FILE: fileType = rdoModelObjects::SMR; break;
			case PMD_FILE : fileType = rdoModelObjects::PMD; break;
		}
		studioApp.mainFrame->output.appendStringToBuild( it->message, fileType, it->lineNo - 1 );
	}
}

void RDOStudioModel::buildNotify( string str )
{
	studioApp.mainFrame->output.appendStringToBuild( str );
}

void RDOStudioModel::debugNotify( string str )
{
	studioApp.mainFrame->output.appendStringToDebug( str );
}

RDOEditorTabCtrl* RDOStudioModel::getTab() const
{
	RDOStudioModelDoc* doc = studioApp.getModelDoc();
	if ( doc ) {
		RDOStudioModelView* view = doc->getView();
		if ( view ) {
			return view->tab;
		}
	}
	return NULL;
}

void RDOStudioModel::newModelFromRepository()
{
	if ( studioApp.modelDocTemplate ) {

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		studioApp.modelDocTemplate->OpenDocumentFile( NULL );
		setName( kernel.getRepository()->getName() );

		RDOEditorTabCtrl* tab = getTab();
		if ( tab ) {
			for ( int i = 0; i < tab->getItemCount(); i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();
				if ( useTemplate ) {
					int resID = -1;
					switch ( i ) {
						case RDOEDIT_PAT: resID = ID_INSERT_PAT_PATOPERATION; break;
						case RDOEDIT_RTP: resID = ID_INSERT_RTP_RTPPERMANENT; break;
						case RDOEDIT_RSS: resID = ID_INSERT_RSS_RSS; break;
						case RDOEDIT_OPR: resID = ID_INSERT_OPR_OPR; break;
						case RDOEDIT_FRM: resID = ID_INSERT_FRM_FRM; break;
						case RDOEDIT_FUN: resID = ID_INSERT_FUN_FUN; break;
						case RDOEDIT_SMR: resID = ID_INSERT_SMR_SMR; break;
					}
					if ( resID != - 1 ) {
						string s = format( resID );
						if ( !s.empty() ) {
							int incPos = -1;
							switch ( resID ) {
								case ID_INSERT_PAT_PATOPERATION : incPos = 9; break;
								case ID_INSERT_RTP_RTPPERMANENT : incPos = 15; break;
								case ID_INSERT_RSS_RSS          : incPos = 13; break;
								case ID_INSERT_OPR_OPR          : incPos = 14; break;
								case ID_INSERT_FRM_FRM          : incPos = 7; break;
								case ID_INSERT_FUN_FUN          : incPos = 10; break;
								case ID_INSERT_SMR_SMR          : incPos = 17; break;
							}
							edit->replaceCurrent( s, incPos );
						}
					}
				}
				edit->setModifyFalse();
			}
		}

		CWnd* wnd = studioApp.mainFrame->GetActiveFrame();
		if ( maximize && wnd && wnd != studioApp.mainFrame ) {
			studioApp.mainFrame->MDIMaximize( wnd );
		}
	}
}

void RDOStudioModel::openModelFromRepository()
{
	if ( studioApp.modelDocTemplate ) {

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		studioApp.modelDocTemplate->OpenDocumentFile( NULL );
		setName( kernel.getRepository()->getName() );

		RDOEditorTabCtrl* tab = getTab();
		if ( tab ) {
			for ( int i = 0; i < tab->getItemCount(); i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();
				stringstream stream;
				bool canLoad = true;
				switch ( i ) {
					case RDOEDIT_PAT: kernel.getRepository()->loadPAT( stream ); break;
					case RDOEDIT_RTP: kernel.getRepository()->loadRTP( stream ); break;
					case RDOEDIT_RSS: kernel.getRepository()->loadRSS( stream ); break;
					case RDOEDIT_OPR: kernel.getRepository()->loadOPR( stream ); break;
					case RDOEDIT_FRM: kernel.getRepository()->loadFRM( stream ); break;
					case RDOEDIT_FUN: kernel.getRepository()->loadFUN( stream ); break;
					case RDOEDIT_DPT: kernel.getRepository()->loadDPT( stream ); break;
					case RDOEDIT_SMR: kernel.getRepository()->loadSMR( stream ); break;
					case RDOEDIT_PMD: kernel.getRepository()->loadPMD( stream ); break;
					default: canLoad = false; break;
				}
				if ( canLoad ) {
					edit->load( stream );
					edit->setReadOnly( kernel.getRepository()->isReadOnly() );
				}
				edit->setCurrentPos( 0 );
				edit->setModifyFalse();
			}
		}

		CWnd* wnd = studioApp.mainFrame->GetActiveFrame();
		if ( maximize && wnd && wnd != studioApp.mainFrame ) {
			studioApp.mainFrame->MDIMaximize( wnd );
		}
	}
}

void RDOStudioModel::saveModelToRepository()
{
	RDOEditorTabCtrl* tab = getTab();
	if ( tab ) {
		for ( int i = 0; i < tab->getItemCount(); i++ ) {
			RDOEditorEdit* edit = tab->getItemEdit( i );
			stringstream stream;
			edit->save( stream );
			switch ( i ) {
				case RDOEDIT_PAT: kernel.getRepository()->savePAT( stream ); break;
				case RDOEDIT_RTP: kernel.getRepository()->saveRTP( stream ); break;
				case RDOEDIT_RSS: kernel.getRepository()->saveRSS( stream ); break;
				case RDOEDIT_OPR: kernel.getRepository()->saveOPR( stream ); break;
				case RDOEDIT_FRM: kernel.getRepository()->saveFRM( stream ); break;
				case RDOEDIT_FUN: kernel.getRepository()->saveFUN( stream ); break;
				case RDOEDIT_DPT: kernel.getRepository()->saveDPT( stream ); break;
				case RDOEDIT_SMR: kernel.getRepository()->saveSMR( stream ); break;
				case RDOEDIT_PMD: kernel.getRepository()->savePMD( stream ); break;
			}
			edit->setModifyFalse();
		}
	}
	setName( kernel.getRepository()->getName() );
	studioApp.insertReopenItem( kernel.getRepository()->getFullName() );
}

bool RDOStudioModel::canCloseModel()
{
	bool flag = true;
	if ( isModify() && closeWithDocDelete ) {
		int res = AfxGetMainWnd()->MessageBox( format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
		switch ( res ) {
			case IDYES   : flag = true; break;
			case IDNO    : flag = true; break;
			case IDCANCEL: flag = false; break;
		}
	}
	if ( !flag ) {
		canNotCloseByModel = true;
	}
	return flag;
}

bool RDOStudioModel::canCloseDocument()
{
	bool flag = true;
	if ( isModify() ) {
		int res = AfxGetMainWnd()->MessageBox( format( ID_MSG_MODELSAVE_QUERY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
		switch ( res ) {
			case IDYES   : flag = saveModel(); break;
			case IDNO    : flag = true; break;
			case IDCANCEL: flag = false; break;
		}
	}
	if ( flag ) {
		closeWithDocDelete = false;
		closeModel();
		closeWithDocDelete = true;
	}
	return flag;
}

void RDOStudioModel::closeModelFromRepository()
{
	if ( closeWithDocDelete ) {
		RDOStudioModelDoc* doc = studioApp.getModelDoc();
		if ( doc ) {
			doc->OnCloseDocument();
		}
	}
	if ( !canNotCloseByModel ) {
		canNotCloseByModel = false;
	}
}

void RDOStudioModel::canNotCloseModelFromRepository() const
{
	if ( !canNotCloseByModel ) {
		AfxMessageBox( format( ID_MSG_MODELCLOSE_ERROR ).c_str(), MB_ICONSTOP | MB_OK );
	}
}

string RDOStudioModel::getName() const
{
	return name;
}

void RDOStudioModel::setName( const string& str )
{
	name = str;
	static char szDelims[] = " \t\n\r";
	name.erase( 0, name.find_first_not_of( szDelims ) );
	name.erase( name.find_last_not_of( szDelims ) + 1, string::npos );
	studioApp.getModelDoc()->SetTitle( name.c_str() );
}

bool RDOStudioModel::isModify() const
{
	bool flag = false;
	RDOStudioModelDoc* doc = studioApp.getModelDoc();
	if ( doc ) {
		doc->updateModify();
		flag = doc->IsModified() ? true : false;
	}
	return flag;
}
