#include "stdafx.h"
#include "rdotraceroperation.h"
#include "rdotracervalues.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperationBase
// ----------------------------------------------------------------------------
RDOTracerOperationBase::RDOTracerOperationBase( RDOTracerPattern* const pat )
	: RDOTracerSerie( RDOST_OPERATION ),
	pattern( pat )
{
}

RDOTracerOperationBase::~RDOTracerOperationBase()
{
}

void RDOTracerOperationBase::incOperationsCount( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* newval = new RDOTracerValue( time, eventIndex );
	RDOTracerValue* prevval = getLastValue();
	newval->value = prevval ? prevval->value + 1 : 1;
	addValue( newval );
}

void RDOTracerOperationBase::getCaptions( vector<string> &captions, const int val_count ) const
{
	RDOTracerSerie::getCaptionsInt( captions, val_count );
}

void RDOTracerOperationBase::monitorTime( RDOTracerTimeNow* const time, const int eventIndex )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerOperation
// ----------------------------------------------------------------------------
RDOTracerOperation::RDOTracerOperation( RDOTracerPattern* const pat ) :
	RDOTracerOperationBase( pat )
{
}

RDOTracerOperation::~RDOTracerOperation()
{
}

void RDOTracerOperation::start( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerOperationBase::incOperationsCount( time, eventIndex );
}

void RDOTracerOperation::accomplish( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* newval = new RDOTracerValue( time, eventIndex );
	newval->value = getLastValue()->value - 1;
	addValue( newval );
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerIrregularEvent
// ----------------------------------------------------------------------------
RDOTracerEvent::RDOTracerEvent( RDOTracerPattern* const pat ) :
	RDOTracerOperationBase( pat )
{
}

RDOTracerEvent::~RDOTracerEvent()
{
}

void RDOTracerEvent::occurs( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerOperationBase::incOperationsCount( time, eventIndex );
}

void RDOTracerEvent::monitorTime( RDOTracerTimeNow* const time, const int eventIndex )
{
	RDOTracerValue* prevval = getLastValue();
	RDOTracerValue* newval = NULL;
	if ( prevval && prevval->value != 0 ) {
		if ( *prevval->getModelTime() != *time ) {
			newval = new RDOTracerValue( prevval->getModelTime(), prevval->getModelTime()->eventCount );
			newval->value = 0;
		}
	}
	if ( !prevval ) {
		newval = new RDOTracerValue( time, eventIndex );
		newval->value = 0;
	}
	if( newval ) {
		addValue( newval );
	}
}