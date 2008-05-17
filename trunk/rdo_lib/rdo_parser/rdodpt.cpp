#include "pch.h"
#include "rdodpt.h"
#include "rdoopr.h"
#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdorss.h"
#include <rdo_ie.h>
#include <rdo_rule.h>
#include <rdo_operation.h>
#include <rdo_keyboard.h>
#include <rdoprocess.h>
#include <rdo_logic_dptfree.h>
#include <rdo_logic_dptsome.h>
#include <rdodptrtime.h>
#include <rdo_resources.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

int proc_rtp_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void proc_rtp_error( char* mes )
{
}

int proc_rss_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void proc_rss_error( char* mes )
{
}

int proc_opr_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void proc_opr_error( char* mes )
{
}

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
		parser()->error( _pattern_src_info, rdo::format("�� ������ �������: %s", _pattern_src_info.src_text().c_str()) );
	}
}

void RDODPTActivity::addParam( const std::string& _param, const YYLTYPE& _param_pos )
{
	if ( m_pattern->params.size() <= m_currParam ) {
		if ( _param_pos.first_line == src_pos().m_first_line ) {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� �������� '%s'", m_pattern->name().c_str(), name().c_str()) );
			} else {
				parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� ���������� '%s'", m_pattern->name().c_str(), name().c_str()) );
			}
			parser()->error_push_only( m_pattern->src_info(), "��. �������" );
			parser()->error_push_done();
		} else {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error( _param_pos, "��� �������� ������ ������������� ����������" );
			} else {
				parser()->error( _param_pos, "��� ���������� ������ ������������� ����������" );
			}
		}
	}
	RDOFUNFunctionParam* param = m_pattern->params.at( m_currParam );
	switch ( param->getType()->getType() ) {
		case rdoRuntime::RDOValue::rvt_int : parser()->error( _param_pos, rdo::format("��������� �������� ������ ����: %s", param->getType()->src_text().c_str()) ); break;
		case rdoRuntime::RDOValue::rvt_real: parser()->error( _param_pos, rdo::format("��������� �������� ������������� ����: %s", param->getType()->src_text().c_str()) ); break;
		case rdoRuntime::RDOValue::rvt_enum: break;
		default: parser()->error( src_info(), "���������� ������: ���������� ��� ���� RDOValue" );
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSEnumValue( _param, _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), m_currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("�������� ������� %s.%s = %s", m_pattern->name().c_str(), param->name().c_str(), _param.c_str())) );
	m_activity->addParamCalc( calc );
	m_currParam++;
}

void RDODPTActivity::addParam( int _param, const YYLTYPE& _param_pos ) 
{
	if ( m_pattern->params.size() <= m_currParam ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� �������� '%s'", m_pattern->name().c_str(), name().c_str()) );
		} else {
			parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� ���������� '%s'", m_pattern->name().c_str(), name().c_str()) );
		}
		parser()->error_push_only( m_pattern->src_info(), "��. �������" );
		parser()->error_push_done();
	}
	RDOFUNFunctionParam* param = m_pattern->params.at( m_currParam );
	switch ( param->getType()->getType() ) {
		case rdoRuntime::RDOValue::rvt_int :
		case rdoRuntime::RDOValue::rvt_real: break;
		case rdoRuntime::RDOValue::rvt_enum: parser()->error( _param_pos, rdo::format("��������� �������� ������������� ����: %s", param->getType()->src_text().c_str()) ); break;
		default: parser()->error( src_info(), "���������� ������: ���������� ��� ���� RDOValue" );
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSIntValue( _param, _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), m_currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("�������� ������� %s.%s = %d", m_pattern->name().c_str(), param->name().c_str(), _param)) );
	m_activity->addParamCalc( calc );
	m_currParam++;
}

