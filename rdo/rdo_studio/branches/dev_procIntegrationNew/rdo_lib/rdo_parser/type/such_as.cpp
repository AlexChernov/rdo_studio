/*
 * copyright: (c) RDO-Team, 2011
 * filename : such_as.cpp
 * author   : ������ ������
 * date     : 09.01.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/such_as.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamSuchAs
// ----------------------------------------------------------------------------
RDOTypeParamSuchAs::RDOTypeParamSuchAs(CREF(LPRDOParam) pParam)
	: TypeInfo(pParam->getTypeInfo())
	, m_pParam(pParam               )
{}

RDOTypeParamSuchAs::~RDOTypeParamSuchAs()
{}

CREF(LPRDOParam) RDOTypeParamSuchAs::getParam() const
{
	return m_pParam;
}

CLOSE_RDO_PARSER_NAMESPACE
