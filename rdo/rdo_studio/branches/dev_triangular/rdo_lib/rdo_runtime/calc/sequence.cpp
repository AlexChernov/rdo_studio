/*
 * copyright: (c) RDO-Team, 2011
 * filename : sequence.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/sequence.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
REF(RDOValue) RDOCalcSeqInit::doCalc(PTR(RDORuntime) runtime)	
{
	m_gen->setSeed(m_base);
	return m_value;
}

RDOCalcSeqInit::~RDOCalcSeqInit() 
{ 
//	delete m_gen;
}

RDOValue RDOCalcSeqNextUniform::getNextValue(PTR(RDORuntime) runtime)
{
	if (runtime->getFuncArgument(0).getDouble() > runtime->getFuncArgument(1).getDouble())
	{
		runtime->error(rdo::format(_T("��� ������������������ ���� uniform ����� ������� ��� ���������: ������ �������, �������� �������"), this));
	}
	return m_gen->next(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble());
}

RDOValue RDOCalcSeqNextNormal::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble());
}

RDOValue RDOCalcSeqNextExponential::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next(runtime->getFuncArgument(0).getDouble());
}

RDOValue RDOCalcSeqNextTriangular::getNextValue(PTR(RDORuntime) runtime)
{
	if (runtime->getFuncArgument(0).getDouble() > runtime->getFuncArgument(1).getDouble())
	{
		runtime->error(rdo::format(_T("��� ����������� ������ ������������� ����� ������� ��� ���������: ����� �������, ����� ��� ������� ������������, ������ �������"), this));
	}
	
	if (runtime->getFuncArgument(1).getDouble() > runtime->getFuncArgument(2).getDouble())
	{
		runtime->error(rdo::format(_T("��� ����������� ������ ������������� ����� ������� ��� ���������: ����� �������, ����� ��� ������� ������������, ������ �������"), this));
	}
	return m_gen->next(runtime->getFuncArgument(0).getDouble(), runtime->getFuncArgument(1).getDouble(), runtime->getFuncArgument(2).getDouble());
}

RDOValue RDOCalcSeqNextByHist::getNextValue(PTR(RDORuntime) runtime)
{
	return m_gen->next();
}

CLOSE_RDO_RUNTIME_NAMESPACE
