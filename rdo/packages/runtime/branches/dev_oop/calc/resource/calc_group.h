/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_group.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     ����� ��� ������ �������� ������ ����
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_GROUP_H_
#define _LIB_RUNTIME_CALC_RESOURCE_GROUP_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������� ����� ��� ��������� ������
CALC_SUB(RDOFunCalcGroup, RDOFunCalc)
{
protected:
	int        m_nResType;
	LPRDOCalc  m_pCondition;

	RDOFunCalcGroup(int nResType, CREF(LPRDOCalc) pCondition);
};

/*!
  \def     DEFINE_CALC_GROUP
  \brief   ��������� �����
*/
#define DEFINE_CALC_GROUP(CalcName) \
CALC_SUB(RDOFunCalc##CalcName, RDOFunCalcGroup) \
{ \
DECLARE_FACTORY(RDOFunCalc##CalcName) \
private: \
	RDOFunCalc##CalcName(int nResType, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcGroup(nResType, pCondition) \
	{} \
 	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime); \
};

/*!
  \class   RDOFunCalcExist
  \brief   ���������� �������� �������������
*/
DEFINE_CALC_GROUP(Exist);

/*!
  \class   RDOFunCalcNotExist
  \brief   ���������� �������� ��������� �������������
*/
DEFINE_CALC_GROUP(NotExist);

/*!
  \class   RDOFunCalcForAll
  \brief   ���������� �������� ��������
*/
DEFINE_CALC_GROUP(ForAll);

/*!
  \class   RDOFunCalcNotForAll
  \brief   ���������� �������� ��������� ��������
*/
DEFINE_CALC_GROUP(NotForAll);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_RESOURCE_GROUP_H_
