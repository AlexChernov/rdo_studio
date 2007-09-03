#include "pch.h"
#include "rdortp.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdoDefines.h"
#include "rdoparser_lexer.h"
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void rtperror( char* mes )
{
}

// ----------------------------------------------------------------------------
// ---------- RDORTPDefVal - �������� ��-���������, ������� �����
// ----------------------------------------------------------------------------
RDORTPDefVal::RDORTPDefVal( RDOParser* _parser, const RDORTPDefVal& dv ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( dv.src_info() ),
	exist( dv.exist )
{
}

RDORTPDefVal::RDORTPDefVal( RDOParser* _parser, bool _exist ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo(),
	exist( _exist )
{
}

RDORTPDefVal::RDORTPDefVal( RDOParser* _parser, bool _exist, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	exist( _exist )
{
}

int RDORTPDefVal::getIntValue() const
{
	getParser()->error( src_info(), "�������� �������� ��-���������" );
//	getParser()->error( "Invalid default value" );
	return 0;	// unreachable code...
}

double RDORTPDefVal::getRealValue() const
{
	getParser()->error( src_info(), "�������� �������� ��-���������" );
	return 0.;	// unreachable code...
}

const std::string& RDORTPDefVal::getEnumValue() const
{
	getParser()->error( src_info(), "�������� �������� ��-���������" );
#pragma warning( disable: 4172 )
	return "";	// unreachable code...
#pragma warning( default: 4172 )
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
void RDORTPParamType::checkParamType( const RDOFUNArithm* const action ) const
{
	switch ( getType() ) {
		case RDORTPParamType::pt_int: {
			if ( action->getType() == RDORTPParamType::pt_real ) {
				getParser()->warning( action->src_info(), "������� ������������� ����� � �����, �������� ������ ������" );
			} else if ( action->getType() != RDORTPParamType::pt_int ) {
				getParser()->error( action->src_info(), "�������������� �����. ��������� ������������� ��������" );
			} else {
				const rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<const rdoRuntime::RDOCalcConst*>(action->getCalc());
				if ( calc_const ) {
					rdoRuntime::RDOValue value = calc_const->calcValueBase( getParser()->runtime );
					checkRSSIntValue( value, action->src_info() );
				}
			}
			break;
		}
		case RDORTPParamType::pt_real: {
			if ( action->getType() != RDORTPParamType::pt_real && action->getType() != RDORTPParamType::pt_int ) {
				getParser()->error( action->src_info(), "�������������� �����. ��������� ������������ ��������" );
			} else {
				const rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<const rdoRuntime::RDOCalcConst*>(action->getCalc());
				if ( calc_const ) {
					rdoRuntime::RDOValue value = calc_const->calcValueBase( getParser()->runtime );
					checkRSSRealValue( value, action->src_info() );
				}
			}
			break;
		}
		case RDORTPParamType::pt_enum: {
			if ( action->getType() == RDORTPParamType::pt_unknow ) {
				if ( static_cast<const RDORTPEnumParamType*>(this)->enu->findEnumValueWithoutThrow( action->str ) == -1 ) {
					if ( static_cast<const RDORTPEnumParamType*>(this)->enum_fun ) {
						getParser()->error( action->src_info(), rdo::format("�������� '%s' �� ����� �������� ����������� �������: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					} else {
						getParser()->error( action->src_info(), rdo::format("�������� '%s' �� �������� ��������� ������������� ���������: %s", action->str.c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
				}
			} else if ( action->getType() != RDORTPParamType::pt_enum ) {
				getParser()->error_push_only( action->src_info(), rdo::format("�������������� �����. ��������� ������������ ���: %s", src_text().c_str()) );
				getParser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), rdo::format("��������� ��������: %s", static_cast<const RDORTPEnumParamType*>(this)->enu->src_text().c_str()) );
				getParser()->error_push_done();
			} else if ( action->enu != static_cast<const RDORTPEnumParamType*>(this)->enu ) {
				if ( action->enu->enumVals == static_cast<const RDORTPEnumParamType*>(this)->enu->enumVals ) {
					getParser()->error_push_only( action->src_info(), "������������ ��������� ������������ ���� � ����������� ����������" );
					getParser()->error_push_only( action->enu->src_info(), action->enu->src_text() );
					getParser()->error_push_only( action->src_info(), "�" );
					getParser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), static_cast<const RDORTPEnumParamType*>(this)->enu->src_text() );
					getParser()->error_push_only( action->src_info(), "��������, ������� ������������ ������ �� ��� ��� ������������, � ������ ��� such_as �� ����, ����� ��������� ����� ����� ���������� � �����������" );
				} else {
					getParser()->error_push_only( action->src_info(), "�������������� ������������ �����" );
					getParser()->error_push_only( action->enu->src_info(), action->enu->src_text() );
					getParser()->error_push_only( action->src_info(), "�" );
					getParser()->error_push_only( static_cast<const RDORTPEnumParamType*>(this)->enu->src_info(), static_cast<const RDORTPEnumParamType*>(this)->enu->src_text() );
				}
				getParser()->error_push_done();
			}
			break;
		}
	}
}

void RDORTPParamType::checkParamType( const rdoRuntime::RDOValue& value, const RDOParserSrcInfo& value_info ) const
{
	switch ( getType() ) {
		case RDORTPParamType::pt_int: {
			checkRSSIntValue( value, value_info );
			break;
		}
		case RDORTPParamType::pt_real: {
			checkRSSRealValue( value, value_info );
			break;
		}
		case RDORTPParamType::pt_enum: {
			if ( static_cast<const RDORTPEnumParamType*>(this)->enu->findEnumValueWithoutThrow( value_info.src_text() ) == -1 ) {
				getParser()->error( value_info, rdo::format("�������� '%s' �� �������� ��������� ������������� ���������: %s", value_info.src_text().c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
			}
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	name( _name ),
	parType( _parType ),
	resType( NULL )
{
	setSrcText( name + ": " + parType->src_text() );
}

RDORTPParam::RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo(),
	name( _name ),
	parType( _parType ),
	resType( _parent )
{
	setSrcText( name + ": " + parType->src_text() );
}

RDORTPParam::RDORTPParam( RDORTPResType* _parent, const std::string& _name, const RDORTPParamType* const _parType, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	name( _name ),
	parType( _parType ),
	resType( _parent )
{
	setSrcText( name + ": " + parType->src_text() );
}

int RDORTPParam::writeModelStructure() const
{
	getParser()->modelStructure << getName() << " ";
	getType()->writeModelStructure();
//	getParser()->modelStructure << " ";
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
RDORTPResType::RDORTPResType( RDOParser* _parser, const std::string& _name, const bool _permanent ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo(),
	name( _name ),
	number( _parser->getRTP_id() ),
	permanent( _permanent )
{
	setSrcText( name );
	getParser()->insertRTPResType( this );
}

void RDORTPResType::addParam( const RDORTPParam* const param )
{
	if ( findRTPParam( param->getName() ) ) {
		getParser()->lexer_loc_restore();
		getParser()->error( rdoSimulator::RDOSyntaxError::RTP_SECOND_PARAM_NAME, param->getName().c_str() );
	}
	params.push_back( param );
}

const RDORTPParam* RDORTPResType::findRTPParam( const std::string& param ) const
{
	std::vector<const RDORTPParam *>::const_iterator it = std::find_if( params.begin(), params.end(), compareName2<RDORTPParam>(param) );
	return it != params.end() ? *it : NULL;
}

int RDORTPResType::getRTPParamNumber( const std::string& param ) const
{
	std::vector<const RDORTPParam *>::const_iterator it = std::find_if(params.begin(), 
		params.end(), 
		compareName2<RDORTPParam>(param));
	if(it != params.end())
		return it - params.begin();

	return -1;
}

int RDORTPResType::writeModelStructure() const
{
	getParser()->modelStructure << getNumber() << " " << getName() << " " << getParams().size() << std::endl;
	for(int i = 0; i < getParams().size(); i++)
	{
		getParser()->modelStructure << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure();
	}
	return 0;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntDiap - ��������, ��������, [1 .. 4]
// ----------------------------------------------------------------------------
RDORTPIntDiap::RDORTPIntDiap( RDOParser* _parser ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo(),
	exist( false ),
	min_value( 0 ),
	max_value( 0 )
{
}

RDORTPIntDiap::RDORTPIntDiap( RDOParser* _parser, const RDORTPIntDiap& diap ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( diap.src_info() ),
	exist( diap.exist ),
	min_value( diap.min_value ),
	max_value( diap.max_value )
{
}

RDORTPIntDiap::RDORTPIntDiap( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	exist( false ),
	min_value( 0 ),
	max_value( 0 )
{
}

RDORTPIntDiap::RDORTPIntDiap( RDOParser* _parser, int _min_value, int _max_value, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	exist( true ),
	min_value( _min_value ),
	max_value( _max_value )
{
	if ( min_value > max_value ) {
		getParser()->error( _max_value_pos, "����� ������� ��������� ������ ���� ������ ������" );
	}
	setSrcText( rdo::format("[%d..%d]", min_value, max_value) );
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
RDORTPIntParamType::RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPIntDefVal(0) ),
	diap( new RDORTPIntDiap(_parent->getParser()) )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPIntDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	diap( _diap )
{
	init();
}

void RDORTPIntParamType::init()
{
	diap->reparent( this );
	dv->reparent( this );
	std::string src_text = "integer";
	if ( diap->isExist() ) {
		src_text += " " + diap->src_text();
	}
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	if ( dv->isExist() ) {
		getRSSIntValue( static_cast<RDORTPIntDefVal*>(dv)->getIntValue(), dv->src_info() );
	}
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( *static_cast<RDORTPIntDefVal*>(dv) );
	RDORTPIntParamType* _type = new RDORTPIntParamType( getParent(), _diap, _dv, _src_info );
	_dv->setSrcInfo( _src_info );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPIntDiap*      _diap = new RDORTPIntDiap( *diap );
	RDORTPIntDefVal*    _dv   = new RDORTPIntDefVal( getParser(), defVal, defVal_info );
	RDORTPIntParamType* _type = new RDORTPIntParamType( getParent(), _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	getParser()->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������ ����: %f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPIntParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	getParser()->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������ ����: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPIntParamType::writeModelStructure() const
{
	getParser()->modelStructure << "I" << std::endl;
	return 0;
}

void RDORTPIntParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	getParser()->error( _src_info, rdo::format("��������� ����� �����, ������� '%s'", val.c_str()) );
}

void RDORTPIntParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	if ( diap->isExist() ) {
		if ( val < diap->min_value || val > diap->max_value ) {
			if ( _src_info.src_filetype() == diap->src_filetype() && _src_info.src_pos().last_line == diap->src_pos().last_line ) {
				getParser()->error( _src_info, rdo::format("�������� ������� �� ���������� �������� [%d..%d]: %d", diap->min_value, diap->max_value, val) );
			} else {
				getParser()->error_push_only( _src_info, rdo::format("�������� ������� �� ���������� �������� [%d..%d]: %d", diap->min_value, diap->max_value, val) );
				getParser()->error_push_only( diap->src_info(), rdo::format("��. �������� ���������") );
				getParser()->error_push_done();
			}
//			getParser()->error( ("integer value " + toString(val) + " out of range[" + toString(diap->min_value) + ", " + toString(diap->max_value) + "]").c_str() );
		}
	}
}

void RDORTPIntParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	getParser()->error( _src_info, rdo::format("��������� ����� �����, ������� '%f'", val) );
}

rdoRuntime::RDOValue RDORTPIntParamType::getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !dv->isExist() ) {
		getParser()->error( _src_info, "��� �������� ��-���������" );
//		getParser()->error( "No default value" );
	}
	return rdoRuntime::RDOValue( dv->getIntValue() );
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSEnumValue( val, _src_info );
	return 0; // �� ����������� ��-�� checkRSSEnumValue
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSIntValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPIntParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
	return 0;; // �� ����������� ��-�� checkRSSRealValue
}

int RDORTPIntParamType::getDiapTableFunc() const 
{
	if ( !diap->isExist() ) {
		getParser()->error( src_info(), "��� ��������� ��������� ������� ������ ���� ����� ���������� ��������" );
//		getParser()->error("integer table function parameter must have range");
	}
	if ( diap->min_value != 1 ) {
		getParser()->error( src_info(), rdo::format("����������� �������� ��������� ������ ���� 1, ������� �������� [%d..%d]", diap->min_value, diap->max_value) );
//		getParser()->error("integer table function parameter must have minimum value = 1");
	}
	return diap->max_value - diap->min_value + 1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealDiap - ��������, ��������, [1.2 .. 4.78]
// ----------------------------------------------------------------------------
RDORTPRealDiap::RDORTPRealDiap( RDOParser* _parser ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo(),
	exist( false ),
	min_value( 0 ),
	max_value( 0 )
{
}

RDORTPRealDiap::RDORTPRealDiap( RDOParser* _parser, const RDORTPRealDiap& diap ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( diap.src_info() ),
	exist( diap.exist ),
	min_value( diap.min_value ),
	max_value( diap.max_value )
{
}

RDORTPRealDiap::RDORTPRealDiap( RDOParser* _parser, const RDOParserSrcInfo& _src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	exist( false ),
	min_value( 0 ),
	max_value( 0 )
{
}

RDORTPRealDiap::RDORTPRealDiap( RDOParser* _parser, double _min_value, double _max_value, const RDOParserSrcInfo& _src_info, const YYLTYPE& _max_value_pos ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	exist( true ),
	min_value( _min_value ),
	max_value( _max_value )
{
	if ( min_value > max_value ) {
		getParser()->error( _max_value_pos, "����� ������� ��������� ������ ���� ������ ������" );
	}
	setSrcText( rdo::format("[%f..%f]", min_value, max_value) );
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
RDORTPRealParamType::RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPRealDefVal(0) ),
	diap( new RDORTPRealDiap(_parent->getParser()) )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPRealDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	diap( _diap )
{
	init();
}

void RDORTPRealParamType::init()
{
	diap->reparent( this );
	dv->reparent( this );
	std::string src_text = "real";
	if ( diap->isExist() ) {
		src_text += " " + diap->src_text();
	}
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	if ( dv->isExist() ) {
		getRSSRealValue( static_cast<RDORTPRealDefVal*>(dv)->getRealValue(), dv->src_info() );
	}
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( *static_cast<RDORTPRealDefVal*>(dv) );
	RDORTPRealParamType* _type = new RDORTPRealParamType( getParent(), _diap, _dv, _src_info );
	_dv->setSrcInfo( _src_info );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( getParser(), defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( getParent(), _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	RDORTPRealDiap*      _diap = new RDORTPRealDiap( *diap );
	RDORTPRealDefVal*    _dv   = new RDORTPRealDefVal( getParser(), defVal, defVal_info );
	RDORTPRealParamType* _type = new RDORTPRealParamType( getParent(), _diap, _dv, _src_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPRealParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	getParser()->error( defVal_info, rdo::format("�������� �������� ��-��������� ��� ��������� ������������� ����: %s", defVal.c_str()) );
	return NULL;
}

int RDORTPRealParamType::writeModelStructure() const
{
	getParser()->modelStructure << "R" << std::endl;
	return 0;
}

void RDORTPRealParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	getParser()->error( _src_info, rdo::format("��������� ������������ �����, ������� '%s'", val.c_str()) );
}

void RDORTPRealParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
}

void RDORTPRealParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	if ( diap->isExist() ) {
		if ( val < diap->min_value || val > diap->max_value ) {
			if ( _src_info.src_filetype() == diap->src_filetype() && _src_info.src_pos().last_line == diap->src_pos().last_line ) {
				getParser()->error( _src_info, rdo::format("�������� ������� �� ���������� �������� [%f..%f]: %f", diap->min_value, diap->max_value, val) );
			} else {
				getParser()->error_push_only( _src_info, rdo::format("�������� ������� �� ���������� �������� [%f..%f]: %f", diap->min_value, diap->max_value, val) );
				getParser()->error_push_only( diap->src_info(), rdo::format("��. �������� ���������") );
				getParser()->error_push_done();
			}
//			getParser()->error(("real value " + toString(*val) + " out of range[" + toString(diap->min_value) + ", " + toString(diap->max_value) + "]").c_str());
		}
	}
}

rdoRuntime::RDOValue RDORTPRealParamType::getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !dv->isExist() ) {
		getParser()->error( _src_info, "��� �������� ��-���������" );
	}
	return rdoRuntime::RDOValue( dv->getRealValue() );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSEnumValue( val, _src_info );
	return 0; // �� ����������� ��-�� checkRSSEnumValue
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSIntValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

rdoRuntime::RDOValue RDORTPRealParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSRealValue( val, _src_info );
	return rdoRuntime::RDOValue( val );
}

int RDORTPRealParamType::getDiapTableFunc() const
{
	getParser()->error( src_info(), "�������� ��������� ������� ����� ���� ������ ��� ������������� ����" );
//	getParser()->error( "unexpected real table function parameter" );
	return 0;		// unreachable code...
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
void RDORTPEnum::add( const std::string& next, const YYLTYPE& _pos )
{
	if ( std::find( enumVals.begin(), enumVals.end(), next ) != enumVals.end() ) {
		getParser()->error( _pos, rdoSimulator::RDOSyntaxError::RTP_SECOND_ENUM_VALUE, next.c_str() );
	}
	enumVals.push_back( next );
}

int RDORTPEnum::findEnumValueWithThrow( const RDOParserSrcInfo& _src_info, const std::string& val ) const
{
	int result = findEnumValueWithoutThrow( val );
	if ( result == -1 ) {
		getParser()->error_push_only( _src_info, rdoSimulator::RDOSyntaxError::RTP_WRONG_ENUM_PARAM_VALUE, val.c_str() );
		getParser()->error_push_only( src_info(), rdo::format("��������� ��������: %s", src_text().c_str()) );
		getParser()->error_push_done();
	}
	return result;
}

int RDORTPEnum::findEnumValueWithoutThrow( const std::string& val ) const
{
	std::vector< std::string >::const_iterator it = std::find( enumVals.begin(), enumVals.end(), val );
	return it != enumVals.end() ? it - enumVals.begin() : -1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
RDORTPEnumParamType::RDORTPEnumParamType( const RDOParserObject* _parent, RDORTPEnum* _enu, RDORTPEnumDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	enu( _enu ),
	enum_name( "" ),
	enum_fun( false )
{
	enu->reparent( this );
	dv->reparent( this );
	init_src_info();
	getParser()->insertEnum( this );
}

void RDORTPEnumParamType::init_src_info()
{
	std::string src_text = enu->src_text();
	if ( dv->isExist() ) {
		src_text += " = " + dv->src_text();
	}
	setSrcText( src_text );
	setSrcPos( enu->getPosAsYY(), dv->getPosAsYY() );
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const RDOParserSrcInfo& _src_info ) const 
{ 
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( *static_cast<RDORTPEnumDefVal*>(dv) );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( getParent(), _enu, _dv, _src_info );
	_dv->setSrcFileType( _src_info.src_filetype() );
	_dv->setSrcPos( _src_info.src_pos().last_line, _src_info.src_pos().last_pos, _src_info.src_pos().last_line, _src_info.src_pos().last_pos );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( int defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	enu->findEnumValueWithThrow( defVal_info, rdo::format("%d", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( double defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const
{
	enu->findEnumValueWithThrow( defVal_info, rdo::format("%f", defVal) );
	return NULL;
}

const RDORTPParamType* RDORTPEnumParamType::constructSuchAs( const std::string& defVal, const RDOParserSrcInfo& _src_info, const RDOParserSrcInfo& defVal_info ) const 
{
	enu->findEnumValueWithThrow( defVal_info, defVal );
	RDORTPEnumDefVal*    _dv   = new RDORTPEnumDefVal( getParser(), defVal );
	RDORTPEnum*          _enu  = enu;
	RDORTPEnumParamType* _type = new RDORTPEnumParamType( getParent(), _enu, _dv, _src_info );
	_dv->setSrcInfo( defVal_info );
	_type->setSrcText( _src_info.src_text() );
	if ( _dv->isExist() ) {
		_type->setSrcText( _type->src_text() + " = " + _dv->src_text() );
	}
	return _type;
}

int RDORTPEnumParamType::writeModelStructure() const
{
	getParser()->modelStructure << "E " << enu->enumVals.size() << std::endl;
	for(int i = 0; i < enu->enumVals.size(); i++)
		getParser()->modelStructure << "    " << i << " " << enu->enumVals.at(i) << std::endl;
	return 0;
}

void RDORTPEnumParamType::checkRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	enu->findEnumValueWithThrow( _src_info, val );
}

void RDORTPEnumParamType::checkRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const
{
	getParser()->error_push_only( _src_info, rdo::format("��������� ������������ ���, ������� '%d'", val) );
	if ( src_info().src_pos().last_line != _src_info.src_pos().last_line ) {
		getParser()->error_push_only( src_info(), rdo::format("��. ������������: %s", src_text().c_str()) );
	}
	getParser()->error_push_done();
}

void RDORTPEnumParamType::checkRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	getParser()->error( _src_info, rdo::format("��������� ������������ ���, ������� '%f'", val) );
	if ( src_info().src_pos().last_line != _src_info.src_pos().last_line ) {
		getParser()->error_push_only( src_info(), rdo::format("��. ������������: %s", src_text().c_str()) );
	}
	getParser()->error_push_done();
}

rdoRuntime::RDOValue RDORTPEnumParamType::getParamDefaultValue( const RDOParserSrcInfo& _src_info ) const 
{
	if ( !dv->isExist() ) {
		getParser()->error( _src_info, "��� �������� ��-���������" );
	}
	return rdoRuntime::RDOValue( enu->findEnumValueWithThrow( _src_info, dv->getEnumValue() ) );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSEnumValue( const std::string& val, const RDOParserSrcInfo& _src_info ) const
{
	return enu->findEnumValueWithThrow( _src_info, val );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSIntValue( int val, const RDOParserSrcInfo& _src_info ) const 
{
	checkRSSIntValue( val, _src_info );
	return 0;; // �� ����������� ��-�� checkRSSIntValue
}

rdoRuntime::RDOValue RDORTPEnumParamType::getRSSRealValue( double val, const RDOParserSrcInfo& _src_info ) const
{
	checkRSSRealValue( val, _src_info );
	return 0;; // �� ����������� ��-�� checkRSSRealValue
}

int RDORTPEnumParamType::getDiapTableFunc() const 
{
	return enu->enumVals.size();
}

} // namespace rdoParse
