/*!
  \copyright (c) RDO-Team, 2011
  \file      rdocalc.inl
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.07.2009
  \brief     �����������
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalc
// --------------------------------------------------------------------------------
inline rbool RDOCalc::compare(CREF(LPRDOCalc) pCalc) const
{
	UNUSED(pCalc);
	return false;
}

CLOSE_RDO_RUNTIME_NAMESPACE
