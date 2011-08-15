/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      arithm.h
 * @authors   ���� ���������, ������ ������
 * @date      13.03.2011
 * @brief     �������������� ��������
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_ARITHM_H_
#define _LIB_RUNTIME_CALC_ARITHM_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/calc/binary.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @typedef RDOCalcPlus
 * @brief   �������� �������� ����
 *********************************************************************************/
typedef RDOCalcBinary<RDOValue, (&RDOValue::operator+), OperatorType::OT_ARITHM> RDOCalcPlus;  DECLARE_POINTER(RDOCalcPlus);

/******************************************************************************//**
 * @typedef RDOCalcMinus
 * @brief   �������� �������� �����
 *********************************************************************************/
typedef RDOCalcBinary<RDOValue, (&RDOValue::operator-), OperatorType::OT_ARITHM> RDOCalcMinus; DECLARE_POINTER(RDOCalcMinus);

/******************************************************************************//**
 * @typedef RDOCalcMult
 * @brief   �������� �������� ��������
 *********************************************************************************/
typedef RDOCalcBinary<RDOValue, (&RDOValue::operator*), OperatorType::OT_ARITHM> RDOCalcMult;  DECLARE_POINTER(RDOCalcMult);

/******************************************************************************//**
 * @class   RDOCalcDiv
 * @brief   �������� �������� ��������
 *********************************************************************************/
class RDOCalcDiv: public RDOCalcBinary<RDOValue, (&RDOValue::operator/), OperatorType::OT_ARITHM>
{
DECLARE_FACTORY(RDOCalcDiv);
private:
	RDOCalcDiv(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECALRE_ICalc;
};
DECLARE_POINTER(RDOCalcDiv);

/******************************************************************************//**
 * @class   RDOCalcPlusEnumSafe
 * @brief   �������� �������� ��������
 *********************************************************************************/
CALC_SUB(RDOCalcPlusEnumSafe, RDOCalcPlus)
{
DECLARE_FACTORY(RDOCalcPlusEnumSafe)
private:
	RDOCalcPlusEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcMultEnumSafe
 * @brief   �������� �������� ��������
 *********************************************************************************/
CALC_SUB(RDOCalcMultEnumSafe, RDOCalcMult)
{
DECLARE_FACTORY(RDOCalcMultEnumSafe)
private:
	RDOCalcMultEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight);

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/arithm.inl"

#endif // _LIB_RUNTIME_CALC_ARITHM_H_
