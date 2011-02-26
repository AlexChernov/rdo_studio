/*
 * copyright: (c) RDO-Team, 2010
 * filename : local_variable.cpp
 * author   : ������ ������
 * date     : 02.12.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/variable_container.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- VariableContainer
// ----------------------------------------------------------------------------
VariableContainer::VariableContainer(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPLocalVariable) pLocalVariable)
	: m_pCalc        (pCalc        )
	, m_pLocalVariable(pLocalVariable)
{}

CREF(rdoRuntime::LPRDOCalc) VariableContainer::getCalc() const
{
	return m_pCalc;
}

CREF(LPLocalVariable) VariableContainer::getLocalVariable() const
{
	return m_pLocalVariable;
}

CLOSE_RDO_PARSER_NAMESPACE
