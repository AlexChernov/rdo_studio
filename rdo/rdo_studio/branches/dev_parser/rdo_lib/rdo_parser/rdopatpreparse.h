/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopatpreparse.h
 * author   : ����� �������
 * date     : 22.05.10
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPATPREPARSE_H_
#define _RDOPATPREPARSE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/calc_event_plan.h"
#include "rdo_lib/rdo_runtime/rdo_event.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
OBJECT(RDOEvent)
{
DECLARE_FACTORY(RDOEvent);
public:
	typedef std::list<rdoRuntime::LPRDOCalcEventPlan> CalcList;

	CREF(tstring)    name           () const;
	void             attachCalc     (CREF(rdoRuntime::LPRDOCalcEventPlan) pCalc);
	REF(CalcList)    getCalcList    ();
	LPIBaseOperation getRuntimeEvent() const;
	void             setRuntimeEvent(LPIBaseOperation pRuntimeEvent);

private:
	RDOEvent(CREF(tstring) name);
	virtual ~RDOEvent();

	tstring          m_name;
	CalcList         m_calcList;
	LPIBaseOperation m_runtimeEvent;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdopatpreparse.inl"

#endif // _RDOPATPREPARSE_H_
