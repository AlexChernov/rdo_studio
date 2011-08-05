/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_pattern.cpp
 * author   : ������ ������, ����� �������
 * date     : 13.04.2008
 * bref     : �������� �������� ������ ��� �������� ���� ����� ����������� � �������
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/pch.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_event.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPattern
// ----------------------------------------------------------------------------
RDOPattern::RDOPattern(rbool trace)
	: RDORuntimeObject  ()
	, RDOTraceableObject(trace)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPatternEvent
// ----------------------------------------------------------------------------
RDOPatternEvent::RDOPatternEvent(rbool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternEvent::~RDOPatternEvent()
{}

double RDOPatternEvent::getNextTimeInterval( CREF(LPRDORuntime) pRuntime )
{
	double time_next = m_timeCalc->calcValue( pRuntime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	pRuntime->error( rdo::format("������� ������������� ������� � �������. ��������� ������� ��� $Time ����� ������������� ��������: %f", time_next), m_timeCalc );
	return 0;
}

LPIEvent RDOPatternEvent::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) oprName)
{
	LPIEvent ev = RF(RDOEvent)::create(pRuntime, this, traceable(), oprName);
	pRuntime->addRuntimeEvent(parent, ev);
	return ev;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule(rbool trace)
	: RDOPattern(trace)
{}

RDOPatternRule::~RDOPatternRule()
{}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer logic, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIRule rule = RF(RDORule)::create(pRuntime, this, traceable(), _oprName);
	pRuntime->addRuntimeRule(logic, rule);
	return rule;
}

LPIRule RDOPatternRule::createActivity(LPIBaseOperationContainer logic, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIRule rule = RF(RDORule)::create(pRuntime, this, traceable(), pCondition, _oprName);
	pRuntime->addRuntimeRule(logic, rule);
	return rule;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation(rbool trace)
	: RDOPattern(trace)
	, m_timeCalc(NULL )
{}

RDOPatternOperation::~RDOPatternOperation()
{}

double RDOPatternOperation::getNextTimeInterval( CREF(LPRDORuntime) pRuntime )
{
	double time_next = m_timeCalc->calcValue( pRuntime ).getDouble();
	if ( time_next >= 0 ) return time_next;
	pRuntime->error( rdo::format("������� ������������� ��������� �������� � �������. ��������� ������� ��� $Time ����� ������������� ��������: %f", time_next), m_timeCalc );
	return 0;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIOperation operation = RF(RDOOperation)::create(pRuntime, this, traceable(), _oprName);
	pRuntime->addRuntimeOperation(parent, operation);
	return operation;
}

LPIOperation RDOPatternOperation::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIOperation operation = RF(RDOOperation)::create(pRuntime, this, traceable(), pCondition, _oprName);
	pRuntime->addRuntimeOperation(parent, operation);
	return operation;
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard(rbool trace)
	: RDOPatternOperation(trace)
{}

RDOPatternKeyboard::~RDOPatternKeyboard()
{}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(tstring) _oprName)
{
	LPIKeyboard keyboard = RF(RDOKeyboard)::create(pRuntime, this, traceable(), _oprName);
	pRuntime->addRuntimeOperation(parent, keyboard);
	return keyboard;
}

LPIKeyboard RDOPatternKeyboard::createActivity(LPIBaseOperationContainer parent, CREF(LPRDORuntime) pRuntime, CREF(LPRDOCalc) pCondition, CREF(tstring) _oprName)
{
	LPIKeyboard keyboard = RF(RDOKeyboard)::create(pRuntime, this, traceable(), pCondition, _oprName);
	pRuntime->addRuntimeOperation(parent, keyboard);
	return keyboard;
}

CLOSE_RDO_RUNTIME_NAMESPACE
