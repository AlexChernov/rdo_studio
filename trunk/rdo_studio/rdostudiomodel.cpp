#include "stdafx.h"
#include "rdostudiomodel.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdo_edit/rdoeditortabctrl.h"
#include "edit_ctrls/rdobuildedit.h"
#include "edit_ctrls/rdodebugedit.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdorepository.h>
#include <rdobinarystream.h>

using namespace std;
using namespace rdoEditor;
using namespace RDOSimulatorNS;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
RDOStudioModel* model = NULL;

RDOStudioModel::RDOStudioModel():
	modelDocTemplate( NULL ),
	useTemplate( false ),
	closeWithDocDelete( true ),
	showCanNotCloseModelMessage( true ),
	openError( false ),
	frmDescribed( false ),
	modelTime( 0 ),
	showMode( SM_NoShow )
{
	model = this;

	modelDocTemplate = new CMultiDocTemplate( IDR_MODEL_TYPE, RUNTIME_CLASS(RDOStudioModelDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioModelView) );
	AfxGetApp()->AddDocTemplate( modelDocTemplate );

	kernel.setNotifyReflect( RDOKernel::newModel, newModelNotify );
	kernel.setNotifyReflect( RDOKernel::openModel, openModelNotify );
	kernel.setNotifyReflect( RDOKernel::saveModel, saveModelNotify );
	kernel.setNotifyReflect( RDOKernel::canCloseModel, canCloseModelNotify );
	kernel.setNotifyReflect( RDOKernel::closeModel, closeModelNotify );
	kernel.setNotifyReflect( RDOKernel::canNotCloseModel, canNotCloseModelNotify );

	kernel.setNotifyReflect( RDOKernel::beforeModelStart, beforeModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::afterModelStart, afterModelStartNotify );
	kernel.setNotifyReflect( RDOKernel::endExecuteModel, endExecuteModelNotify );
	kernel.setNotifyReflect( RDOKernel::modelStopped, stopModelNotify );
	kernel.setNotifyReflect( RDOKernel::parseSuccess, parseSuccessNotify );
	kernel.setNotifyReflect( RDOKernel::parseError, parseErrorNotify );
	kernel.setNotifyReflect( RDOKernel::executeError, executeErrorNotify );
	kernel.setNotifyReflect( RDOKernel::showFrame, showFrameNotify );

	kernel.setNotifyReflect( RDOKernel::buildString, buildNotify );
	kernel.setNotifyReflect( RDOKernel::debugString, debugNotify );
}

RDOStudioModel::~RDOStudioModel()
{
	closeModel();
	model = NULL;
}

void RDOStudioModel::newModel( const bool _useTemplate )
{
	useTemplate = _useTemplate;
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	kernel.getRepository()->newModel();
	output->updateLogConnection();
}

bool RDOStudioModel::openModel( const string& modelName ) const
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	output->showBuild();
	output->appendStringToBuild( format( IDS_MODEL_LOADING_BEGIN ) );
	const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
	static_cast<bool>(openError) = false;
	bool flag = kernel.getRepository()->openModel( modelName );
	if ( flag && !openError ) {
		rdo::binarystream stream;
		kernel.getRepository()->loadPMV( stream );
		output->appendStringToResults( stream.str() );
		output->updateLogConnection();
		output->appendStringToBuild( format( IDS_MODEL_LOADING_OK ) );
	} else {
		output->appendStringToBuild( format( IDS_MODEL_LOADING_FAILD ) );
	}
	return flag;
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
	stopModel();
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->clearDebug();
	output->clearResults();
	output->clearFind();
	kernel.getRepository()->closeModel();
}

void RDOStudioModel::buildModel() const
{
	if ( saveModel() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		output->appendStringToBuild( format( IDS_MODEL_BUILDING_BEGIN ) );
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
		kernel.getSimulator()->parseModel();
	}
}

void RDOStudioModel::runModel() const
{
	if ( !isRunning() && saveModel() ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->clearBuild();
		output->clearDebug();
		output->clearResults();
		output->showBuild();
		output->appendStringToBuild( format( IDS_MODEL_RUNNING_BEGIN ) );
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
		kernel.getSimulator()->runModel();
	}
}

