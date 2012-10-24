/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_const.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     ������ ��������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_
#define _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \defgroup calc_const ����������� ����
  \class    RDOCalcConst
  \brief    ���������
  \details  ���������� ����������� ��������
  \ingroup  calc calc_const
*/
CALC(RDOCalcConst)
{
DECLARE_FACTORY(RDOCalcConst)
public:
	//! ���������� �������� ���������, ����� ���� ����������� � parser-time
	//! \result �������� ���������
	CREF(RDOValue) getValue() const;

private:
	//! ���������������� ��������� ��������� ���������� �������� ������ \ref RDOCalc::m_value
	//! \param value - �������� ���������
	RDOCalcConst(CREF(RDOValue) value);

	RDOValue m_value;

	//! ���������� �������� ���������
	//! \param pRuntime - �� ������������
	//! \result �������� ���������
	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_CALC_PROCEDURAL_CONST_H_
