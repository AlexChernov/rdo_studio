/*!
  \copyright (c) RDO-Team, 2011
  \file      atom.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      09.04.2011
  \brief     
  \indent    4T
*/

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/atom.h"
#include "rdo_lib/rdo_parser/type/info.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_runtime/calc/unary.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- RDOType__unknow
// ----------------------------------------------------------------------------
LPRDOType RDOType__unknow::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoParse::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � ������������ ����"), from_src_info.src_text().c_str()));
	return NULL;
}

RDOValue RDOType__unknow::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ������������ ����"), from.src_info().src_text().c_str()));
	rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
	rdoParse::g_error().push_done();
	return RDOValue();
}

rdoRuntime::LPRDOCalc RDOType__unknow::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOType__unknow::get_default() const
{
	return rdoRuntime::RDOValue();
}

void RDOType__unknow::writeModelStructure(REF(std::ostream) stream) const
{
	rdoParse::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� ����������� ��� � �����"));
	NEVER_REACH_HERE;
}

// ----------------------------------------------------------------------------
// ---------- RDOType__void
// ----------------------------------------------------------------------------
LPRDOType RDOType__void::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	if (from->type()->typeID() == rdoRuntime::RDOType::t_void)
	{
		return rdo::Factory<RDOType__void>::create();
	}
	rdoParse::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � void"), from_src_info.src_text().c_str()));
	return NULL;
}

RDOValue RDOType__void::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	if (from->type()->typeID() == rdoRuntime::RDOType::t_void)
	{
		return from;
	}

	rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � void-����"), from.src_info().src_text().c_str()));
	rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
	rdoParse::g_error().push_done();
	return RDOValue();
}

rdoRuntime::LPRDOCalc RDOType__void::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOType__void::get_default() const
{
	return rdoRuntime::RDOValue();
}

void RDOType__void::writeModelStructure(REF(std::ostream) stream) const
{
	rdoParse::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� void-��� � �����"));
	NEVER_REACH_HERE;
}

// ----------------------------------------------------------------------------
// ---------- RDOType__int
// ----------------------------------------------------------------------------
LPRDOType RDOType__int::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int :
			return rdo::Factory<RDOType__int>::create();
		case rdoRuntime::RDOType::t_real:
			rdoParse::g_error().warning(src_info, rdo::format(_T("�������������� '%s' � '%s', �������� ������ ������"), from->name().c_str(), name().c_str()));
			return rdo::Factory<RDOType__real>::create();
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ������������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__int::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__int)>(this), from.src_info());
		ASSERT(pType);
		toValue = RDOValue(from->getInt(), from.src_info(), pType);
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ������ ����"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__int::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	if (pType->typeID() == rdoRuntime::RDOType::t_real)
	{
		return rdo::Factory<rdoRuntime::RDOCalcDoubleToInt>::create(pCalc->src_pos(), pCalc);
	}
	return pCalc;
}

rdoRuntime::RDOValue RDOType__int::get_default() const
{
	return rdoRuntime::RDOValue(0);
}

void RDOType__int::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("I") << std::endl;
}

// ----------------------------------------------------------------------------
// ---------- RDOType__real
// ----------------------------------------------------------------------------
LPRDOType RDOType__real::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int :
		case rdoRuntime::RDOType::t_real:
			return rdo::Factory<RDOType__real>::create();
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ������������ ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__real::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__real)>(this), from.src_info());
		ASSERT(pType);
		toValue = RDOValue(from->getDouble(), from.src_info(), pType);
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ������������� ����"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__real::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOType__real::get_default() const
{
	return rdoRuntime::RDOValue(0.0);
}

void RDOType__real::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("R") << std::endl;
}

// ----------------------------------------------------------------------------
// ---------- RDOType__string
// ----------------------------------------------------------------------------
LPRDOType RDOType__string::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_string:
			return rdo::Factory<RDOType__string>::create();
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ��������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__string::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__string)>(this), from.src_info());
		ASSERT(pType);
		toValue = RDOValue(from->getString(), from.src_info(), pType);
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ���������� ����"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__string::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOType__string::get_default() const
{
	return rdoRuntime::RDOValue(_T(""));
}

void RDOType__string::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("S") << std::endl;
}

// ----------------------------------------------------------------------------
// ---------- RDOType__identificator
// ----------------------------------------------------------------------------
LPRDOType RDOType__identificator::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
	case rdoRuntime::RDOType::t_identificator:
		return rdo::Factory<RDOType__identificator>::create();
	default:
		rdoParse::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � ��������������"), from_src_info.src_text().c_str()));
	}
	return NULL;
}

RDOValue RDOType__identificator::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from.typeID())
	{
	case rdoRuntime::RDOType::t_identificator:
		return from;
	default:
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ���� �������������"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return RDOValue();
}

rdoRuntime::LPRDOCalc RDOType__identificator::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOType__identificator::get_default() const
{
	return rdoRuntime::RDOValue(_T(""), rdoRuntime::g_identificator);
}

void RDOType__identificator::writeModelStructure(REF(std::ostream) stream) const
{
	rdoParse::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� ��� ������������� � �����"));
	NEVER_REACH_HERE;
}

// ----------------------------------------------------------------------------
// ---------- RDOType__bool
// ----------------------------------------------------------------------------
LPRDOType RDOType__bool::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType::t_bool:
			return rdo::Factory<RDOType__bool>::create();
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ��������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return NULL;
}

RDOValue RDOType__bool::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(const_cast<PTR(RDOType__bool)>(this), from.src_info());
		ASSERT(pType);
		toValue = RDOValue(from->getBool(), from.src_info(), pType);
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ���������� ����"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

rdoRuntime::LPRDOCalc RDOType__bool::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOType__bool::get_default() const
{
	return rdoRuntime::RDOValue(false);
}

void RDOType__bool::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("B") << std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE
