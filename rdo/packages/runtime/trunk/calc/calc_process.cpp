/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_process.cpp
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      10.03.2011
  \brief     RDOCalc ��� �������� ���������� � ������� �� �� � �������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_activity_i.h"
#include "simulator/runtime/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcProcessControl
// --------------------------------------------------------------------------------
RDOCalcProcessControl::RDOCalcProcessControl(LPIPROCBlock pBlock, rsint relResNum)
	: m_Block    (pBlock   )
	, m_relResNum(relResNum)
{}

REF(RDOValue) RDOCalcProcessControl::doCalc(CREF(LPRDORuntime) pRuntime)
{
	//�� m_relResNum ����� ����� ������ (m_Transact) � �������� ��� � �������
	ruint resID = pRuntime->getCurrentActivity()->getResByRelRes(m_relResNum);
	LPRDOResource     pResource = pRuntime->getResourceByID(resID);
	/// @todo ���������, ����� �� ��������� �������� � ������, ����� ������� object_static_cast ������ object_dynamic_cast
	LPRDOPROCTransact pTransact = pResource.object_dynamic_cast<RDOPROCTransact>();
	if (pTransact)
	{
		pTransact->setBlock(m_Block);
		// ���������� � ����� ������ ����� ����� ������������ ��������
		m_Block.query_cast<IPROCBlock>()->transactGoIn(pTransact);
	}

	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetTermNow
// --------------------------------------------------------------------------------
REF(RDOValue) RDOCalcGetTermNow::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_value = pRuntime->getCurrentTerm();
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
