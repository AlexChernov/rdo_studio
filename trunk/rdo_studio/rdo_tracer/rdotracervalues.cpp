#include "stdafx.h"
#include "rdotracervalues.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerTimeNow
// ----------------------------------------------------------------------------
RDOTracerTimeNow::RDOTracerTimeNow( const double _time ) : time( _time )
{
	eventCount = 0;
}

RDOTracerTimeNow::RDOTracerTimeNow( const double _time, const int _event_count )
{
	time = _time;
	eventCount = _event_count;
}

RDOTracerTimeNow& RDOTracerTimeNow::operator =( const RDOTracerTimeNow& timenow )
{
	time         = timenow.time;
	eventCount   = timenow.eventCount;

	return *this;
}

bool RDOTracerTimeNow::operator ==( const RDOTracerTimeNow& timenow ) const
{
	return time         == timenow.time &&
	       eventCount   == timenow.eventCount;
}

bool RDOTracerTimeNow::operator !=( const RDOTracerTimeNow& timenow ) const
{
	return !(*this == timenow);
}

bool RDOTracerTimeNow::compareTimes( const RDOTracerTimeNow* timenow )
{
	bool res = false;
	if( timenow ) {
		res = ( time >= timenow->time );
	}
	return res;
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerValue
// ----------------------------------------------------------------------------
RDOTracerValue::RDOTracerValue( RDOTracerTimeNow* const timenow, const int _eventIndex, const double _value )
	: modeltime( timenow ),
	eventIndex( _eventIndex ),
	value( _value )
{
}

RDOTracerValue::~RDOTracerValue()
{
}
