/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_base.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      16.05.2007
  \brief     �����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_BASE_H_
#define _LIB_RUNTIME_CALC_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/smart_ptr/factory.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);

//! \defgroup calc      ����� ������ ������
//! \defgroup calc_base ����������� ����

/*!
  \class    RDOCalc
  \brief    ����������� "�����������", �� �� ����.
  \details  C��������������� ������ ������� �� ������������������� ������, ��� �� ��� � ���������������� ��������� ������� �� ��������������� ������. ������ ����� ��������� �������, ����������� ����.
  \ingroup  calc calc_base
*/
OBJECT(RDOCalc) IS INSTANCE_OF(RDOSrcInfo)
{
//! \details ������� ��� ��������������� �������� �����
DECLARE_FACTORY(RDOCalc)

public:
	//! ������ �������� �����. �������� ��������� �������, ����� ������� � ����� ������������� ��������
	//! \param pRuntime - ��������� �� ������ runtime'�.
	//!                   ������������ ��� ������� � �� ������, ���������� �������, ��������� ������ � ��.
	//! \result ����������� ������ ��������
	REF(RDOValue) calcValue(CREF(LPRDORuntime) pRuntime);

	//! ��������� ���� ������ ��� �������� (���������� ������ � ��������� �� ������������).
	//! ������������� ���� ����� � ����� ������ �������������.
	//! \param pCalc - ����, � ������� ��������� ��������� �������
	//! \result true, ���� ����� ����������. �������� �� ��������� false
	virtual rbool compare(CREF(LPRDOCalc) pCalc) const;

protected:
	RDOCalc();
	virtual ~RDOCalc();

	//! ������� ������ ������� ��������� �������� ����� � ���������� ��� � \ref doCalc
	RDOValue m_value;

	//! ����������� �������� �����.
	//! ����� ������ ���� ���������� � �������� (�������� ������).
	//! \param pRuntime - ��������� �� ������ runtime'�.
	//!                   ������������ ��� ������� � �� ������, ���������� �������, ��������� ������ � ��.
	//! \exception RDORuntimeException
	//! \result ����������� ������ ��������
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime) = 0;
};

#define DECLARE_ICalc \
private:              \
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

//! \def    CALC_SUB
//! \brief  ��������� �����-�������
#define CALC_SUB(TYPE, PARENT) \
PREDECLARE_POINTER(TYPE);      \
class TYPE: public PARENT

//! \def    CALC
//! \brief  ��������� �����-������� �� RDOCalc
#define CALC(TYPE) CALC_SUB(TYPE, RDOCalc)

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/calc_base.inl"

#endif // _LIB_RUNTIME_CALC_BASE_H_
