/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_nop.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     ������ ��������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_CALC_NOP_H_
#define _LIB_RUNTIME_CALC_CALC_NOP_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������ ��������
CALC(RDOCalcNop)
{
DECLARE_FACTORY(RDOCalcNop)
private:
	RDOCalcNop();
	virtual ~RDOCalcNop();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_CALC_NOP_H_
