/*
 * copyright: (c) RDO-Team, 2011
 * filename : enum.cpp
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/enum.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEnumType
// ----------------------------------------------------------------------------
RDOEnumType::RDOEnumType()
	: RDOType(rdoRuntime::g_unknow)
{
	m_pType = rdo::Factory<rdoRuntime::RDOEnumType>::create();
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

inline tstring RDOEnumType::name() const
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
		case rdoRuntime::RDOType::t_string       :
		case rdoRuntime::RDOType::t_identificator:
		{
			if (getEnums()->exist(from_src_info.src_text()))
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
	return NULL;
}

RDOValue RDOEnumType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	LPRDOEnumType pEnum(const_cast<PTR(RDOEnumType)>(this));
	try
	{
		switch (from.typeID())
		{
		case rdoRuntime::RDOType::t_identificator:
			if (getEnums()->findEnum(from->getIdentificator()) != rdoRuntime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				toValue = RDOValue(rdoRuntime::RDOValue(getEnums(), from->getIdentificator()), from.src_info(), pType);
			}
			else
			{
				toValue = RDOValue(rdo::Factory<RDOType__unknow>::create(), from.src_info());
			}
			break;

		case rdoRuntime::RDOType::t_string:
			if (getEnums()->findEnum(from->getAsString()) != rdoRuntime::RDOEnumType::END)
			{
				LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, to_src_info);
				ASSERT(pType);
				toValue = RDOValue(rdoRuntime::RDOValue(getEnums(), from->getAsString()), from.src_info(), pType);
			}
			else
			{
				toValue = RDOValue(rdo::Factory<RDOType__unknow>::create(), from.src_info());
			}
			break;

		case rdoRuntime::RDOType::t_enum:
			if (m_pType == from.typeInfo()->type()->type())
				toValue = from;
			break;
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

void RDOEnumType::add(CREF(RDOValue) next)
{
	if (getEnums()->findEnum(next->getAsString()) != rdoRuntime::RDOEnumType::END)
	{
		rdoParse::g_error().error(next.src_info(), rdo::format(_T("�������� ������������� ���� ��� ����������: %s"), next.src_text().c_str()));
	}
	getEnums()->add(next->getAsString());
}

CLOSE_RDO_PARSER_NAMESPACE