void RDODPTActivity::addParam( double _param, const YYLTYPE& _param_pos ) 
{
	if ( m_pattern->params.size() <= m_currParam ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� �������� '%s'", m_pattern->name().c_str(), name().c_str()) );
		} else {
			parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� ���������� '%s'", m_pattern->name().c_str(), name().c_str()) );
		}
		parser()->error_push_only( m_pattern->src_info(), "��. �������" );
		parser()->error_push_done();
	}
	RDOFUNFunctionParam* param = m_pattern->params.at( m_currParam );
	switch ( param->getType()->getType() ) {
		case rdoRuntime::RDOValue::rvt_int : parser()->error( _param_pos, rdo::format("��������� �������� ������ ����: %s", param->getType()->src_text().c_str()) ); break;
		case rdoRuntime::RDOValue::rvt_real: break;
		case rdoRuntime::RDOValue::rvt_enum: parser()->error( _param_pos, rdo::format("��������� �������� ������������� ����: %s", param->getType()->src_text().c_str()) ); break;
		default: parser()->error( src_info(), "���������� ������: ���������� ��� ���� RDOValue" );
	}
	rdoRuntime::RDOValue val = param->getType()->getRSSRealValue( _param, _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), m_currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("�������� ������� %s.%s = %f", m_pattern->name().c_str(), param->name().c_str(), _param)) );
	m_activity->addParamCalc( calc );
	m_currParam++;
}

void RDODPTActivity::addParam( const YYLTYPE& _param_pos ) 
{
	if ( m_pattern->params.size() <= m_currParam ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� �������� '%s'", m_pattern->name().c_str(), name().c_str()) );
		} else {
			parser()->error_push_only( _param_pos, rdo::format("������� ����� ���������� ��� ������� '%s' ��� �������� ���������� '%s'", m_pattern->name().c_str(), name().c_str()) );
		}
		parser()->error_push_only( m_pattern->src_info(), "��. �������" );
		parser()->error_push_done();
	}
	RDOFUNFunctionParam* param = m_pattern->params.at( m_currParam );
	if ( !param->getType()->getDV().isExist() ) {
		parser()->error_push_only( _param_pos, rdo::format("��� �������� ��-��������� ��� ��������� '%s'", param->src_text().c_str()) );
		parser()->error_push_only( param->src_info(), rdo::format("��. �������� '%s', ��� '%s'", param->src_text().c_str(), param->getType()->src_text().c_str()) );
		parser()->error_push_done();
	}
	rdoRuntime::RDOValue val = param->getType()->getDefaultValue( _param_pos );
	rdoRuntime::RDOSetPatternParamCalc* calc = new rdoRuntime::RDOSetPatternParamCalc( parser()->runtime(), m_currParam, val );
	calc->setSrcInfo( RDOParserSrcInfo(_param_pos, rdo::format("�������� ������� %s.%s = *", m_pattern->name().c_str(), param->name().c_str())) );
	m_activity->addParamCalc( calc );
	m_currParam++;
}

