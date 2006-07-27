#pragma warning(disable : 4786)  

#include "../rdo_runtime/memcheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <sstream>
#include <algorithm>

#include "rdosimwin.h"
#include <rdokernel.h>
#include <rdorepository.h>

#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdosmr.h"
#include "rdofrm.h"

namespace rdoSimulator
{
// --------------------------------------------------------------------
// ---------- RDORuntimeTracer
// --------------------------------------------------------------------
class RDORuntimeTracer: public RDOTrace, public RDOEndL
{
private:
	LPVOID pParam;
  	rdoRuntime::TracerCallBack tracerCallBack;
	std::stringstream stream;

public:
	std::ostream& getOStream() { return stream; }
	RDOEndL& getEOL() { return *this; }
	void onEndl()
	{
		tracerCallBack(&stream.str(), pParam);
		stream.str("");
	}
	RDORuntimeTracer(rdoRuntime::TracerCallBack _tracerCallBack, LPVOID _pParam):
		tracerCallBack(_tracerCallBack), pParam(_pParam) { isNullTracer = false; }
};

// --------------------------------------------------------------------
// ---------- RDOSimResulter
// --------------------------------------------------------------------
class RDOSimResulter: public rdoRuntime::RDOResult
{
private:
	std::ostream& stream;

public:
	RDOSimResulter(std::ostream &_stream): stream(_stream) {isNullResult = false;}
	virtual std::ostream &getOStream() { return stream; }
};

} // namespace rdoSimulator

namespace rdoRuntime {
// --------------------------------------------------------------------
// ---------- RDOThreadRunTime
// --------------------------------------------------------------------
RDOThreadRunTime::RDOThreadRunTime():
	RDOThreadMT( "RDOThreadRunTime" ),
	simulator( NULL ),
	runtime_error( false )
{
	notifies.push_back( RT_RUNTIME_GET_TIMENOW );
	notifies.push_back( RT_RUNTIME_GET_FRAME );
	after_constructor();
}

void RDOThreadRunTime::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_RUNTIME_GET_TIMENOW: {
			*static_cast<double*>(msg.param) = simulator->runtime->getTimeNow();
			break;
		}
		case RT_THREAD_CLOSE: {
			broadcastMessage( RT_RUNTIME_MODEL_STOP_BEFORE );
			break;
		}
		case RT_RUNTIME_GET_FRAME: {
			msg.lock();
			GetFrame* getframe = static_cast<GetFrame*>(msg.param);
			simulator->runtime->allFrames.at(getframe->frame_number)->prepareFrame( getframe->frame, simulator->runtime );
			msg.unlock();
			break;
		}
	}
}

void RDOThreadRunTime::start()
{
#ifdef TR_TRACE
	trace( thread_name + " start begin" );
#endif

	broadcastMessage( RT_RUNTIME_MODEL_START_BEFORE );

	simulator = kernel->simulator();

	RDOTrace* tracer;
	rdoRuntime::RDOResult* resulter;

	// Creating tracer and resulter //////////////////////////////////
	if ( simulator->parser->smr->traceFileName == NULL ) {
		tracer = new RDOTrace();
	} else {
		tracer = new rdoSimulator::RDORuntimeTracer( rdoSimulator::tracerCallBack, simulator );
	}

	if ( simulator->parser->smr->resultsFileName == NULL ) {
		resulter = new rdoRuntime::RDOResult();
	} else {
		simulator->resultString.str("");
		resulter = new rdoSimulator::RDOSimResulter( simulator->resultString );
	}

	// RDO config initialization
	simulator->runtime->config.showAnimation = simulator->parser->smr->showMode;
	int size = simulator->runtime->allFrames.size();
	for ( int i = 0; i < size; i++ ) {
		simulator->runtime->config.allFrameNames.push_back( *simulator->runtime->allFrames.at(i)->getName() );
	}

//	simulator->runtime->config.currFrameToShow = simulator->parser->smr->frameNumber - 1;
	simulator->runtime->config.keysPressed.clear();
	simulator->runtime->config.mouseClicked = false;
	simulator->runtime->config.activeAreasMouseClicked.clear();
	simulator->runtime->config.frames.clear();
	simulator->runtime->config.currTime = 0;
	simulator->runtime->config.newTime = 0;
	if ( simulator->parser->smr->showRate ) {
		simulator->runtime->config.showRate = *simulator->parser->smr->showRate;
	} else {
		simulator->runtime->config.showRate = 60;	// default
	}
	simulator->runtime->config.realTimeDelay = 0;

	// Modelling
	simulator->runtime->tracerCallBack = rdoSimulator::tracerCallBack;
	simulator->runtime->param = simulator;
	simulator->runtime->frameCallBack = rdoSimulator::frameCallBack;
	simulator->canTrace = true;

	try {
		simulator->exitCode = rdoModel::EC_OK;
		simulator->runtime->rdoInit( tracer, resulter );
	}
	catch( rdoParse::RDOSyntaxException& ) {
		runtime_error = true;
	}
	catch( rdoParse::RDOInternalException& ex ) {
		runtime_error = true;
		std::string mess = "Internal exception: " + ex.mess;
		sendMessage( kernel, RDOThread::RT_DEBUG_STRING, &mess );
	}

	broadcastMessage( RT_RUNTIME_MODEL_START_AFTER );

#ifdef TR_TRACE
	trace( thread_name + " start end, runing simulation" );
#endif
}

