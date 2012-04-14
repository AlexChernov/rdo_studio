/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_braces.h
  \author    ������ ������
  \date      16.04.2011
  \brief     ������ ���������� � ����� C++
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ����������� ������
CALC(RDOCalcOpenBrace)
{
DECLARE_FACTORY(RDOCalcOpenBrace)
private:
	RDOCalcOpenBrace();

	DECLARE_ICalc;
};

//! ����������� ������
CALC(RDOCalcCloseBrace)
{
DECLARE_FACTORY(RDOCalcCloseBrace)
private:
	RDOCalcCloseBrace();

	DECLARE_ICalc;
};

//! �������� ���������� �� ������
CALC(RDOCalcBodyBrace)
{
DECLARE_FACTORY(RDOCalcBodyBrace)
public:
	void addCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcBodyBrace();

	RDOCalcList m_calcList;

	DECLARE_ICalc;
};

//! �������� ���������� �� ������ � ������������ ������� return � break
CALC_SUB(RDOCalcFunBodyBrace, RDOFunCalc)
{
DECLARE_FACTORY(RDOCalcFunBodyBrace)
public:
	void addFunCalc(CREF(LPRDOCalc) pCalc);
	void addRetCalc(CREF(LPRDOCalc) pCalc);

private:
	RDOCalcFunBodyBrace();

	RDOCalcList m_calcFunList;

	DECLARE_ICalc;
};

//! ��������� �������
CALC(RDOCalcFunEnd)
{
DECLARE_FACTORY(RDOCalcFunEnd)
private:
	RDOCalcFunEnd();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_BRACES_H_
