#pragma warning(disable : 4786)  

#include "..\\memcheck.h"
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

#include "rdosimwin.h"
#include "rdokernel.h"
#include "rdorepository.h"

#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdosmr.h"
#include "rdofrm.h"

namespace RDOSimulatorNS
{

class RDORuntimeTracer: public RDOTrace, public RDOEndL
{
	LPVOID pParam;
  	TracerCallBack tracerCallBack;
	stringstream stream;
public:
   ostream &getOStream() { return stream; }
   RDOEndL& getEOL() { return *this; }
	void onEndl()
	{
		tracerCallBack(&stream.str(), pParam);
		stream.str("");
	}
	RDORuntimeTracer(TracerCallBack _tracerCallBack, LPVOID _pParam):
		tracerCallBack(_tracerCallBack), pParam(_pParam) { isNullTracer = false; }
};


const vector<RDOFrame *>& RdoSimulator::getFrames()
{
	return frames;
}

void RdoSimulator::addKeyPressed(int scanCode)
{
	scanCodes.push_back(scanCode);
}

void RdoSimulator::addAreaPressed(string& areaName)
{
	areasActivated.push_back(areaName);
}

void frameCallBack(rdoRuntime::RDOConfig *config, void *param)
{
	RdoSimulator *simulator = (RdoSimulator *)param;
	if((config->showAnimation == SM_Animation) && (config->showAnimation == SM_Animation))
	{
		Sleep(config->realTimeDelay);

		simulator->frames = config->frames;

		kernel.notify(RDOKernel::showFrame);

		config->keysPressed.insert(config->keysPressed.end(), simulator->scanCodes.begin(), simulator->scanCodes.end());
		simulator->scanCodes.clear();

		config->activeAreasMouseClicked.insert(config->activeAreasMouseClicked.end(), simulator->areasActivated.begin(), simulator->areasActivated.end());
		simulator->areasActivated.clear();

		for(int i = 0; i < config->frames.size(); i++)
			delete config->frames.at(i);
		config->frames.clear();
	}
	else
	{
		simulator->frames.clear();
		kernel.notify(RDOKernel::showFrame);
	}

	config->showRate = simulator->getShowRate();
	config->showAnimation = simulator->getShowMode();
}

void tracerCallBack(string *newString, void *param)
{
	RdoSimulator *simulator = (RdoSimulator *)param;
//	kernel.notifyString(RDOKernel.traceString, newString->c_str());
	int pos = 0;
	for(;;)
	{
		int next = newString->find('\n', pos);
		string str = newString->substr(pos, next-pos);
		kernel.notifyString(RDOKernel.traceString, str.c_str());
		if(next == string::npos)
			break;
		pos = next + 1;
		if(pos >= newString->length())
			break;

		int aa = 0;
	}
}

RdoSimulator::RdoSimulator(): 
	runtime(NULL), 
	parser(NULL), 
	th(NULL)
{}

RdoSimulator::~RdoSimulator()
{
	terminateModel();
	closeModel();
}

class RDOSimResulter: public RDOResult
{
	ostream &stream;
public:
	RDOSimResulter(ostream &_stream): stream(_stream) {isNullResult = false;}
   virtual std::ostream &getOStream() { return stream; }
};

UINT RunningThreadControllingFunction( LPVOID pParam )
{
	RdoSimulator *simulator = (RdoSimulator *)pParam;

	RDOTrace *tracer;
	rdoRuntime::RDOResult *resulter;

/////////////////   Creating tracer and resulter //////////////////////////////////
		if(simulator->parser->smr->traceFileName == NULL) 
			tracer = new RDOTrace();
		else
			tracer = new RDORuntimeTracer(tracerCallBack, pParam);
//			tracer = new RDOTrace(*simulator->parser->smr->traceFileName + ".trc");
//			tracer = new RDOTrace("trace.trc");

		resulter;
		if(simulator->parser->smr->resultsFileName == NULL) 
			resulter = new rdoRuntime::RDOResult();
		else
//			resulter = new rdoRuntime::RDOResult((*simulator->parser->smr->resultsFileName + ".pmv").c_str());
			resulter = new RDOSimResulter(simulator->resultString);


/////////  RDO config initialization ////////////////////////
		simulator->runtime->config.showAnimation = simulator->parser->smr->showMode;
		int size = simulator->runtime->allFrames.size();
		for(int i = 0; i < size; i++)
			simulator->runtime->config.allFrameNames.push_back(*simulator->runtime->allFrames.at(i)->getName());

		simulator->runtime->config.currFrameToShow = simulator->parser->smr->frameNumber - 1;
		simulator->runtime->config.keysPressed.clear();
		simulator->runtime->config.mouseClicked = false;
		simulator->runtime->config.activeAreasMouseClicked.clear();
		simulator->runtime->config.frames.clear();
		simulator->runtime->config.currTime = 0;
		simulator->runtime->config.newTime = 0;
		if(simulator->parser->smr->showRate)
			simulator->runtime->config.showRate = *simulator->parser->smr->showRate;
		else
			simulator->runtime->config.showRate = 60;	// default
		simulator->runtime->config.realTimeDelay = 0;




////////////////////////////////////////////////////////////////
/////////////////   Modelling	//////////////////////////////////
////////////////////////////////////////////////////////////////


	simulator->runtime->tracerCallBack = tracerCallBack;
	simulator->runtime->param = pParam;
	simulator->runtime->frameCallBack = frameCallBack;

	try
	{
		simulator->runtime->rdoInit(tracer, resulter);
		simulator->runtime->rdoRun();
		simulator->runtime->rdoDestroy();
		kernel.notify(RDOKernel::endExecuteModel);
//		kernel.debug("End executing\n");
	}
	catch(RDOSyntaxException &) 
	{
		kernel.notify(RDOKernel::executeError);
	}
	catch(RDOInternalException &ex)		  
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.debug(mess.c_str());
		kernel.notify(RDOKernel::executeError);
	}

