/*!
  \copyright (c) RDO-Team, 2011
  \file      enum.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/enum.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_error.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOEnumType
// --------------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
	: RDOType(rdo::Factory<rdoRuntime::RDOEnumType>::create())
{
	ASSERT(m_pType);
}

RDOEnumType::RDOEnumType(CREF(rdoRuntime::LPRDOEnumType) pEnumType)
	: RDOType(rdoRuntime::g_unknow)
{
	rdoRuntime::LPRDOEnumType pType = rdo::Factory<rdoRuntime::RDOEnumType>::create();
	m_pType = pType;
	ASSERT(m_pType);

	rdoRuntime::RDOEnumType::Enums::const_iterator it = pEnumType->begin();
	while (it != pEnumType->end())
	{
		pType->add(*it);
	}
}

RDOEnumType::~RDOEnumType()
{}

tstring RDOEnumType::name() const
{
	tstring str = _T("(");
	rdoRuntime::RDOEnumType::const_iterator it = getEnums()->begin();
	while (it != getEnums()->end())
	{
		str += *it;
		it++;
		if (it != getEnums()->end())
		{
			str += _T(", ");
		}
	}
	str += _T(")");
	return str;
}

LPRDOType RDOEnumType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->type()->typeID())
	{
		case rdoRuntime::RDOType__int::t_enum:
		{
			LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
			//! ��� ���� � ��� �� ���
			if (pEnum == from)
				return pEnum;

			//! ���� ������, �������� ������
			if (pEnum.compare(from.object_static_cast<RDOEnumType>()))
			{
				rdoParser::g_error().push_only(src_info,     _T("������������ ��������� ������������ ���� � ����������� ����������"));
				rdoParser::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParser::g_error().push_only(src_info,     _T("�"));
				rdoParser::g_error().push_only(from_src_info, from_src_info.src_text());
				rdoParser::g_error().push_only(src_info,     _T("��������, ������� ������������ ������ �� ��� ��� ������������, � ������ ��� such_as �� ����, ����� ��������� ����� ����� ���������� � �����������"));
			}
			else
			{
				rdoParser::g_error().push_only(src_info,     _T("�������������� ������������ �����"));
				rdoParser::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParser::g_error().push_only(src_info,     _T("�"));
				rdoParser::g_error().push_only(from_src_info, from_src_info.src_text());
			}
			rdoParser::g_error().push_done();
			break;
		}
		case rdoRuntime::RDOType::t_string       :
		case rdoRuntime::RDOType::t_identificator:
		{
			if (getEnums()->exist(from_src_info.src_text()))
			{
				LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
				return pEnum;
			}
			rdoParser::g_error().error(src_info, rdo::format(_T("�������� '%s' �� �������� ��������� ������������� ���� %s"), from_src_info.src_text().c_str(), to_src_info.src_text().c_str()));
			break;
		}
		default:
			rdoParser::g_error().push_only(src_info,    rdo::format(_T("��������� �������� ������������� ����, �������: %s"), from_src_info.src_text().c_str()));
			rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParser::g_error().push_done();
			break;
	}
	return NULL;
}

LPRDOValue RDOEnumType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDOValue pToValue;
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	try
	{
		switch (pFrom->typeID())
		{
		case rdoRuntime::RDOType::t_identificator:
			if (getEnums()->findEnum(pFrom->value().getIdentificator()) != rdoRuntime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				pToValue = rdo::Factory<RDOValue>::create(rdoRuntime::RDOValue(getEnums(), pFrom->value().getIdentificator()), pFrom->src_info(), pType);
				ASSERT(pToValue);
			}
			else
			{
				pToValue = RDOValue::getUnknow(pFrom->src_info());
				ASSERT(pToValue);
			}
			break;

		case rdoRuntime::RDOType::t_string:
			if (getEnums()->findEnum(pFrom->value().getAsString()) != rdoRuntime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				pToValue = rdo::Factory<RDOValue>::create(rdoRuntime::RDOValue(getEnums(), pFrom->value().getAsString()), pFrom->src_info(), pType);
				ASSERT(pToValue);
			}
			else
			{
				pToValue = RDOValue::getUnknow(pFrom->src_info());
				ASSERT(pToValue);
			}
			break;

		case rdoRuntime::RDOType::t_enum:
			if (m_pType == pFrom->typeInfo()->type()->type())
				pToValue = rdo::Factory<RDOValue>::create(pFrom);
			break;
		}
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (!pToValue || pToValue->typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParser::g_error().push_only(src_info,    rdo::format(_T("�������� �������� ��������� ������������� ����: %s"), pFrom->src_info().src_text().c_str()));
		rdoParser::g_error().push_only(to_src_info, rdo::format(_T("��������� ��������: %s"), name().c_str()));
		rdoParser::g_error().push_done();
	}
	return pToValue;
}

rdoRuntime::LPRDOCalc RDOEnumType::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOEnumType::get_default() const
{
	return rdoRuntime::RDOValue(getEnums(), 0);
}

void RDOEnumType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "E " << getEnums()->getValues().size() << std::endl;
	for (ruint i = 0; i < getEnums()->getValues().size(); i++)
	{
		stream << "    " << i << " " << getEnums()->getValues().at(i) << std::endl;
	}
}

void RDOEnumType::add(CREF(LPRDOValue) pNext)
{
	ASSERT(pNext);

	if (getEnums()->findEnum(pNext->value().getAsString()) != rdoRuntime::RDOEnumType::END)
	{
		rdoParser::g_error().error(pNext->src_info(), rdo::format(_T("�������� ������������� ���� ��� ����������: %s"), pNext->src_text().c_str()));
	}
	getEnums()->add(pNext->value().getAsString());
}

CLOSE_RDO_PARSER_NAMESPACE
