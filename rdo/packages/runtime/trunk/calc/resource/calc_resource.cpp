/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_resource.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      19.12.2011
  \brief     ����� ��� ��������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type.h"
#include "simulator/runtime/rdo_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResourceHelper
// --------------------------------------------------------------------------------
rbool RDOCalcGetResourceHelper::getResource(CREF(LPRDORuntime) pRuntime, ruint resourceID, REF(RDOValue) result)
{
	LPRDOResource pResource = pRuntime->getResourceByID(resourceID);
	if (!pResource)
		false;

	LPRDOType pType(pResource->getResType());
	ASSERT(pType);

	result = RDOValue(pType, pResource);
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResourceByID
// --------------------------------------------------------------------------------
RDOCalcGetResourceByID::RDOCalcGetResourceByID(CREF(ruint) resourceID)
	: m_resourceID(resourceID)
{}

REF(RDOValue) RDOCalcGetResourceByID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, m_resourceID, m_value))
	{
		pRuntime->error(_T("�� ������ ������"), this);
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResParamByCalc
// --------------------------------------------------------------------------------
RDOCalcGetResParamByCalc::RDOCalcGetResParamByCalc(CREF(LPRDOCalc) pResource, ruint paramID)
	: m_pResource(pResource)
	, m_paramID  (paramID  )
{
	ASSERT(m_pResource);
}

REF(RDOValue) RDOCalcGetResParamByCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = m_pResource->calcValue(pRuntime).getPointerSafety<RDOResourceType>();
	ASSERT(pResource);
	m_value = pResource->getParam(m_paramID);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetUnknowResParam
// --------------------------------------------------------------------------------
RDOCalcGetUnknowResParam::RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName)
	: m_resName(resName)
	, m_parName(parName)
{}

REF(RDOValue) RDOCalcGetUnknowResParam::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->error(rdo::format(_T("������� ������������ �������������� ������: %s.%s"), m_resName.c_str(), m_parName.c_str()), this);
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetResourceParamCalc
// --------------------------------------------------------------------------------
RDOSetResourceParamCalc::RDOSetResourceParamCalc(ruint resourceID, ruint paramID, CREF(LPRDOCalc) pCalc)
	: m_resourceID(resourceID)
	, m_paramID   (paramID   )
	, m_pCalc     (pCalc     )
{
	m_value = true;
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->src_info());
	}
}

REF(RDOValue) RDOSetResourceParamCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->setResParamVal(m_resourceID, m_paramID, m_pCalc->calcValue(pRuntime));
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSetRelResParamDiapCalc
// --------------------------------------------------------------------------------
RDOSetRelResParamDiapCalc::RDOSetRelResParamDiapCalc(ruint relResID, ruint paramID, CREF(RDOValue) minValue, CREF(RDOValue) maxValue, CREF(LPRDOCalc) pCalc)
	: m_relResID(relResID)
	, m_paramID (paramID )
	, m_pCalc   (pCalc   )
	, m_minValue(minValue)
	, m_maxValue(maxValue)
{
	m_value = true;
	if (m_pCalc)
	{
		setSrcInfo(m_pCalc->src_info());
	}
}

REF(RDOValue) RDOSetRelResParamDiapCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	m_pCalc->calcValue(pRuntime);
	m_value = pRuntime->getResParamVal(pRuntime->getCurrentActivity()->getResByRelRes(m_relResID), m_paramID);
	if (m_value < m_minValue || m_value > m_maxValue)
	{
		pRuntime->error(rdo::format(_T("�������� ������� �� ���������� �������� [%s..%s]: %s"), m_minValue.getAsString().c_str(), m_maxValue.getAsString().c_str(), m_value.getAsString().c_str()), this);
	}
	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