void RDOStudioModel::stopModel() const
{
	if ( isRunning() ) kernel.getSimulator()->stopModel();
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

void RDOStudioModel::beforeModelStartNotify()
{
	model->beforeModelStart();
}

void RDOStudioModel::afterModelStartNotify()
{
	RDOStudioModelDoc* doc = model->getModelDoc();
	if ( doc ) {
		doc->running = true;
	}
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->showDebug();
	output->appendStringToDebug( format( IDS_MODEL_STARTED ) );
	const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
	int index = model->frameManager.getLastShowedFrame();
	if ( index != -1 ) {
		RDOStudioFrameView* view = model->frameManager.getFrameView( index );
		if ( view ) view->SetFocus();
	}
}

void RDOStudioModel::endExecuteModelNotify()
{
	model->stopModelFromSimulator();

	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->appendStringToDebug( format( IDS_MODEL_FINISHED ) );
	const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

	string str = kernel.getSimulator()->getResults().str();
	if ( str.length() ) {
		rdo::binarystream stream;
		stream.write( str.c_str(), str.length() );
		kernel.getRepository()->savePMV( stream );
		output->showResults();
		output->appendStringToResults( str );
	}
}

void RDOStudioModel::stopModelNotify()
{
	model->stopModelFromSimulator();

	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->appendStringToDebug( format( IDS_MODEL_STOPED ) );
	const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
}

void RDOStudioModel::stopModelFromSimulator()
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		doc->running = false;
	}
	frameManager.clear();
	frameManager.bmp_clear();
}

void RDOStudioModel::parseSuccessNotify()
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	int i = 0;
	for ( vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
		output->appendStringToBuild( it->message, it->file, it->lineNo - 1 );
		i++;
	}
	output->appendStringToBuild( format( IDS_MODEL_BUILDING_RESULTS, i ) );
	if ( i ) {
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->gotoNext();
	}
}

void RDOStudioModel::parseErrorNotify()
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	int i = 0;
	for ( vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
		output->appendStringToBuild( it->message, it->file, it->lineNo - 1 );
		i++;
	}
	if ( i ) {
		output->appendStringToBuild( format( IDS_MODEL_BUILDING_RESULTS, i ) );
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->gotoNext();
	}
}

void RDOStudioModel::executeErrorNotify()
{
	RDOStudioOutput* output = &studioApp.mainFrame->output;
	output->clearBuild();
	output->showBuild();
	output->appendStringToBuild( format( IDS_MODEL_RUNTIMEERROR ) );
	vector<RDOSyntaxError>* errors = kernel.getSimulator()->getErrors();
	int i = 0;
	for ( vector<RDOSyntaxError>::iterator it = errors->begin(); it != errors->end(); it++ ) {
		output->appendStringToBuild( it->message, it->file, it->lineNo - 1 );
		i++;
	}
	model->stopModelFromSimulator();
	if ( i ) {
		const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->gotoNext();
	}
}

void RDOStudioModel::showFrameNotify()
{
	model->showFrame();
}

void RDOStudioModel::buildNotify( string str )
{
	studioApp.mainFrame->output.appendStringToBuild( str );
}

void RDOStudioModel::debugNotify( string str )
{
	studioApp.mainFrame->output.appendStringToDebug( str );
}

RDOStudioModelDoc* RDOStudioModel::getModelDoc() const
{
	POSITION pos = modelDocTemplate->GetFirstDocPosition();
	if ( pos ) {
		return static_cast<RDOStudioModelDoc*>(modelDocTemplate->GetNextDoc( pos ));
	}
	return NULL;
}

void RDOStudioModel::updateModify() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		doc->updateModify();
	}
}

