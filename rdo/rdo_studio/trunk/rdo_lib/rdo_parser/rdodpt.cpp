#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdoopr.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_ie.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptfree.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptsome.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptprior.h"
#include "rdo_lib/rdo_runtime/rdodptrtime.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"

namespace rdoParse 
{

int dptlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void dpterror(PTR(char) mes)
{}

int proc_rtp_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rtp_error(PTR(char) mes)
{}

int proc_rss_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_rss_error(PTR(char) mes)
{}

int proc_opr_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void proc_opr_error(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivity
// ----------------------------------------------------------------------------
RDODPTActivity::RDODPTActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	m_pattern( NULL ),
	m_currParam( 0 ),
	m_activity( NULL )
{
	parser()->checkActivityName( src_info() );
	m_pattern = parser()->findPATPattern( _pattern_src_info.src_text() );
	if ( !m_pattern ) {
		parser()->error().error( _pattern_src_info, rdo::format("�� ������ �������: %s", _pattern_src_info.src_text().c_str()) );
	}
}

void RDODPTActivity::addParam(const RDOValue& param)
{
	if (m_pattern->m_paramList.size() <= m_currParam)
	{
		if (param.src_pos().m_first_line == src_pos().m_first_line)
		{
			if (dynamic_cast<RDOOPROperation*>(this))
			{
				parser()->error().push_only(param, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� �������� '%s'", m_pattern->name().c_str(), name().c_str()));
			}
			else
			{
				parser()->error().push_only(param, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� ���������� '%s'", m_pattern->name().c_str(), name().c_str()));
			}
			parser()->error().push_only(m_pattern->src_info(), "��. �������");
			parser()->error().push_done();
		}
		else
		{
			if (dynamic_cast<RDOOPROperation*>(this))
			{
				parser()->error().error(param, "��� �������� ������ ������������� ����������");
			}
			else
			{
				parser()->error().error(param, "��� ���������� ������ ������������� ����������");
			}
		}
	}
	rdoRuntime::RDOValue val;
	RDOFUNFunctionParam* pat_param = m_pattern->m_paramList.at(m_currParam);
	if (param->getAsString() == "*")
	{
		if (!pat_param->getType()->default().defined())
		{
			parser()->error().push_only(param, rdo::format("��� �������� ��-��������� ��� ��������� '%s'", pat_param->src_text().c_str()));
			parser()->error().push_only(pat_param->src_info(), rdo::format("��. �������� '%s', ��� '%s'", pat_param->src_text().c_str(), pat_param->getType()->src_text().c_str()));
			parser()->error().push_done();
		}
		val = pat_param->getType()->default().value();
	}
	else
	{
		val = pat_param->getType()->value_cast(param).value();
	}
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc(parser()->runtime(), m_currParam, val);
	calc->setSrcInfo(RDOParserSrcInfo(param.getPosAsYY(), rdo::format("�������� ������� %s.%s = %s", m_pattern->name().c_str(), pat_param->name().c_str(), param->getAsString().c_str())));
	m_activity->addParamCalc(calc);
	m_currParam++;
}

void RDODPTActivity::endParam( const YYLTYPE& _param_pos )
{
	if ( m_pattern->m_paramList.size() > m_currParam ) {
		RDOFUNFunctionParam* param = m_pattern->m_paramList.at( m_currParam );
		parser()->error().push_only( _param_pos, rdo::format("������� �� ��� ��������� ������� '%s':", m_pattern->src_text().c_str()) );
		for ( unsigned int i = m_currParam; i < m_pattern->m_paramList.size(); i++ ) {
			param = m_pattern->m_paramList.at( i );
			parser()->error().push_only( param->src_info(), rdo::format("��������� �������� '%s' ����� ��� '%s'", param->name().c_str(), param->getType()->src_text().c_str()) );
		}
		parser()->error().push_done();
	}
	if (m_pattern->getType() == RDOPATPattern::PT_Keyboard)
	{
		LPIKeyboard keyboard = m_activity;
		ASSERT(keyboard);
		if (!keyboard->hasHotKey())
		{
			if (dynamic_cast<RDOOPROperation*>(this))
			{
				parser()->error().push_only( _param_pos, "��� ������������ �������� ������ ���� ������� �������" );
			}
			else
			{
				parser()->error().push_only( _param_pos, "��� ���������� ������ ���� ������� �������" );
			}
			parser()->error().push_only( m_pattern->src_info(), "��. �������" );
			parser()->error().push_done();
		}
	}
}

bool RDODPTActivity::setPrior(RDOFUNArithm* prior)
{
	LPIPriority prior_activity = m_activity;
	if (prior_activity)
	{
		return prior_activity->setPrior(prior->createCalc());
	}
	return false;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey( LPIBaseOperationContainer dpt, const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( parent, _src_info, _pattern_src_info )
{
	switch ( pattern()->getType() )
	{
		case RDOPATPattern::PT_IE:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternIrregEvent*>(pattern()->getPatRuntime())->createActivity( dpt, parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Rule:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternRule*>(pattern()->getPatRuntime())->createActivity( dpt, parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Operation:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternOperation*>(pattern()->getPatRuntime())->createActivity( dpt, parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Keyboard:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternKeyboard*>(pattern()->getPatRuntime())->createActivity( dpt, parser()->runtime(), name() );
			break;
		}
		default:
		{
			parser()->error().push_only( src_info(), "����������� ��� �������" );
			parser()->error().push_only( pattern()->src_info(), "��. �������" );
			parser()->error().push_done();
		}
	}
}

void RDODPTActivityHotKey::addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos )
{
	if ( pattern()->getType() != RDOPATPattern::PT_Keyboard )
	{
		parser()->error().push_only( hotkey_pos, "������� ������� ������������ ������ � ������������ ���������" );
		parser()->error().push_only( pattern()->src_info(), "��. �������" );
		parser()->error().push_done();
	}
	LPIKeyboard keyboard = m_activity;
	ASSERT(keyboard);
	switch (keyboard->addHotKey(parser()->runtime(), hotKey))
	{
		case rdoRuntime::RDOKeyboard::addhk_ok      : {
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_already : {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error().error( hotkey_pos, rdo::format("��� �������� '%s' ������� ��� ���������", src_text().c_str()) );
			} else {
				parser()->error().error( hotkey_pos, rdo::format("��� ���������� '%s' ������� ��� ���������", src_text().c_str()) );
			}
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_notfound: {
			parser()->error().error( hotkey_pos, rdo::format("����������� �������: %s", hotKey.c_str()) );
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_dont    : {
			parser()->error().push_only( src_info(), rdo::format("�������� '%s' �� �������� ������������", src_text().c_str()) );
			parser()->error().push_only( pattern()->src_info(), "��. �������" );
			parser()->error().push_done();
			break;
		}
		default: {
			parser()->error().error( src_info(), "���������� ������: RDOOPROperation::addHotKey" );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
RDODPTFreeActivity::RDODPTFreeActivity( LPIBaseOperationContainer dpt, const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( dpt, parent, _src_info, _pattern_src_info )
{
	parser()->insertDPTFreeActivity( this );
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
RDODPTFree::RDODPTFree( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>( _parser, _src_info )
{
	parser()->checkDPTName(src_info());
	m_rt_logic = F(rdoRuntime::RDODPTFree)::create(parser()->runtime());
	m_rt_logic->init(parser()->runtime());
	parser()->insertDPTFree(this);
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity( LPIBaseOperationContainer dpt, const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( dpt, _parent, _src_info, _pattern_src_info )
{
}

// ----------------------------------------------------------------------------
// ---------- RDODPTPriorActivity
// ----------------------------------------------------------------------------
RDODPTPriorActivity::RDODPTPriorActivity( LPIBaseOperationContainer dpt, const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( dpt, _parent, _src_info, _pattern_src_info )
{
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info, LPILogic _parent ):
	RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>( _parser, _src_info )
{
	parser()->checkDPTName(src_info());
	m_rt_logic = F(rdoRuntime::RDODPTSome)::create(parser()->runtime(), _parent);
	m_rt_logic->init(parser()->runtime());
	parser()->insertDPTSome(this);
}

void RDODPTSome::end()
{
	if ( getConditon() )
	{
		m_rt_logic->setCondition( getConditon()->getCalc() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior( RDOParser* _parser, const RDOParserSrcInfo& _src_info, LPILogic _parent ):
	RDOLogicActivity<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>( _parser, _src_info )
{
	parser()->checkDPTName(src_info());
	m_rt_logic = F(rdoRuntime::RDODPTPrior)::create(parser()->runtime(), _parent);
	m_rt_logic->init(parser()->runtime());
	parser()->insertDPTPrior(this);
}

void RDODPTPrior::end()
{
	if ( getConditon() )
	{
		m_rt_logic->setCondition( getConditon()->getCalc() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity( LPIBaseOperationContainer dpt, const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( _parent, _src_info, _pattern_src_info ),
	m_value( IDPTSearchActivity::vt_before ),
	m_ruleCost( NULL )
{
	if ( pattern()->getType() != RDOPATPattern::PT_Rule ) {
		parser()->error().push_only( src_info(), "������ ������������� ������� ����� ���� ������������ � ����� �������� ������� ���� search" );
		parser()->error().push_only( pattern()->src_info(), "��. �������" );
		parser()->error().push_done();
	}
	for ( std::vector< RDORelevantResource* >::const_iterator i = pattern()->rel_res_begin(); i != pattern()->rel_res_end(); i++ ) {
		if ( ((*i)->m_statusBegin == rdoRuntime::RDOResource::CS_Create) || ((*i)->m_statusBegin == rdoRuntime::RDOResource::CS_Erase) ) {
			parser()->error().push_only( src_info(), rdo::format("� ������������� ������� '%s' ������ ��������� ��� ������� �������, �.�. ��� ������������ � ����� ���� search", src_text().c_str()) );
			parser()->error().push_only( pattern()->src_info(), "��. �������" );
			parser()->error().push_only( (*i)->src_info(), "��. ����������� ������" );
			parser()->error().push_done();
//			parser()->error().error( "Rule: " + name() + " Cannot be used in search activity because of bad converter status" );
		}
	}
	m_activity = F(rdoRuntime::RDORule)::create( parser()->runtime(), static_cast<rdoRuntime::RDOPatternRule*>(pattern()->getPatRuntime()), true, name());
}

void RDODPTSearchActivity::setValue( IDPTSearchActivity::ValueTime value, RDOFUNArithm* ruleCost, const YYLTYPE& _param_pos )
{
	endParam( _param_pos );
	m_value    = value;
	m_ruleCost = ruleCost;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace, LPILogic _parent ):
	RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>( _parser, _src_info ),
	m_trace( trace ),
	m_closed( false ),
	m_parent( _parent )
{
	parser()->checkDPTName( src_info() );
	parser()->insertDPTSearch( this );
}

void RDODPTSearch::end()
{
	rdoRuntime::RDOCalc* condCalc = m_conditon ? m_conditon->getCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
	rdoRuntime::RDOCalc* termCalc = m_termConditon ? m_termConditon->getCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );

	m_rt_logic = F(rdoRuntime::RDODPTSearchRuntime)::create( parser()->runtime(),
		m_parent,
		condCalc,
		termCalc,
		m_evalBy->createCalc(),
		m_compTops,
		m_trace );
	ASSERT(m_rt_logic);
	m_rt_logic->init(parser()->runtime());

	int size = getActivities().size();
	for ( int i = 0; i < size; i++ )
	{
		const RDODPTSearchActivity* activity = getActivities().at(i);
		LPIDPTSearchActivity act = F(rdoRuntime::RDODPTSearchActivity)::create(
			activity->activity(),
			activity->getValue(),
			activity->getRuleCost()->createCalc()
		);
		LPIDPTSearchLogic searchLogic = m_rt_logic;
		ASSERT(searchLogic);
		searchLogic->addActivity(act);
	}
	m_closed = true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
std::string RDOPROCProcess::s_name_prefix = "";
std::string RDOPROCProcess::s_name_sufix  = "s";

RDOPROCProcess::RDOPROCProcess(PTR(RDOParser) parser, CREF(RDOParserSrcInfo) info)
	: RDOParserObject (parser)
	, RDOParserSrcInfo(info  )
	, m_closed        (false )
	, m_parent        (NULL  )
{
	this->parser()->insertPROCProcess(this);
	m_runtime = F(rdoRuntime::RDOPROCProcess)::create(info.src_text(), this->parser()->runtime());
	m_runtime.query_cast<ILogic>()->init(this->parser()->runtime());
}

bool RDOPROCProcess::setPrior(RDOFUNArithm* prior)
{
	LPILogic runtime_logic = getRunTime();
	LPIPriority priority = runtime_logic.query_cast<IPriority>();
	if (priority)
	{
		return priority->setPrior(prior->createCalc());
	}
	return false;
}

void RDOPROCProcess::end()
{
	m_closed = true;
	if ( getConditon() )
	{
		getRunTime()->setCondition( getConditon()->getCalc() );
	}
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		m_child.push_back( value );
		value->m_parent = this;
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
RDOPROCOperator::RDOPROCOperator( RDOPROCProcess* _process, const std::string& _name ):
	RDOParserObject( _process ),
	name		   ( _name    ),
	process		   ( _process )
{
	process->m_operations.push_back( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time )
	: RDOPROCOperator( _process, _name )
{
	runtime = F(rdoRuntime::RDOPROCGenerate)::create(parser()->getLastPROCProcess()->getRunTime(), time);
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
void RDOPROCBlockForQueue::createRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name )
{
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList( parser );
	// �������� ������
	rdoMBuilder::RDOResource rss( rtp, res_name );
	// ������� ��� � �������
	rssList.append<rdoParse::RDORSSResource>( rss );
}

bool RDOPROCBlockForQueue::checkType( RDOParser *parser, rdoMBuilder::RDOResType rtp, const RDOParserSrcInfo& info )
{
	// "�����_�������"
	std::string rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// ��� ������, �������� ��� �� ������� ��������� "�����_�������"
	if (!rtp.m_params[rtp_param_name].exist())
		parser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� integer '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));

	CREF(rdoMBuilder::RDOResType::Param) param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if (param.typeID() != rdoRuntime::RDOType::t_int)
		parser->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' �������� '%s' �� �������� ���������� int"), rtp.name().c_str(), rtp_param_name.c_str()));

	return true;
}
rdoMBuilder::RDOResType RDOPROCBlockForQueue::createType( RDOParser *parser, const std::string& rtp_name, const RDOParserSrcInfo& info )
{
	// "�����_�������"
	std::string rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
	// �������� ����� ������� �� ���������
	RDOValue default(rdoRuntime::RDOPROCQueue::getDefaultValue());
	// �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList( parser );
	// �������� ��� �������
	rdoMBuilder::RDOResType rtp( rtp_name );
	// �������� �������� ���� integer
	rtp.m_params.append(rdoMBuilder::RDOResType::Param(rtp_param_name, g_int, default));
	// ������� ��� �������
	if (!rtpList.append( rtp ))
	{
		parser->error().error(info, rdo::format("������ �������� ���� �������: %s", rtp_name.c_str()));
	}
	return rtp;
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
void RDOPROCQueue::create_runtime_Queue(RDOParser *parser)
{
	CPTR(RDORSSResource) rss = parser->findRSSResource(Res);
	if (rss)
	{
		const tstring res_name = rss->name();
		// �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(parser);
		// �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		// "�����_�������"
		const tstring rtp_param_name = rdoRuntime::RDOPROCQueue::getQueueParamName();
		parser_for_runtime.Id_res = rss->getID();
		parser_for_runtime.Id_param = rtp.m_params[rtp_param_name].id();
	}
	else
	{
		parser->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCQueue: �� ����� parser-������ '%s'"), Res.c_str()));
	}
	runtime = F(rdoRuntime::RDOPROCQueue)::create(parser->getLastPROCProcess()->getRunTime(), parser_for_runtime);
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
void RDOPROCDepart::create_runtime_Depart( RDOParser *parser )
{
	const RDORSSResource* rss = parser->findRSSResource( Res );
	if( rss ){
		const std::string res_name = rss->name();
		// �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList( parser );
		// �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		// "�����_�������"
		std::string rtp_param_name = rdoRuntime::RDOPROCDepart::getDepartParamName();
		parser_for_runtime.Id_res = rss->getID();
		parser_for_runtime.Id_param = rtp.m_params[rtp_param_name].id(); 
	}	
	else
	{
		parser->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCQueue: �� ����� parser-������ '%s'"), Res.c_str()));
	}
	runtime = F(rdoRuntime::RDOPROCDepart)::create(parser->getLastPROCProcess()->getRunTime(), parser_for_runtime);
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
bool RDOPROCBlockForSeize::checkType( RDOParser *parser, rdoMBuilder::RDOResType rtp, const RDOParserSrcInfo& info )
{
	// "���������"
	std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "��������"
	std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	// "�����"
	std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();
	// ��� ������, �������� ��� �� ������� ������������� ���������
	if (!rtp.m_params[rtp_param_name].exist())
		parser->error().error(info, rdo::format("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str()));

	const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if (param.typeID() != rdoRuntime::RDOType::t_enum)
		parser->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' �������� '%s' �� �������� ���������� ������������� ����"), rtp.name().c_str(), rtp_param_name.c_str()));

	// ������ �������� ���� ��������
	if (!param.getEnum()->getEnums().exist(rtp_state_free) || !param.getEnum()->getEnums().exist(rtp_state_buzy))
		parser->error().error(param.src_info(), rdo::format(_T("� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s"), rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str()));

	return true;
}

void RDOPROCBlockForSeize::createRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name )
{
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList( parser );
	// �������� ������
	rdoMBuilder::RDOResource rss( rtp, res_name );
	// ������� ��� � �������
	rssList.append<rdoParse::RDOPROCResource>( rss );
}

void RDOPROCBlockForSeize::reobjectRes( RDOParser *parser, rdoMBuilder::RDOResType rtp, const std::string& res_name )
{
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList( parser );
	// �������� ������
	rdoMBuilder::RDOResource rssNew( rtp, res_name );
	// ������� ��� � �������
	rssList.replace<rdoParse::RDOPROCResource>( rssNew );
}

rdoMBuilder::RDOResType RDOPROCBlockForSeize::createType( RDOParser *parser, const std::string& rtp_name, const RDOParserSrcInfo& info )
{
	// "���������"
	std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "��������"
	std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	// "�����"
	std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList( parser );
	// �������� ��� �������
	rdoMBuilder::RDOResType rtp( rtp_name );
	// �������� �������� ������������� ���� - "���������"
	rtp.m_params.append(rdoMBuilder::RDOResType::Param(
		rtp_param_name,
		rdoRuntime::RDOEnumType::Enums(rtp_state_free)(rtp_state_buzy),
		RDOValue(rtp_state_free)
	));
	// ������� ��� �������
	if ( !rtpList.append( rtp ) )
	{
		parser->error().error( info, rdo::format("������ �������� ���� �������: %s", rtp_name.c_str()) );
	}
	return rtp;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
void RDOPROCSeize::create_runtime_Seize ( RDOParser *parser )
{
	std::list< std::string >::iterator it = Resources.begin();
	while ( it != Resources.end() ) 
	{
		std::string aaa = *it;
		const RDORSSResource* rss = parser->findRSSResource((*it));
		if ( rss )
		{
			const std::string res_name = rss->name();
			// �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList( parser );
			// �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			// "���������"
			std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			// �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtp_param_name].exist())
			{
				parser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res = rss->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			parser_for_runtime.push_back(bbb);
		}
		else
		{
			parser->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCSeize: �� ����� parser-������ '%s'"), it->c_str()));
		}
		it++;
	}

	int ccc = parser_for_runtime.size();
	if( ccc>0 )
	{
		runtime = F(rdoRuntime::RDOPROCSeize)::create(parser->getLastPROCProcess()->getRunTime(), parser_for_runtime);
	}
	else
	{
		parser->error().error(RDOParserSrcInfo(), _T("���������� ������: ���� Seize ��������"));
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
void RDOPROCRelease::create_runtime_Release ( RDOParser *parser )
{
	std::list< std::string >::iterator it = Resources.begin();
	while ( it != Resources.end() ) 
	{
		std::string aaa = *it;
		const RDORSSResource* rss = parser->findRSSResource((*it));
		if ( rss )
		{
			const std::string res_name = rss->name();
			// �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList( parser );
			// �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			// "���������"
			std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			// �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtp_param_name].exist())
			{
				parser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res = rss->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			parser_for_runtime.push_back(bbb);
		}
		else
		{
			parser->error().error(RDOParserSrcInfo(), rdo::format(_T("���������� ������ RDOPROCRelease: �� ����� parser-������ '%s'"), it->c_str()));
		}
		it++;
	}

	int ccc = parser_for_runtime.size();
	if( ccc>0 )
	{
		runtime = F(rdoRuntime::RDOPROCRelease)::create(parser->getLastPROCProcess()->getRunTime(), parser_for_runtime);
	}
	else
	{
		parser->error().error(RDOParserSrcInfo(), _T("���������� ������: ���� Release ��������"));
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time ):
	RDOPROCOperator( _process, _name )
{
	runtime = F(rdoRuntime::RDOPROCAdvance)::create(parser()->getLastPROCProcess()->getRunTime(), time);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name, const unsigned int& _term):
	RDOPROCOperator( _process, _name )
{
	runtime = F(rdoRuntime::RDOPROCTerminate)::create(parser()->getLastPROCProcess()->getRunTime(), _term);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
RDOPROCAssign::RDOPROCAssign( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* value, int Id_res, int Id_param ):
	RDOPROCOperator( _process, _name )
{
	runtime = F(rdoRuntime::RDOPROCAssign)::create(parser()->getLastPROCProcess()->getRunTime(), value, Id_res, Id_param);
}

} // namespace rdoParse