void RDODPTActivity::endParam( const YYLTYPE& _param_pos )
{
	if ( m_pattern->params.size() > m_currParam ) {
		RDOFUNFunctionParam* param = m_pattern->params.at( m_currParam );
		parser()->error_push_only( _param_pos, rdo::format("������� �� ��� ��������� ������� '%s':", m_pattern->src_text().c_str()) );
		for ( unsigned int i = m_currParam; i < m_pattern->params.size(); i++ ) {
			param = m_pattern->params.at( i );
			parser()->error_push_only( param->src_info(), rdo::format("��������� �������� '%s' ����� ��� '%s'", param->name().c_str(), param->getType()->src_text().c_str()) );
		}
		parser()->error_push_done();
	}
	if ( m_pattern->getType() == RDOPATPattern::PT_Keyboard && !static_cast<rdoRuntime::RDOKeyboard*>(m_activity)->hasHotKey() ) {
		if ( dynamic_cast<RDOOPROperation*>(this) ) {
			parser()->error_push_only( _param_pos, "��� ������������ �������� ������ ���� ������� �������" );
		} else {
			parser()->error_push_only( _param_pos, "��� ���������� ������ ���� ������� �������" );
		}
		parser()->error_push_only( m_pattern->src_info(), "��. �������" );
		parser()->error_push_done();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( parent, _src_info, _pattern_src_info )
{
	switch ( pattern()->getType() )
	{
		case RDOPATPattern::PT_IE:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternIrregEvent*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Rule:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternRule*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Operation:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternOperation*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		case RDOPATPattern::PT_Keyboard:
		{
			m_activity = static_cast<rdoRuntime::RDOPatternKeyboard*>(pattern()->getPatRuntime())->createActivity( parser()->runtime(), name() );
			break;
		}
		default:
		{
			parser()->error_push_only( src_info(), "����������� ��� �������" );
			parser()->error_push_only( pattern()->src_info(), "��. �������" );
			parser()->error_push_done();
		}
	}
}

void RDODPTActivityHotKey::addHotKey( const std::string& hotKey, const YYLTYPE& hotkey_pos )
{
	if ( pattern()->getType() != RDOPATPattern::PT_Keyboard )
	{
		parser()->error_push_only( hotkey_pos, "������� ������� ������������ ������ � ������������ ���������" );
		parser()->error_push_only( pattern()->src_info(), "��. �������" );
		parser()->error_push_done();
	}
	switch ( static_cast<rdoRuntime::RDOKeyboard*>(m_activity)->addHotKey( parser()->runtime(), hotKey ) ) {
		case rdoRuntime::RDOKeyboard::addhk_ok      : {
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_already : {
			if ( dynamic_cast<RDOOPROperation*>(this) ) {
				parser()->error( hotkey_pos, rdo::format("��� �������� '%s' ������� ��� ���������", src_text().c_str()) );
			} else {
				parser()->error( hotkey_pos, rdo::format("��� ���������� '%s' ������� ��� ���������", src_text().c_str()) );
			}
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_notfound: {
			parser()->error( hotkey_pos, rdo::format("����������� �������: %s", hotKey.c_str()) );
			break;
		}
		case rdoRuntime::RDOKeyboard::addhk_dont    : {
			parser()->error_push_only( src_info(), rdo::format("�������� '%s' �� �������� ������������", src_text().c_str()) );
			parser()->error_push_only( pattern()->src_info(), "��. �������" );
			parser()->error_push_done();
			break;
		}
		default: {
			parser()->error( src_info(), "���������� ������: RDOOPROperation::addHotKey" );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
RDODPTFreeActivity::RDODPTFreeActivity( const RDOParserObject* parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( parent, _src_info, _pattern_src_info )
{
	parser()->insertDPTFreeActivity( this );
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
RDODPTFree::RDODPTFree( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>( _parser, _src_info )
{
	parser()->checkDPTName( src_info() );
	m_rt_logic = new rdoRuntime::RDODPTFree( parser()->runtime() );
	parser()->insertDPTFree( this );
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivityHotKey( _parent, _src_info, _pattern_src_info )
{
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>( _parser, _src_info )
{
	parser()->checkDPTName( src_info() );
	m_rt_logic = new rdoRuntime::RDODPTSome( parser()->runtime() );
	parser()->insertDPTSome( this );
}

void RDODPTSome::end()
{
	if ( getConditon() )
	{
		m_rt_logic->setCondition( getConditon()->createCalc() );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity( const RDOParserObject* _parent, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& _pattern_src_info ):
	RDODPTActivity( _parent, _src_info, _pattern_src_info ),
	m_value( rdoRuntime::RDODPTSearch::Activity::vt_before ),
	m_ruleCost( NULL )
{
	if ( pattern()->getType() != RDOPATPattern::PT_Rule ) {
		parser()->error_push_only( src_info(), "������ ������������� ������� ����� ���� ������������ � ����� �������� ������� ���� search" );
		parser()->error_push_only( pattern()->src_info(), "��. �������" );
		parser()->error_push_done();
	}
	for ( std::vector< RDORelevantResource* >::const_iterator i = pattern()->rel_res_begin(); i != pattern()->rel_res_end(); i++ ) {
		if ( ((*i)->begin == rdoRuntime::RDOResource::CS_Create) || ((*i)->begin == rdoRuntime::RDOResource::CS_Erase) ) {
			parser()->error_push_only( src_info(), rdo::format("� ������������� ������� '%s' ������ ��������� ��� ������� �������, �.�. ��� ������������ � ����� ���� search", src_text().c_str()) );
			parser()->error_push_only( pattern()->src_info(), "��. �������" );
			parser()->error_push_only( (*i)->src_info(), "��. ����������� ������" );
			parser()->error_push_done();
//			parser()->error( "Rule: " + name() + " Cannot be used in search activity because of bad converter status" );
		}
	}
	m_activity = new rdoRuntime::RDORule( parser()->runtime(), static_cast<rdoRuntime::RDOPatternRule*>(pattern()->getPatRuntime()), true, name() );
}

void RDODPTSearchActivity::setValue( rdoRuntime::RDODPTSearch::Activity::ValueTime value, RDOFUNArithm* ruleCost, const YYLTYPE& _param_pos )
{
	endParam( _param_pos );
	m_value    = value;
	m_ruleCost = ruleCost;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch( RDOParser* _parser, const RDOParserSrcInfo& _src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace ):
	RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>( _parser, _src_info ),
	m_trace( trace ),
	m_closed( false )
{
	parser()->checkDPTName( src_info() );
	parser()->insertDPTSearch( this );
}

void RDODPTSearch::end()
{
	rdoRuntime::RDOCalc* condCalc = m_conditon ? m_conditon->createCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
	rdoRuntime::RDOCalc* termCalc = m_termConditon ? m_termConditon->createCalc() : new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );

	m_rt_logic = new rdoRuntime::RDODPTSearchRuntime( parser()->runtime(),
		condCalc,
		termCalc,
		m_evalBy->createCalc(),
		m_compTops );

	m_rt_logic->traceFlag = m_trace;

	int size = getActivities().size();
	for ( int i = 0; i < size; i++ ) {
		const RDODPTSearchActivity* activity = getActivities().at(i);
		rdoRuntime::RDOSearchActivityRuntime* act = new rdoRuntime::RDOSearchActivityRuntime(
			dynamic_cast<rdoRuntime::RDORule*>(activity->activity()),
			activity->getValue(),
			activity->getRuleCost()->createCalc());
		m_rt_logic->addActivity( act );
	}
	m_closed = true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
std::string RDOPROCProcess::s_name_prefix = "";
std::string RDOPROCProcess::s_name_sufix  = "s";

RDOPROCProcess::RDOPROCProcess( RDOParser* _parser, const std::string& name ):
	RDOParserObject( _parser ),
	m_name( name ),
	m_closed( false ),
	m_parent( NULL ),
	m_runtime( NULL )
{
	parser()->insertPROCProcess( this );
	m_runtime = new rdoRuntime::RDOPROCProcess( m_name, parser()->runtime() );
}

void RDOPROCProcess::end()
{
	m_closed = true;
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
	name( _name ),
	process( _process )
{
	process->m_operations.push_back( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time ):
	RDOPROCOperator( _process, _name ),
	runtime( NULL )
{
	runtime = new rdoRuntime::RDOPROCGenerate( parser()->getLastPROCProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
/*RDOPROCSeize::RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = parser()->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCSeize( parser()->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		parser()->error( "���������� ������ RDOPROCSeize: �� ����� parser-������" );
	}
}
*/
RDOPROCSeize::RDOPROCSeize( RDOPROCProcess* _process, const std::string& _name ):
	RDOPROCOperator( _process, _name )
{
//	parser()->insertDPTSeize( this );
}

void RDOPROCSeize::create_runtime_Seize( RDOParser *parser )
{
std::list< std::string >::iterator it = Resourses.begin();
	while ( it != Resourses.end() ) {
		std::string aaa = *it;
	const RDORSSResource* rss = parser->findRSSResource((*it));
		if( rss ){
		const std::string res_name = rss->getName();
		// �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList( parser );
		// �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		// "���������"
		std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			// �������� ��� �� ������� ������������� ���������

			if ( !rtp.m_params[rtp_param_name].exist() ) {
			parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
			}
		rdoRuntime::parser_for_Seize bbb;
		bbb.Id_res = rss->getNumber();
		bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
		parser_for_runtime.push_back(bbb);
		}	else {
		parser->error( "���������� ������ RDOPROCSeize: �� ����� parser-������" );
		}
	it++;
	}

int aaa = parser_for_runtime.size();
if( aaa>0 )
runtime = new rdoRuntime::RDOPROCSeize( parser->getLastDPTProcess()->getRunTime(), parser_for_runtime );
else
parser->error( "���������� ������: ���� Seize ��������" );
}

/*
void RDOPROCSeize::makeSeizeType	(  RDOParser *parser, const std::string &resName, const RDOParserSrcInfo& info  )
{

}

void RDOPROCSeize::makeSeizeResourse	(  RDOParser *parser, const std::string& res_name, const RDOParserSrcInfo& info )
{
TRACE( "%s SEIZE_good\n", res_name.c_str());
// "���������"
std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
// "��������"
std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
// "�����"
std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

// �������� ������ ���� ����� ��������
rdoMBuilder::RDOResTypeList rtpList( parser );
// �������� ������ ���� ��������
rdoMBuilder::RDOResourceList rssList( parser );

rdoMBuilder::RDOResType rtp;
bool rssMustCreate = !rssList[res_name].exist();
if ( rssMustCreate )
{
		// ������ �� �����, �������� ������� ����
		// ������������ ��� ���� �� ����� �������
		std::string rtp_name( RDOPROCProcess::name_prefix + res_name + RDOPROCProcess::name_sufix );
		// ����� ��� �������
		if ( !rtpList[rtp_name].exist() )
		{
		//	parser->error( info, rdo::format("�� ������ ��� �������: %s", rtp_name.c_str()) );
		// �������� ��� �������
		rdoMBuilder::RDOResType rtp_(rtp_name);
		// �������� �������� ������������� ����
		rtp_.m_params.append( rdoMBuilder::RDOResType::Param(rtp_param_name, new rdoRuntime::RDOEnum(parser->runtime(), rdoRuntime::RDOEnum::EnumArray(rtp_state_free)(rtp_state_buzy)), rtp_state_free) );
		rtp = rtp_;
		}
		else
		{
		rtp = rtpList[rtp_name];
		// ��� ������, �������� ��� �� ������� ������������� ���������
		const rdoMBuilder::RDOResType& rtp = rtpList[rtp_name];
			if ( !rtp.m_params[rtp_param_name].exist() ) {
			parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
			}
		const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
		// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
			// ��� ������ �������� ��� ���������
			if ( param.getType() != rdoRuntime::RDOValue::Type::rvt_enum ) {
			parser->error( rdo::format( "� ���� ������� '%s' �������� '%s' �� �������� ���������� ������������� ����", rtp.name().c_str(), rtp_param_name.c_str() ) );
			}
		// ������ �������� ���� ��������
			if ( !param.getEnum().exist(rtp_state_free) || !param.getEnum().exist(rtp_state_buzy) )
			{
			parser->error( rdo::format( "� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
			}
		}
	// ������� ��� �������	
	if ( !rtpList.append( rtp ) )
	{
	parser->error( info, rdo::format("������ �������� ���� �������: %s", rtp_name.c_str()) );
	}
}
else
{
		// ������� ������, ����� ��� ���
		rtp = rssList[res_name].getType();
}

	// ��������� ��� �� ������� ������������� ���������
	if ( !rtp.m_params[rtp_param_name].exist() ) {
		parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
	const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if ( param.getType() != rdoRuntime::RDOValue::Type::rvt_enum ) {
		// �������� ��������� ����, �� �� �� ������������� ����
		parser->error( rdo::format( "� ���� ������� '%s' �������� '%s' �� �������� ������������ �����", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
	// ������ �������� ���� ��������
	if ( !param.getEnum().exist(rtp_state_free) || !param.getEnum().exist(rtp_state_buzy) )
	{
		parser->error( rdo::format( "� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
	}

	if ( rssMustCreate )
	{
		// �������� ������
		rdoMBuilder::RDOResource rss( rtp, res_name );
//		res->setTrace( true );
		// ������� ��� � �������
		rssList.append( rss );
	}
}
*/










void RDOPROCSeize::makeSeizeResourse	(  RDOParser *parser, const std::string& res_name, const RDOParserSrcInfo& info )
{
	// "���������"
	std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
	// "��������"
	std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
	// "�����"
	std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

	// �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList( parser );
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList( parser );

	rdoMBuilder::RDOResType rtp;
	bool rssMustCreate = !rssList[res_name].exist();
	if ( rssMustCreate )
	{
		// ������ �� �����, �������� ������� ����
		// ������������ ��� ���� �� ����� �������
		std::string rtp_name( RDOPROCProcess::name_prefix + res_name + RDOPROCProcess::name_sufix );
		// ����� ��� �������
		if ( !rtpList[rtp_name].exist() )
		{
			parser->error( info, rdo::format("�� ������ ��� �������: %s", rtp_name.c_str()) );
		}
		rtp = rtpList[rtp_name];
	}
	else
	{
		// ������� ������, ����� ��� ���
		rtp = rssList[res_name].getType();
	}

	// ��������� ��� �� ������� ������������� ���������
	if ( !rtp.m_params[rtp_param_name].exist() ) {
		parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
	const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if ( param.getType() != rdoRuntime::RDOValue::rvt_enum ) {
		// �������� ��������� ����, �� �� �� ������������� ����
		parser->error( rdo::format( "� ���� ������� '%s' �������� '%s' �� �������� ������������ �����", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
	// ������ �������� ���� ��������
	if ( !param.getEnum().exist(rtp_state_free) || !param.getEnum().exist(rtp_state_buzy) )
	{
		parser->error( rdo::format( "� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
	}

	if ( rssMustCreate )
	{
		// �������� ������
		rdoMBuilder::RDOResource rss( rtp, res_name );
//		res->setTrace( true );
		// ������� ��� � �������
		rssList.append<rdoParse::RDOPROCResource>(rss );
	}
}


void RDOPROCSeize::makeSeizeType	(  RDOParser *parser, const std::string &resName, const RDOParserSrcInfo& info  )
{
// ������������ ��� ���� �� ����� �������
std::string rtp_name( RDOPROCProcess::name_prefix + resName + RDOPROCProcess::name_sufix );
// "���������"
std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
// "��������"
std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
// "�����"
std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

// �������� ������ ���� ����� ��������
rdoMBuilder::RDOResTypeList rtpList( parser );
// ����� ��� �������, ���� ��� ���, �� ������� ���
if ( !rtpList[rtp_name].exist() )
	{
	// �������� ��� �������
	rdoMBuilder::RDOResType rtp(rtp_name);
	// �������� �������� ������������� ����
	rtp.m_params.append( rdoMBuilder::RDOResType::Param(rtp_param_name, new rdoRuntime::RDOEnum(parser->runtime(), rdoRuntime::RDOEnum::EnumArray(rtp_state_free)(rtp_state_buzy)), rtp_state_free) );
	// ������� ��� �������
		if ( !rtpList.append( rtp ) )
		{
		parser->error( info, rdo::format("������ �������� ���� �������: %s", rtp_name.c_str()) );
		}
	}
else
{
// ��� ������, �������� ��� �� ������� ������������� ���������
const rdoMBuilder::RDOResType& rtp = rtpList[rtp_name];
	if ( !rtp.m_params[rtp_param_name].exist() ) {
		parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
// ��� ������ �������� ��� ���������
	if ( param.getType() != rdoRuntime::RDOValue::rvt_enum ) {
		parser->error( rdo::format( "� ���� ������� '%s' �������� '%s' �� �������� ���������� ������������� ����", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
// ������ �������� ���� ��������
	if ( !param.getEnum().exist(rtp_state_free) || !param.getEnum().exist(rtp_state_buzy) )
	{
		parser->error( rdo::format( "� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
	}
}
}











// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name):
	RDOPROCOperator( _process, _name )
{
}

/*RDOPROCRelease::RDOPROCRelease( RDOPROCProcess* _process, const std::string& _name, const std::string& res_name ):
	RDOPROCOperator( _process, _name )
{
	const RDORSSResource* rss = parser()->findRSSResource( res_name );
	if ( rss ) {
		runtime = new rdoRuntime::RDOPROCRelease( parser()->getLastDPTProcess()->getRunTime(), rss->getNumber() );
	} else {
		// error: �� ����� parser-������
	}
}*/

void RDOPROCRelease::create_runtime_Release( RDOParser *parser )
{
std::list< std::string >::iterator it = Resourses.begin();
	while ( it != Resourses.end() ) {
		std::string aaa = *it;
	const RDORSSResource* rss = parser->findRSSResource((*it));
		if( rss ){
		const std::string res_name = rss->getName();
		// �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList( parser );
		// �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		// "���������"
		std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			// �������� ��� �� ������� ������������� ���������

			if ( !rtp.m_params[rtp_param_name].exist() ) {
			parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
			}
		rdoRuntime::parser_for_Seize bbb;
		bbb.Id_res = rss->getNumber();
		bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
		parser_for_runtime.push_back(bbb);
		}	else {
		parser->error( "���������� ������ RDOPROCRelease: �� ����� parser-������" );
		}
	it++;
	}

int aaa = parser_for_runtime.size();
if( parser_for_runtime.size()>0 )
runtime = new rdoRuntime::RDOPROCRelease( parser->getLastDPTProcess()->getRunTime(), parser_for_runtime );
else
parser->error( "���������� ������: ���� Release �� ����� ��������" );
}

void RDOPROCRelease::checkReleaseType	(  RDOParser *parser, const std::string &resName, const RDOParserSrcInfo& info  )
{
	/*
// ������������ ��� ���� �� ����� �������
std::string rtp_name( rdoParse::RDOPROCProcess::name_prefix + resName + rdoParse::RDOPROCProcess::name_sufix );
// "���������"
std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
// "��������"
std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
// "�����"
std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

// �������� ������ ���� ����� ��������
rdoMBuilder::RDOResTypeList rtpList( parser );
// ����� ��� �������, ���� ��� ���, ������ ������
if ( !rtpList[rtp_name].exist() )
	{
	// �������� ��� �������
	rdoMBuilder::RDOResType rtp(rtp_name);
	// �������� �������� ������������� ����
	rtp.m_params.append( rdoMBuilder::RDOResType::Param(rtp_param_name, new rdoRuntime::RDOEnum(parser->runtime(), rdoRuntime::RDOEnum::EnumArray(rtp_state_free)(rtp_state_buzy)), rtp_state_free) );
	// ������� ��� �������
		if ( !rtpList.append( rtp ) )
		{
		parser->error( info, rdo::format("��� ������� %s �� ����������", rtp_name.c_str()) );
		}
	}
	else
	{
	// ��� ������, �������� ��� �� ������� ������������� ���������
	const rdoMBuilder::RDOResType& rtp = rtpList[rtp_name];
		if ( !rtp.m_params[rtp_param_name].exist() ) {
		parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
		}
	const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
		// ��� ������ �������� ��� ���������
		if ( param.getType() != rdoRuntime::RDOValue::Type::rvt_enum ) {
		parser->error( rdo::format( "� ���� ������� '%s' �������� '%s' �� �������� ���������� ������������� ����", rtp.name().c_str(), rtp_param_name.c_str() ) );
		}
	// ������ �������� ���� ��������
		if ( !param.getEnum().exist(rtp_state_free) || !param.getEnum().exist(rtp_state_buzy) )
		{
		parser->error( rdo::format( "� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
		}
	}
TRACE( "%s SEIZE_good\n", resName.c_str());
*/
}



void RDOPROCRelease::checkReleaseResourse	(  RDOParser *parser, const std::string& res_name, const RDOParserSrcInfo& info )
{
// "���������"
std::string rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
// "��������"
std::string rtp_state_free = rdoRuntime::RDOPROCBlockForSeize::getStateEnumFree();
// "�����"
std::string rtp_state_buzy = rdoRuntime::RDOPROCBlockForSeize::getStateEnumBuzy();

// �������� ������ ���� ����� ��������
rdoMBuilder::RDOResTypeList rtpList( parser );
// �������� ������ ���� ��������
rdoMBuilder::RDOResourceList rssList( parser );

rdoMBuilder::RDOResType rtp;
bool rssMustCreate = !rssList[res_name].exist();
	if ( rssMustCreate )
	{
		parser->error( info, rdo::format("������� %s �� ������", res_name.c_str()) );
	}
	else
	{
		// ������� ������, ����� ��� ���
		rtp = rssList[res_name].getType();
	}

	// ��������� ��� �� ������� ������������� ���������
	if ( !rtp.m_params[rtp_param_name].exist() ) {
		parser->error( rdo::format( "� ���� ������� '%s' ��� ��������� ������������� ���� '%s'", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
	const rdoMBuilder::RDOResType::Param& param = rtp.m_params[rtp_param_name];
	// �������� ��������� ����, ���� ���������, ����� � ��� ���� �������� �������� � �����
	// ��� ������ �������� ��� ���������
	if ( param.getType() != rdoRuntime::RDOValue::Type::rvt_enum ) {
		// �������� ��������� ����, �� �� �� ������������� ����
		parser->error( rdo::format( "� ���� ������� '%s' �������� '%s' �� �������� ������������ �����", rtp.name().c_str(), rtp_param_name.c_str() ) );
	}
	// ������ �������� ���� ��������
	if ( !param.getEnum().exist(rtp_state_free) || !param.getEnum().exist(rtp_state_buzy) )
	{
		parser->error( rdo::format( "� ���� ������� '%s' ������������ �������� '%s' ������ ����� ��� ������� ��� ������������ ��������: %s � %s", rtp.name().c_str(), param.name().c_str(), rtp_state_free.c_str(), rtp_state_buzy.c_str() ) );
	}
TRACE( "%s SEIZE_good\n", res_name.c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance( RDOPROCProcess* _process, const std::string& _name, rdoRuntime::RDOCalc* time ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCAdvance( parser()->getLastPROCProcess()->getRunTime(), time );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate( RDOPROCProcess* _process, const std::string& _name ):
	RDOPROCOperator( _process, _name )
{
	runtime = new rdoRuntime::RDOPROCTerminate( parser()->getLastPROCProcess()->getRunTime() );
}

} // namespace rdoParse
