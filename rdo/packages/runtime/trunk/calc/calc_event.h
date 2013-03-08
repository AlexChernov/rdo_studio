/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_event.h
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      26.05.2010
  \brief     RDOCalc ��� ������������ � ��������� �������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_EVENT_H_
#define _LIB_RUNTIME_CALC_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������� ����� ��� ���������� ���������
CALC(RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEvent)
public:
	void setEvent(CREF(LPIBaseOperation) pEvent);

protected:
	RDOCalcEvent();

	LPIBaseOperation m_pEvent;
};

//! ������������ �������
CALC_SUB(RDOCalcEventPlan, RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEventPlan)
private:
	RDOCalcEventPlan(CREF(LPRDOCalc) pTimeCalc);

	LPRDOCalc m_pTimeCalc;

	DECLARE_ICalc;
};

//! ��������� �������
CALC_SUB(RDOCalcEventStop, RDOCalcEvent)
{
DECLARE_FACTORY(RDOCalcEventStop)
private:
	RDOCalcEventStop();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_EVENT_H_
