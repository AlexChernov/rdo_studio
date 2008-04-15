#ifndef RULETRACE_H
#define RULETRACE_H

#include "rdo.h"
#include "rdotrace.h"

namespace rdoRuntime {

class RDORuleTrace: public RDORule, public RDOPatternTrace, public RDORuntimeContainer
{
friend class RDOTrace;

protected:
	RDORuleTrace( RDORuntime* sim, bool trace );
	virtual void onBeforeRule( RDOSimulator* sim );
	virtual void onAfterRule( RDOSimulator* sim, bool inSearch = false );

private:
   virtual const std::string& tracePatternId() const = 0;
};

} // namespace rdoRuntime

#endif // RULETRACE_H
