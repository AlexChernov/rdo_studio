#include "pch.h"
#include "rdosmr.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdoparser_lexer.h"
#include "rdoparser_rdo.h"
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int smr_file_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void smr_file_error( char* mes )
{
}

int smr_sim_lex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void smr_sim_error( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOSMR
// ----------------------------------------------------------------------------
RDOSMR::RDOSMR( RDOParser* _parser, const std::string& _modelName ):
	RDOParserObject( _parser ),
	frameNumber( 1 ),
	showRate( 60 ),
	runStartTime( 0 ),
	traceStartTime( rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME ),
	traceEndTime  ( rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME ),
	showMode( rdoSimulator::SM_NoShow ),
	terminateIf( NULL )
{
	setFile( "Model_name", _modelName );
	parser()->setSMR( this );
}

void RDOSMR::setShowMode( rdoSimulator::ShowMode _showMode )
{
	showMode = _showMode;
}

void RDOSMR::setFrameNumber( int value, const YYLTYPE& pos )
{
	if ( value <= 0 ) {
		parser()->error( pos, "����� ����� ������ ���� ������ ����" );
	}
	if ( parser()->runtime()->allFrames.size() + 1 <= (unsigned int)value ) {
		parser()->error( pos, rdo::format("�������������� ����: %d", value) );
	}
	frameNumber = value;
}

void RDOSMR::setShowRate( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error( pos, "������� ������ ���� ������ ����" );
	}
	showRate = value;
}

void RDOSMR::setRunStartTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error( pos, "��������� ��������� ����� ������ ���� ������ ����" );
	}
	runStartTime = value;
}

void RDOSMR::setTraceStartTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error( pos, "��������� ����� ����������� ������ ���� ������ ����" );
	}
	if ( getTraceEndTime() != rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceEndTime() <= value ){
		parser()->error_push_only( pos, "��������� ����� ����������� ������ ���� ������ ���������" );
		parser()->error_push_only( traceEndTime_pos, "��. �������� ����� �����������" );
		parser()->error_push_done();
	}
	traceStartTime     = value;
	traceStartTime_pos = pos;
}

void RDOSMR::setTraceEndTime( double value, const YYLTYPE& pos )
{
	if ( value < 0 ) {
		parser()->error( pos, "�������� ����� ����������� ������ ���� ������ ����" );
	}
	if ( getTraceStartTime() != rdoRuntime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceStartTime() >= value ) {
		parser()->error_push_only( pos, "�������� ����� ����������� ������ ���� ������ ����������" );
		parser()->error_push_only( traceStartTime_pos, "��. ��������� ����� �����������" );
		parser()->error_push_done();
	}
	traceEndTime     = value;
	traceEndTime_pos = pos;
}

void RDOSMR::setTerminateIf( RDOFUNLogic* logic )
{
	if ( terminateIf ) {
		parser()->error_push_only( logic->src_info(), "Terminate_if ��� ���������" );
	//	parser()->error_push_only( logic->src_info(), "Second Terminate_if entry" );
		parser()->error_push_only( terminateIf->src_info(), "��. ������ �����������" );
		parser()->error_push_done();
	}
	terminateIf = logic;
	parser()->runtime()->setTerminateIf( logic->getCalc() );
}

void RDOSMR::setConstValue( const RDOParserSrcInfo& const_info, RDOFUNArithm* arithm )
{
	const RDOFUNConstant* const cons = parser()->findFUNConstant( const_info.src_text() );
	if ( !cons ) {
		parser()->error( const_info, rdo::format("��������� '%s' �� �������", const_info.src_text().c_str()) );
	}
	cons->getType()->checkParamType( arithm );
	rdoRuntime::RDOCalc* calc = arithm->createCalc( cons->getType() );
	parser()->runtime()->addInitCalc( new rdoRuntime::RDOCalcSetConst( parser()->runtime(), cons->getNumber(), calc ) );
	parser()->insertChanges( cons->src_text(), arithm->src_text() );
}

void RDOSMR::setResParValue( const RDOParserSrcInfo& res_info, const RDOParserSrcInfo& par_info, RDOFUNArithm* arithm )
{
	const RDORSSResource* res = parser()->findRSSResource( res_info.src_text() );
	if ( !res ) {
		parser()->error( res_info.src_info(), rdo::format("������ '%s' �� ������", res_info.src_text().c_str()) );
//		parser()->error( res_info.src_info(), "Undefined resource name: " + resName );
	}
	const RDORTPParam* param = res->getType()->findRTPParam( par_info.src_text() );
	if ( !param ) {
		parser()->error_push_only( par_info.src_info(), rdo::format("�������� '%s' �� ������", par_info.src_text().c_str()) );
		parser()->error_push_only( res->src_info(), "��. ������" );
		parser()->error_push_only( res->getType()->src_info(), "��. ��� �������" );
		parser()->error_push_done();
//		parser()->error( par_info.src_info(), "Undefined resource parameter name: " + parName);
	}
	param->getType()->checkParamType( arithm );
	unsigned int parNumb = res->getType()->getRTPParamNumber( par_info.src_text() );
	rdoRuntime::RDOCalc* calc = arithm->createCalc( param->getType() );
	parser()->runtime()->addInitCalc( new rdoRuntime::RDOSetResourceParamCalc( parser()->runtime(), res->getID(), parNumb, calc ) );
	parser()->insertChanges( res_info.src_text() + "." + par_info.src_text(), arithm->src_text() );
}

void RDOSMR::setSeed( const RDOParserSrcInfo& seq_info, int base )
{
	const RDOFUNSequence* seq = parser()->findFUNSequence( seq_info.src_text() );
	if ( !seq ) {
		parser()->error( seq_info, rdo::format("������������������ '%s' �� �������", seq_info.src_text().c_str()) );
//		parser()->error( "Undefined sequence: " + seqName );
	}
	seq->init_calc->setBase( base );
	parser()->insertChanges( seq->src_text() + ".Seed", rdo::format("%d", base) );
}

void RDOSMR::insertBreakPoint( const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic )
{
	std::vector< BreakPoint* >::const_iterator it = breakPoints.begin();
	while ( it != breakPoints.end() ) {
		if ( (*it)->src_text() == _src_info.src_text() ) {
			parser()->error_push_only( _src_info, rdo::format("����� �������� � ������ '%s' ��� ����������", _src_info.src_text().c_str()) );
			parser()->error_push_only( (*it)->src_info(), "��. ������ �����������" );
			parser()->error_push_done();
		}
		it++;
	}
	breakPoints.push_back( new BreakPoint( this, _src_info, _logic ) );
}

RDOSMR::BreakPoint::BreakPoint( RDOSMR* smr, const RDOParserSrcInfo& _src_info, RDOFUNLogic* _logic ):
	RDOParserObject( smr ),
	RDOParserSrcInfo( _src_info )
{
	parser()->runtime()->insertBreakPoint( src_text(), _logic->getCalc() );
}

} // namespace rdoParse 
