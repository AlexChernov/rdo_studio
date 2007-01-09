#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdotrace.h"

RDORuleTrace::RDORuleTrace( RDOSimulatorTrace* i_sim ):
	RDOTraceableObject( i_sim ),
	RDOPatternTrace(),
	trace( false )
{
	id = i_sim->activityCounter++;
}

void RDORuleTrace::onBeforeRule(RDOSimulator *sim)
{
}

void RDORuleTrace::onAfterRule( RDOSimulator* sim, bool inSearch )
{
	if ( !inSearch ) {
		RDOTraceableObject::sim->getTracer()->writeRule( this, RDOTraceableObject::sim );
	}
}
