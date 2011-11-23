/*!
  \copyright (c) RDO-Team, 2011
  \file      logic.inl
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      31.07.2011
  \brief     ���������� ���������
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcAnd
// --------------------------------------------------------------------------------
inline RDOCalcAnd::RDOCalcAnd(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: parent_type(pLeft, pRight)
{
	m_value_true  = 1;
	m_value_false = 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcOr
// --------------------------------------------------------------------------------
inline RDOCalcOr::RDOCalcOr(CREF(LPRDOCalc) pLeft, CREF(LPRDOCalc) pRight)
	: parent_type(pLeft, pRight)
{
	m_value_true  = 1;
	m_value_false = 0;
}

CLOSE_RDO_RUNTIME_NAMESPACE
