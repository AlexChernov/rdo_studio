/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      statements.cpp
 * @authors   ������ ������, ����� �������
 * @date      16.04.2011
 * @brief     ����������
 * @indent    4T
 *********************************************************************************/

// ---------------------------------------------------------------------------- PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/statements.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcNoChange
// --------------------------------------------------------------------------------
RDOCalcNoChange::RDOCalcNoChange()
{}

REF(RDOValue) RDOCalcNoChange::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = RDOValue(0);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcIf
// --------------------------------------------------------------------------------
RDOCalcIf::RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement)
	: m_pCondition(pCondition)
	, m_pStatement(pStatement)
{
	ASSERT(m_pCondition);
	ASSERT(m_pStatement);
}

REF(RDOValue) RDOCalcIf::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = RDOValue(false);
	return (m_pCondition->calcValue(pRuntime).getAsBool()) ? m_pStatement->calcValue(pRuntime) : (m_value);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcIfElse
// --------------------------------------------------------------------------------
RDOCalcIfElse::RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement)
	: m_pCondition    (pCondition    )
	, m_pIfStatement  (pIfStatement  )
	, m_pElseStatement(pElseStatement)
{
	ASSERT(m_pCondition    );
	ASSERT(m_pIfStatement  );
	ASSERT(m_pElseStatement);
}

REF(RDOValue) RDOCalcIfElse::doCalc(CREF(LPRDORuntime) pRuntime)
{
	return (m_pCondition->calcValue(pRuntime).getAsBool()) ? m_pIfStatement->calcValue(pRuntime) : m_pElseStatement->calcValue(pRuntime);
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFor
// --------------------------------------------------------------------------------
RDOCalcFor::RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression, CREF(LPRDOCalc) pStatement)
	: m_pDeclaration(pDeclaration)
	, m_pCondition  (pCondition  )
	, m_pExpression (pExpression )
	, m_pStatement  (pStatement  )
{
	ASSERT(m_pDeclaration);
	ASSERT(m_pCondition  );
	ASSERT(m_pExpression );
	ASSERT(m_pStatement  );
}

REF(RDOValue) RDOCalcFor::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_CONTINUE)
	{
		m_value = m_pDeclaration->calcValue(pRuntime);
		while (m_pCondition->calcValue(pRuntime).getAsBool())
		{
			m_value = m_pStatement->calcValue(pRuntime);
			m_pExpression->calcValue(pRuntime);
			if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_BREAK)
			{
				pRuntime->setFunBreakFlag(RDORuntime::FBF_CONTINUE);
				return m_value;
			}
			if(pRuntime->getFunBreakFlag() == RDORuntime::FBF_RETURN)
			{
				return m_value;
			}
		}
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunReturn
// --------------------------------------------------------------------------------
RDOCalcFunReturn::RDOCalcFunReturn(CREF(LPRDOCalc) pReturn)
	: m_pReturn(pReturn)
{}

REF(RDOValue) RDOCalcFunReturn::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = m_pReturn->calcValue(pRuntime);
	pRuntime->setFunBreakFlag(RDORuntime::FBF_RETURN);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcFunBreak
// --------------------------------------------------------------------------------
RDOCalcFunBreak::RDOCalcFunBreak()
{}

REF(RDOValue) RDOCalcFunBreak::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setFunBreakFlag(RDORuntime::FBF_BREAK);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
