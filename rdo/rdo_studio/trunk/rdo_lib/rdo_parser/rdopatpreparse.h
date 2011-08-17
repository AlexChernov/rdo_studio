/*
  \copyright (c) RDO-Team, 2011
  \file      rdopatpreparse.h
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      22.05.10
  \brief     
  \indent    4T
 */

#ifndef _RDOPATPREPARSE_H_
#define _RDOPATPREPARSE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/calc/event_plan.h"
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
	typedef std::list<rdoRuntime::LPRDOCalcEvent> CalcList;

	CREF(tstring)          name           () const;
	void                   attachCalc     (CREF(rdoRuntime::LPRDOCalcEventStop) pCalc);
	void                   attachCalc     (CREF(rdoRuntime::LPRDOCalcEventPlan) pCalc);
	REF(CalcList)          getCalcList    ();
	LPIBaseOperation       getRuntimeEvent() const;
	void                   setRuntimeEvent(LPIBaseOperation pRuntimeEvent);
	rbool                  getRegular     () const;
	rdoRuntime::LPRDOCalc  getInitCalc    () const;
	void                   setInitCalc    (CREF(rdoRuntime::LPRDOCalc) pCalc);

private:
	RDOEvent(CREF(tstring) name, rbool regular);
	virtual ~RDOEvent();

	tstring                m_name;
	rbool                  m_regullar;
	CalcList               m_calcList;
	LPIBaseOperation       m_runtimeEvent;
	rdoRuntime::LPRDOCalc  m_pInitCalc;
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/rdopatpreparse.inl"

#endif // _RDOPATPREPARSE_H_
