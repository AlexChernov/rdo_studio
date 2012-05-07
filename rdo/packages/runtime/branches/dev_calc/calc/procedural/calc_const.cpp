/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_const.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     ������ ��������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcConst
// --------------------------------------------------------------------------------
RDOCalcConst::RDOCalcConst(CREF(RDOValue) value)
{
	ASSERT(value.typeID() != RDOType::t_unknow);
	m_value = value;
}

CREF(RDOValue) RDOCalcConst::getValue() const
{
	return m_value;
}

void RDOCalcConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->stack().push(getValue());
}

CLOSE_RDO_RUNTIME_NAMESPACE
