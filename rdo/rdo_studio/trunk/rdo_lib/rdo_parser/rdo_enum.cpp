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
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPEnum
// ----------------------------------------------------------------------------
RDORTPEnum::RDORTPEnum(CPTR(RDOParserObject) parent, CREF(RDOValue) first)
	: RDOTypeParam(rdoRuntime::g_unknow, parent, RDOParserSrcInfo())
{
	m_type = new rdoRuntime::RDOEnumType(parser()->runtime());
	add(first);
}

RDORTPEnum::~RDORTPEnum()
{}

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

void RDORTPEnum::add(CREF(RDOValue) next)
{
	if (__enum()->findEnum(next->getIdentificator()) != rdoRuntime::RDOEnumType::END)
	{
		parser()->error().error(next.src_info(), rdo::format(_T("�������� ������������� ���� ��� ����������: %s"), next.src_text().c_str()));
	}
	__enum()->add(next->getIdentificator());
}

rdoRuntime::RDOValue RDORTPEnum::findEnumValueWithThrow(CREF(RDOParserSrcInfo) src_info, CREF(tstring) value) const
{
	rdoRuntime::RDOValue result = cast(rdoRuntime::RDOValue(value, rdoRuntime::g_identificator));
	if (result.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		parser()->error().push_only(src_info, rdo::format(_T("�������� �������� ��������� ������������� ����: %s"), value.c_str()));
		parser()->error().push_only(this->src_info(), rdo::format(_T("��������� ��������: %s"), __enum()->asString().c_str()));
		parser()->error().push_done();
	}
	return result;
}

rdoRuntime::RDOValue RDORTPEnum::getFirstValue() const
{
	if (__enum()->getValues().empty())
	{
		parser()->error().error(src_info(), _T("���������� ������: ������ ������������ ���"));
	}
	return rdoRuntime::RDOValue(*__enum());
}

CLOSE_RDO_PARSER_NAMESPACE
