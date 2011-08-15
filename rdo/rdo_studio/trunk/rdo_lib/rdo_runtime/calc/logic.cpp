/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      logic.cpp
 * @authors   ���� ���������, ������ ������
 * @date      13.03.2011
 * @brief     ���������� ���������
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/logic.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcAnd
// ********************************************************************************
REF(RDOValue) RDOCalcAnd::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<calc_type>();

	if (!m_pLeft->calcValue(pRuntime).getAsBool())
		return m_value_false;

	if (!m_pRight->calcValue(pRuntime).getAsBool())
		return m_value_false;

	return m_value_true;
}

// ********************************************************************************
// ******************** RDOCalcAnd
// ********************************************************************************
REF(RDOValue) RDOCalcOr::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<calc_type>();

	if (m_pLeft->calcValue(pRuntime).getAsBool())
		return m_value_true;

	if (m_pRight->calcValue(pRuntime).getAsBool())
		return m_value_true;

	return m_value_false;
}

CLOSE_RDO_RUNTIME_NAMESPACE
