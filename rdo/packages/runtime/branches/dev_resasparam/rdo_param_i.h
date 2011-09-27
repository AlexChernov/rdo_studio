/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_param_i.h
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      09.12.2010
  \brief     ��������� IParam
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PARAM_I_H_
#define _LIB_RUNTIME_PARAM_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
// --------------------------------------------------------------------------------

/*!
  \interface IParam
  \brief     ��������� ��� ����������
  \todo      ��������� ��� ��������� � ��� �� ���� ���������
*/
class IParam
{
public:
	virtual rbool setParam(CREF(rdoRuntime::LPRDOCalc) pParam) = 0;
	virtual rbool endParam() = 0;
};

#define DECLARE_IParam \
	virtual rbool setParam(CREF(rdoRuntime::LPRDOCalc) pParam); \
	virtual rbool endParam();

#endif // _LIB_RUNTIME_PARAM_I_H_
