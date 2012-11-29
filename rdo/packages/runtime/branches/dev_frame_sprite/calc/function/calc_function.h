/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     ������� � �������� FUN
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_FUNCTION_H_
#define _LIB_RUNTIME_CALC_FUNCTION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������� ���� ������� � �������� FUN
CALC(RDOFunCalc)
{
public:
	virtual void addRetCalc(CREF(LPRDOCalc) pCalc);

protected:
	RDOFunCalc();
};

//! ��������� �������
CALC_SUB(RDOFuncTableCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(CREF(LPRDOCalcConst) pResult);

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgument);

	typedef  std::vector<LPRDOCalcConst>  ResultList;

	ResultList  m_pResultList;
	LPRDOCalc   m_pArgument;

	DECLARE_ICalc;
};

//! ������� ���� ������
CALC_SUB(RDOFunListCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunListCalc)
public:
	void addCase(CREF(LPRDOCalc) pCase, CREF(LPRDOCalcConst) pResult);

private:
	RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue);

	typedef  std::vector<LPRDOCalcConst>  ResultList;

	RDOCalcList     m_caseList;
	ResultList      m_resultList;
	LPRDOCalcConst  m_pDefaultValue;

	DECLARE_ICalc;
};

//! ��������������� �������
CALC_SUB(RDOFunAlgorithmicCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(CREF(LPRDOCalc) pCondition, CREF(LPRDOCalc) pAction);

protected:
	RDOFunAlgorithmicCalc();

	RDOCalcList  m_conditionList;
	RDOCalcList  m_actionList;

	DECLARE_ICalc;
};

//! �������� �������
CALC(RDOCalcFuncParam)
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(ruint paramID, CREF(RDOSrcInfo) src_info);

	ruint m_paramID;

	DECLARE_ICalc;
};

//! ��������� ��������� � �������� FUN
CALC(RDOCalcGetConst)
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(ruint constantID);

	ruint m_constantID;

	DECLARE_ICalc;
};

//! ������������� ���������
CALC(RDOCalcSetConst)
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(ruint constantID, CREF(LPRDOCalc) pCalc);

	ruint      m_constantID;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

//! ����� ������� (function-caller)
CALC(RDOCalcFunctionCaller)
{
DECLARE_FACTORY(RDOCalcFunctionCaller)
public:
	void addParameter   (CREF(LPRDOCalc) pParam   );
	void setFunctionCalc(CREF(LPRDOCalc) pFunction);

private:
	RDOCalcFunctionCaller(CREF(LPRDOCalc) pFunction);

	RDOCalcList  m_paramList;
	LPRDOCalc    m_pFunction;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_FUNCTION_H_
