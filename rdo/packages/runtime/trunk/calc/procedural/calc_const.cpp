/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_const.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     ������ ��������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
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

RDOValue RDOCalcConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