void RDOThreadRunTime::idle()
{
#ifdef RDO_MT
	if ( broadcast_waiting || !was_start || was_close ) {
		RDOThread::idle();
		return;
	}
#endif
//	TRACE( "R. %d, %d, %d, %d\n", ::GetCurrentProcess(), ::GetCurrentProcessId(), ::GetCurrentThread(), ::GetCurrentThreadId() );
	try {
		if ( runtime_error || !simulator->runtime->rdoNext() ) {
			sendMessage( this, RT_THREAD_CLOSE );
		}
	}
	catch ( rdoParse::RDOSyntaxException& ) {
		runtime_error = true;
	}
	catch ( rdoParse::RDOInternalException& ex ) {
		runtime_error = true;
		std::string mess = "Internal exception: " + ex.mess;
		sendMessage( kernel, RDOThread::RT_DEBUG_STRING, &mess );
	}
}

void RDOThreadRunTime::stop()
{
#ifdef TR_TRACE
	trace( thread_name + " stop begin" );
#endif

	try {
		simulator->runtime->rdoPostProcess();
	}
	catch( rdoParse::RDOSyntaxException& ) {
		runtime_error = true;
	}
	catch( rdoParse::RDOInternalException& ex ) {
		runtime_error = true;
		std::string mess = "Internal exception: " + ex.mess;
		sendMessage( kernel, RDOThread::RT_DEBUG_STRING, &mess );
	}

	broadcastMessage( RT_RUNTIME_MODEL_STOP_AFTER );

	RDOThread::stop();

#ifdef TR_TRACE
	trace( thread_name + " stop end" );
#endif
}

} // namespace rdoRuntime

