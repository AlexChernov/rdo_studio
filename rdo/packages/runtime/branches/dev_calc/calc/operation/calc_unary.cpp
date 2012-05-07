/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_unary.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ������� ���������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_unary.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- ������� ��������
// --------------------------------------------------------------------------------
void RDOCalcDoubleToIntByResult::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pOperation->calcValue(pRuntime);

	if (m_round)
	{
		pRuntime->stack().push(pRuntime->stack().pop().getInt());
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcInt (���������� � ������)
// --------------------------------------------------------------------------------
void RDOCalcInt::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pOperation->calcValue(pRuntime);
	RDOValue res = pRuntime->stack().pop();
	pRuntime->stack().push(res > 0
		? RDOValue((int)(res.getDouble() + 0.5))
		: RDOValue((int)(res.getDouble() - 0.5))
	);
}

CLOSE_RDO_RUNTIME_NAMESPACE
