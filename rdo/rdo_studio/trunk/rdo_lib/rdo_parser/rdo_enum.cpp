/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_enum.cpp
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
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
	: RDOType(&rdoRuntime::g_unknow)
{
	m_type = new rdoRuntime::RDOEnumType(RDOParser::s_parser()->runtime());
}

RDOEnumType::~RDOEnumType()
{
	delete m_type;
}

inline tstring RDOEnumType::name() const
{
	tstring str = _T("(");
	rdoRuntime::RDOEnumType::const_iterator it = getEnums().begin();
	while (it != getEnums().end())
	{
		str += *it;
		it++;
		if (it != getEnums().end())
		{
			str += _T(", ");
		}
	}
	str += _T(")");
	return str;
}

LPRDOType RDOEnumType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type().typeID())
	{
		case rdoRuntime::RDOType__int::t_enum:
		{
			LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
			//! ��� ���� � ��� �� ���
			if (pEnum == from)
				return pEnum;

			//! ���� ������, �������� ������
			if (pEnum.compare(from.lp_cast<LPRDOEnumType>()))
			{
				rdoParse::g_error().push_only(src_info,     _T("������������ ��������� ������������ ���� � ����������� ����������"));
				rdoParse::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("�"));
				rdoParse::g_error().push_only(from_src_info, from_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("��������, ������� ������������ ������ �� ��� ��� ������������, � ������ ��� such_as �� ����, ����� ��������� ����� ����� ���������� � �����������"));
			}
			else
			{
				rdoParse::g_error().push_only(src_info,     _T("�������������� ������������ �����"));
				rdoParse::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("�"));
				rdoParse::g_error().push_only(from_src_info, from_src_info.src_text());
			}
			rdoParse::g_error().push_done();
			break;
		}
		case rdoRuntime::RDOType::t_identificator:
		{
			if (getEnums().exist(from_src_info.src_text()))
			{
				LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
				return pEnum;
			}
			rdoParse::g_error().error(src_info, rdo::format(_T("�������� '%s' �� �������� ��������� ������������� ���� %s"), from_src_info.src_text().c_str(), to_src_info.src_text().c_str()));
			break;
		}
		default:
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� �������� ������������� ����, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
			break;
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOEnumType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	try
	{
		switch (from.typeID())
		{
			case rdoRuntime::RDOType::t_identificator: {
				toValue = (getEnums().findEnum(from->getIdentificator()) != rdoRuntime::RDOEnumType::END) ?
					RDOValue(rdoRuntime::RDOValue(getEnums(), from->getIdentificator()), from.src_info()) :
					RDOValue(rdoRuntime::RDOValue(rdoRuntime::g_unknow), from.src_info());
				break;
			}
			case rdoRuntime::RDOType::t_string: {
				toValue = (getEnums().findEnum(from->getAsString()) != rdoRuntime::RDOEnumType::END) ?
					RDOValue(rdoRuntime::RDOValue(getEnums(), from->getAsString()), from.src_info()) :
					RDOValue(rdoRuntime::RDOValue(rdoRuntime::g_unknow), from.src_info());
				break;
			}
			case rdoRuntime::RDOType::t_enum: {
				if (m_type == &from.type()->type())
					toValue = from;
				break;
			}
		}
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("�������� �������� ��������� ������������� ����: %s"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��������� ��������: %s"), name().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

PTR(rdoRuntime::RDOCalc) RDOEnumType::calc_cast(PTR(rdoRuntime::RDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

void RDOEnumType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "E " << getEnums().getValues().size() << std::endl;
	for (ruint i = 0; i < getEnums().getValues().size(); i++)
	{
		stream << "    " << i << " " << getEnums().getValues().at(i) << std::endl;
	}
}

void RDOEnumType::add(CREF(RDOValue) next)
{
	if (getEnums().findEnum(next->getIdentificator()) != rdoRuntime::RDOEnumType::END)
	{
		rdoParse::g_error().error(next.src_info(), rdo::format(_T("�������� ������������� ���� ��� ����������: %s"), next.src_text().c_str()));
	}
	__enum()->add(next->getIdentificator());
}

//rdoRuntime::RDOValue RDOEnumType::findEnumValueWithThrow(CREF(RDOParserSrcInfo) src_info, CREF(tstring) value) const
//{
//	rdoRuntime::RDOValue result = value_cast(rdoRuntime::RDOValue(value, rdoRuntime::g_identificator));
//	if (result.typeID() == rdoRuntime::RDOType::t_unknow)
//	{
//		rdoParse::g_error().push_only(src_info, rdo::format(_T("�������� �������� ��������� ������������� ����: %s"), value.c_str()));
//		rdoParse::g_error().push_only(this->src_info(), rdo::format(_T("��������� ��������: %s"), getEnums().asString().c_str()));
//		rdoParse::g_error().push_done();
//	}
//	return result;
//}

//rdoRuntime::RDOValue RDOEnumType::getFirstValue() const
//{
//	if (getEnums().getValues().empty())
//	{
//		rdoParse::g_error().error(src_info(), _T("���������� ������: ������ ������������ ���"));
//	}
//	return rdoRuntime::RDOValue(getEnums());
//}

CLOSE_RDO_PARSER_NAMESPACE
