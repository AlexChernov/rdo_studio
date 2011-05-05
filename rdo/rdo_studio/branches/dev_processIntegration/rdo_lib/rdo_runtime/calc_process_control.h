/*
 * copyright: (c) RDO-Team, 2011
 * filename : calc_process_control.h
 * author   : ����� �������
 * date     : 10.03.2011
 * bref     : RDOCalc ��� �������� ���������� � ������� �� �� � �������
 * indent   : 4T
 */

#ifndef _CALC_PROCESS_CONTROL_H_
#define _CALC_PROCESS_CONTROL_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcProcessControl
// ----------------------------------------------------------------------------
CALC(RDOCalcProcessControl)
{
DECLARE_FACTORY(RDOCalcProcessControl)
public:
protected:
	RDOCalcProcessControl(LPIPROCBlock pBlock, PTR(RDOPROCTransact) pTransact);
	DECALRE_ICalc;
private:
	LPIPROCBlock         m_Block;
	PTR(RDOPROCTransact) m_Transact;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _CALC_PROCESS_CONTROL_H_
