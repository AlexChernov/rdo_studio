/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     ������� � �������� FUN
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOFunCalc
// --------------------------------------------------------------------------------
RDOFunCalc::RDOFunCalc()
{}

void RDOFunCalc::addRetCalc(CREF(LPRDOCalc) pCalc)
{
	UNUSED(pCalc);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFuncTableCalc
// --------------------------------------------------------------------------------
// ������� ���� �������
// --------------------------------------------------------------------------------
RDOFuncTableCalc::RDOFuncTableCalc(CREF(LPRDOCalc) pArgument)
	: m_pArgument(pArgument)
{
	ASSERT(m_pArgument);
}

void RDOFuncTableCalc::addResultCalc(CREF(LPRDOCalcConst) pResult)
{
	m_pResultList.push_back(pResult);
}

void RDOFuncTableCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pArgument->calcValue(pRuntime);
	ruint index = pRuntime->stack().pop().getUInt();
	m_pResultList[index]->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunListCalc
// --------------------------------------------------------------------------------
// ������� ���� ������
// --------------------------------------------------------------------------------
RDOFunListCalc::RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue)
	: m_pDefaultValue(pDefaultValue)
{}

void RDOFunListCalc::addCase(CREF(LPRDOCalc) pCase, CREF(LPRDOCalcConst) pResult)
{
	ASSERT(pCase  );
	ASSERT(pResult);

	m_caseList  .push_back(pCase  ); 
	m_resultList.push_back(pResult);
}

void RDOFunListCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ResultList::const_iterator resultIt = m_resultList.begin();
	STL_FOR_ALL_CONST(m_caseList, caseIt)
	{
		(*caseIt)->calcValue(pRuntime);
		if (pRuntime->stack().pop().getAsBool())
		{
			(*resultIt)->calcValue(pRuntime);
			return;
		}
		++resultIt;
	}
	m_pDefaultValue->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFunAlgorithmicCalc
// --------------------------------------------------------------------------------
RDOFunAlgorithmicCalc::RDOFunAlgorithmicCalc()
{}

void RDOFunAlgorithmicCalc::addCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pAction)
{
	ASSERT(pCondition);
	ASSERT(pAction   );

	m_conditionList.push_back(pCondition);
	m_actionList   .push_back(pAction   );
}

void RDOFunAlgorithmicCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	CalcList::const_iterator actionIt = m_actionList.begin();
	STL_FOR_ALL_CONST(m_conditionList, conditionIt)
	{
		(*conditionIt)->calcValue(pRuntime);
		if (pRuntime->stack().pop().getAsBool())
		{
			(*actionIt)->calcValue(pRuntime);
			return;
		}
		++actionIt;
	}

	// �� ���� ���� ����� �� ������, �.�. ��������� conditions ������ ���� ��������� ��-���������
	pRuntime->error().push(_T("���������� ������, RDOFunAlgorithmicCalc"), srcInfo());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFuncParam
// --------------------------------------------------------------------------------
RDOCalcFuncParam::RDOCalcFuncParam(ruint paramID, CREF(RDOSrcInfo) src_info)
	: m_paramID(paramID)
{
	setSrcInfo(src_info);
}

void RDOCalcFuncParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->stack().push(pRuntime->getFuncArgument(m_paramID));
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetConst
// --------------------------------------------------------------------------------
RDOCalcGetConst::RDOCalcGetConst(ruint constantID)
	: m_constantID(constantID)
{}

void RDOCalcGetConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->stack().push(pRuntime->getConstValue(m_constantID));
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcSetConst
// --------------------------------------------------------------------------------
RDOCalcSetConst::RDOCalcSetConst(ruint constantID, CREF(LPRDOCalc) pCalc)
	: m_constantID(constantID)
	, m_pCalc     (pCalc     )
{
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->srcInfo());
	}
}

void RDOCalcSetConst::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCalc->calcValue(pRuntime);
	pRuntime->setConstValue(m_constantID, pRuntime->stack().pop());
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunctionCaller
// --------------------------------------------------------------------------------
RDOCalcFunctionCaller::RDOCalcFunctionCaller(CREF(LPRDOCalc) pFunction)
	: m_pFunction(pFunction)
{}

void RDOCalcFunctionCaller::addParameter(CREF(LPRDOCalc) pParam)
{
	m_paramList.push_back(pParam);
}

void RDOCalcFunctionCaller::setFunctionCalc(CREF(LPRDOCalc) pFunction)
{
	m_pFunction = pFunction;
}

void RDOCalcFunctionCaller::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->pushFuncTop();
	STL_FOR_ALL_CONST(m_paramList, paramIt)
	{
		(*paramIt)->calcValue(pRuntime);
		pRuntime->pushFuncArgument(pRuntime->stack().pop());
	}
	pRuntime->resetFuncTop(m_paramList.size());

	m_pFunction->calcValue(pRuntime);

	STL_FOR_ALL_CONST(m_paramList, paramIt)
	{
		pRuntime->popFuncArgument();
	}
	pRuntime->popFuncTop();
}

CLOSE_RDO_RUNTIME_NAMESPACE
