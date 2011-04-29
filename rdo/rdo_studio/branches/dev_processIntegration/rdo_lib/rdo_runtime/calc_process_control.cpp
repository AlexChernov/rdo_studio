/*
 * copyright: (c) RDO-Team, 2011
 * filename : calc_process_control.cpp
 * author   : ����� �������
 * date     : 10.03.2011
 * bref     : RDOCalc ��� �������� ���������� � ������� �� �� � �������
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/calc_process_control.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcEvent
// ----------------------------------------------------------------------------
RDOCalcProcessControl::RDOCalcProcessControl(LPIPROCBlock pBlock):
	m_Block(pBlock)
{}

REF(RDOValue) RDOCalcProcessControl::doCalc(PTR(RDORuntime) pRuntime)
{
	PTR(RDOPROCTransact) pTransact = new RDOPROCTransact(pRuntime, m_Block);
	ASSERT(pTransact);

	pTransact->setBlock(m_Block);
	// ���������� � ����� ������ ����� ����� ������������ ��������
	m_Block.query_cast<IPROCBlock>()->transactGoIn(pTransact);

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
