/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_resource.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      19.12.2011
  \brief     ����� ��� ��������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_H_
#define _LIB_RUNTIME_CALC_RESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/equaltype.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ��������������� ����� ��� ������������ ������� �� ID � ������������ ��� � RDOValue
class RDOCalcGetResourceHelper
{
public:
	static rbool getResource(CREF(LPRDORuntime) pRuntime, ruint resourceID, REF(RDOValue) result);
};

//! ��������� ������� �� ID
CALC(RDOCalcGetResourceByID)
{
DECLARE_FACTORY(RDOCalcGetResourceByID)
private:
	RDOCalcGetResourceByID(CREF(ruint) resourceID);

	ruint m_resourceID;

	DECLARE_ICalc;
};

//! ��������� �������� ������� �� ����� ������� � ID ���������
CALC(RDOCalcGetResourceParam)
{
DECLARE_FACTORY(RDOCalcGetResourceParam)
private:
	RDOCalcGetResourceParam(CREF(LPRDOCalc) pResource, ruint paramID);

	LPRDOCalc m_pResource;
	int       m_paramID;

	DECLARE_ICalc;
};

//! ��������� ��������������� �������
CALC(RDOCalcGetUnknowResParam)
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName);

	tstring m_resName;
	tstring m_parName;

	DECLARE_ICalc;
};

//! ��������� �������� ��������� ������� ����� �������������� �� SMR
CALC(RDOSetResourceParamCalc)
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(ruint resourceID, ruint paramID, CREF(LPRDOCalc) pCalc);

	ruint      m_resourceID;
	ruint      m_paramID;
	LPRDOCalc  m_pCalc;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_RESOURCE_H_
