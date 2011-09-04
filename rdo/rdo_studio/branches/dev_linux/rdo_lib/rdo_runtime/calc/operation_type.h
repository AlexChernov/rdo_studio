/*!
  \copyright (c) RDO-Team, 2011
  \file      operation_type.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ��� ��������: �������������� ��� ���������� ���������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_OPERATION_TYPE_H_
#define _LIB_RUNTIME_CALC_OPERATION_TYPE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   OperatorType
  \brief   ��� ���������
*/
class OperatorType
{
public:
	//! ��� �������� � ���
	enum Type
	{
		OT_ARITHM, //!< �������������� ���������
		OT_LOGIC   //!< ���������� ���������
	};

	template <Type>
	static REF(ruint) getCalcCounter();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/calc/operation_type.inl"

#endif // _LIB_RUNTIME_CALC_OPERATION_TYPE_H_
