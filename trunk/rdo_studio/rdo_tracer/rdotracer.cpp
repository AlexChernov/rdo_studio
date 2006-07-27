#include "stdafx.h"
#include "rdotracer.h"
#include "../rdostudiomodel.h"
#include "../rdostudiothread.h"
#include <rdokernel.h>
#include <rdorepository.h>
#include <rdosimwin.h>

#include "../rdostudiomainfrm.h"
#include "../rdostudioapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace rdoRepository;
using namespace rdoSimulator;
using namespace rdoTracer;

RDOTracer* tracer = NULL;

static bool clear_after_stop = false;

// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------
RDOTracer::RDOTracer(): RDOTracerBase( "RDOStudioTracerGUI", static_cast<RDOKernelGUI*>(studioApp.studioGUI) )
{
	clear_after_stop = false;
	
	tracer = this;

	notifies.push_back( RT_REPOSITORY_MODEL_CLOSE );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_OK );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_BY_USER );
	notifies.push_back( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
	notifies.push_back( RT_RUNTIME_MODEL_START_BEFORE );
	notifies.push_back( RT_RUNTIME_TRACE_STRING );

	after_constructor();
}

RDOTracer::~RDOTracer()
{
	tracer = NULL;
}

void RDOTracer::proc( RDOThread::RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE: {
			clear_after_stop = model->isRunning();
			if ( !clear_after_stop ) {
				clear();
			}
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_BEFORE: {
			clear();
			setShowMode( kernel->simulator()->getShowMode() );
			setModelName( kernel->repository()->getName() );
			try {
				studioApp.mainFrame->output.appendStringToDebug( rdo::format( IDS_TRACER_GETTING_MODEL_STRUCTURE ) );
				getModelStructure( kernel->simulator()->getModelStructure() );
				studioApp.mainFrame->output.appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_NAME_OK ) );
			} catch ( ... ) {
				studioApp.mainFrame->output.appendStringToDebug( rdo::format( IDS_MODEL_RESOURCE_LOADING_NAME_FAILED ) );
			}
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK           :
		case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER      :
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR: {
			if ( clear_after_stop ) {
				clear();
				clear_after_stop = false;
			}
			setDrawTrace( true );
			break;
		}
		case RDOThread::RT_RUNTIME_TRACE_STRING: {
			msg.lock();
			getTraceString( *static_cast<std::string*>(msg.param) );
			msg.unlock();
			break;
		}
	}
}

void RDOTracer::setShowMode( const rdoSimulator::ShowMode value )
{
	if ( value == SM_NoShow ) {
		setDrawTrace( false );
	} else {
		setDrawTrace( true );
	}
}
