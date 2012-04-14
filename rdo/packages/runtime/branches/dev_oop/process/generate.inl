/*!
  \copyright (c) RDO-Team, 2012
  \file      generate.inl
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     ���������� �������� GENERATE
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_logic.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPROCGenerate
// --------------------------------------------------------------------------------
inline RDOPROCGenerate::RDOPROCGenerate(LPIPROCProcess process, CREF(LPRDOCalc) pTime, CREF(LPRDOCalc) pCreateAndGoOnTransactCalc, int maxTransCount)
	: RDOPROCBlock                (process                   )
	, timeNext                    (0.0                       )
	, m_pTimeCalc                 (pTime                     )
	, m_pCreateAndGoOnTransactCalc(pCreateAndGoOnTransactCalc)
	, m_maxTransCount             (maxTransCount             )
{
	m_TransCount = 0;
}

CLOSE_RDO_RUNTIME_NAMESPACE
