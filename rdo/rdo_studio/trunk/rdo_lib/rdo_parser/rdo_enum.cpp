#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdoparser_enum.h"
#include "rdo_lib/rdo_parser/rdoparser.h"

namespace rdoParse 
{

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
RDORTPEnum::RDORTPEnum(CPTR(RDOParserObject) parent, CREF(RDOValue) first)
	: RDOType        (rdoRuntime::g_unknow)
	, RDOParserObject(parent              )
{
	m_type = new rdoRuntime::RDOEnumType(parser()->runtime());
	add(first);
}

RDORTPEnum::~RDORTPEnum()
{
}

CPTR(RDOType) RDORTPEnum::cast(CREF(RDOType) toType) const
{
	switch (toType->typeID())
	{
		case rdoRuntime::RDOType__int::t_enum: return operator==(static_cast<CREF(RDORTPEnum)>(toType)) ? this : NULL;
	}
	return NULL;
}

rdoRuntime::RDOValue RDORTPEnum::cast(CREF(rdoRuntime::RDOValue) from) const
{
	return m_type->cast(from);
}

void RDORTPEnum::add( const RDOValue& next )
{
	if ( __enum()->findEnum( next->getIdentificator() ) != rdoRuntime::RDOEnumType::END )
	{
		parser()->error().error( next.src_info(), rdo::format("�������� ������������� ���� ��� ����������: %s", next.src_text().c_str()) );
	}
	__enum()->add( next->getIdentificator() );
}

rdoRuntime::RDOValue RDORTPEnum::findEnumValueWithThrow( const RDOParserSrcInfo& val_src_info, const std::string& val ) const
{
	rdoRuntime::RDOValue result = cast(rdoRuntime::RDOValue(val, rdoRuntime::g_identificator));
	if (result.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		parser()->error().push_only( val_src_info, rdo::format("�������� �������� ��������� ������������� ����: %s", val.c_str()) );
		parser()->error().push_only( src_info(), rdo::format("��������� ��������: %s", __enum()->asString().c_str()) );
		parser()->error().push_done();
	}
	return result;
}

rdoRuntime::RDOValue RDORTPEnum::getFirstValue() const
{
	if ( __enum()->getValues().empty() )
	{
		parser()->error().error( src_info(), "���������� ������: ������ ������������ ���" );
	}
	return rdoRuntime::RDOValue( *__enum() );
}

} // namespace rdoParse
