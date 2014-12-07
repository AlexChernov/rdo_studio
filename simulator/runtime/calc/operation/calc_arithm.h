#ifndef _LIB_RUNTIME_CALC_OPERATION_ARITHM_H_
#define _LIB_RUNTIME_CALC_OPERATION_ARITHM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

typedef RDOCalcBinary<BinaryOperatorNonConstP1<RDOValue&, &RDOValue::operator= >, OperatorType::OT_ARITHM> RDOCalcSet;  DECLARE_POINTER(RDOCalcSet);

typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator+ >, OperatorType::OT_ARITHM> RDOCalcPlus;  DECLARE_POINTER(RDOCalcPlus);

typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator- >, OperatorType::OT_ARITHM> RDOCalcMinus; DECLARE_POINTER(RDOCalcMinus);

typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator* >, OperatorType::OT_ARITHM> RDOCalcMult;  DECLARE_POINTER(RDOCalcMult);

class RDOCalcDiv: public RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator/ >, OperatorType::OT_ARITHM>
{
DECLARE_FACTORY(RDOCalcDiv);
private:
	typedef RDOCalcBinary<BinaryOperatorConstP1<RDOValue, &RDOValue::operator/ >, OperatorType::OT_ARITHM> parent_type;

	RDOCalcDiv(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

	DECLARE_ICalc;
};
DECLARE_POINTER(RDOCalcDiv);

PREDECLARE_POINTER(RDOCalcPlusEnumSafe);
class RDOCalcPlusEnumSafe: public RDOCalcPlus
{
DECLARE_FACTORY(RDOCalcPlusEnumSafe)
private:
	RDOCalcPlusEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcMultEnumSafe);
class RDOCalcMultEnumSafe: public RDOCalcMult
{
DECLARE_FACTORY(RDOCalcMultEnumSafe)
private:
	RDOCalcMultEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight);

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_OPERATION_ARITHM_H_
