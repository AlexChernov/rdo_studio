#include "pch.h"
#include "rdoparser_enum.h"
#include "rdoparser.h"

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
RDORTPEnum::RDORTPEnum( const RDOParserObject* _parent, const RDOValue& first ):
	RDOType( g_unknow ),
	RDOParserObject( _parent )
{
	m_type = new rdoRuntime::RDOEnumType( parser()->runtime() );
	add( first );
}

RDORTPEnum::~RDORTPEnum()
{
}

const RDOType* RDORTPEnum::cast( const RDOType& toType ) const
{
	switch ( toType->typeID() )
	{
		case rdoRuntime::RDOType__int::t_enum: return operator==(static_cast<const RDORTPEnum&>(toType)) ? this : NULL;
	}
	return NULL;
}

void RDORTPEnum::add( const RDOValue& next )
{
	if ( __enum()->findEnum( next->getIdentificator() ) != rdoRuntime::RDOEnumType::END )
	{
		parser()->error( next.src_info(), rdo::format("�������� ������������� ���� ��� ����������: %s", next.src_text().c_str()) );
	}
	__enum()->add( next->getIdentificator() );
}

rdoRuntime::RDOValue RDORTPEnum::findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const
{
	unsigned int result = __enum()->findEnum( val );
	if ( result == rdoRuntime::RDOEnumType::END )
	{
		parser()->error_push_only( val_src_info, rdo::format("�������� �������� ��������� ������������� ����: %s", val.c_str()) );
		parser()->error_push_only( src_info(), rdo::format("��������� ��������: %s", __enum()->asString().c_str()) );
		parser()->error_push_done();
	}
	return rdoRuntime::RDOValue( *__enum(), val );
}

rdoRuntime::RDOValue RDORTPEnum::getFirstValue() const
{
	if ( __enum()->getValues().empty() )
	{
		parser()->error( src_info(), "���������� ������: ������ ������������ ���" );
	}
	return rdoRuntime::RDOValue( *__enum() );
}

} // namespace rdoParse
