/*
 * copyright: (c) RDO-Team, 2010
 * filename : pattern.cpp
 * author   : ������ ������
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/pattern.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextPattern
// ----------------------------------------------------------------------------
ContextPattern::ContextPattern()
{
	m_pLocalVariableList = rdo::Factory<LocalVariableList>::create();
	ASSERT(m_pLocalVariableList);
}

LPLocalVariableList ContextPattern::getLocalMemory()
{
	return m_pLocalVariableList;
}

CLOSE_RDO_PARSER_NAMESPACE
