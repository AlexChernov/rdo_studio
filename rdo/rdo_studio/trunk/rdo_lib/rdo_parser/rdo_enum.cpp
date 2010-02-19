/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_enum.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_enum.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
RDORTPEnum::RDORTPEnum(CREF(RDOValue) first)
	: RDOTypeParam(rdoRuntime::g_unknow, RDOParserSrcInfo())
{
	m_type = new rdoRuntime::RDOEnumType(RDOParser::s_parser()->runtime());
	add(first);
}

RDORTPEnum::~RDORTPEnum()
{}

CPTR(RDOType) RDORTPEnum::type_cast(CREF(RDOType) toType) const
{
	switch (toType->typeID())
	{
		case rdoRuntime::RDOType__int::t_enum: return operator==(static_cast<CREF(RDORTPEnum)>(toType)) ? this : NULL;
	}
	return NULL;
}

rdoRuntime::RDOValue RDORTPEnum::value_cast(CREF(rdoRuntime::RDOValue) from) const
{
	return m_type->value_cast(from);
}

void RDORTPEnum::add(CREF(RDOValue) next)
{
	if (__enum()->findEnum(next->getIdentificator()) != rdoRuntime::RDOEnumType::END)
	{
		rdoParse::g_error().error(next.src_info(), rdo::format(_T("�������� ������������� ���� ��� ����������: %s"), next.src_text().c_str()));
	}
	__enum()->add(next->getIdentificator());
}

rdoRuntime::RDOValue RDORTPEnum::findEnumValueWithThrow(CREF(RDOParserSrcInfo) src_info, CREF(tstring) value) const
{
	rdoRuntime::RDOValue result = value_cast(rdoRuntime::RDOValue(value, rdoRuntime::g_identificator));
	if (result.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info, rdo::format(_T("�������� �������� ��������� ������������� ����: %s"), value.c_str()));
		rdoParse::g_error().push_only(this->src_info(), rdo::format(_T("��������� ��������: %s"), __enum()->asString().c_str()));
		rdoParse::g_error().push_done();
	}
	return result;
}

rdoRuntime::RDOValue RDORTPEnum::getFirstValue() const
{
	if (__enum()->getValues().empty())
	{
		rdoParse::g_error().error(src_info(), _T("���������� ������: ������ ������������ ���"));
	}
	return rdoRuntime::RDOValue(*__enum());
}

CLOSE_RDO_PARSER_NAMESPACE
