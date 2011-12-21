/*!
  \copyright (c) RDO-Team, 2011
  \file      arithm.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     �������������� ��������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/operation/operation_type.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcDiv
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcDiv::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	REF(RDOValue) rVal = m_pRight->calcValue(pRuntime);
	if (rVal == 0)
	{
		pRuntime->error(_T("������� �� ����") , this);
//		pRuntime->error(_T("Division by zero"), this);
	}
	m_value = m_pLeft->calcValue(pRuntime) / rVal;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPlusEnumSafe
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcPlusEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	m_value = m_pLeft->calcValue(pRuntime).getEnumAsInt() + m_pRight->calcValue(pRuntime).getEnumAsInt();
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcMultEnumSafe
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcMultEnumSafe::doCalc(CREF(LPRDORuntime) pRuntime)
{
	++OperatorType::getCalcCounter<OperatorType::Type(calc_type)>();
	m_value = m_pLeft->calcValue(pRuntime).getEnumAsInt() * m_pRight->calcValue(pRuntime).getEnumAsInt();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
