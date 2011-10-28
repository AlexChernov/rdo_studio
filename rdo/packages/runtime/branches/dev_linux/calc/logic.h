/*!
  \copyright (c) RDO-Team, 2011
  \file      logic.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ���������� ���������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_LOGIC_H_
#define _LIB_RUNTIME_CALC_LOGIC_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/binary.h"
#include "simulator/runtime/calc/unary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOCalcAnd
  \brief   �������� ���������� �
*/
class RDOCalcAnd: public RDOCalcBinary<RDOValue, &RDOValue::operator&&, OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcAnd)
private:
	RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

/*!
  \class   RDOCalcOr
  \brief   �������� ���������� ���
*/
class RDOCalcOr: public RDOCalcBinary<RDOValue, &RDOValue::operator||, OperatorType::OT_LOGIC>
{
DECLARE_FACTORY(RDOCalcOr)
private:
	RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	RDOValue m_value_true;
	RDOValue m_value_false;

	DECLARE_ICalc;
};

/*!
  \typedef RDOCalcNot
  \brief   �������� ���������
*/
#ifdef OST_WINDOWS
	typedef RDOCalcUnary <rbool, (&RDOValue::operator!) , OperatorType::OT_LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#else if OST_LINUX
	typedef RDOCalcUnary <rbool, &RDOValue::operator! , OperatorType::OT_LOGIC> RDOCalcNot;         DECLARE_POINTER(RDOCalcNot);
#endif

/*!
  \typedef RDOCalcIsEqual
  \brief   �������� ���������
*/
#ifdef OST_WINDOWS
	typedef RDOCalcBinary<rbool, (&RDOValue::operator==), OperatorType::OT_LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#else if OST_LINUX
	typedef RDOCalcBinary<rbool, &RDOValue::operator==, OperatorType::OT_LOGIC> RDOCalcIsEqual;     DECLARE_POINTER(RDOCalcIsEqual);
#endif

/*!
  \typedef RDOCalcIsNotEqual
  \brief   �������� �����������
*/
#ifdef OST_WINDOWS
	typedef RDOCalcBinary<rbool, (&RDOValue::operator!=), OperatorType::OT_LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#else if OST_LINUX
	typedef RDOCalcBinary<rbool, &RDOValue::operator!=, OperatorType::OT_LOGIC> RDOCalcIsNotEqual;  DECLARE_POINTER(RDOCalcIsNotEqual);
#endif

/*!
  \typedef RDOCalcIsLess
  \brief   �������� ������
*/
#ifdef OST_WINDOWS
	typedef RDOCalcBinary<rbool, (&RDOValue::operator<), OperatorType::OT_LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#else if OST_LINUX
	typedef RDOCalcBinary<rbool, &RDOValue::operator< , OperatorType::OT_LOGIC> RDOCalcIsLess;      DECLARE_POINTER(RDOCalcIsLess);
#endif

/*!
  \typedef RDOCalcIsGreater
  \brief   �������� ������
*/
#ifdef OST_WINDOWS
	typedef RDOCalcBinary<rbool, (&RDOValue::operator>) , OperatorType::OT_LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#else if OST_LINUX
	typedef RDOCalcBinary<rbool, &RDOValue::operator> , OperatorType::OT_LOGIC> RDOCalcIsGreater;   DECLARE_POINTER(RDOCalcIsGreater);
#endif

/*!
  \typedef RDOCalcIsLEQ
  \brief   �������� ������ ��� �����
*/
#ifdef OST_WINDOWS
	typedef RDOCalcBinary<rbool, (&RDOValue::operator<=), OperatorType::OT_LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#else if OST_LINUX
	typedef RDOCalcBinary<rbool, &RDOValue::operator<=, OperatorType::OT_LOGIC> RDOCalcIsLEQ;       DECLARE_POINTER(RDOCalcIsLEQ);
#endif

/*!
  \typedef RDOCalcIsGEQ
  \brief   �������� ������ ��� �����
*/
#ifdef OST_WINDOWS
	typedef RDOCalcBinary<rbool, (&RDOValue::operator>=), OperatorType::OT_LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#else if OST_LINUX
	typedef RDOCalcBinary<rbool, &RDOValue::operator>=, OperatorType::OT_LOGIC> RDOCalcIsGEQ;       DECLARE_POINTER(RDOCalcIsGEQ);
#endif

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/logic.inl"

#endif // _LIB_RUNTIME_CALC_LOGIC_H_
