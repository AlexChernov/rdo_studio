/*!
  \copyright (c) RDO-Team, 2011
  \file      statements.h
  \authors   ������ ������
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.04.2011
  \brief     ����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_STATEMENTS_H_
#define _LIB_RUNTIME_CALC_STATEMENTS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcNoChange
  \brief   RDOCalc ��� ������������ ��������� ������������ �������
*/
CALC(RDOCalcNoChange)
{
	DECLARE_FACTORY(RDOCalcNoChange)
private:
	RDOCalcNoChange();
	DECLARE_ICalc;
};

/*!
  \class   RDOCalcIf
  \brief   RDOCalc ��� ������������ ��������� ���������
*/
CALC(RDOCalcIf)
{
	DECLARE_FACTORY(RDOCalcIf)
private:
	RDOCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pStatement;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcIfElse
  \brief   RDOCalc ��� ������� ��������� ���������
*/
CALC(RDOCalcIfElse)
{
	DECLARE_FACTORY(RDOCalcIfElse)
private:
	RDOCalcIfElse(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pIfStatement, CREF(LPRDOCalc) pElseStatement);

	LPRDOCalc m_pCondition;
	LPRDOCalc m_pIfStatement;
	LPRDOCalc m_pElseStatement;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcFor
  \brief   RDOCalc ��� ������������ ��������� for
*/
CALC(RDOCalcFor)
{
	DECLARE_FACTORY(RDOCalcFor)
private:
	RDOCalcFor(CREF(LPRDOCalc) pDeclaration, CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pExpression, CREF(LPRDOCalc) pStatement);

	LPRDOCalc m_pDeclaration;
	LPRDOCalc m_pCondition;
	LPRDOCalc m_pExpression;
	LPRDOCalc m_pStatement;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcFunReturn
  \brief   RDOCalc ��� ��������� ������ �� �������
*/
CALC(RDOCalcFunReturn)
{
	DECLARE_FACTORY(RDOCalcFunReturn)
private:
	RDOCalcFunReturn(CREF(LPRDOCalc) pReturn);

	LPRDOCalc m_pReturn;
	DECLARE_ICalc;
};

/*!
  \class   RDOCalcFunBreak
  \brief   RDOCalc ��� ��������� ������ �� ���� �����
*/
CALC(RDOCalcFunBreak)
{
	DECLARE_FACTORY(RDOCalcFunBreak)
private:
	RDOCalcFunBreak();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_STATEMENTS_H_