RDOEditorTabCtrl* RDOStudioModel::getTab() const
{
	RDOStudioModelDoc* doc = getModelDoc();
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
	if ( modelDocTemplate ) {

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		modelDocTemplate->OpenDocumentFile( NULL );
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
				edit->clearUndoBuffer();
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
	if ( modelDocTemplate ) {

		BOOL maximize = false;
		if ( !studioApp.mainFrame->MDIGetActive( &maximize ) ) {
			maximize = true;
		}

		modelDocTemplate->OpenDocumentFile( NULL );
		setName( kernel.getRepository()->getName() );

		RDOStudioOutput* output = &studioApp.mainFrame->output;
		RDOEditorTabCtrl* tab = getTab();
		if ( tab ) {
			int cnt = tab->getItemCount();
			studioApp.mainFrame->beginProgress( 0, cnt * 2 + 1 );
			studioApp.mainFrame->stepProgress();
			for ( int i = 0; i < cnt; i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				edit->setReadOnly( false );
				edit->clearAll();
				rdo::binarystream stream;
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
				studioApp.mainFrame->stepProgress();
				if ( canLoad ) {
					bool stream_error = stream.rdstate() & ios_base::failbit ? true : false;
					if ( !stream_error ) {
						edit->load( stream );
						edit->setReadOnly( kernel.getRepository()->isReadOnly() );
					} else {
						int obj = 0;
						switch ( i ) {
							case RDOEDIT_PAT: obj = IDS_MODEL_PATTERNS;      break;
							case RDOEDIT_RTP: obj = IDS_MODEL_RESOURCETYPES; break;
							case RDOEDIT_RSS: obj = IDS_MODEL_RESOURCES;     break;
							case RDOEDIT_OPR: obj = IDS_MODEL_OPERATIONS;    break;
							case RDOEDIT_FRM: obj = IDS_MODEL_FRAMES;        break;
							case RDOEDIT_FUN: obj = IDS_MODEL_FUNCTIONS;     break;
							case RDOEDIT_DPT: obj = IDS_MODEL_DPTS;          break;
							case RDOEDIT_PMD: obj = IDS_MODEL_PMDS;          break;
						}
						if ( obj ) {
							output->appendStringToBuild( format( IDS_MODEL_CANNOT_LOAD, format( obj ).c_str() ) );
							const_cast<rdoEditCtrl::RDOBuildEdit*>(output->getBuild())->UpdateWindow();
						}
						openError = true;
					}
				}
				edit->setCurrentPos( 0 );
				edit->setModifyFalse();
				edit->clearUndoBuffer();
				studioApp.mainFrame->stepProgress();
			}
			studioApp.mainFrame->endProgress();
		}

		CWnd* wnd = studioApp.mainFrame->GetActiveFrame();
		if ( maximize && wnd && wnd != studioApp.mainFrame ) {
			studioApp.mainFrame->MDIMaximize( wnd );
		}

		updateFrmDescribed();
	}
}

void RDOStudioModel::saveModelToRepository()
{
	bool smr_modified = false;
	RDOEditorTabCtrl* tab = getTab();
	if ( tab ) {
		int cnt = tab->getItemCount();
		int progress_cnt = 0;
		for ( int i = 0; i < cnt; i++ ) {
			if ( tab->getItemEdit( i )->isModify() ) progress_cnt++;
		}
		if ( progress_cnt ) {
			studioApp.mainFrame->beginProgress( 0, progress_cnt * 2 + 1 );
			studioApp.mainFrame->stepProgress();
			for ( int i = 0; i < cnt; i++ ) {
				RDOEditorEdit* edit = tab->getItemEdit( i );
				if ( edit->isModify() ) {
					rdo::binarystream stream;
					edit->save( stream );
					studioApp.mainFrame->stepProgress();
					switch ( i ) {
						case RDOEDIT_PAT: kernel.getRepository()->savePAT( stream ); break;
						case RDOEDIT_RTP: kernel.getRepository()->saveRTP( stream ); break;
						case RDOEDIT_RSS: kernel.getRepository()->saveRSS( stream ); break;
						case RDOEDIT_OPR: kernel.getRepository()->saveOPR( stream ); break;
						case RDOEDIT_FRM: kernel.getRepository()->saveFRM( stream ); break;
						case RDOEDIT_FUN: kernel.getRepository()->saveFUN( stream ); break;
						case RDOEDIT_DPT: kernel.getRepository()->saveDPT( stream ); break;
						case RDOEDIT_SMR: kernel.getRepository()->saveSMR( stream ); smr_modified = true; break;
						case RDOEDIT_PMD: kernel.getRepository()->savePMD( stream ); break;
					}
					edit->setModifyFalse();
				}
				studioApp.mainFrame->stepProgress();
			}
			studioApp.mainFrame->endProgress();
		}
	}
	setName( kernel.getRepository()->getName() );
	studioApp.insertReopenItem( kernel.getRepository()->getFullName() );

	if ( smr_modified ) updateFrmDescribed();
}

void RDOStudioModel::updateFrmDescribed()
{
	rdo::binarystream smrStream;
	kernel.getRepository()->loadSMR( smrStream );
	rdoModelObjects::RDOSMRFileInfo fileInfo;
	kernel.getSimulator()->parseSMRFileInfo( smrStream, fileInfo );
	frmDescribed = !fileInfo.frame_file.empty();
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
		showCanNotCloseModelMessage = false;
	}
	return flag;
}

void RDOStudioModel::closeModelFromRepository()
{
	if ( closeWithDocDelete ) {
		RDOStudioModelDoc* doc = getModelDoc();
		if ( doc ) {
			doc->OnCloseDocument();
		}
	}
	if ( !showCanNotCloseModelMessage ) {
		showCanNotCloseModelMessage = true;
	}
}

