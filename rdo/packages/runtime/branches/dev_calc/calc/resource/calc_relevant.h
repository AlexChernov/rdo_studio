/*!
  \copyright (c) RDO-Team, 2011
  \file      relres.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.12.2009
  \brief     RDOCalc ��� ������� ����������� ��������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_REL_RES_H_
#define _LIB_RUNTIME_CALC_REL_RES_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/equaltype.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! �������� ������ �� ID ������������ �������
CALC(RDOGetResourceByRelevantResourceID)
{
DECLARE_FACTORY(RDOGetResourceByRelevantResourceID)
private:
	RDOGetResourceByRelevantResourceID(ruint relevantResourceID);

	ruint m_relevantResourceID;

	DECLARE_ICalc;
};


//! �������� ������������ �������
//! @todo �������, ������ ���� ������������ \ref RDOCalcGetResParamByCalc
CALC(RDOGetRelResParamCalc)
{
DECLARE_FACTORY(RDOGetRelResParamCalc)
private:
	RDOGetRelResParamCalc(ruint relResID, ruint paramID);

	ruint m_relResID;
	ruint m_paramID;

	DECLARE_ICalc;
};

/*!
  \class   RDOSetRelResParamCalc
  \tparam  equalType ��� ������������
  \brief   RDOCalc ��� ��������� �������� ��������� ������������ �������
*/
template <EqualType equalType>
class RDOSetRelResParamCalc: public RDOCalc
{
public:
	RDOSetRelResParamCalc(ruint relResID, ruint paramID, CREF(LPRDOCalc) pCalc = NULL);
	virtual ~RDOSetRelResParamCalc();

protected:
	ruint     m_relResID;
	ruint     m_paramID;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

/*!
  \class   RDOSetRelResParamDiapCalc
  \brief   RDOCalc ����������� ��������� ���������
*/
CALC(RDOSetRelResParamDiapCalc)
{
DECLARE_FACTORY(RDOSetRelResParamDiapCalc)
private:
	RDOSetRelResParamDiapCalc(ruint relResID, ruint paramID, CREF(RDOValue) minValue, CREF(RDOValue) maxValue, CREF(LPRDOCalc) pCalc);

	ruint     m_relResID;
	ruint     m_paramID;
	LPRDOCalc m_pCalc;
	RDOValue  m_minValue;
	RDOValue  m_maxValue;

	DECLARE_ICalc;
};

/*!
  \class   RDOEraseResRelCalc
  \brief   RDOCalc ��� �������� ������������ �������
*/
CALC(RDOEraseResRelCalc)
{
DECLARE_FACTORY(RDOEraseResRelCalc)
public:
	CREF(tstring) getName() const;

private:
	RDOEraseResRelCalc(ruint relResID, CREF(tstring) relResName);

	ruint   m_relResID;
	tstring m_relResName;

	DECLARE_ICalc;
};

//! ���������� ������ ��������� �������
CALC(RDOCalcGetGroupFunctionResource)
{
DECLARE_FACTORY(RDOCalcGetGroupFunctionResource)
private:
	RDOCalcGetGroupFunctionResource();

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/resource/calc_relevant.inl"

#endif // _LIB_RUNTIME_CALC_REL_RES_H_
