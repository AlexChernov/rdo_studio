/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_watch.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     ���������� ����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_WATCH_H_
#define _LIB_RUNTIME_CALC_WATCH_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/result/result_group_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������ ����� ���������� �� ������ �����������
CALC(RDOCalcWatchGroupStart)
{
DECLARE_FACTORY(RDOCalcWatchGroupStart)
protected:
	RDOCalcWatchGroupStart(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStart();

	LPIPokazGroup m_pResultGroup;

	DECLARE_ICalc;
};

//! ��������� ����� ���������� �� ������ �����������
CALC(RDOCalcWatchGroupStop)
{
DECLARE_FACTORY(RDOCalcWatchGroupStop)
protected:
	RDOCalcWatchGroupStop(CREF(LPIPokazGroup) pResultGroup);
	virtual ~RDOCalcWatchGroupStop();

	LPIPokazGroup m_pResultGroup;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_WATCH_H_
