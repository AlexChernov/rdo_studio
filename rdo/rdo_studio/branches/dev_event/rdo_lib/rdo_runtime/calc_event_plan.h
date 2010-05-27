/*
 * copyright: (c) RDO-Team, 2010
 * filename : calc_event_plan.h
 * author   : ����� �������
 * date     : 26.05.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCALCEVENTPLAN_H_
#define _RDOCALCEVENTPLAN_H_

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
class RDOCalcEventPlan: public RDOCalc
{
public:
	RDOCalcEventPlan(PTR(RDORuntimeParent) parent, CREF(LPIBaseOperation) event, PTR(RDOCalc) timeCalc)
		: RDOCalc   (parent  )
		, m_event   (event   )
		, m_timeCalc(timeCalc)
	{}

private:
	LPIBaseOperation m_event;
	PTR(RDOCalc)     m_timeCalc;

	virtual REF(RDOValue) doCalc(PTR(RDORuntime) runtime);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _RDOCALCEVENTPLAN_H_
