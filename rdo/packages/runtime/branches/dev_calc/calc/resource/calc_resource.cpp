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
// -------------------- RDOCalcGetResourceByCalcID
// --------------------------------------------------------------------------------
RDOCalcGetResourceByCalcID::RDOCalcGetResourceByCalcID(CREF(LPRDOCalc) pGetResourceID)
	: m_pGetResourceID(pGetResourceID)
{
	ASSERT(m_pGetResourceID);
}

REF(RDOValue) RDOCalcGetResourceByCalcID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	if (!RDOCalcGetResourceHelper::getResource(pRuntime, m_pGetResourceID->calcValue(pRuntime).getUInt(), m_value))
	{
		pRuntime->error(_T("�� ������ ������"), this);
	}
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcGetResID
// --------------------------------------------------------------------------------
RDOCalcGetResID::RDOCalcGetResID(CREF(LPRDOCalc) pCalcGetResource)
	: m_pCalcGetResource(pCalcGetResource)
{}

REF(RDOValue) RDOCalcGetResID::doCalc(CREF(LPRDORuntime) pRuntime)
{
	LPRDOResource pResource = m_pCalcGetResource->calcValue(pRuntime).getPointer<RDOResource>();
	ASSERT(pResource);
	m_value = pResource->getTraceID();
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
inline RDOSetResourceParamCalc::RDOSetResourceParamCalc(ruint resourceID, ruint paramID, CREF(LPRDOCalc) pCalc)
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

CLOSE_RDO_RUNTIME_NAMESPACE
