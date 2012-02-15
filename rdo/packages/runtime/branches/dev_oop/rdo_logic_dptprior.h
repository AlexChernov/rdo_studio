/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_dptprior.h
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      04.11.2009
  \brief     DPTPrior
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_DPTPRIOR_H_
#define _LIB_RUNTIME_LOGIC_DPTPRIOR_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOOrderDPTPrior
  \brief   ���������� ������� ��� ������������ ����� ��
*/
class RDOOrderDPTPrior
{
public:
	static LPIBaseOperation sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container);
};

/*!
  \class   RDOLogicDPTPrior
  \brief   ������ �� ��� DPTPrior
*/
class RDOLogicDPTPrior: public RDOLogic<RDOOrderDPTPrior>
{
protected:
	DEFINE_IFACTORY(RDOLogicDPTPrior);

	RDOLogicDPTPrior(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer parent)
		: RDOLogic<RDOOrderDPTPrior>(pRuntime, parent)
	{}
	virtual ~RDOLogicDPTPrior()
	{}
};

/*!
  \class   RDODPTPrior
  \brief   ����� �������� ������� DPTPrior
*/
class RDODPTPrior: public RDOLogicDPTPrior, public RDOPatternPrior
{
DEFINE_IFACTORY(RDODPTPrior);
QUERY_INTERFACE_BEGIN
QUERY_INTERFACE_PARENT(RDOLogicDPTPrior)
QUERY_INTERFACE_PARENT(RDOPatternPrior )
QUERY_INTERFACE_END
private:
	RDODPTPrior(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer pParent = NULL);
	virtual ~RDODPTPrior();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_logic_dptprior.inl"

#endif // _LIB_RUNTIME_LOGIC_DPTPRIOR_H_