	try
	{
		simulator->runtime->rdoDestroy();
	}
	catch(RDOSyntaxException &) 
	{
		kernel.notify(RDOKernel::executeError);
	}
	catch(RDOInternalException &ex)		  
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.debug(mess.c_str());
		kernel.notify(RDOKernel::executeError);
	}

	simulator->th = NULL;

	return 0;
}

bool RdoSimulator::parseModel()
{
//	kernel.notifyString(RDOKernel::buildString, "Start parsing\n");
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	ostringstream consol;

	try {
/////////////////   SMR file //////////////////////////////////
		rdo::binarystream SMRstream;
		kernel.getRepository()->loadSMR(SMRstream);
		if(SMRstream.good())
			parser->parseSMR1(&SMRstream, &consol);

/////////////////   RTP file //////////////////////////////////
		rdo::binarystream RTPstream1;
		kernel.getRepository()->loadRTP(RTPstream1);
		if(RTPstream1.good())
			parser->parseRTP(&RTPstream1, &consol);

/////////////////   RSS file //////////////////////////////////
		rdo::binarystream RSSstream;
		kernel.getRepository()->loadRSS(RSSstream);
		if(RSSstream.good())
			parser->parseRSS(&RSSstream, &consol);

/////////////////   FUN file //////////////////////////////////
		rdo::binarystream FUNstream;
		kernel.getRepository()->loadFUN(FUNstream);
		if(FUNstream.good())
			parser->parseFUN(&FUNstream, &consol);

/////////////////   PAT file //////////////////////////////////
		rdo::binarystream PATstream;
		kernel.getRepository()->loadPAT(PATstream);
		if(PATstream.good())
			parser->parsePAT(&PATstream, &consol);

/////////////////   OPR file //////////////////////////////////
		rdo::binarystream OPRstream;
		kernel.getRepository()->loadOPR(OPRstream);
		if(OPRstream.good())
			parser->parseOPR(&OPRstream, &consol);

/////////////////   DPT file //////////////////////////////////
		rdo::binarystream DPTstream;
		kernel.getRepository()->loadDPT(DPTstream);
		if(DPTstream.good())
			parser->parseDPT(&DPTstream, &consol);

/////////////////   PMD file //////////////////////////////////
		rdo::binarystream PMDstream;
		kernel.getRepository()->loadPMD(PMDstream);
		if(PMDstream.good())
			parser->parsePMD(&PMDstream, &consol);

/////////////////   FRM file //////////////////////////////////
		rdo::binarystream FRMstream;
		kernel.getRepository()->loadFRM(FRMstream);
		if(FRMstream.good())
			parser->parseFRM(&FRMstream, &consol);

/////////////////   SMR file //////////////////////////////////
		rdo::binarystream SMRstream2;
		kernel.getRepository()->loadSMR(SMRstream2);
		if(SMRstream2.good())
			parser->parseSMR2(&SMRstream2, &consol);
	}
	catch(RDOSyntaxException &) 
	{
//		string mess = ex.getType() + " : " + ex.mess;
//		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseError);
		closeModel();
		return false;
	}
	catch(RDOInternalException &ex)
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseError);
		closeModel();
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
	
	kernel.notify(RDOKernel::parseSuccess);
