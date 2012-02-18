/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_priority.h
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      04.11.2009
  \brief     �������� ���������� �����������. ������������ ��� ���������� ��.
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PRIORITY_H_
#define _LIB_RUNTIME_PRIORITY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_priority_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOPatternPrior
  \brief     ��������� ��������
*/
class RDOPatternPrior: public IPriority
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPriority)
QUERY_INTERFACE_END

protected:
	RDOPatternPrior();
	virtual ~RDOPatternPrior();

private:
	virtual LPRDOCalc getPrior();
	virtual rbool setPrior(CREF(LPRDOCalc) pPrior);

	LPRDOCalc m_pPrior;
};

/*!
  \class     RDODPTActivityCompare
  \brief     ���������� ����������� �����������
*/
class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare(CREF(LPRDORuntime) pRuntime);
	rbool operator() (CREF(LPIBaseOperation) opr1, CREF(LPIBaseOperation) opr2);

private:
	LPRDORuntime m_pRuntime;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_priority.inl"

#endif // _LIB_RUNTIME_PRIORITY_H_
