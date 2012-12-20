/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_arithm.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     �������������� ��������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_ARITHM_H_
#define _LIB_RUNTIME_CALC_OPERATION_ARITHM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! �������� ������������
typedef RDOCalcBinary<BinaryOperatorNonConstP1<REF(RDOValue), &RDOValue::operator= >, OperatorType::OT_ARITHM> RDOCalcSet;  DECLARE_POINTER(RDOCalcSet);

//! �������� �������� ����
typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator+ >, OperatorType::OT_ARITHM> RDOCalcPlus;  DECLARE_POINTER(RDOCalcPlus);

//! �������� �������� �����
typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator- >, OperatorType::OT_ARITHM> RDOCalcMinus; DECLARE_POINTER(RDOCalcMinus);

//! �������� �������� ��������
typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator* >, OperatorType::OT_ARITHM> RDOCalcMult;  DECLARE_POINTER(RDOCalcMult);

//! �������� �������� ���������
class RDOCalcDiv: public RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator/ >, OperatorType::OT_ARITHM>
{
DECLARE_FACTORY(RDOCalcDiv);
private:
	typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator/ >, OperatorType::OT_ARITHM> parent_type;

	RDOCalcDiv(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECLARE_ICalc;
};
DECLARE_POINTER(RDOCalcDiv);

//! �������� �������� �������� �� ������������
CALC_SUB(RDOCalcPlusEnumSafe, RDOCalcPlus)
{
DECLARE_FACTORY(RDOCalcPlusEnumSafe)
private:
	RDOCalcPlusEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECLARE_ICalc;
};

//! �������� �������� �������� �� ������������
CALC_SUB(RDOCalcMultEnumSafe, RDOCalcMult)
{
DECLARE_FACTORY(RDOCalcMultEnumSafe)
private:
	RDOCalcMultEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_arithm.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_ARITHM_H_