namespace rdoSimulator
{
// --------------------------------------------------------------------
// ---------- RDOThreadSimulator
// --------------------------------------------------------------------
RDOThreadSimulator::RDOThreadSimulator():
	RDOThreadMT( "RDOThreadSimulator" ),
	runtime( NULL ), 
	parser( NULL ),
	thread_runtime( NULL ),
	exitCode( rdoModel::EC_OK )
//	shiftPressed(false),
//	ctrlPressed(false)
{
	notifies.push_back( RT_STUDIO_MODEL_BUILD );
	notifies.push_back( RT_STUDIO_MODEL_RUN );
	notifies.push_back( RT_STUDIO_MODEL_STOP );
	notifies.push_back( RT_SIMULATOR_GET_LIST );
	notifies.push_back( RT_SIMULATOR_GET_ERRORS );
	notifies.push_back( RT_THREAD_STOP_AFTER );
	after_constructor();
}

RDOThreadSimulator::~RDOThreadSimulator()
{
	terminateModel();
	closeModel();
}

void RDOThreadSimulator::proc( RDOMessageInfo& msg )
{
	switch ( msg.message ) {
		case RT_STUDIO_MODEL_BUILD: {
			parseModel();
			break;
		}
		case RT_STUDIO_MODEL_RUN: {
			runModel();
			break;
		}
		case RT_STUDIO_MODEL_STOP: {
			stopModel();
			break;
		}
		case RT_SIMULATOR_GET_LIST: {
			msg.lock();
			GetList* getlist = static_cast<GetList*>(msg.param);
			switch ( getlist->type ) {
				case GetList::frames: {
					if ( runtime ) {
						int size = runtime->allFrames.size();
						for ( int i = 0; i < size; i++ ) {
							getlist->list->push_back( *runtime->allFrames.at(i)->getName() );
						}
					}
					break;
				}
				case GetList::bitmaps: {
					if ( runtime ) {
						int size = runtime->allFrames.size();
						for ( int i = 0; i < size; i++ ) {
							runtime->allFrames.at(i)->getAllBitmaps( *getlist->list );
						}
					}
					break;
				}
			}
			msg.unlock();
			break;
		}
		case RT_SIMULATOR_GET_ERRORS: {
			std::vector< RDOSyntaxError >* errors = getErrors();
			msg.lock();
			std::vector< RDOSyntaxError >* msg_errors = static_cast<std::vector< RDOSyntaxError >*>(msg.param);
			msg_errors->assign( errors->begin(), errors->end() );
			msg.unlock();
			break;
		}
		case RT_THREAD_STOP_AFTER: {
			if ( msg.from == thread_runtime ) {
				// rdoModel::EC_ParserError   - �� ������������ � run-time
				// rdoModel::EC_ModelNotFound - �� ������������ � run-time
				// rdoModel::EC_UserBreak     - ��������������� � simulator, �������� RT_THREAD_STOP_AFTER �� �����������
				if ( !thread_runtime->runtime_error ) {
					// ������������ ���� ���������
					// �������� ��� ��������� �� runtime�, �.�. rdoModel::EC_OK (��-���������) ����� ���������� �� rdoModel::EC_NoMoreEvents
					exitCode = runtime->whyStop;
					broadcastMessage( RT_SIMULATOR_MODEL_STOP_OK );
					closeModel();
				} else {
					// ������������ ����, �� �� ���������
					exitCode = rdoModel::EC_RunTimeError;
					broadcastMessage( RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR );
					closeModel();
				}
				// ����� ��������� ����, ���� ������� � �������
				// �������� ��� ��� ��������, �.�. thread_destroy �� ������ �������������� � thread_runtime ���� �������������� RT_THREAD_STOP_AFTER
#ifdef RDO_MT
				delete thread_runtime->thread_destroy;
				thread_runtime->thread_destroy = NULL;
#endif
				thread_runtime = NULL;
			}
			break;
		}
	}
}

const std::vector< RDOFrame* >& RDOThreadSimulator::getFrames()
{
	return frames;
}

/*
void RDOThreadSimulator::addKeyPressed(int scanCode)
{
//	scanCodes.push_back(scanCode);
//	kernel.debug("addKeyPressed: %d", scanCode); 
}
  */
void RDOThreadSimulator::keyDown(int scanCode)
{
//	if(scanCode == VK_SHIFT)
//		shiftPressed = true;
//	else if(scanCode == VK_CONTROL)
//		ctrlPressed = true;

	scanCodes.push_back(scanCode);
//	kernel.debug("keyDown: %d", scanCode); 
}

void RDOThreadSimulator::keyUp(int scanCode)
{
//	if(scanCode == VK_SHIFT)
//		shiftPressed = false;
//	else if(scanCode == VK_CONTROL)
//		ctrlPressed = false;

	scanCodes.erase(std::remove(scanCodes.begin(), scanCodes.end(), scanCode), scanCodes.end());
//	kernel.debug("keyUp: %d", scanCode); 
}

void RDOThreadSimulator::addAreaPressed(std::string& areaName)
{
	areasActivated.push_back(areaName);
}

void frameCallBack( rdoRuntime::RDOConfig* config, void* param )
{
	return;
	RDOThreadSimulator* simulator = static_cast<RDOThreadSimulator*>(param);
	// UA 14.12.05 // ��-�� ����, ��� ����� ������ ������� ����������,
	// ������� ����� ����� � ���������
	while ( simulator->getShowMode() == SM_Monitor ) {
		::Sleep( 500 );
	}
	if((config->showAnimation == SM_Animation) && (simulator->showMode == SM_Animation))
	{
		Sleep(config->realTimeDelay);

		simulator->frames = config->frames;

//		simulator->thread_runtime->broadcastMessage( RDOThread::RT_RUNTIME_FRAME_SHOW );
/*
		if(simulator->scanCodes.size() > 0)
		{
			kernel.debug("Scan Codes:"); 
			for(int j = 0; j < simulator->scanCodes.size(); j++)
				kernel.debug(" %d", simulator->scanCodes[j]);

			kernel.debug("\n"); 
		}
  */
//		simulator->scanCodes.erase(remove(simulator->scanCodes.begin(), simulator->scanCodes.end(), VK_SHIFT), simulator->scanCodes.end());
//		simulator->scanCodes.erase(remove(simulator->scanCodes.begin(), simulator->scanCodes.end(), VK_CONTROL), simulator->scanCodes.end());
		config->keysPressed.insert(config->keysPressed.end(), simulator->scanCodes.begin(), simulator->scanCodes.end());
//		simulator->scanCodes.clear();
//		if(simulator->shiftPressed)
//			config->keysPressed.push_back(VK_SHIFT);
//		if(simulator->ctrlPressed)
//			config->keysPressed.push_back(VK_CONTROL);

		config->activeAreasMouseClicked.insert(config->activeAreasMouseClicked.end(), simulator->areasActivated.begin(), simulator->areasActivated.end());
		simulator->areasActivated.clear();

		for(int i = 0; i < config->frames.size(); i++)
			delete config->frames.at(i);
		config->frames.clear();
	}
	else
	{
		simulator->frames.clear();
//		simulator->thread_runtime->broadcastMessage( RDOThread::RT_RUNTIME_FRAME_SHOW );
	}

	config->showRate = simulator->getShowRate();
	config->showAnimation = simulator->getShowMode();
}

void tracerCallBack( std::string* newString, void* param )
{
	RDOThreadSimulator* simulator = static_cast<RDOThreadSimulator*>(param);
//	kernel.notifyString(RDOKernel.traceString, newString->c_str());
	int pos = 0;
	if(newString->empty())
		return;

	if(!simulator->canTrace)
		return;

	for(;;)
	{
		int next = newString->find('\n', pos);
		std::string str = newString->substr(pos, next-pos);
		simulator->thread_runtime->broadcastMessage( RDOThread::RT_RUNTIME_TRACE_STRING, &str );
		if(next == std::string::npos)
			break;
		pos = next + 1;
		if(pos >= newString->length())
			break;

		int aa = 0;
	}
}

bool RDOThreadSimulator::parseModel()
{
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	try {
		exitCode = rdoModel::EC_OK;
		parser->parse();
	}
	catch ( rdoParse::RDOSyntaxException& /*ex*/ ) {
//		string mess = ex.getType() + " : " + ex.mess;
//		kernel.notifyString(RDOKernel::buildString, mess);
		exitCode = rdoModel::EC_ParserError;
		broadcastMessage( RT_SIMULATOR_PARSE_ERROR );
		closeModel();
//		kernel.callback(RDOKernel::modelExit, rdoModel::EC_ParserError);
		return false;
	}
	catch ( rdoParse::RDOInternalException& ex ) {
		std::string mess = "Internal exception: " + ex.mess;
		broadcastMessage( RT_SIMULATOR_PARSE_STRING, &mess );
		exitCode = rdoModel::EC_ParserError;
		broadcastMessage( RT_SIMULATOR_PARSE_ERROR );
		closeModel();
//		kernel.callback(RDOKernel::modelExit, rdoModel::EC_ParserError);
		return false;
	}

	showMode = getInitialShowMode();
	showRate = getInitialShowRate();

/*
	if(parser->errors.size() > 0)
	{
		for(int i = 0; i < parser->errors.size(); i++)
		{
			kernel.notifyString(RDOKernel::buildString, parser->errors.at(i).message);
		}

		return false;
	}
*/

	broadcastMessage( RT_SIMULATOR_PARSE_OK );

//	kernel.notifyString(RDOKernel::buildString, getModelStructure().str().c_str());

	return true;
}

void RDOThreadSimulator::runModel()
{
	if ( parseModel() ) {
		exitCode = rdoModel::EC_OK;
		thread_runtime = new rdoRuntime::RDOThreadRunTime();
	}
}

void RDOThreadSimulator::stopModel()
{
	canTrace = false;
	terminateModel();
	broadcastMessage( RT_SIMULATOR_MODEL_STOP_BY_USER );
	closeModel();
	// UA 19.08.04 // ������� ��� ��������
//	kernel.callback(RDOKernel::modelExit, rdoModel::EC_UserBreak);
}

void RDOThreadSimulator::terminateModel()
{
	if ( thread_runtime ) {
		exitCode = rdoModel::EC_UserBreak;
		// ��������� ����������� �� ��������� run-time-�����, �.�. ��������� � ����
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.erase( std::find(notifies.begin(), notifies.end(), RT_THREAD_STOP_AFTER) );
#ifdef RDO_MT
		notifies_mutex.Unlock();
		CEvent* thread_destroy = thread_runtime->thread_destroy;
#endif

		sendMessage( thread_runtime, RDOThread::RT_THREAD_CLOSE );

#ifdef RDO_MT
		thread_destroy->Lock();
		delete thread_destroy;
#endif
		thread_runtime = NULL;

		// ����� ������ ����������� �� ��������� run-time-�����, ����� ���������� ���������� ���������� ������ (��� �� run-time-error)
#ifdef RDO_MT
		notifies_mutex.Lock();
#endif
		notifies.push_back( RT_THREAD_STOP_AFTER );
#ifdef RDO_MT
		notifies_mutex.Unlock();
#endif
	}
}

void RDOThreadSimulator::closeModel()
{
	if ( runtime ) {
		delete runtime;
		runtime = NULL;
	}
	if ( parser ) {
		delete parser;
		parser = NULL;
	}
}

void RDOThreadSimulator::parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info )
{
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	try {
		parser->parse( rdoModelObjects::obPRE, smr );
	}
	catch( rdoParse::RDOSyntaxException& ) {
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
		closeModel();
		info.error = true;
		return;
	}
	catch( rdoParse::RDOInternalException& ex ) {
		std::string mess = "Internal exception: " + ex.mess;
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_STRING, &mess );
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
		closeModel();
		info.error = true;
		return;
	}

	if ( !parser->smr ) {
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_STRING, &std::string("SMR File seems to be empty\n") );
		broadcastMessage( RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR );
		closeModel();
		info.error = true;
		return;
	}

