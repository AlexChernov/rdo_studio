/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      sequence.cpp
 * @authors   ���� ���������, ������ ������
 * @date      13.03.2011
 * @brief     ������������������
 * @indent    4T
 *********************************************************************************/

// **************************************************************************** PCH
#include "rdo_lib/rdo_runtime/pch.h"
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/sequence.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOCalcSeqInit
// ********************************************************************************
REF(RDOValue) RDOCalcSeqInit::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_gen->setSeed(m_base);
	return m_value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
//	delete m_gen;
}

// ********************************************************************************
// ******************** RDOCalcSeqNextUniform
// ********************************************************************************
RDOValue RDOCalcSeqNextUniform::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	RDOValue from = pRuntime->getFuncArgument(0);
	RDOValue to   = pRuntime->getFuncArgument(1);
	if (from > to)
	{
		pRuntime->error(
			rdo::format(_T("��� ������������������ ���� uniform ������ ������� ��������� ������ ���� ������ ���� ����� �������, ������� ��������: %s..%s")
				, from.getAsString().c_str()
				, to  .getAsString().c_str())
			, this
		);
	}
	return m_gen->next(from.getDouble(), to.getDouble());
}

// ********************************************************************************
// ******************** RDOCalcSeqNextNormal
// ********************************************************************************
RDOValue RDOCalcSeqNextNormal::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next(pRuntime->getFuncArgument(0).getDouble(), pRuntime->getFuncArgument(1).getDouble());
}

// ********************************************************************************
// ******************** RDOCalcSeqNextExponential
// ********************************************************************************
RDOValue RDOCalcSeqNextExponential::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next(pRuntime->getFuncArgument(0).getDouble());
}

// ********************************************************************************
// ******************** RDOCalcSeqNextTriangular
// ********************************************************************************
RDOValue RDOCalcSeqNextTriangular::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	RDOValue from = pRuntime->getFuncArgument(0);
	RDOValue top = pRuntime->getFuncArgument(1);
	RDOValue to = pRuntime->getFuncArgument(2);
	if ((from > top) || (top > to))
	{
		pRuntime->error(rdo::format(_T("��� ����������� ������ ������������� ����� ������� ����� �������, ����� ��� ������� ������������, ������ �������: %s, %s, %s")
			, from.getAsString().c_str()
			, top.getAsString().c_str()
			, to.getAsString().c_str()
		, this));
	}
	return m_gen->next(from.getDouble(), top.getDouble(), to.getDouble());
}

// ********************************************************************************
// ******************** RDOCalcSeqNextByHist
// ********************************************************************************
RDOValue RDOCalcSeqNextByHist::getNextValue(CREF(LPRDORuntime) pRuntime)
{
	return m_gen->next();
}

CLOSE_RDO_RUNTIME_NAMESPACE