void RDOStudioModel::canNotCloseModelFromRepository() const
{
	if ( showCanNotCloseModelMessage ) {
		AfxMessageBox( format( ID_MSG_MODELCLOSE_ERROR ).c_str(), MB_ICONSTOP | MB_OK );
	}
}

string RDOStudioModel::getName() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		return doc->getName();
	}
	return "";
}

void RDOStudioModel::setName( const string& str )
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		doc->setName( str );
	}
}

bool RDOStudioModel::isModify() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		return doc->isModify();
	}
	return false;
}

bool RDOStudioModel::isRunning() const
{
	RDOStudioModelDoc* doc = getModelDoc();
	if ( doc ) {
		return doc->isRunning();
	}
	return false;
}

void RDOStudioModel::beforeModelStart()
{
	frameManager.bmp_clear();
	frameManager.clear();

	if ( frmDescribed ) {
		RDOStudioOutput* output = &studioApp.mainFrame->output;
		output->showDebug();
		output->appendStringToDebug( format( IDS_MODEL_RESOURCE_LOADING_BEGIN ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();

		vector< const string* > bitmaps = kernel.getSimulator()->getAllBitmaps();
		vector< const string* >::iterator bmp = bitmaps.begin();
		while ( bmp != bitmaps.end() ) {
			frameManager.bmp_insert( *(*bmp) );
			bmp++;
		}

		vector< const string* > frames = kernel.getSimulator()->getAllFrames();
		vector< const string* >::iterator it = frames.begin();
		while ( it != frames.end() ) {
			frameManager.insertItem( *(*it) );
			it++;
		}
		frameManager.expand();
		int initFrameNumber = kernel.getSimulator()->getInitialFrameNumber() - 1;
		modelTime = 0;
		showMode  = kernel.getSimulator()->getInitialShowMode();
		frameManager.setLastShowedFrame( initFrameNumber );
		if ( showMode == SM_Animation && initFrameNumber >= 0 && initFrameNumber < frameManager.count() ) {
			RDOStudioFrameDoc* doc = frameManager.connectFrameDoc( initFrameNumber );
			if ( doc ) {
				doc->SetTitle( format( IDS_FRAME_NAME, frameManager.getFrameName( initFrameNumber ).c_str() ).c_str() );
				frameManager.getFrameView( initFrameNumber )->SetFocus();
			}
		}
		output->appendStringToDebug( format( IDS_MODEL_RESOURCE_LOADING_OK ) );
		const_cast<rdoEditCtrl::RDODebugEdit*>(output->getDebug())->UpdateWindow();
	} else {
		modelTime = 0;
		showMode  = RDOSimulatorNS::SM_NoShow;
		frameManager.setLastShowedFrame( -1 );
	}
}

void RDOStudioModel::showFrame()
{
	modelTime = kernel.getSimulator()->getModelTime();
	studioApp.mainFrame->showNewModelTime( modelTime );
	while ( getShowMode() == SM_Monitor ) {};
	const vector<RDOFrame *>& frames = kernel.getSimulator()->getFrames();
	vector<RDOFrame *>::const_iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( *it ) {
			frameManager.showFrame( *it, index );
		}
		it++;
		index++;
	}
}

void RDOStudioModel::updateStyleOfAllModel() const
{
	POSITION pos = modelDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioModelDoc* doc = static_cast<RDOStudioModelDoc*>(modelDocTemplate->GetNextDoc( pos ));
		RDOEditorTabCtrl* tab = doc->getView()->tab;
		for ( int i = 0; i < tab->getItemCount(); i++ ) {
			tab->getItemEdit( i )->setEditorStyle( &studioApp.mainFrame->style_editor );
		}
	}
}

void RDOStudioModel::setShowMode( const ShowMode value )
{
	showMode = value;
	if ( showMode == SM_Animation ) {
		RDOStudioFrameDoc* doc = frameManager.getFirstExistDoc();
		if ( !doc ) {
			doc = frameManager.connectFrameDoc( frameManager.getLastShowedFrame() );
			if ( doc ) {
				doc->SetTitle( format( IDS_FRAME_NAME, frameManager.getFrameName( frameManager.getLastShowedFrame() ).c_str() ).c_str()  );
			}
		}
	}
	if ( showMode == SM_NoShow ) {
		frameManager.closeAll();
	}
	kernel.getSimulator()->setShowMode( showMode );
}

double RDOStudioModel::getShowRate() const
{
	return kernel.getSimulator()->getShowRate();
}

void RDOStudioModel::setShowRate( const double value ) const
{
	kernel.getSimulator()->setShowRate( value );
}