//	kernel.notifyString(RDOKernel::buildString, "SMR File read successfully\n");

	if(parser->smr->modelName)
		info.model_name = *parser->smr->modelName;

	if(parser->smr->resourceFileName)
		info.resource_file = *parser->smr->resourceFileName;

	if(parser->smr->oprIevFileName)
		info.oprIev_file = *parser->smr->oprIevFileName;

	if(parser->smr->frameFileName)
		info.frame_file = *parser->smr->frameFileName;

	if(parser->smr->statisticFileName)
		info.statistic_file = *parser->smr->statisticFileName;

	if(parser->smr->resultsFileName)
		info.results_file = *parser->smr->resultsFileName;

	if(parser->smr->traceFileName)
		info.trace_file = *parser->smr->traceFileName;

	closeModel();
	info.error = false;
}

std::vector< RDOSyntaxError >* RDOThreadSimulator::getErrors()
{
	std::vector< RDOSyntaxError >* res = NULL;

	if ( !parser ) return NULL;

	res = &parser->errors;
	if ( res->size() > 0 ) {
		return res;
	}

	res = &runtime->errors;
	return res;
}

ShowMode RDOThreadSimulator::getInitialShowMode()
{
	return parser->smr->showMode;
}

int RDOThreadSimulator::getInitialFrameNumber()
{
	return parser->smr->frameNumber;
}

double RDOThreadSimulator::getInitialShowRate()
{
	return *parser->smr->showRate;
}

std::stringstream &RDOThreadSimulator::getModelStructure()
{
	return parser->getModelStructure();
}

std::stringstream &RDOThreadSimulator::getResults()
{
	return resultString;
}

} // namespace rdoSimulator
