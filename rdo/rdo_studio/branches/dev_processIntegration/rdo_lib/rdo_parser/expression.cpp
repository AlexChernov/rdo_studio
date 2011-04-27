/*
 * copyright: (c) RDO-Team, 2011
 * filename : expression.cpp
 * author   : ������ ������
 * date     : 07.03.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/expression.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

Expression::Expression(CREF(LPRDOType) pType, CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(rdoRuntime::RDOSrcInfo) src_info)
	: m_pType(pType)
	, m_pCalc(pCalc)
{
	ASSERT(m_pType);
	ASSERT(m_pCalc);
	setSrcInfo(src_info);
}

Expression::~Expression()
{}

CREF(LPRDOType) Expression::type() const
{
	return m_pType;
}

CREF(rdoRuntime::LPRDOCalc) Expression::calc() const
{
	return m_pCalc;
}

CREF(rdoRuntime::RDOSrcInfo) Expression::src_info() const
{
	ASSERT(m_pCalc);
	return m_pCalc->src_info();
}

void Expression::setSrcInfo(CREF(rdoRuntime::RDOSrcInfo) src_info)
{
	ASSERT(m_pCalc);
	m_pCalc->setSrcInfo(src_info);
}

RDOValue Expression::constant() const
{
	ASSERT(m_pCalc);
	ASSERT(m_pType);

	rdoRuntime::LPRDOCalcConst pCalc = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	if (pCalc)
	{
		return RDOValue(pCalc->getValue(), m_pType, src_info());
	}

	return RDOValue();
}

CLOSE_RDO_PARSER_NAMESPACE
