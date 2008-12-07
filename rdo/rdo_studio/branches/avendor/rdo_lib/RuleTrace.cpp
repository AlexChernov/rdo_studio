#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RdoTrace.h"

RDORuleTrace::RDORuleTrace(RDOSimulatorTrace *i_sim):
   RDOTraceableObject(i_sim), RDOPattern(i_sim), trace(false)
{}

void RDORuleTrace::onBeforeRule(RDOSimulator *sim)
{
}

void RDORuleTrace::onAfterRule(RDOSimulator *sim, bool inSearch)
{
   onAfter(sim);
   if(!inSearch)
      RDOTraceableObject::sim->getTracer()->writeRule(this, RDOTraceableObject::sim);

   RDOTraceableObject::sim->clearJustCreatedFlags();
}
