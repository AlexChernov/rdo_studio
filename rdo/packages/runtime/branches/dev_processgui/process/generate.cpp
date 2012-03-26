/*!
  \copyright (c) RDO-Team, 2012
  \file      generate.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     ���������� �������� GENERATE
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
#include "utils/rdomacros.h"
#include "simulator/runtime/process/generate.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
void RDOPROCGenerate::onStart( CREF(LPRDORuntime) pRuntime )
{
	calcNextTimeInterval(pRuntime);
}

rbool RDOPROCGenerate::onCheckCondition( CREF(LPRDORuntime) pRuntime )
{
	if (m_maxTransCount > 0)
	{
		if (m_TransCount < m_maxTransCount)
		{
			return pRuntime->getCurrentTime() >= timeNext ? true : false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return pRuntime->getCurrentTime() >= timeNext ? true : false;
	}
}

IBaseOperation::BOResult RDOPROCGenerate::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	++m_TransCount;

	RDOValue pValue = m_pCreateAndGoOnTransactCalc->calcValue(pRuntime);

	LPRDOPROCTransact pTransact = pValue.getPointer<RDOPROCTransact>();

	pTransact->setBlock(this);
	pTransact->next();

	PTR(RDOTrace) tracer = pRuntime->getTracer();
	if (!tracer->isNull())
	{
		tracer->getOStream() << pTransact->traceResourceState('\0', pRuntime) << tracer->getEOL();
	}

	calcNextTimeInterval(pRuntime);
	return IBaseOperation::BOR_done;
}

void RDOPROCGenerate::calcNextTimeInterval( CREF(LPRDORuntime) pRuntime )
{
	pRuntime->addTimePoint( timeNext = m_pTimeCalc->calcValue(pRuntime).getDouble() + pRuntime->getCurrentTime(), m_process, this );
}

void RDOPROCGenerate::onStop(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
}

void RDOPROCGenerate::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	UNUSED(pRuntime);
	UNUSED(pParam  );
}

IBaseOperation::BOResult RDOPROCGenerate::onContinue(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return IBaseOperation::BOR_cant_run;
}

CLOSE_RDO_RUNTIME_NAMESPACE
