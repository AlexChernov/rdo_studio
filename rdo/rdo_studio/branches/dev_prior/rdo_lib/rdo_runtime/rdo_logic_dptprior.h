#ifndef RDO_LOGIC_DPTPRIOR_H
#define RDO_LOGIC_DPTPRIOR_H

#include "rdo_logic.h"
#include "rdo_priority.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
class RDODPTPrior: public RDOLogic, public RDOPatternPrior
{
DEFINE_FACTORY(RDODPTPrior);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogic)
QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END

private:
	LPIBaseOperationContainer  m_troubleContainer;

	virtual void actionWithRDOOprContainer(PTR(RDOSimulator) sim);
	virtual rbool onCheckChildCondition(PTR(RDOSimulator) sim);

	RDODPTPrior (RDOSimulator* sim);
	virtual ~RDODPTPrior();

	IBaseOperation::BOResult RDODPTPrior::onDoOperation( PTR(RDOSimulator) );

};

} // namespace rdoRuntime

#endif // RDO_LOGIC_DPTPRIOR_H
