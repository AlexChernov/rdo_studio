#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdodpt.h"
#include "rdoparser.h"
#include "rdopatrtime.h"
#include "rdoparser_lexer.h"
#include "rdorss.h"

#include <rdoprocess.h>

namespace rdoParse 
{

int dptlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void dpterror( char* mes )
{
}

int dpt_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void dpt_rss_error( char* mes )
{
}

int dpt_opr_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void dpt_opr_error( char* mes )
{
}

/////////////////////////  "SEARCH" DECISION POINT /////////////////////////

RDODPTSearch::RDODPTSearch( std::string* _name, DPTSearchTrace _trace ):
	name( _name ),
	trace( _trace ),
	lastActivity( NULL )
{
	if(std::find_if(parser->getDPTSome().begin(), parser->getDPTSome().end(), compareName<RDODPTSome>(_name)) != parser->getDPTSome().end())
		parser->error("DPT name: " + *_name + " already defined");

	if(std::find_if(parser->getDPTSearch().begin(), parser->getDPTSearch().end(), compareName<RDODPTSearch>(_name)) != parser->getDPTSearch().end())
		parser->error("DPT name: " + *_name + " already defined");

	parser->insertDPTSearch( this );
}

void RDODPTSearch::addNewActivity( std::string* _name, std::string* _ruleName )
{
	parser->checkActivityName( _name );
	lastActivity = new RDODPTSearchActivity( _name, _ruleName );
	activities.push_back( lastActivity );
}

void RDODPTSearch::setActivityValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost)
{
	lastActivity->setValue(_value, _ruleCost);
}

void RDODPTSearch::end()
{
	rdoRuntime::RDOCalc *condCalc;
	if(!conditon)
		condCalc = new rdoRuntime::RDOCalcConst(1);
	else
		condCalc = conditon->calc;

	rdoRuntime::RDOSearchRuntime *dpt = new rdoRuntime::RDOSearchRuntime(parser->runTime, 
		condCalc, 
		termConditon->calc, 
		evalBy->createCalc(), 
		compTops);

	switch(trace)
	{
	case DPTnotrace:
		dpt->traceFlag = DPT_no_trace;
		break;
	case DPTtracestat:
		dpt->traceFlag = DPT_trace_stat;
		break;
	case DPTtracetops:
		dpt->traceFlag = DPT_trace_tops;
		break;
	case DPTtraceall:
		dpt->traceFlag = DPT_trace_all;
		break;
	};

	parser->runTime->addDPT(dpt);

	int size = activities.size();
	for(int i = 0; i < size; i++)
	{
		RDODPTSearchActivity *curr = activities.at(i);
		rdoRuntime::RDOSearchActivityRuntime *act = curr->createActivityRuntime(parser->runTime);
		dpt->addActivity(act);
	}
}

RDODPTSearchActivity::RDODPTSearchActivity(std::string *_name, std::string *_ruleName)
	: name(_name) 
{
	const RDOPATPattern *pat = parser->findPattern(_ruleName);
	pat->testGoodForSearchActivity();
	rule = (RDOPATPatternRule *)pat;
	currParamNum = 0;
}

void RDODPTSearchActivity::setValue(DPTSearchValue _value, RDOFUNArithm *_ruleCost)
{
	if(currParamNum != rule->params.size())
		parser->error("Too few parameters for rule: " + *rule->getName());

	value = _value;
	ruleCost = _ruleCost;
}

void RDODPTSearchActivity::addParam(int _param) 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam(double *_param) 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam(std::string *_param) 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTSearchActivity::addParam() 
{
	if(currParamNum >= rule->params.size())
		parser->error("Too many parameters for rule: " + *rule->getName());

	RDOFUNFunctionParam *param = rule->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

rdoRuntime::RDOSearchActivityRuntime *RDODPTSearchActivity::createActivityRuntime( rdoRuntime::RDORuntime* sim )
{
	rdoRuntime::RDOActivityRuleRuntime *activity = new rdoRuntime::RDOActivityRuleRuntime(sim, rule->patRuntime, true, name);
	rdoRuntime::RDOSearchActivityRuntime *act = new rdoRuntime::RDOSearchActivityRuntime(sim, 
		dynamic_cast<RDORule *>(activity),
		(value == DPT_value_after), 
		ruleCost->createCalc());

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(i, params.at(i)));

	return act;
}

/////////////////////////  "SOME" DECISION POINT //////////////////////

RDODPTSome::RDODPTSome( std::string* _name ):
	name( _name ),
	lastActivity( NULL )
{
	if(std::find_if(parser->getDPTSome().begin(), parser->getDPTSome().end(), compareName<RDODPTSome>(_name)) != parser->getDPTSome().end())
		parser->error("DPT name: " + *_name + " already defined");

	if(std::find_if(parser->getDPTSearch().begin(), parser->getDPTSearch().end(), compareName<RDODPTSearch>(_name)) != parser->getDPTSearch().end())
		parser->error("DPT name: " + *_name + " already defined");

	parser->insertDPTSome( this );
}

void RDODPTSome::addNewActivity( std::string* _name, std::string* _patternName )
{
	parser->checkActivityName( _name );
	lastActivity = new RDODPTSomeActivity( _name, _patternName );
	activities.push_back( lastActivity );
}

void RDODPTSome::end()
{
	parser->runTime->dptCounter++;
	int size = activities.size();
	for(int i = 0; i < size; i++)
		activities.at(i)->createActivityRuntime(conditon);
}

