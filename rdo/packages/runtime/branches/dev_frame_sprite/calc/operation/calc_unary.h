/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_unary.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ������� ���������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_UNARY_H_
#define _LIB_RUNTIME_CALC_OPERATION_UNARY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/calc/operation/operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������� ����� ��� �������� ���������
class RDOCalcUnaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc);

protected:
	RDOCalcUnaryBase(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;
};

/*!
  \class   RDOCalcUnary
  \tparam  ret_type  ������������ ��������
  \tparam  pOperator ��������
  \tparam  CalcType  ��� ��������� ���������
  \brief   ������� ������� ��������
*/
template <typename ret_type, ret_type (RDOValue::*pOperator)() const, typename OperatorType::Type CalcType>
class RDOCalcUnary: public RDOCalcUnaryBase
{
friend class rdo::Factory<RDOCalcUnary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)() const;

	static RDOSrcInfo     getStaticSrcInfo(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pUnaryCalc);
	static value_operator getOperation    ();

protected:
	RDOCalcUnary(CREF(RDOSrcInfo::Position) position, CREF(LPRDOCalc) pOperation);

private:
	DECLARE_ICalc;
};

//! ������� �������� ����
typedef RDOCalcUnary<RDOValue, &RDOValue::operator-, OperatorType::OT_ARITHM> RDOCalcUMinus;

//! �������� �������������� ������������� ����� � �����
typedef RDOCalcUnary<rsint,    &RDOValue::getInt   , OperatorType::OT_ARITHM> RDOCalcDoubleToInt;

//! �������������� ������������� � ����� �� ���� lvalue
CALC_SUB(RDOCalcDoubleToIntByResult, RDOCalc)
{
DECLARE_FACTORY(RDOCalcDoubleToIntByResult)
public:
	void needRound();

private:
	RDOCalcDoubleToIntByResult(CREF(LPRDOCalc) pOper);

	rbool     m_round;
	LPRDOCalc m_pOperation;

	DECLARE_ICalc;
};

//! ���������� � ������
CALC_SUB(RDOCalcInt, RDOCalc)
{
DECLARE_FACTORY(RDOCalcInt)
private:
	RDOCalcInt(CREF(LPRDOCalc) pOperation);

	LPRDOCalc m_pOperation;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_unary.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_UNARY_H_
