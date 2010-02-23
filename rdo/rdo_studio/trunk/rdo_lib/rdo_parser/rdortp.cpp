#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoParse 
{

int rtplex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rtperror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDORTPParamType
// ----------------------------------------------------------------------------
void RDORTPParamType::checkParamType( const RDOFUNArithm* const action ) const
{
	switch ( typeID() )
	{
		case rdoRuntime::RDOType::t_int:
		{
			if ( action->typeID() == rdoRuntime::RDOType::t_real )
			{
				parser()->error().warning( action->src_info(), "������� ������������� ����� � �����, �������� ������ ������" );
			}
			else if ( action->typeID() != rdoRuntime::RDOType::t_int )
			{
				parser()->error().error( action->src_info(), "�������������� �����. ��������� ������������� ��������" );
			}
			rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(action->calc());
			if ( calc_const )
			{
				rdoRuntime::RDOValue value = calc_const->calcValue( parser()->runtime() );
				checkValue( RDOValue(value, action->src_info()) );
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			if ( action->typeID() != rdoRuntime::RDOType::t_real && action->typeID() != rdoRuntime::RDOType::t_int )
			{
				parser()->error().error( action->src_info(), "�������������� �����. ��������� ������������ ��������" );
			}
			else
			{
				rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<rdoRuntime::RDOCalcConst*>(action->calc());
				if ( calc_const )
				{
					rdoRuntime::RDOValue value = calc_const->calcValue( parser()->runtime() );
					checkValue( RDOValue(value, action->src_info()) );
				}
			}
			break;
		}
		case rdoRuntime::RDOType::t_bool:
		{
			if ( action->typeID() != rdoRuntime::RDOType::t_bool )
			{
				parser()->error().error( action->src_info(), "�������������� �����. ��������� ��������� ��������" );
			}
			break;
		}
		case rdoRuntime::RDOType::t_enum:
		{
			if ( action->typeID() == rdoRuntime::RDOType::t_identificator )
			{
				if ( static_cast<const RDORTPEnumParamType*>(this)->m_enum->getEnums().findEnum( action->value()->getAsString() ) == rdoRuntime::RDOEnumType::END )
				{
					if ( static_cast<const RDORTPEnumParamType*>(this)->enum_fun )
					{
						parser()->error().error( action->src_info(), rdo::format("�������� '%s' �� ����� �������� ����������� �������: %s", action->value()->getAsString().c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
					else
					{
						parser()->error().error( action->src_info(), rdo::format("�������� '%s' �� �������� ��������� ������������� ���������: %s", action->value()->getAsString().c_str(), static_cast<const RDORTPEnumParamType*>(this)->enum_name.c_str()) );
					}
				}
			}
			else if ( action->typeID() != rdoRuntime::RDOType::t_enum )
			{
				parser()->error().push_only( action->src_info(), rdo::format("�������������� �����. ��������� ������������ ���: %s", src_text().c_str()) );
				parser()->error().push_only( static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_info(), rdo::format("��������� ��������: %s", static_cast<const RDORTPEnumParamType*>(this)->m_enum->getEnums().asString().c_str()) );
				parser()->error().push_done();
			}
			else if ( &action->enumType() != static_cast<const RDORTPEnumParamType*>(this)->m_enum )
			{
				if ( action->enumType() == *static_cast<const RDORTPEnumParamType*>(this)->m_enum )
				{
					parser()->error().push_only( action->src_info(), "������������ ��������� ������������ ���� � ����������� ����������" );
					parser()->error().push_only( static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_info(), static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_text() );
					parser()->error().push_only( action->src_info(), "�" );
					parser()->error().push_only( action->enumType().src_info(), action->enumType().src_text() );
					parser()->error().push_only( action->src_info(), "��������, ������� ������������ ������ �� ��� ��� ������������, � ������ ��� such_as �� ����, ����� ��������� ����� ����� ���������� � �����������" );
				}
				else
				{
					parser()->error().push_only( action->src_info(), "�������������� ������������ �����" );
					parser()->error().push_only( action->enumType().src_info(), action->enumType().src_text() );
					parser()->error().push_only( action->src_info(), "�" );
					parser()->error().push_only( static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_info(), static_cast<const RDORTPEnumParamType*>(this)->m_enum->src_text() );
				}
				parser()->error().push_done();
			}
			break;
		}
		case rdoRuntime::RDOType::t_string:
		{
			if ( action->typeID() != rdoRuntime::RDOType::t_string )
			{
				parser()->error().error( action->src_info(), "�������������� �����. ��������� ������" );
			}
			break;
		}
		default: parser()->error().error( src_info(), "���������� ������: ���������� ��� ���� RDOValue" );
	}
}

rdoRuntime::RDOValue RDORTPParamType::getDefaultValue( const RDOValue& value ) const 
{
	if ( !m_dv->isExist() )
	{
		parser()->error().push_only( value.src_info(), "��� �������� ��-���������" );
		parser()->error().push_only( src_info(), "��. �������� ���������" );
		parser()->error().push_done();
	}
	return m_dv->value().value();
}

ruint RDORTPParamType::getDiapTableFunc() const
{
	parser()->error().error( src_info(), "�������� ��������� ������� ����� ���� ������ ��� ������������� ����" );
	return 0;		// unreachable code...
}

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
RDORTPParam::RDORTPParam( RDORTPResType* _parent, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _src_info ),
	m_parType( _parType ),
	m_resType( _parent )
{
}

RDORTPParam::RDORTPParam( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	m_parType( _parType ),
	m_resType( NULL )
{
}

void RDORTPParam::writeModelStructure( std::ostream& stream ) const
{
	stream << name() << " ";
	getType()->writeModelStructure( stream );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNConst
// ----------------------------------------------------------------------------
RDOFUNConst::RDOFUNConst( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _parType ):
	RDORTPParam( _parser, _src_info, _parType )
{
}

// ----------------------------------------------------------------------------
// ---------- RDORTPResType
// ----------------------------------------------------------------------------
RDORTPResType::RDORTPResType(PTR(RDOParser) _parser, CREF(RDOParserSrcInfo) _src_info, rbool _permanent):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	m_number( _parser->getRTP_id() ),
	m_permanent( _permanent )
{
	parser()->insertRTPResType( this );
}

RDORTPResType::~RDORTPResType()
{
}

void RDORTPResType::addParam(CPTRC(RDORTPParam) param)
{
	if (findRTPParam(param->name()))
	{
		parser()->error().error(param->src_info(), rdo::format("�������� ��� ����������: %s", param->name().c_str()));
	}
	m_params.push_back(param);
}

void RDORTPResType::addParam(CREF(tstring) param_name, rdoRuntime::RDOType::TypeID param_typeID)
{
}

const RDORTPParam* RDORTPResType::findRTPParam(CREF(tstring) param) const
{
	std::vector< const RDORTPParam* >::const_iterator it = std::find_if( m_params.begin(), m_params.end(), compareName<RDORTPParam>(param) );
	return it != m_params.end() ? *it : NULL;
}

ruint RDORTPResType::getRTPParamNumber(CREF(tstring) param) const
{
	std::vector< const RDORTPParam* >::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(param) );
	return it != m_params.end() ? it - m_params.begin() : UNDEFINED_PARAM;
}

void RDORTPResType::writeModelStructure( std::ostream& stream ) const
{
	stream << getNumber() << " " << name() << " " << getParams().size() << std::endl;
	for ( ruint i = 0; i < getParams().size(); i++ )
	{
		stream << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure( stream );
	}
}

// ----------------------------------------------------------------------------
// ---------- RDORTPIntParamType
// ----------------------------------------------------------------------------
RDORTPIntParamType::RDORTPIntParamType( RDOParser* _parser, RDORTPIntDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPDefVal(_parent->parser()) ),
	m_diap( new RDORTPIntDiap(_parent->parser()) )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPIntParamType::RDORTPIntParamType( const RDOParserObject* _parent, RDORTPIntDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	m_diap( _diap )
{
	init();
}

void RDORTPIntParamType::init()
{
	m_diap->reparent( this );
	m_dv->reparent( this );
	std::string src_text = "integer";
	if ( m_diap->isExist() ) {
		src_text += " " + m_diap->src_text();
	}
	if ( m_dv->isExist() ) {
		src_text += " = " + m_dv->value().src_text();
	}
	setSrcText( src_text );
	if ( m_dv->isExist() )
	{
		checkValue( m_dv->value() );
	}
}

RDORTPParamType* RDORTPIntParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_int )
	{
		parser()->error().error( defValue.src_info(), rdo::format("�������� �������� ��-��������� ��� ��������� ������ ����: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPIntDiap*      diap = new RDORTPIntDiap( *m_diap );
	RDORTPIntParamType* type = new RDORTPIntParamType( parent(), diap, dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPIntParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "I" << std::endl;
}

void RDORTPIntParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_int )
	{
		if ( m_diap->isExist() )
		{
			if ( value.value().getInt() < m_diap->getMin() || value.value().getInt() > m_diap->getMax() )
			{
				if ( value.src_filetype() == m_diap->src_filetype() && value.src_pos().m_last_line == m_diap->src_pos().m_last_line )
				{
					parser()->error().error( value.src_info(), rdo::format("�������� ������� �� ���������� �������� [%d..%d]: %d", m_diap->getMin(), m_diap->getMax(), value.value().getInt()) );
				}
				else
				{
					parser()->error().push_only( value.src_info(), rdo::format("�������� ������� �� ���������� �������� [%d..%d]: %d", m_diap->getMin(), m_diap->getMax(), value.value().getInt()) );
					parser()->error().push_only( m_diap->src_info(), rdo::format("��. �������� ���������") );
					parser()->error().push_done();
				}
			}
		}
	}
	else
	{
		parser()->error().error( value.src_info(), rdo::format("��������� ����� �����, ������� '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPIntParamType::getValue( const RDOValue& value ) const
{
	checkValue(value);
	return type().value_cast(value.value());
}

ruint RDORTPIntParamType::getDiapTableFunc() const 
{
	if ( !m_diap->isExist() ) {
		parser()->error().error( src_info(), "��� ��������� ��������� ������� ������ ���� ����� ���������� ��������" );
	}
	if ( m_diap->getMin() != 1 ) {
		parser()->error().error( src_info(), rdo::format("����������� �������� ��������� ������ ���� 1, ������� �������� [%d..%d]", m_diap->getMin(), m_diap->getMax()) );
	}
	return m_diap->getMax() - m_diap->getMin() + 1;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPRealParamType
// ----------------------------------------------------------------------------
RDORTPRealParamType::RDORTPRealParamType( RDOParser* _parser, RDORTPRealDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parser, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent ):
	RDORTPParamType( _parent, new RDORTPDefVal(_parent->parser()) ),
	m_diap( new RDORTPRealDiap(_parent->parser()) )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv ):
	RDORTPParamType( _parent, _dv ),
	m_diap( _diap )
{
	init();
}

RDORTPRealParamType::RDORTPRealParamType( const RDOParserObject* _parent, RDORTPRealDiap* _diap, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	m_diap( _diap )
{
	init();
}

void RDORTPRealParamType::init()
{
	m_diap->reparent( this );
	m_dv->reparent( this );
	std::string src_text = "real";
	if ( m_diap->isExist() )
	{
		src_text += " " + m_diap->src_text();
	}
	if ( m_dv->isExist() )
	{
		src_text += " = " + m_dv->value().src_text();
	}
	setSrcText( src_text );
	if ( m_dv->isExist() )
	{
		checkValue( m_dv->value() );
	}
}

RDORTPParamType* RDORTPRealParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_int && defValue.value().typeID() != rdoRuntime::RDOType::t_real)
	{
		parser()->error().error( defValue.src_info(), rdo::format("�������� �������� ��-��������� ��� ��������� ������������� ����: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPRealDiap*      diap = new RDORTPRealDiap( *m_diap );
	RDORTPRealParamType* type = new RDORTPRealParamType( parent(), diap, dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPRealParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "R" << std::endl;
}

void RDORTPRealParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_real || value.value().typeID() == rdoRuntime::RDOType::t_int )
	{
		if ( m_diap->isExist() )
		{
			if ( value.value().getDouble() < m_diap->getMin() || value.value().getDouble() > m_diap->getMax() )
			{
				if ( value.src_filetype() == m_diap->src_filetype() && value.src_pos().m_last_line == m_diap->src_pos().m_last_line )
				{
					parser()->error().error( value.src_info(), rdo::format("�������� ������� �� ���������� �������� [%f..%f]: %f", m_diap->getMin(), m_diap->getMax(), value.value().getDouble()) );
				}
				else
				{
					parser()->error().push_only( value.src_info(), rdo::format("�������� ������� �� ���������� �������� [%f..%f]: %f", m_diap->getMin(), m_diap->getMax(), value.value().getDouble()) );
					parser()->error().push_only( m_diap->src_info(), rdo::format("��. �������� ���������") );
					parser()->error().push_done();
				}
			}
		}
	}
	else
	{
		parser()->error().error( value.src_info(), rdo::format("��������� ������������ �����, ������� '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPRealParamType::getValue( const RDOValue& value ) const
{
	checkValue(value);
	return type().value_cast(value.value());
}

// ----------------------------------------------------------------------------
// ---------- RDORTPEnumParamType
// ----------------------------------------------------------------------------
RDORTPEnumParamType::RDORTPEnumParamType( const RDOParserObject* _parent, RDOEnumType* _enu, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info ),
	m_enum( _enu ),
	enum_name( "" ),
	enum_fun( false )
{
	m_dv->reparent( this );
	init_src_info();
	parser()->insertRTPEnumParamType( this );
}

void RDORTPEnumParamType::init_src_info()
{
	std::string src_text = m_enum->src_text();
	if ( m_dv->isExist() )
	{
		src_text += " = " + m_dv->value().src_text();
		setSrcPos( m_enum->getPosAsYY(), m_dv->value().getPosAsYY() );
	}
	else
	{
		setSrcPos( m_enum->getPosAsYY() );
	}
	setSrcText( src_text );
}

RDORTPParamType* RDORTPEnumParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_enum && defValue.value().typeID() != rdoRuntime::RDOType::t_identificator)
	{
		parser()->error().error( defValue.src_info(), rdo::format("�������� �������� ��-��������� ��� ��������� ������������� ����: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		m_enum->findEnumValueWithThrow( such_as_src_info, defValue.value().getAsString() );
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcFileType( such_as_src_info.src_filetype() );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDOEnumType*          enu  = m_enum;
	RDORTPEnumParamType* type = new RDORTPEnumParamType( parent(), enu, dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPEnumParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "E " << m_enum->getEnums().getValues().size() << std::endl;
	for ( ruint i = 0; i < m_enum->getEnums().getValues().size(); i++ )
	{
		stream << "    " << i << " " << m_enum->getEnums().getValues().at(i) << std::endl;
	}
}

void RDORTPEnumParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_enum || value.value().typeID() == rdoRuntime::RDOType::t_identificator )
	{
		m_enum->findEnumValueWithThrow( value.src_info(), value.value().getAsString() );
	}
	else
	{
		parser()->error().push_only( value.src_info(), rdo::format("��������� ������������ ���, ������� '%s'", value.value().getAsString().c_str()) );
		if ( src_pos().m_last_line != value.src_pos().m_last_line )
		{
			parser()->error().push_only( src_info(), rdo::format("��. ������������: %s", src_text().c_str()) );
		}
		parser()->error().push_done();
	}
}

rdoRuntime::RDOValue RDORTPEnumParamType::getValue( const RDOValue& value ) const
{
	checkValue(value);
	return m_enum->findEnumValueWithThrow( value.src_info(), value.value().getAsString() );
}

rdoRuntime::RDOValue RDORTPEnumParamType::getDefaultValue( const RDOValue& value ) const 
{
	rdoRuntime::RDOValue identificator = RDORTPParamType::getDefaultValue( value );
	return m_enum->findEnumValueWithThrow( value, identificator.getAsString() );
}

ruint RDORTPEnumParamType::getDiapTableFunc() const 
{
	return m_enum->getEnums().getValues().size();
}

// ----------------------------------------------------------------------------
// ---------- RDORTPStringParamType
// ----------------------------------------------------------------------------
RDORTPStringParamType::RDORTPStringParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info )
{
}

RDORTPParamType* RDORTPStringParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_string)
	{
		parser()->error().error( defValue.src_info(), rdo::format("�������� �������� ��-��������� ��� ��������� ���������� ����: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPStringParamType* type = new RDORTPStringParamType( parent(), dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPStringParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_string )
	{
	}
	else
	{
		parser()->error().error( value.src_info(), rdo::format("��������� ������, ������� '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPStringParamType::getValue( const RDOValue& value ) const
{
	checkValue(value);
	return type().value_cast(value.value());
}

void RDORTPStringParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "S" << std::endl;
}

// ----------------------------------------------------------------------------
// ---------- RDORTPBoolParamType
// ----------------------------------------------------------------------------
RDORTPBoolParamType::RDORTPBoolParamType( const RDOParserObject* _parent, RDORTPDefVal* _dv, const RDOParserSrcInfo& _src_info ):
	RDORTPParamType( _parent, _dv, _src_info )
{
}

RDORTPParamType* RDORTPBoolParamType::constructorSuchAs( const RDOParserSrcInfo& such_as_src_info, const RDOValue& defValue ) const
{
	if ( defValue.defined() && defValue.value().typeID() != rdoRuntime::RDOType::t_bool)
	{
		parser()->error().error( defValue.src_info(), rdo::format("�������� �������� ��-��������� ��� ��������� ���������� ����: %s", defValue.value().getAsString().c_str()) );
	}
	RDORTPDefVal* dv;
	if ( defValue.defined() )
	{
		dv = new RDORTPDefVal( parser(), defValue );
	}
	else
	{
		dv = new RDORTPDefVal( *m_dv );
		dv->setSrcInfo( such_as_src_info );
		dv->setSrcPos( such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos, such_as_src_info.src_pos().m_last_line, such_as_src_info.src_pos().m_last_pos );
	}
	RDORTPBoolParamType* type = new RDORTPBoolParamType( parent(), dv, such_as_src_info );
	type->setSrcText( such_as_src_info.src_text() );
	if ( dv->isExist() )
	{
		type->setSrcText( type->src_text() + " = " + dv->value().src_text() );
	}
	return type;
}

void RDORTPBoolParamType::checkValue( const RDOValue& value ) const
{
	if ( value.value().typeID() == rdoRuntime::RDOType::t_bool )
	{
	}
	else
	{
		parser()->error().error( value.src_info(), rdo::format("��������� ��������� ��������, ������� '%s'", value.value().getAsString().c_str()) );
	}
}

rdoRuntime::RDOValue RDORTPBoolParamType::getValue( const RDOValue& value ) const
{
	checkValue(value);
	return type().value_cast(value.value());
}

void RDORTPBoolParamType::writeModelStructure( std::ostream& stream ) const
{
	stream << "B" << std::endl;
}

/*
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyMembershiftFun - �-�� �������������� ��������� �����
// ----------------------------------------------------------------------------
RDORTPFuzzyMembershiftFun::RDORTPFuzzyMembershiftFun( RDOParser* _parser ):
	RDOParserObject( _parser )
{
/*	for ( ruint i = 0; i < m_points.size(); i++ )
	{
//		double x = m_points[i]->getX();
	}

	Items::iterator it = m_points.begin();
	while ( it != m_points.end() )
	{
		double x = (*it)->getX();
		it++;
	}
}
// ----------------------------------------------------------------------------
// ---------- RDORTPFuzzyTerm - �������� ������
// ----------------------------------------------------------------------------
RDORTPFuzzyTerm::RDORTPFuzzyTerm( RDOParser* _parser, const RDOParserSrcInfo& _src_info, RDORTPFuzzyMembershiftFun* membersfift_fun):
	RDOParserObject( _parser )
{

}*/

} // namespace rdoParse
