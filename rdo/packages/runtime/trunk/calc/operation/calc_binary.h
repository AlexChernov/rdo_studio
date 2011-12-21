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

/*!
  \class   RDOCalcBinaryBase
  \brief   ������� ����� ��� �������� ����������
*/
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

/*!
  \class   RDOCalcBinary
  \tparam  ret_type  ������������ ��������
  \tparam  pOperator ��������
  \tparam  CalcType  ��� ��������� ���������
  \brief   �������� ��������
*/
template <typename ret_type, ret_type (RDOValue::*pOperator)(CREF(RDOValue) rdovalue) const, typename OperatorType::Type CalcType>
class RDOCalcBinary: public RDOCalcBinaryBase
{
friend class rdo::Factory<RDOCalcBinary<ret_type, pOperator, CalcType> >;
public:
	enum { calc_type = CalcType };
	typedef ret_type (RDOValue::*value_operator)(CREF(RDOValue) rdovalue) const;

	LPRDOCalc      getLeft        () const;
	LPRDOCalcConst getRightAsConst() const;
	void           setRight       (CREF(LPRDOCalc) pRight);

	static RDOSrcInfo     getStaticSrcInfo(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);
	static value_operator getOperation    ();

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
