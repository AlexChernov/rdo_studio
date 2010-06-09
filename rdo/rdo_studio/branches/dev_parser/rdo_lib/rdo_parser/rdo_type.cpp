/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type.cpp
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_runtime/rdo_exception.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ATOM_TYPE_PARSER
// ----------------------------------------------------------------------------
#define DECLARE_ATOM_TYPE_PARSER(Type, TypeName)    \
tstring RDOType__##Type::name() const               \
{                                                   \
	return TypeName;                                \
}                                                   \
RDOType__##Type g_##Type##_type;                    \
rdo::smart_ptr<RDOType__##Type> g_##Type(&g_##Type##_type, 2); \

DECLARE_ATOM_TYPE_PARSER(unknow,        _T("unknow")       );
DECLARE_ATOM_TYPE_PARSER(identificator, _T("identificator"));
DECLARE_ATOM_TYPE_PARSER(int,           _T("integer")      );
DECLARE_ATOM_TYPE_PARSER(real,          _T("real")         );
DECLARE_ATOM_TYPE_PARSER(bool,          _T("bool")         );
DECLARE_ATOM_TYPE_PARSER(string,        _T("string")       );

// ----------------------------------------------------------------------------
// ---------- RDOType
// ----------------------------------------------------------------------------

//! RDOType__unknow
LPRDOType RDOType__unknow::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoParse::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � ������������ ����"), from_src_info.src_text().c_str()));
	return rdo::smart_ptr_null();
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

RDOValue RDOType__unknow::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(), g_unknow, RDOParserSrcInfo());
}

void RDOType__unknow::writeModelStructure(REF(std::ostream) stream) const
{
	rdoParse::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� ����������� ��� � �����"));
	NEVER_REACH_HERE;
}

//! RDOType__int
LPRDOType RDOType__int::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_int :
			return g_int;
		case rdoRuntime::RDOType::t_real:
			rdoParse::g_error().warning(src_info, rdo::format(_T("�������������� '%s' � '%s', �������� ������ ������"), from->name().c_str(), name().c_str()));
			return g_real;
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ������������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOType__int::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getInt(), g_int, from.src_info());
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
		return new rdoRuntime::RDOCalcDoubleToInt(RDOParser::s_parser()->runtime(), pCalc);
	}
	return pCalc;
}

RDOValue RDOType__int::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(0), g_int, RDOParserSrcInfo());
}

void RDOType__int::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("I") << std::endl;
}

//! RDOType__real
LPRDOType RDOType__real::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_int :
		case rdoRuntime::RDOType::t_real:
			return g_real;
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ������������ ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOType__real::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getDouble(), g_real, from.src_info());
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

RDOValue RDOType__real::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(0.0), g_real, RDOParserSrcInfo());
}

void RDOType__real::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("R") << std::endl;
}

//! RDOType__string
LPRDOType RDOType__string::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_string:
			return g_string;
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ��������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOType__string::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getString(), g_string, from.src_info());
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

RDOValue RDOType__string::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(_T("")), g_string, RDOParserSrcInfo());
}

void RDOType__string::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("S") << std::endl;
}

//! RDOType__identificator
LPRDOType RDOType__identificator::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoParse::g_error().error(src_info, rdo::format(_T("���������� ������ �������. ���������� ������������� ��� '%s' � ��������������"), from_src_info.src_text().c_str()));
	return rdo::smart_ptr_null();
}

RDOValue RDOType__identificator::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	rdoParse::g_error().push_only(src_info,    rdo::format(_T("���������� ������������� �������� '%s' � ���� �������������"), from.src_info().src_text().c_str()));
	rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
	rdoParse::g_error().push_done();
	return RDOValue();
}

rdoRuntime::LPRDOCalc RDOType__identificator::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return parent_type::calc_cast(pCalc, pType);
}

RDOValue RDOType__identificator::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(_T(""), rdoRuntime::g_identificator), g_identificator, RDOParserSrcInfo());
}

void RDOType__identificator::writeModelStructure(REF(std::ostream) stream) const
{
	rdoParse::g_error().error(RDOParserSrcInfo(), _T("���������� ������ �������. ���������� �������� ��� ������������� � �����"));
	NEVER_REACH_HERE;
}

//! RDOType__bool
LPRDOType RDOType__bool::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType::t_bool:
			return g_bool;
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ��������� ��������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOType__bool::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		toValue = RDOValue(from->getBool(), g_bool, from.src_info());
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

RDOValue RDOType__bool::get_default() const
{
	return RDOValue(rdoRuntime::RDOValue(false), g_bool, RDOParserSrcInfo());
}

void RDOType__bool::writeModelStructure(REF(std::ostream) stream) const
{
	stream << _T("B") << std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE
