#ifndef _LIB_RUNTIME_SIM_TRACE_H_
#define _LIB_RUNTIME_SIM_TRACE_H_

#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_simulator.h"

namespace rdoParse {
class RDODPTSome;
}

OPEN_RDO_RUNTIME_NAMESPACE

class RDODPTSearchTrace;
class RDODPTSearchTrace;
class RDOEvent;
class RDORule;
class RDOOperation;

PREDECLARE_POINTER(RDOSimulatorTrace);

CLASS(RDOSimulatorTrace): INSTANCE_OF(RDOSimulator)
{
DECLARE_FACTORY(RDOSimulatorTrace)
public:
	enum { UNDEFINE_TIME = -1 };

	virtual void rdoInit();

	RDOTrace* getTracer() const            { return m_tracer;                }
	rbool     canTrace() const             { return getTracer()->canTrace(); }

	double getTraceStartTime() const       { return traceStartTime;          }
	void setTraceStartTime( double value ) { traceStartTime = value;         }

	double getTraceEndTime() const         { return traceEndTime;            }
	void setTraceEndTime( double value )   { traceEndTime = value;           }

	virtual void onNewTimeNow() {
		if ( timeForTrace() ) {
			getTracer()->startWriting();
		} else {
			getTracer()->stopWriting();
		}
	}

	void memory_insert( unsigned int mem ) {
		memory_current += mem;
		if ( memory_current > memory_max ) memory_max = memory_current;
	}
	void memory_remove( unsigned int mem ) {
		memory_current -= mem;
	}
	unsigned int memory_get() const {
		return memory_max;
	}

	ruint getResourceId();
	void incrementResourceIdReference(int id);

	void freeOperationId(int id);
	int getFreeOperationId(); 
	void onResourceErase(CREF(LPRDOResource) pResource);

	int getFreeEventId()
	{
		return m_eventCounter++;
	}
	int getFreeActivityId()
	{
		return m_activityCounter++;
	}

	int getFreeDPTId()
	{
		return m_dptCounter++;
	}

protected:
	RDOSimulatorTrace():
		RDOSimulator(),
		m_tracer( NULL ),
		traceStartTime( UNDEFINE_TIME ),
		traceEndTime( UNDEFINE_TIME ),
		m_ieCounter( 1 ),
		m_eventCounter( 1 ),
		m_activityCounter( 1 ),
		m_dptCounter( 1 ),
		maxResourcesId( 0 ),
		maxOperationId( 1 ),
		memory_current( 0 ),
		memory_max( 0 )
	{
	}
	virtual ~RDOSimulatorTrace();

	void copyFrom(CREF(LPRDOSimulatorTrace) pOther);

	RDOTrace* m_tracer;

	int maxOperationId;

	virtual void preProcess();
	virtual void postProcess();
	void checkRSSDefinedResources();

	virtual std::list<LPRDOResource> getResourcesBeforeSim() const = 0;

private:
	double traceStartTime;
	double traceEndTime;

	ruint maxResourcesId;

	std::list<ruint> freeResourcesIds;
	typedef std::map<int, int> MAPII;
	MAPII resourcesIdsRefs;
	std::list< int > freeOperationsIds;

	void eraseFreeResourceId(ruint id);

	int m_ieCounter;
	int m_eventCounter;
	int m_activityCounter;
	int m_dptCounter;

	void addTemplateDecisionPoint ( RDODPTSearchTrace *dp   );
	void addTemplateEvent         ( RDOEvent          *ev   );
	void addTemplateRule          ( RDORule           *rule );
	void addTemplateOperation     ( RDOOperation      *op   );

	ruint memory_current;
	ruint memory_max;

	rbool timeForTrace() const
	{
		if ( getTraceStartTime() != UNDEFINE_TIME && getTraceStartTime() > getCurrentTime() ) return false;
		if ( getTraceEndTime()   != UNDEFINE_TIME && getTraceEndTime()   < getCurrentTime() ) return false;
		return true;
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_SIM_TRACE_H_
