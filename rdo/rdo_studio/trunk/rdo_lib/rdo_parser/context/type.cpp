/*
 * copyright: (c) RDO-Team, 2011
 * file     : type.cpp
 * author   : ������ ������
 * date     : 30.10.2010
 * brief    : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/type.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- TypeContext
// ----------------------------------------------------------------------------
TypeContext::TypeContext(CREF(LPTypeInfo) pType)
	: m_pType(pType)
{}

CREF(LPTypeInfo) TypeContext::getTypeInfo() const
{
	return m_pType;
}

CLOSE_RDO_PARSER_NAMESPACE