RDODPTSomeActivity::RDODPTSomeActivity(std::string *_name, std::string *_patternName)
	: name(_name) 
{
	pattern = parser->findPattern(_patternName);
	pattern->testGoodForSomeActivity();
	currParamNum = 0;
}

void RDODPTSomeActivity::addParam(int _param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam(double *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam(std::string *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTSomeActivity::addParam() 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

void RDODPTSomeActivity::createActivityRuntime(RDOFUNLogic *conditon)
{
	rdoRuntime::RDOActivityRuntime *activity;
	if(!conditon)
		activity = pattern->patRuntime->createActivity(name);
	else
		activity = pattern->patRuntime->createActivity(conditon->calc, name);

	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(i, params.at(i)));
}


/////////////////////////  FREE ACTIVITIES /////////////////////////

RDODPTFreeActivity::RDODPTFreeActivity( std::string* _name, std::string* _patternName ):
	name( _name )
{
	parser->checkActivityName( _name );
	parser->insertDPTFreeActivity( this );

	pattern = parser->findPattern(_patternName);
	if ( !pattern ) {
		parser->error( rdo::format("�� ������ �������: %s", _patternName->c_str()) );
	}
	pattern->testGoodForFreeActivity();
	currParamNum = 0;
}

void RDODPTFreeActivity::addParam(int _param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam(double *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam(std::string *_param) 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue(_param);
	params.push_back(val);
}

void RDODPTFreeActivity::addParam() 
{
	if(currParamNum >= pattern->params.size())
		parser->error("Too many parameters for pattern: " + *pattern->getName());

	RDOFUNFunctionParam *param = pattern->params[currParamNum++];
	rdoRuntime::RDOValue val = param->getType()->getRSSDefaultValue();
	params.push_back(val);
}

void RDODPTFreeActivity::end()
{
	rdoRuntime::RDOActivityRuntime *activity = pattern->patRuntime->createActivity(name);
	int size = params.size();
	for(int i = 0; i < size; i++)
		activity->addParamCalc(new rdoRuntime::RDOSetPatternParamCalc(i, params.at(i)));

	size = hotKeys.size();
	for(i = 0; i < size; i++)
		activity->addHotKey(hotKeys.at(i));
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
std::string RDOPROCProcess::name_prefix = "";
std::string RDOPROCProcess::name_sufix  = "s";

RDOPROCProcess::RDOPROCProcess( const std::string& _name ):
	RDODeletable(),
	name( _name ),
	m_end( false ),
	parent( NULL ),
	runtime( NULL )
{
	parser->insertDPTProcess( this );
	runtime = new rdoRuntime::RDOPROCProcess( name, parser->runTime );
}

void RDOPROCProcess::end()
{
	m_end = true;
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		child.push_back( value );
		value->parent = this;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
bool RDOPROCTransact::created = false;

RDOPROCTransact::RDOPROCTransact():
	RDORTPResType( parser->registerName( "���������" ), false )
{
	// �������� �������� ������������� ���� '�����_��������'
	addParam( new RDORTPParamDesc( parser->registerName( "�����_��������" ), new RDORTPRealResParam() ) );
	// �������� �������� ������ ���� '������_���'
	addParam( new RDORTPParamDesc( parser->registerName( "������_���" ), new RDORTPIntResParam() ) );
	// ������ ���� ������ ��������� �� ����
	RDOPROCTransact::created = true;
}

RDOPROCTransact::~RDOPROCTransact()
{
	RDOPROCTransact::created = false;
}

RDOPROCTransact* RDOPROCTransact::makeRTP()
{
	if ( RDOPROCTransact::created ) {
		RDOPROCTransact* rtp = static_cast<RDOPROCTransact*>(const_cast<RDORTPResType*>(parser->findRTPResType( "���������" )));
		return rtp;
	} else {
		RDOPROCTransact* rtp = new RDOPROCTransact();
		return rtp;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
RDOPROCOperator::RDOPROCOperator( const std::string& _name, RDOPROCProcess* _process ):
	RDODeletable(),
	name( _name ),
	process( _process )
{
	if ( !process ) process = parser->getLastDPTProcess();
	process->operations.push_back( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate( const std::string& _name, rdoRuntime::RDOCalc* time, RDOPROCProcess* _process ):
	RDOPROCOperator( _name, _process ),
	runtime( NULL )
{
	runtime = new rdoRuntime::RDOPROCGenerate( parser->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize( const std::string& _name, const std::string* res_name, RDOPROCProcess* _process ):
	RDOPROCOperator( _name, _process )
{
	const RDORSSResource* rss = parser->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCSeize( parser->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		// error: �� ����� parser-������
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease( const std::string& _name, const std::string* res_name, RDOPROCProcess* _process ):
	RDOPROCOperator( _name, _process )
{
	const RDORSSResource* rss = parser->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCRelease( parser->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		// error: �� ����� parser-������
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance( const std::string& _name, rdoRuntime::RDOCalc* time, RDOPROCProcess* _process ):
	RDOPROCOperator( _name, _process )
{
	runtime = new rdoRuntime::RDOPROCAdvance( parser->getLastDPTProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate( const std::string& _name, RDOPROCProcess* _process ):
	RDOPROCOperator( _name, _process )
{
	runtime = new rdoRuntime::RDOPROCTerminate( parser->getLastDPTProcess()->getRunTime() );
}

} // namespace rdoParse
