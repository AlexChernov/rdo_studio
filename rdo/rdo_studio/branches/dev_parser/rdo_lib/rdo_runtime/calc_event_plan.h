/*
 * copyright: (c) RDO-Team, 2010
 * filename : calc_event_plan.h
 * author   : ����� �������
 * date     : 26.05.2010
 * bref     : RDOCalc ��� ������������ �������
 * indent   : 4T
 */

#ifndef _CALC_EVENT_PLAN_H_
#define _CALC_EVENT_PLAN_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEventPlan
// ----------------------------------------------------------------------------
CALC(RDOCalcEventPlan)
{
public:
	RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc);

	void setEvent(CREF(LPIBaseOperation) event);

private:
	LPIBaseOperation  m_event;
	LPRDOCalc         m_pTimeCalc;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _CALC_EVENT_PLAN_H_
