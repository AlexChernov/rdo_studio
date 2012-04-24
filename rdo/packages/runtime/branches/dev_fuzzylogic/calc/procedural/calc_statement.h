/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_statement.h
  \authors   ������ ������
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \authors   ���������� ������� (kurt.gigacore@gmail.com)
  \date      16.04.2011
  \brief     ����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_STATEMENT_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_STATEMENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������ ��������
//! @todo �������� �� CalcNop ?
CALC(RDOCalcNoChange)
{
DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECLARE_ICalc;
};

//! �������� �������� if () then {}
CALC(RDOCalcIf)
{
DECLARE_FACTORY(RDOCalcIf)
public:
	void  setThenStatement(CREF(LPRDOCalc) pStatement);
	void  setElseStatement(CREF(LPRDOCalc) pStatement);

	rbool hasElse() const;

private:
	typedef  std::pair<LPRDOCalc, LPRDOCalc>  Statements;

	RDOCalcIf(CREF(LPRDOCalc) pCondition);

	LPRDOCalc   m_pCondition;
	Statements  m_statements;

	DECLARE_ICalc;
};

//! �������� ����� for
CALC(RDOCalcFor)
{
DECLARE_FACTORY(RDOCalcFor)
public:
	void setStatement(CREF(LPRDOCalc) pStatement);

private:
	RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression);

	LPRDOCalc m_pDeclaration;
	LPRDOCalc m_pCondition;
	LPRDOCalc m_pExpression;
	LPRDOCalc m_pStatement;

	DECLARE_ICalc;
};

//! �������� �������� return
CALC(RDOCalcFunReturn)
{
DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(CREF(LPRDOCalc) pReturn);

	LPRDOCalc m_pReturn;
	DECLARE_ICalc;
};

//! �������� �������� break
CALC(RDOCalcFunBreak)
{
DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECLARE_ICalc;
};

//! ������ ����������
CALC(RDOCalcStatementList)
{
DECLARE_FACTORY(RDOCalcStatementList)
public:
	void        addCalcStatement(CREF(LPRDOCalc) pStatement);
	RDOCalcList statementList();

private:
	RDOCalcStatementList();

	RDOCalcList m_calcStatementList;

	DECLARE_ICalc;
};

//! ������� ��� break
CALC(RDOCalcBreakCatch)
{
DECLARE_FACTORY(RDOCalcBreakCatch)
public:
	void addStatementList(CREF(LPRDOCalc) pStatementList);

private:
	RDOCalcBreakCatch();
	
	LPRDOCalc m_pStatementList;

	DECLARE_ICalc;
};

//! ������� ��� return
CALC(RDOCalcReturnCatch)
{
DECLARE_FACTORY(RDOCalcReturnCatch)
public:
	void addStatementList(CREF(LPRDOCalc) pStatementList);

private:
	RDOCalcReturnCatch();
	
	LPRDOCalc m_pStatementList;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_STATEMENT_H_
