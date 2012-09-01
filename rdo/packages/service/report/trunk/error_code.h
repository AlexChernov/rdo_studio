/*!
  \copyright (c) RDO-Team, 2012
  \file      error_code.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      16.02.2012
  \brief     ���� ��������� �������������
  \indent    4T
*/

#ifndef _SIMULATOR_REPORT_ERROR_CODE_H_
#define _SIMULATOR_REPORT_ERROR_CODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/report/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SIMULATION_REPORT_NAMESPACE

//! ��� ��������� ���������� ��� ������� ������
enum RDOExitCode
{
	EC_OK = 0,        //!< ������ ���������������� ��� ����������� ��� ������
	EC_ParserError,   //!< ���������� ����������� � �������
	EC_RunTimeError,  //!< ������ ������ ���������� � �������
	EC_UserBreak,     //!< ������ ����������� �������������
	EC_ModelNotFound, //!< ���� ������� ������ �� ������
	EC_NoMoreEvents   //!< ������ ����������� ������, ��� ������ ������ ������������
};

CLOSE_RDO_SIMULATION_REPORT_NAMESPACE

#endif // _SIMULATOR_REPORT_ERROR_CODE_H_
