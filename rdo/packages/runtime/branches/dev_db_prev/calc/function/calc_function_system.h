/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_system.h
  \author    ���� ���������
  \date      20.12.2011
  \brief     ��������� �������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_
#define _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ��������� �������� ���������� �������
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECLARE_ICalc;
};

//! ��������� ��������� ������� ������ ������
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_FUNCTION_SYSTEM_H_
