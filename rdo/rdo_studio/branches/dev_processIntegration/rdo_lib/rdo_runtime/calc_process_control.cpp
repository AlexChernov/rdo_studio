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
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOCalcProcessControl
// ----------------------------------------------------------------------------
RDOCalcProcessControl::RDOCalcProcessControl(LPIPROCBlock pBlock, rsint relResNum)
	: m_Block    (pBlock   )
	, m_relResNum(relResNum)
{}

REF(RDOValue) RDOCalcProcessControl::doCalc(PTR(RDORuntime) pRuntime)
{
//�� m_relResNum ����� ����� ������ (m_Transact) � �������� ��� � �������
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResNum);
	LPRDOResource     pResource = pRuntime->getResourceByID(resID);
	//! @TODO ���������, ����� �� ��������� �������� � ������, ����� ������� object_static_cast ������ object_dynamic_cast
	LPRDOPROCTransact pTransact = pResource.object_dynamic_cast<RDOPROCTransact>();
	if (pTransact)
	{
		pTransact->setBlock(m_Block);
		// ���������� � ����� ������ ����� ����� ������������ ��������
		m_Block.query_cast<IPROCBlock>()->transactGoIn(pTransact);
	}

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
