/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_system.cpp
  \author    ���� ���������
  \date      20.12.2011
  \brief     ��������� �������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function_system.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTimeNow
// --------------------------------------------------------------------------------
RDOValue RDOCalcGetTimeNow::doCalc(CREF(LPRDORuntime) pRuntime)
{
	return pRuntime->getTimeNow();
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetSeconds
// --------------------------------------------------------------------------------
RDOValue RDOCalcGetSeconds::doCalc(CREF(LPRDORuntime) pRuntime)
{
	return pRuntime->getSeconds();
}

CLOSE_RDO_RUNTIME_NAMESPACE
