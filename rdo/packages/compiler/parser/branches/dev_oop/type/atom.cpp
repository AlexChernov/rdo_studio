/*!
  \copyright (c) RDO-Team, 2011
  \file      atom.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/atom.h"
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ATOM_TYPE_PARSER
// --------------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName) \
tstring RDOType__##Type::name() const            \
{                                                \
	return TypeName;                             \
}

DECLARE_ATOM_TYPE_PARSER(unknow,        _T("unknow")       );
DECLARE_ATOM_TYPE_PARSER(void,          _T("void")         );
DECLARE_ATOM_TYPE_PARSER(identificator, _T("identificator"));
DECLARE_ATOM_TYPE_PARSER(int,           _T("integer")      );
DECLARE_ATOM_TYPE_PARSER(real,          _T("real")         );
DECLARE_ATOM_TYPE_PARSER(bool,          _T("bool")         );
DECLARE_ATOM_TYPE_PARSER(string,        _T("string")       );

// --------------------------------------------------------------------------------
// -------------------- RDOType__unknow
// --------------------------------------------------------------------------------
LPRDOType RDOType__unknow::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(pFrom      );
	UNUSED(to_src_info);

	rdoParser::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � ������������ ����"), from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__unknow::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	rdoParser::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ������������ ����"), pFrom->src_info().src_text().c_str()));
	rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
	rdoParser::g_error().push_done();
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__unknow::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__unknow::get_default() const
{
	return rdo::runtime::RDOValue();
}

void RDOType__unknow::writeModelStructure(REF(std::ostream) stream) const
{
	UNUSED(stream);
	rdoParser::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� ����������� ��� � �����"));
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__void
// --------------------------------------------------------------------------------
LPRDOType RDOType__void::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(to_src_info);
	ASSERT(pFrom      );

	if (pFrom->type()->typeID() == rdo::runtime::RDOType::t_void)
	{
		return rdo::Factory<RDOType__void>::create();
	}
	rdoParser::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � void"), from_src_info.src_text().c_str()));
	return NULL;
}

LPRDOValue RDOType__void::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	if (pFrom->value().type()->typeID() == rdo::runtime::RDOType::t_void)
	{
		return rdo::Factory<RDOValue>::create(pFrom);
	}

	rdoParser::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � void-����"), pFrom->src_info().src_text().c_str()));
	rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
	rdoParser::g_error().push_done();
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__void::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__void::get_default() const
{
	return rdo::runtime::RDOValue();
}

void RDOType__void::writeModelStructure(REF(std::ostream) stream) const
{
	UNUSED(stream);
	rdoParser::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� void-��� � �����"));
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__int
// --------------------------------------------------------------------------------
LPRDOType RDOType__int::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_int :
			return rdo::Factory<RDOType__int>::create();
		case rdo::runtime::RDOType::t_real:
			rdoParser::g_error().warning(src_info, rdo::format(_T("�������������� '%s' � '%s', �������� ������ ������"), pFrom->name().c_str(), name().c_str()));
			return rdo::Factory<RDOType__real>::create();
		default:
			rdoParser::g_error().push_only(src_info,    rdo::format(_T("��������� ������������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__int::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__int)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getInt(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdoParser::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ������ ����"), pFrom->src_info().src_text().c_str()));
		rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__int::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	if (pType->typeID() == rdo::runtime::RDOType::t_real)
	{
		return rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(pCalc->srcInfo().src_pos(), pCalc);
	}
	return pCalc;
}

rdo::runtime::RDOValue RDOType__int::get_default() const
{
	return rdo::runtime::RDOValue(0);
}

void RDOType__int::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("I") << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__real
// --------------------------------------------------------------------------------
LPRDOType RDOType__real::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_int :
		case rdo::runtime::RDOType::t_real:
			return rdo::Factory<RDOType__real>::create();
		default:
			rdoParser::g_error().push_only(src_info,    rdo::format(_T("��������� ������������ ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__real::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__real)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getDouble(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdoParser::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ������������� ����"), pFrom->src_info().src_text().c_str()));
		rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__real::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__real::get_default() const
{
	return rdo::runtime::RDOValue(0.0);
}

void RDOType__real::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("R") << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__string
// --------------------------------------------------------------------------------
LPRDOType RDOType__string::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_string:
			return rdo::Factory<RDOType__string>::create();
		default:
			rdoParser::g_error().push_only(src_info,    rdo::format(_T("��������� ��������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__string::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__string)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getString(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdoParser::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ���������� ����"), pFrom->src_info().src_text().c_str()));
		rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__string::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__string::get_default() const
{
	return rdo::runtime::RDOValue(_T(""));
}

void RDOType__string::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("S") << std::endl;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__identificator
// --------------------------------------------------------------------------------
LPRDOType RDOType__identificator::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(to_src_info);
	ASSERT(pFrom      );

	switch (pFrom->type()->typeID())
	{
	case rdo::runtime::RDOType::t_identificator:
		return rdo::Factory<RDOType__identificator>::create();
	default:
		rdoParser::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � ��������������"), from_src_info.src_text().c_str()));
	}
	return NULL;
}

LPRDOValue RDOType__identificator::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->typeID())
	{
	case rdo::runtime::RDOType::t_identificator:
		return rdo::Factory<RDOValue>::create(pFrom);
	default:
		rdoParser::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ���� �������������"), pFrom->src_info().src_text().c_str()));
		rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParser::g_error().push_done();
	}
	return LPRDOValue(NULL);
}

rdo::runtime::LPRDOCalc RDOType__identificator::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__identificator::get_default() const
{
	return rdo::runtime::RDOValue(_T(""), rdo::runtime::g_identificator);
}

void RDOType__identificator::writeModelStructure(REF(std::ostream) stream) const
{
	UNUSED(stream);
	rdoParser::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� ��� ������������� � �����"));
	NEVER_REACH_HERE;
}

// --------------------------------------------------------------------------------
// -------------------- RDOType__bool
// --------------------------------------------------------------------------------
LPRDOType RDOType__bool::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	switch (pFrom->type()->typeID())
	{
		case rdo::runtime::RDOType::t_bool:
			return rdo::Factory<RDOType__bool>::create();
		default:
			rdoParser::g_error().push_only(src_info,    rdo::format(_T("��������� ��������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParser::g_error().push_done();
	}
	return NULL;
}

LPRDOValue RDOType__bool::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__bool)>(this), pFrom->src_info());
		ASSERT(pType);
		pToValue = rdo::Factory<RDOValue>::create(pFrom->value().getBool(), pFrom->src_info(), pType);
		ASSERT(pToValue);
	}
	catch (CREF(rdo::runtime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		rdoParser::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ���������� ����"), pFrom->src_info().src_text().c_str()));
		rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParser::g_error().push_done();
	}
	return pToValue;
}

rdo::runtime::LPRDOCalc RDOType__bool::calc_cast(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdo::runtime::RDOValue RDOType__bool::get_default() const
{
	return rdo::runtime::RDOValue(false);
}

void RDOType__bool::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("B") << std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE
