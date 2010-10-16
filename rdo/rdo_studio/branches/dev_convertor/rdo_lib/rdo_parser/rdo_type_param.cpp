/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_param.cpp
 * author   : ������ ������
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type_param.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
RDOTypeParam::RDOTypeParam(CREF(LPRDOType) type, CREF(RDOValue) default, CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_type          (type    )
	, m_default       (default )
{
	ASSERT(m_type);
	setSrcText(m_type->name());

	if (m_default.defined())
	{
		m_type->type_cast(m_default.type(), m_default.src_info(), this->src_info(), m_default.src_info());
		m_default = m_type->value_cast(m_default, this->src_info(), m_default.src_info());
		setSrcText(rdo::format(_T("%s = %s"), src_text().c_str(), m_default.src_text().c_str()));
	}
	switch (m_type->type()->typeID())
	{
	case rdoRuntime::RDOType::t_enum :
	case rdoRuntime::RDOType::t_fuzzy: RDOParser::s_parser()->insertPreCastType(this); break;
	}
}

RDOTypeParam::~RDOTypeParam()
{}

LPRDOType RDOTypeParam::type() const
{
	return m_type;
}

void RDOTypeParam::writeModelStructure(REF(std::ostream) stream) const
{
	m_type->writeModelStructure(stream);
}

RDOValue RDOTypeParam::value_cast(CREF(RDOValue) value) const
{
	return m_type->value_cast(value, src_info(), value.src_info());
}

CREF(RDOValue) RDOTypeParam::default() const
{
	return m_default;
}

CLOSE_RDO_PARSER_NAMESPACE
