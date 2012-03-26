/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_relevant.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.04.2011
  \brief     RDOCalc ��� ������� ����������� ��������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_relevant.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/rdo_res_type_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOGetResourceByRelevantResourceID
// --------------------------------------------------------------------------------
RDOGetResourceByRelevantResourceID::RDOGetResourceByRelevantResourceID(ruint relevantResourceID)
	: m_relevantResourceID(relevantResourceID)
{}

REF(RDOValue) RDOGetResourceByRelevantResourceID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, pRuntime->getCurrentActivity()->getResByRelRes(m_relevantResourceID), m_value))
	{
		pRuntime->error().push(_T("�� ������ ������"), srcInfo());
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOEraseResRelCalc
// --------------------------------------------------------------------------------
RDOEraseResRelCalc::RDOEraseResRelCalc(ruint relResID, CREF(tstring) relResName)
	: m_relResID  (relResID  )
	, m_relResName(relResName)
{
	m_value = 1;
}

REF(RDOValue) RDOEraseResRelCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->onEraseRes(pRuntime->getCurrentActivity()->getResByRelRes(m_relResID), this);
	return m_value;
}

CREF(tstring) RDOEraseResRelCalc::getName() const
{
	return m_relResName;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetGroupFunctionResource
// --------------------------------------------------------------------------------
RDOCalcGetGroupFunctionResource::RDOCalcGetGroupFunctionResource()
{}

REF(RDOValue) RDOCalcGetGroupFunctionResource::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = pRuntime->getGroupFuncRes();
	ASSERT(pResource);

	LPRDOType pType(pResource->getResType());
	ASSERT(pType);

	m_value = RDOValue(pType, pResource);
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
