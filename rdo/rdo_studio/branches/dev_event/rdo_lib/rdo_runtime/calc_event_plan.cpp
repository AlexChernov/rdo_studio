/*
 * copyright: (c) RDO-Team, 2010
 * filename : calc_event_plan.cpp
 * author   : ����� �������
 * date     : 26.05.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc_event_plan.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventPlan
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcEventPlan::doCalc(PTR(RDORuntime) runtime)
{
	runtime->addTimePoint(m_timeCalc->calcValue(runtime).getDouble(), m_event);
	return m_timeCalc->calcValue(runtime);
}

CLOSE_RDO_RUNTIME_NAMESPACE