//	kernel.notifyString(RDOKernel::buildString, "End parsing\n");


//	kernel.notifyString(RDOKernel::buildString, getModelStructure().str().c_str());

	return true;
}

void RdoSimulator::runModel()
{
	bool res = parseModel();
	if(res)
	{
		kernel.notify(RDOKernel::beforeModelStart);
//		kernel.debug("Start executing\n");
		th = AfxBeginThread(RunningThreadControllingFunction, (LPVOID)this);
		kernel.notify(RDOKernel::afterModelStart);
	}
}

void RdoSimulator::stopModel()
{
	terminateModel();
	kernel.notify(RDOKernel::modelStopped);
}

void RdoSimulator::terminateModel()
{
	if(th != NULL)
	{
		TerminateThread(th->m_hThread, -1);
		delete th;
		th = NULL;
	}
}

void RdoSimulator::closeModel()
{
	if(runtime)
		delete runtime;
	runtime = NULL;

	if(parser)
		delete parser;
	parser = NULL;
}

void RdoSimulator::parseSMRFileInfo( rdo::binarystream& smr, rdoModelObjects::RDOSMRFileInfo& info )
{
	terminateModel();
	closeModel();

	parser = new rdoParse::RDOParser();
	runtime = parser->runTime;

	stringstream consol;

	try {
		parser->parseSMR1(&smr, &consol);
	}
	catch(RDOSyntaxException &) 
	{
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
		return;
	}
	catch(RDOInternalException &ex)
	{
		string mess = "Internal exception: " + ex.mess;
		kernel.notifyString(RDOKernel::buildString, mess);
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
		return;
	}

	if(!parser->smr)
	{
		kernel.notifyString(RDOKernel::buildString, "SMR File seems to be empty\n");
		kernel.notify(RDOKernel::parseSMRError);
		closeModel();
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
}

vector<RDOSyntaxError>* RdoSimulator::getErrors()
{
	vector<RDOSyntaxError>* res = NULL;

	if(!parser)
		return NULL;

	res = &parser->errors;
	if(res->size() > 0)
		return res;

	res = &runtime->errors;
	return res;
}

double RdoSimulator::getModelTime()
{
	if(runtime)
		return runtime->getTimeNow();
	else
		return 0.;
}

vector<const string *> RdoSimulator::getAllFrames()
{
	vector<const string *> vect;
	if(!runtime)
		return vect;

	int size = runtime->allFrames.size();
	for(int i = 0; i < size; i++)
	{
		RDOFRMFrame *frame = runtime->allFrames.at(i);
		vect.push_back(frame->getName());
	}

	return vect;
}

vector<const string *> RdoSimulator::getAllBitmaps()
{
	vector<const string *> vect;
	if(!runtime)
		return vect;

	int size = runtime->allFrames.size();
	for(int i = 0; i < size; i++)
	{
		RDOFRMFrame *frame = runtime->allFrames.at(i);
		frame->getAllBitmaps(vect);
	}

	return vect;
}

ShowMode RdoSimulator::getInitialShowMode()
{
	return parser->smr->showMode;
}

int RdoSimulator::getInitialFrameNumber()
{
	return parser->smr->frameNumber;
}

double RdoSimulator::getInitialShowRate()
{
	return *parser->smr->showRate;
}

stringstream &RdoSimulator::getModelStructure()
{
	return parser->modelStructure;
}

stringstream &RdoSimulator::getResults()
{
	return resultString;
}


}// namespace RDOSimulatorNS
