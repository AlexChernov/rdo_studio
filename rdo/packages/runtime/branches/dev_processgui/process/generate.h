/*!
  \copyright (c) RDO-Team, 2012
  \file      generate.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     ���������� �������� GENERATE
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_GENERATE_H_
#define _LIB_RUNTIME_PROCESS_GENERATE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOPROCGenerate
  \brief   ���������� ���� GENERATE
*/
class RDOPROCGenerate: public RDOPROCBlock, public IBaseOperation 
{
DEFINE_IFACTORY(RDOPROCGenerate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

public:
	void calcNextTimeInterval(CREF(LPRDORuntime) pRuntime);

private:
	RDOPROCGenerate(LPIPROCProcess process, CREF(LPRDOCalc) pTime, CREF(LPRDOCalc) pCreateAndGoOnTransactCalc, int maxTransCount = 0);

	double     timeNext;
	LPRDOCalc  m_pTimeCalc;
	LPRDOCalc  m_pCreateAndGoOnTransactCalc;
	int        m_maxTransCount;
	int        m_TransCount;

	DECLARE_IBaseOperation;
};

/*!
  \interface IProcAnimation
  \brief     ��������� IProcAnimation
*/
OBJECT_INTERFACE(IProcAnimation)
{
DECLARE_FACTORY(IProcAnimation)
public:
	virtual void setTransCount(int count) = 0;
	
protected:
	IProcAnimation()
	{}
	virtual ~IProcAnimation()
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/generate.inl"

#endif // _LIB_RUNTIME_PROCESS_GENERATE_H_
