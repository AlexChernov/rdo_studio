/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_nop.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     ������ ��������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcNop
// --------------------------------------------------------------------------------
RDOCalcNop::RDOCalcNop()
{}

RDOCalcNop::~RDOCalcNop()
{}

RDOValue RDOCalcNop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE
