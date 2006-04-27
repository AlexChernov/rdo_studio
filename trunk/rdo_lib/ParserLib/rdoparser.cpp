#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdoparser_lexer.h"
#include "rdoparselex.h"
#include "rdoparser.h"
#include "rdoruntime.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdopat.h"
#include "rdodpt.h"
#include "rdosmr.h"

//#include "stdHeaders.h"

namespace rdoParse 
{

void setYylval(int val)
{
	currParser->setYylval((int)val);
}

double *addDouble(double *val)
{
	currParser->allRTPDoubles.push_back(val);
	return val;
}

void addName(string *name)
{
	rdoParse::currParser->allRTPNames.push_back(name); 
}

void rtperror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int rtplex( int* lpval, void* lexer )
{
	((rdoFlexLexer*)lexer)->m_lpval = lpval;
//	return ((rdoFlexLexer*)lexer)->yylex();
//	return rdoParse::currParser->lex( value );
//	rdoParse::rtplval = lpval;
	return rdoParse::currParser->lex();
}
void rsserror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int rsslex()
{
	return rdoParse::currParser->lex();
}
void funerror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int funlex(void)
{
	return rdoParse::currParser->lex();
}
void paterror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int patlex(void)
{
	return rdoParse::currParser->lex();
}
void oprerror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int oprlex(void)
{
	return rdoParse::currParser->lex();
}
void dpterror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int dptlex(void)
{
	return rdoParse::currParser->lex();
}
void pmderror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int frmlex(void)
{
	return rdoParse::currParser->lex();
}
void frmerror( char *mes )
{
	rdoParse::currParser->error(mes);
}
int pmdlex(void)
{
	return rdoParse::currParser->lex();
}
void smr1error( char *mes )
{
//	rdoParse::currParser->error(mes);
	throw RDOSMR1OkException("");
}
int smr1lex(void)
{
	return rdoParse::currParser->lex();
}
void smr2error( char *mes )
{
	rdoParse::currParser->error(mes);
}
int smr2lex(void)
{
	return rdoParse::currParser->lex();
}

stringstream& RDOParser::getModelStructure()
{
	modelStructure.str() = "";
	currParser = this;
	if ( modelStructure.str().empty() ) {
		// RTP
		modelStructure << endl << "$Resource_type" << endl;
		for_each(allRTPResType.begin(), allRTPResType.end(), mem_fun(&RDORTPResType::writeModelStructure));

		// RSS
		modelStructure << endl << "$Resources" << endl;
		for_each(allRSSResource.begin(), allRSSResource.end(), mem_fun(&RDORSSResource::writeModelStructure));

		// PAT
		modelStructure << endl << "$Pattern" << endl;
		for_each(allPATPatterns.begin(), allPATPatterns.end(), mem_fun(&RDOPATPattern::writeModelStructure));

		// OPR
		modelStructure << endl << "$Activities" << endl;
		modelStructure << runTime->writeActivitiesStructure();

		// DPT
		modelStructure << endl << "$Activities" << endl;
		modelStructure << runTime->writeActivitiesStructure();
		for( int i = 0; i < allDPTSearch.size(); i++ ) {
			int counter = 1;
			for( int j = 0; j < allDPTSearch.at(i)->getActivities().size(); j++ ) {
				RDODPTSearchActivity *curr = allDPTSearch.at(i)->getActivities().at(j);
				modelStructure << counter++ << " " << *curr->getName() << " " << curr->getRule()->getPatternId() << endl;
			}
		}

		// PDM
		modelStructure << endl << "$Watching" << endl;
		modelStructure << runTime->writePokazStructure();

	}
	currParser = NULL;
	return modelStructure;
}

RDOParser *currParser;
rdoParserBase* _parser = NULL;

void RDOParser::parseRTP(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	_parser = &parsers[0];

	resourceTypeCounter = 1;
	fileToParse = RTP_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0002");

	currParser = this;
	parsers[0].parse();
	currParser = NULL;

/*
	resourceTypeCounter = 1;
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = RTP_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0002");

	currParser = this;
	rtpparse(); 
	modelStructure << endl << "$Resource_type" << endl;
	for_each(allRTPResType.begin(), allRTPResType.end(), mem_fun(&RDORTPResType::writeModelStructure));
	currParser = NULL;
*/
}

void RDOParser::parseRSS(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	resourceCounter = 0;
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = RSS_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0002");

	currParser = this;
	rssparse(); 
//	modelStructure << endl << "$Resources" << endl;
//	for_each(allRSSResource.begin(), allRSSResource.end(), mem_fun(&RDORSSResource::writeModelStructure));
	currParser = NULL;
}

void RDOParser::parseFUN(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = FUN_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0002");

	currParser = this;
	LoadStdFunctions();
	funparse(); 
	currParser = NULL;
}

void RDOParser::parsePAT(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = PAT_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0002");

	currParser = this;
	patparse(); 
//	modelStructure << endl << "$Pattern" << endl;
//	for_each(allPATPatterns.begin(), allPATPatterns.end(), mem_fun(&RDOPATPattern::writeModelStructure));
	currParser = NULL;
}

void RDOParser::parseOPR(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = OPR_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0030");

	currParser = this;
	oprparse(); 
//	modelStructure << endl << "$Activities" << endl;
//	modelStructure << runTime->writeActivitiesStructure();
	currParser = NULL;
}

void RDOParser::parseDPT(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = DPT_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0030");

	currParser = this;
	dptparse();
/*
	modelStructure << endl << "$Activities" << endl;
	modelStructure << runTime->writeActivitiesStructure();
	for(int i = 0; i < allDPTSearch.size(); i++)
	{
		int counter = 1;
		for(int j = 0; j < allDPTSearch.at(i)->getActivities().size(); j++)
		{
			RDODPTSearchActivity *curr = allDPTSearch.at(i)->getActivities().at(j);
			modelStructure << counter++ << " " << *curr->getName() << " " << 
				curr->getRule()->getPatternId() << endl;
		}
	}
*/
	currParser = NULL;
}

void RDOParser::parsePMD(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = PMD_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0031");

	currParser = this;
	pmdparse(); 
//	modelStructure << endl << "$Watching" << endl;
//	modelStructure << runTime->writePokazStructure();
	currParser = NULL;
}

void RDOParser::parseFRM(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = FRM_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0032");

	currParser = this;
	frmparse(); 
	currParser = NULL;
}

void RDOParser::parseSMR1(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	*arg_yyout << " ";
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = SMR1_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0032");

	currParser = this;
	try
	{
		smr1parse(); 
	}
	catch(RDOSMR1OkException &)
	{
		// Everithing ok, just end of first part parsing
	}

	if(currParser->smr && !currParser->smr->showRate)	// was not initialised
	{
		currParser->smr->showRate = addDouble(new double(60.));
	}

	currParser = NULL;
}

void RDOParser::parseSMR2(std::istream* arg_yyin, std::ostream* arg_yyout) 
{
	rdoLexer lexer(arg_yyin, arg_yyout);
	fileToParse = SMR2_FILE;
	out = arg_yyout;
	if(currParser != NULL)
		throw RDOSyntaxException("Internal error 0033");

	currParser = this;
	smr2parse(); 
	currParser = NULL;
}

void RDOParser::error( const char *mes ) 
{
	rdoModelObjects::RDOFileType ft = rdoModelObjects::PAT;
	switch(fileToParse)
	{
	case RTP_FILE: ft = rdoModelObjects::RTP; break;
	case RSS_FILE: ft = rdoModelObjects::RSS; break;
	case FUN_FILE: ft = rdoModelObjects::FUN; break;
	case PAT_FILE: ft = rdoModelObjects::PAT; break;
	case OPR_FILE: ft = rdoModelObjects::OPR; break;
	case DPT_FILE: ft = rdoModelObjects::DPT; break;
	case PMD_FILE: ft = rdoModelObjects::PMD; break;
	case SMR1_FILE:
	case SMR2_FILE: ft = rdoModelObjects::SMR; break;
	case FRM_FILE: ft = rdoModelObjects::FRM; break;
	}
	errors.push_back(RDOSimulatorNS::RDOSyntaxError(mes, rdoLineno(), ft));
	throw rdoParse::RDOSyntaxException("");
}

int RDOParser::lex()
{
	return rdoYylex();
}

RDOParser::RDOParser():
	out(&std::cout)
{
	runTime = new RDORuntime();
	patternCounter = 1;
	pokazCounter = 1;
	constCounter = 0;
	smr = NULL;
	lastDPTSearch = NULL;
	lastRTPResType = NULL;
	lastRSSResource = NULL;
	lastFUNFunction = NULL;
	lastPATPattern = NULL;

	rdoParserBase rtp( rdoModelObjects::RTP, rtpparse, rtperror, rtplex );
	parsers[0] = rtp;
}

void RDOParser::setYylval(int val)
{
	switch(fileToParse)
	{
	case RTP_FILE:
		*_parser->m_lexer->m_lpval = val;
//		*rtplval = val;
		break;
	case RSS_FILE:
		rsslval = val;
		break;
	case FUN_FILE:
		funlval = val;
		break;
	case PAT_FILE:
		patlval = val;
		break;
	case OPR_FILE:
		oprlval = val;
		break;
	case DPT_FILE:
		dptlval = val;
		break;
	case PMD_FILE:
		pmdlval = val;
		break;
	case FRM_FILE:
		frmlval = val;
		break;
	case SMR1_FILE:
		smr1lval = val;
		break;
	case SMR2_FILE:
		smr2lval = val;
		break;
	default:
		error("Internal error 0003");
	};
}

const RDORTPResType *RDOParser::findRTPResType(const string *const type) const
{
	vector<RDORTPResType *>::const_iterator it = find_if(allRTPResType.begin(), 
		allRTPResType.end(), 
		compareName<RDORTPResType>(type));
	if(it != allRTPResType.end())
		return (*it);

	return NULL;
}

const RDORSSResource *RDOParser::findRSSResource(const string *const name) const
{
	vector<RDORSSResource *>::const_iterator it = find_if(allRSSResource.begin(), 
		allRSSResource.end(), 
		compareName<RDORSSResource>(name));
	if(it != allRSSResource.end())
		return (*it);

	return NULL;
}

const RDOFUNFunction *RDOParser::findFunction(const string *const name) const
{
	vector<RDOFUNFunction *>::const_iterator it = find_if(allFUNFunctions.begin(), 
		allFUNFunctions.end(), 
		compareName<RDOFUNFunction>(name));
	if(it != allFUNFunctions.end())
		return (*it);

	return NULL;
}

const RDOFUNSequence *RDOParser::findSequence(const string *const name) const
{
	vector<RDOFUNSequence *>::const_iterator it = find_if(allFUNSequences.begin(), 
		allFUNSequences.end(), 
		compareName<RDOFUNSequence>(name));
	if(it != allFUNSequences.end())
		return (*it);

	return NULL;
}

const RDOPATPattern *RDOParser::findPattern(const string *const name) const
{
	vector<RDOPATPattern *>::const_iterator it = find_if(allPATPatterns.begin(), 
		allPATPatterns.end(), 
		compareName<RDOPATPattern>(name));
	if(it != allPATPatterns.end())
		return (*it);

	return NULL;
}

RDOParser::~RDOParser()
{
	currParser = this;
	DeleteAllObjects(allRTPNames);
	DeleteAllObjects(allRTPDoubles);
	DeleteAllObjects(allDeletables);
	currParser = NULL;
}

void addCalcToRuntime(RDOCalc *calc) 
{	
	currParser->runTime->allCalcs.push_back(calc); 
}

void RDOParser::LoadStdFunctions()
{
	RDORTPIntResParam *intType = new RDORTPIntResParam(new RDORTPIntDiap(), new RDORTPIntDefVal(false));
	RDORTPRealResParam *realType = new RDORTPRealResParam(new RDORTPRealDiap(), new RDORTPRealDefVal(false));

	RDOFUNFunction *fun = new RDOFUNFunction(registerName("Abs"), realType);
	allFUNFunctions.push_back(fun);
	RDOFUNFunctionParam *param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcAbs();

	fun = new RDOFUNFunction(registerName("ArcCos"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcArcCos();

	fun = new RDOFUNFunction(registerName("ArcSin"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcArcSin();

	fun = new RDOFUNFunction(registerName("ArcTan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcArcTan();

	fun = new RDOFUNFunction(registerName("Cos"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcCos();

	fun = new RDOFUNFunction(registerName("Cotan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcCotan();

	fun = new RDOFUNFunction(registerName("Exp"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcExp();

	fun = new RDOFUNFunction(registerName("Floor"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcFloor();

	fun = new RDOFUNFunction(registerName("Frac"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcFrac();

	fun = new RDOFUNFunction(registerName("IAbs"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcIAbs();

	fun = new RDOFUNFunction(registerName("IMax"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcIMax();

	fun = new RDOFUNFunction(registerName("IMin"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcIMin();

	fun = new RDOFUNFunction(registerName("Int"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcInt();

	fun = new RDOFUNFunction(registerName("IntPower"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), intType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcIntPower();

	fun = new RDOFUNFunction(registerName("Ln"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcLn();

	fun = new RDOFUNFunction(registerName("Log10"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcLog10();

	fun = new RDOFUNFunction(registerName("Log2"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcLog2();

	fun = new RDOFUNFunction(registerName("LogN"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcLogN();

	fun = new RDOFUNFunction(registerName("Max"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcMax();

	fun = new RDOFUNFunction(registerName("Min"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcMin();

	fun = new RDOFUNFunction(registerName("Power"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(registerName("p2"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcPower();

	fun = new RDOFUNFunction(registerName("Round"), intType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcRound();

	fun = new RDOFUNFunction(registerName("Sin"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcSin();

	fun = new RDOFUNFunction(registerName("Sqrt"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcSqrt();

	fun = new RDOFUNFunction(registerName("Tan"), realType);
	allFUNFunctions.push_back(fun);
	param = new RDOFUNFunctionParam(registerName("p1"), realType);
	fun->add(param);
	fun->functionCalc = new RDOFunCalcTan();
}

}		// namespace rdoParse 
