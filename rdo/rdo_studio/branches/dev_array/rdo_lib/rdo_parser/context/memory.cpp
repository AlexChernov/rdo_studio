/*
 * copyright: (c) RDO-Team, 2010
 * filename : memory.cpp
 * author   : ������ ������
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/memory.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextMemory
// ----------------------------------------------------------------------------
ContextMemory::ContextMemory()
{
	m_pLocalVariableList = rdo::Factory<LocalVariableList>::create();
	ASSERT(m_pLocalVariableList);
}

LPLocalVariableList ContextMemory::getLocalMemory()
{
	return m_pLocalVariableList;
}

CLOSE_RDO_PARSER_NAMESPACE
