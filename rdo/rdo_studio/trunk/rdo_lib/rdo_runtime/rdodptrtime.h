#ifndef RDODTPRTIME_H
#define RDODTPRTIME_H

#include <sstream>
#include "rdo_lib/rdo_runtime/searchtrace.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchRuntime - DPT (������� RDODPTSearch)
// ----------------------------------------------------------------------------
class RDODPTSearchRuntime: public RDODPTSearchTrace
{
DEFINE_IFACTORY(RDODPTSearchRuntime);

private:
	RDODPTSearchRuntime( RDORuntime* runtime, LPIBaseOperationContainer parent, CREF(LPRDOCalc) _pCondition, CREF(LPRDOCalc) _pTermCondition, CREF(LPRDOCalc) _pEvaluateBy, bool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag ):
		RDODPTSearchTrace( runtime, parent ),
		pCondition( _pCondition ),
		pTermCondition( _pTermCondition ),
		pEvaluateBy( _pEvaluateBy ),
		compTops( _compTops )
	{
		setTraceID( runtime->getFreeDPTId() );
		traceFlag = _traceFlag;
	}

	LPRDOCalc pCondition;
	LPRDOCalc pTermCondition;
	LPRDOCalc pEvaluateBy;
	bool      compTops;

	virtual bool   onCheckCondition(CREF(RDORuntime) pRuntime) { return pCondition    ->calcValue(pRuntime).getAsBool(); }
	virtual bool   TermCondition   (CREF(RDORuntime) pRuntime) { return pTermCondition->calcValue(pRuntime).getAsBool(); }
	virtual double EvaluateBy      (CREF(RDORuntime) pRuntime) { return pEvaluateBy   ->calcValue(pRuntime).getDouble(); }
	virtual bool   NeedCompareTops ()                          { return compTops; }
};

} // namespace rdoRuntime

#endif // RDODTPRTIME_H
