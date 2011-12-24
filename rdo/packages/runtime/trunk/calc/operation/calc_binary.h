/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_binary.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     �������� ���������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_BINARY_H_
#define _LIB_RUNTIME_CALC_OPERATION_BINARY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/operation/operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������� ����� ��� �������� ����������
class RDOCalcBinaryBase: public RDOCalc
{
public:
	template <class T>
	static LPRDOCalc generateCalc(CREF(LPRDOCalc) pFirst, CREF(LPRDOCalc) pSecond);

protected:
	RDOCalcBinaryBase(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	LPRDOCalc  m_pLeft;
	LPRDOCalc  m_pRight;
};

//! ����������� �������� �������� RDOValue
template <typename ret_type, ret_type (RDOValue::*pMethod)(CREF(RDOValue)) const>
struct BinaryOperatorConstP1
{
	typedef ret_type (RDOValue::*method_type)(CREF(RDOValue)) const;

	static method_type method();
};

//! ������������� �������� �������� RDOValue
template <typename ret_type, ret_type (RDOValue::*pMethod)(CREF(RDOValue))>
struct BinaryOperatorNonConstP1
{
	typedef ret_type (RDOValue::*method_type)(CREF(RDOValue));

	static method_type method();
};

/*!
  \class   RDOCalcBinary
  \tparam  ret_type  ������������ ��������
  \tparam  pOperator ��������
  \tparam  CalcType  ��� ��������� ���������
  \brief   �������� ��������
*/
template <class F, typename OperatorType::Type CalcType>
class RDOCalcBinary: public RDOCalcBinaryBase
{
friend class rdo::Factory<RDOCalcBinary<F, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef F caller_type;

	LPRDOCalc      getLeft        () const;
	LPRDOCalcConst getRightAsConst() const;
	void           setRight       (CREF(LPRDOCalc) pRight);

	static RDOSrcInfo getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

protected:
	RDOCalcBinary(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

private:
	//! ��������� �������� �������� \ref value_operator ��� \ref m_pLeft � \ref m_pRight
	//! \param pRuntime - ��������� �� ������ runtime'�.
	//!                   ������������ ��� ������� � �� ������, ���������� �������, ��������� ������ � ��.
	//! \exception RDORuntimeException
	//! \result ����������� ������ ��������
	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/operation/calc_binary.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_BINARY_H_
