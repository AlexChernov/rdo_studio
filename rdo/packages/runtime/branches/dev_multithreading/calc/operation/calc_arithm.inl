/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_arithm.inl
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      26.07.2011
  \brief     �������������� ��������
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcDiv
// --------------------------------------------------------------------------------
inline RDOCalcDiv::RDOCalcDiv(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: parent_type(pLeft, pRight)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPlusEnumSafe
// --------------------------------------------------------------------------------
inline RDOCalcPlusEnumSafe::RDOCalcPlusEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: RDOCalcPlus(pLeft, pRight)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcMultEnumSafe
// --------------------------------------------------------------------------------
inline RDOCalcMultEnumSafe::RDOCalcMultEnumSafe(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: RDOCalcMult(pLeft, pRight)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
