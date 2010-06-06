/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.cpp
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayValue
 //----------------------------------------------------------------------------
 RDOArrayValue::RDOArrayValue()
{}
 RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) pValue)
{
	m_arrayValue = pValue.m_arrayValue;
}
RDOArrayValue::~RDOArrayValue()
{}
void RDOArrayValue::insert_array(CREF(RDOValue) pArray)
{
	m_arrayValue.push_back(&pArray);
}
 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType(CREF(LPRDOType) pType)
	: RDOType(&rdoRuntime::g_unknow)
	, m_pType(pType                )
{
	ASSERT(m_pType);
	m_type = new rdoRuntime::RDOArrayType(RDOParser::s_parser()->runtime(), m_pType->type());
	rdoParse::RDOParser::s_parser()->insertArrayType(this);
}

RDOArrayType::~RDOArrayType()
{}

inline tstring RDOArrayType::name() const
{
	return rdo::format(_T("array<%s>"), m_pType->name().c_str());
}

LPRDOType RDOArrayType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->typeID())
	{
	case rdoRuntime::RDOType::t_array:
		{
			LPRDOArrayType pArray(const_cast<PTR(RDOArrayType)>(this));
			//! ��� ���� � ��� �� ���
			if (pArray == from)
				return pArray;

			else
			{
				rdoParse::g_error().push_only(src_info,     _T("�������������� �������������� ����������� ��������"));
				rdoParse::g_error().push_only(to_src_info,   to_src_info.src_text());
				rdoParse::g_error().push_only(src_info,     _T("�"));
				rdoParse::g_error().push_only(from_src_info, from_src_info.src_text());
			}
			rdoParse::g_error().push_done();
			break;
		}
	default:
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� �������� ���� ������, �������: %s"), from_src_info.src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
		break;
	}
	return rdo::smart_ptr_null();
}

RDOValue RDOArrayType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	RDOValue toValue;
	LPRDOArrayType pArray(const_cast<PTR(RDOArrayType)>(this));
	try
	{
		if (from.typeID() == rdoRuntime::RDOType::t_array)
			if (m_type == &from.type()->type())
				toValue = from;

		//case rdoRuntime::RDOType::t_int:			toValue = RDOValue(rdoRuntime::RDOValue(getArray(), from->getInt()),			pArray, from.src_info()); break;
		//case rdoRuntime::RDOType::t_real:			toValue = RDOValue(rdoRuntime::RDOValue(getArray(), from->getDouble()),			pArray, from.src_info()); break;
		//case rdoRuntime::RDOType::t_bool:			toValue = RDOValue(rdoRuntime::RDOValue(getArray(), from->getBool()),			pArray, from.src_info()); break;
		//case rdoRuntime::RDOType::t_string:			toValue = RDOValue(rdoRuntime::RDOValue(getArray(), from->getString()),			pArray, from.src_info()); break;
		//case rdoRuntime::RDOType::t_enum:			toValue = RDOValue(rdoRuntime::RDOValue(getArray(), from->getEnum()),			pArray, from.src_info()); break;
		//case rdoRuntime::RDOType::t_identificator:	toValue = RDOValue(rdoRuntime::RDOValue(getArray(), from->getIdentificator()),	pArray, from.src_info()); break;
	}
	catch (CREF(rdoRuntime::RDOValueException))
	{}

	if (toValue.typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("�������� �������� ��������� ��� �������: %s"), from.src_info().src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T(": %s"), name().c_str()));
		rdoParse::g_error().push_done();
	}
	return toValue;
}

PTR(rdoRuntime::RDOCalc) RDOArrayType::calc_cast(PTR(rdoRuntime::RDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

RDOValue RDOArrayType::get_default() const
{
	NEVER_REACH_HERE;
	return RDOValue();
	//LPRDOArrayType pArray(const_cast<PTR(RDOArrayType)>(this));
	//return RDOValue(rdoRuntime::RDOValue(__array()), pArray, RDOParserSrcInfo());
}

void RDOArrayType::writeModelStructure(REF(std::ostream) stream) const
{
	CPTR(RDOArrayType) pArray(const_cast<PTR(RDOArrayType)>(this));
	stream << (*pArray).name()<< std::endl;
}

CLOSE_RDO_PARSER_NAMESPACE
