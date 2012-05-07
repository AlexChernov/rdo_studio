/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_braces.cpp
  \author    ������ ������
  \date      16.04.2011
  \brief     ������ ���������� � ����� C++
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_braces.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcOpenBrace
// --------------------------------------------------------------------------------
RDOCalcOpenBrace::RDOCalcOpenBrace()
{}

void RDOCalcOpenBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOMemory pLocalMemory = rdo::Factory<RDOMemory>::create();
	pRuntime->getMemoryStack()->push(pLocalMemory);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcCloseBrace
// --------------------------------------------------------------------------------
RDOCalcCloseBrace::RDOCalcCloseBrace()
{}

void RDOCalcCloseBrace::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getMemoryStack()->pop();
}

CLOSE_RDO_RUNTIME_NAMESPACE
