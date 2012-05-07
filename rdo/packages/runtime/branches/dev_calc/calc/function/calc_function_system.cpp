/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_system.cpp
  \author    ���� ���������
  \date      20.12.2011
  \brief     ��������� �������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function_system.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTimeNow
// --------------------------------------------------------------------------------
void RDOCalcGetTimeNow::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->stack().push(pRuntime->getTimeNow());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetSeconds
// --------------------------------------------------------------------------------
void RDOCalcGetSeconds::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->stack().push(pRuntime->getSeconds());
}

CLOSE_RDO_RUNTIME_NAMESPACE
