#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_ie.h"
#include "rdo_lib/rdo_runtime/rdo_event.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOPattern
// ----------------------------------------------------------------------------
RDOPattern::RDOPattern( PTR(RDORuntime) runtime, bool trace ):
	RDORuntimeParent( runtime ),
	RDOTraceableObject( trace )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent( PTR(RDORuntime) rTime, bool trace ):
	RDOPattern( rTime, trace ),
	m_timeCalc( NULL )
{
}

double RDOPatternIrregEvent::getNextTimeInterval( PTR(RDORuntime) runtime )
{
	double time_next = m_timeCalc->calcValue( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("������� ������������� ������� � �������. ��������� ������� ��� $Time ����� ������������� ��������: %f", time_next), m_timeCalc );
	return 0;
}

LPIIrregEvent RDOPatternIrregEvent::createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) oprName)
{
	LPIIrregEvent ie = F(RDOIrregEvent)::create(runtime, this, traceable(), oprName);
	runtime->addRuntimeIE(parent, ie);
	return ie;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent( PTR(RDORuntime) rTime, bool trace ):
	RDOPattern( rTime, trace )
{
}

LPIEvent RDOPatternEvent::createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) oprName)
{
	LPIEvent ev = F(RDOEvent)::create(runtime, this, traceable(), oprName);
	runtime->addRuntimeEvent(parent, ev);
	return ev;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule( PTR(RDORuntime) rTime, bool trace ):
	RDOPattern( rTime, trace )
{
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer logic, PTR(RDORuntime) runtime, CREF(tstring) _oprName)
{
	LPIRule rule = F(RDORule)::create(runtime, this, traceable(), _oprName);
	runtime->addRuntimeRule(logic, rule);
	return rule;
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer logic, PTR(RDORuntime) runtime, PTR(RDOCalc) condition, CREF(tstring) _oprName)
{
	LPIRule rule = F(RDORule)::create(runtime, this, traceable(), condition, _oprName);
	runtime->addRuntimeRule(logic, rule);
	return rule;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation( PTR(RDORuntime) rTime, bool trace ):
	RDOPattern( rTime, trace ),
	m_timeCalc( NULL )
{
}

double RDOPatternOperation::getNextTimeInterval( PTR(RDORuntime) runtime )
{
	double time_next = m_timeCalc->calcValue( runtime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	runtime->error( rdo::format("������� ������������� ��������� �������� � �������. ��������� ������� ��� $Time ����� ������������� ��������: %f", time_next), m_timeCalc );
	return 0;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) _oprName)
{
	LPIOperation operation = F(RDOOperation)::create(runtime, this, traceable(), _oprName);
	runtime->addRuntimeOperation(parent, operation);
	return operation;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, PTR(RDOCalc) condition, CREF(tstring) _oprName)
{
	LPIOperation operation = F(RDOOperation)::create(runtime, this, traceable(), condition, _oprName);
	runtime->addRuntimeOperation(parent, operation);
	return operation;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard( PTR(RDORuntime) rTime, bool _trace ):
	RDOPatternOperation( rTime, _trace )
{
}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, CREF(tstring) _oprName)
{
	LPIKeyboard keyboard = F(RDOKeyboard)::create(runtime, this, traceable(), _oprName);
	runtime->addRuntimeOperation(parent, keyboard);
	return keyboard;
}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer parent, PTR(RDORuntime) runtime, PTR(RDOCalc) condition, CREF(tstring) _oprName)
{
	LPIKeyboard keyboard = F(RDOKeyboard)::create(runtime, this, traceable(), condition, _oprName);
	runtime->addRuntimeOperation(parent, keyboard);
	return keyboard;
}

} // namespace rdoRuntime
