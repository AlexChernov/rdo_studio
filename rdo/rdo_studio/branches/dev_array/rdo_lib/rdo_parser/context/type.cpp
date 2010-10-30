/*
 * copyright: (c) RDO-Team, 2010
 * filename : type.cpp
 * author   : ������ ������
 * date     : 30.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/type.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeContext
// ----------------------------------------------------------------------------
TypeContext::TypeContext(CREF(LPRDOTypeParam) pTypeParam)
	: m_pTypeParam(pTypeParam)
{}

LPRDOTypeParam TypeContext::getType() const
{
	return m_pTypeParam;
}

LPContext TypeContext::parser(PTR(RDOParser) pParser, CREF(RDOValue) value)
{
	NEVER_REACH_HERE;
	return NULL;
}

rdoRuntime::LPRDOCalc TypeContext::getCalc()
{
	NEVER_REACH_HERE;
	return NULL;
}

LPLocalVariableList TypeContext::getLocalMemory()
{
	NEVER_REACH_HERE;
	return NULL;
}

CLOSE_RDO_PARSER_NAMESPACE
