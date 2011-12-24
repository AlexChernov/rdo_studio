/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_logic.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ���������� ���������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_
#define _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
#include "simulator/runtime/calc/operation/calc_unary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! �������� ���������� �
class RDOCalcAnd: public RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator&&>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator&&> parent_type;

	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

//! �������� ���������� ���
class RDOCalcOr: public RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator||>
{
DECLARE_FACTORY(RDOCalcOr)
private:
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, RDOValue, &RDOValue::operator||> parent_type;

	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

//! �������� ���������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcUnary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator!)> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcUnary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator! > RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif

//! �������� ���������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator==)> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator== > RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif

//! �������� �����������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator!=)> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator!= > RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif

//! �������� ������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator<)> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator< > RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif

//! �������� ������
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator>)> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator> > RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif

//! �������� ������ ��� �����
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator<=)> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator<= > RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif

//! �������� ������ ��� �����
#ifdef COMPILER_VISUAL_STUDIO
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool, (&RDOValue::operator>=)> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif
#ifdef COMPILER_GCC
	typedef RDOCalcBinary<OperatorType::OT_LOGIC, rbool,  &RDOValue::operator>= > RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_logic.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_LOGIC_H_
