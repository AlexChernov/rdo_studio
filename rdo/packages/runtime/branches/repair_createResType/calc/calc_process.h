/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_process.h
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      10.03.2011
  \brief     RDOCalc ��� �������� ���������� � ������� �� �� � �������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCESS_H_
#define _LIB_RUNTIME_CALC_PROCESS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_res_type_i.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! �������� ��������� � ������� � ��� ������
CALC(RDOCalcProcessControl)
{
DECLARE_FACTORY(RDOCalcProcessControl)
public:
protected:
	RDOCalcProcessControl(LPIPROCBlock pBlock, rsint relResNum);
	DECLARE_ICalc;
private:
	LPIPROCBlock  m_Block;
	rsint         m_relResNum;
};

//! ��������� �������� ������������� ��������
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCESS_H_
