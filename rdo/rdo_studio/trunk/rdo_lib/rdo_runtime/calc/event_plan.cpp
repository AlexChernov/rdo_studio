/*!
  \copyright (c) RDO-Team, 2010
  \file      event_plan.cpp
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      26.05.2010
  \brief     RDOCalc ��� ������������, ������� � ��������� ������� � ������������ �������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/event_plan.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEvent
// --------------------------------------------------------------------------------
RDOCalcEvent::RDOCalcEvent()
{}

void RDOCalcEvent::setEvent(CREF(LPIBaseOperation) pEvent)
{
	ASSERT(pEvent);
	m_pEvent = pEvent;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEventPlan
// --------------------------------------------------------------------------------
RDOCalcEventPlan::RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc)
	: m_pTimeCalc(pTimeCalc)
{
	ASSERT(m_pTimeCalc);
}

REF(RDOValue) RDOCalcEventPlan::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pEvent);
	m_value = m_pTimeCalc->calcValue(pRuntime);
	pRuntime->addTimePoint(m_value.getDouble(), m_pEvent);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcEventStop
// --------------------------------------------------------------------------------
RDOCalcEventStop::RDOCalcEventStop()
{}

REF(RDOValue) RDOCalcEventStop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ASSERT(m_pEvent);
	pRuntime->removeTimePoint(m_pEvent);
	m_value = RDOValue(0);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
