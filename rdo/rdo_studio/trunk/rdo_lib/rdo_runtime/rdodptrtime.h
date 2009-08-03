#ifndef RDODTPRTIME_H
#define RDODTPRTIME_H

#include "searchtrace.h"
#include "rdo_runtime.h"
#include "rdocalc.h"
#include <sstream>

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOSearchActivityRuntime - ���������� ������ DPT (������� RDODPTSearch::Activity)
// ----------------------------------------------------------------------------
class RDOSearchActivityRuntime: public RDOActivityTrace
{
public:
	RDOSearchActivityRuntime(CREF(LPIRule) r, ValueTime valueTime, RDOCalc* cost ):
		RDOActivityTrace( r, valueTime ),
		m_cost( cost )
	{
	}

private:
	RDOCalc* m_cost;

	virtual double cost( RDOSimulator* runtime )
	{
		return m_cost->calcValue( static_cast<RDORuntime*>(runtime) ).getDouble();
	}
};

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchRuntime - DPT (������� RDODPTSearch)
// ----------------------------------------------------------------------------
class RDODPTSearchRuntime: public RDODPTSearchTrace
{
private:
	RDODPTSearchRuntime( RDORuntime* runtime, RDOCalc* _condition, RDOCalc* _termCondition, RDOCalc* _evaluateBy, bool _compTops, RDODPTSearchTrace::DPT_TraceFlag _traceFlag ):
		RDODPTSearchTrace( runtime ),
		condition( _condition ),
		termCondition( _termCondition ),
		evaluateBy( _evaluateBy ),
		compTops( _compTops )
	{
		setTraceID( runtime->getFreeDPTId() );
		traceFlag = _traceFlag;
	}

	RDOCalc* condition;
	RDOCalc* termCondition;
	RDOCalc* evaluateBy;
	bool compTops;

	virtual bool   onCheckCondition( RDOSimulator* sim ) { return condition->calcValue( static_cast<RDORuntime*>(sim) ).getAsBool();     }
	virtual bool   TermCondition( RDOSimulator* sim )    { return termCondition->calcValue( static_cast<RDORuntime*>(sim) ).getAsBool(); }
	virtual double EvaluateBy( RDOSimulator* sim )       { return evaluateBy->calcValue( static_cast<RDORuntime*>(sim) ).getDouble();    }
	virtual bool   NeedCompareTops()                     { return compTops; }
};

} // namespace rdoRuntime

#endif // RDODTPRTIME_H
