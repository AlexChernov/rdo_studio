/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_watch.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     ���������� ����������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_watch.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcWatchGroupStart
// --------------------------------------------------------------------------------
RDOCalcWatchGroupStart::RDOCalcWatchGroupStart(CREF(LPIPokazGroup) pResultGroup)
	: m_pResultGroup(pResultGroup)
{
	m_value = RDOValue(0);
}

RDOCalcWatchGroupStart::~RDOCalcWatchGroupStart()
{}

REF(RDOValue) RDOCalcWatchGroupStart::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStart(pRuntime);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcWatchGroupStop
// --------------------------------------------------------------------------------
RDOCalcWatchGroupStop::RDOCalcWatchGroupStop(CREF(LPIPokazGroup) pResultGroup)
	: m_pResultGroup(pResultGroup)
{
	m_value = RDOValue(0);
}

RDOCalcWatchGroupStop::~RDOCalcWatchGroupStop()
{}

REF(RDOValue) RDOCalcWatchGroupStop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pResultGroup);
	m_pResultGroup->onStop(pRuntime);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
