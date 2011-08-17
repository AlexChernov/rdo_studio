/*
 * copyright: (c) RDO-Team, 2011
 * filename : array.cpp
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/array.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

 //----------------------------------------------------------------------------
 //---------- RDOArrayType
 //----------------------------------------------------------------------------
RDOArrayType::RDOArrayType(CREF(LPTypeInfo) pItemType, CREF(RDOParserSrcInfo) src_info)
	: RDOType          (rdoRuntime::g_unknow)
	, RDOParserSrcInfo (src_info            )
	, m_pItemType      (pItemType           )
{
	ASSERT(pItemType);
	setSrcText(name());
	m_pType = rdo::Factory<rdoRuntime::RDOArrayType>::create(pItemType->type()->type());
	ASSERT(m_pType);
}

RDOArrayType::~RDOArrayType()
{}

inline tstring RDOArrayType::name() const
{
	return rdo::format(_T("array<%s>"), m_pItemType->type()->name().c_str());
}

LPRDOType RDOArrayType::type_cast(CREF(LPRDOType) from, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch (from->typeID())
	{
	case rdoRuntime::RDOType::t_array:
		{
			LPRDOArrayType pArray(const_cast<PTR(RDOArrayType)>(this));
			LPRDOArrayType from(const_cast<PTR(RDOArrayType)>(static_cast<CPTR(RDOArrayType)>(from.get())));
			//! ��� ���� � ��� �� ���
			if (pArray->getItemType()->type()->type_cast(from->getItemType()->type(), from->src_info(), pArray->src_info(), from->src_info()))
			{
				return pArray;
			}
			break;
		}
	default:
		{
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("�������������� ����������� �������")));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
			break;
		}
	}
	return NULL;
}

RDOValue RDOArrayType::value_cast(CREF(RDOValue) from, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	switch(from.typeID())
	{
	case rdoRuntime::RDOType::t_array:
		{
			LPRDOArrayType pThisArray(const_cast<PTR(RDOArrayType)>(this));
			LPRDOArrayValue pThisArrayValue = rdo::Factory<RDOArrayValue>::create(pThisArray);
			ASSERT(pThisArrayValue);
			rdoRuntime::RDOArrayValue pFromArrayValue = from->getArray();
			for (rdoRuntime::RDOArrayValue::Container::iterator it = pFromArrayValue.containerBegin(); it < pFromArrayValue.containerEnd(); ++it)
			{
				RDOValue itemValue = RDOValue((*it), src_info, pThisArray->getItemType());
				pThisArrayValue->insertItem(pThisArray->getItemType()->type()->value_cast(itemValue, to_src_info, src_info));
			}
			return RDOValue(pThisArrayValue);
			break;
		}
	default:
		rdoParse::g_error().push_only(src_info,    rdo::format(_T("�������������� ����������� �������")));
		rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_done();
		break;
	}
	return from;
}

rdoRuntime::LPRDOCalc RDOArrayType::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDOArrayType::get_default() const
{
	return rdoRuntime::RDOArrayValue(getRuntimeArrayType());
}

void RDOArrayType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << "A ";
	m_pItemType->type()->writeModelStructure(stream);
}

CREF(LPTypeInfo) RDOArrayType::getItemType() const
{
	return m_pItemType;
}

rdoRuntime::LPRDOArrayType RDOArrayType::getRuntimeArrayType() const
{
	return m_pType.object_static_cast<rdoRuntime::RDOArrayType>();
}

LPTypeInfo RDOArrayType::typeInfo() const
{
	LPTypeInfo pType = rdo::Factory<TypeInfo>::create(
		const_cast<PTR(RDOArrayType)>(this),
		src_info()
	);
	ASSERT(pType);
	return pType;
}

CLOSE_RDO_PARSER_NAMESPACE
