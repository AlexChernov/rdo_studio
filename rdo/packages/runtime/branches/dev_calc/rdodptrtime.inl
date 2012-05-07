/*!
  \copyright (c) RDO-Team, 2011
  \file      rdodptrtime.inl
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      22.07.2011
  \brief     Runtime ����������� ��� DPTSearch
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDODPTSearchRuntime
// --------------------------------------------------------------------------------
inline RDODPTSearchRuntime::RDODPTSearchRuntime(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent, CREF(LPRDOCalc) _pCondition, CREF(LPRDOCalc) _pTermCondition, CREF(LPRDOCalc) _pEvaluateBy, rbool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag)
	: RDODPTSearchTrace(pRuntime, parent)
	, pCondition       (_pCondition     )
	, pTermCondition   (_pTermCondition )
	, pEvaluateBy      (_pEvaluateBy    )
	, compTops         (_compTops       )
{
	setTraceID(pRuntime->getFreeDPTId());
	traceFlag = _traceFlag;
}

inline rbool  RDODPTSearchRuntime::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	pCondition->calcValue(pRuntime);
	return pRuntime->stack().pop().getAsBool();
}

inline rbool RDODPTSearchRuntime::TermCondition(CREF(LPRDORuntime) pRuntime)
{
	pTermCondition->calcValue(pRuntime);
	return pRuntime->stack().pop().getAsBool();
}

inline double RDODPTSearchRuntime::EvaluateBy(CREF(LPRDORuntime) pRuntime)
{
	pEvaluateBy->calcValue(pRuntime);
	return pRuntime->stack().pop().getDouble();
}

inline rbool RDODPTSearchRuntime::NeedCompareTops()
{
	return compTops;
}

CLOSE_RDO_RUNTIME_NAMESPACE
