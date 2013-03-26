/*!
  \copyright (c) RDO-Team, 2012
  \file      show_mode.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.02.2012
  \brief     ������ ��������
  \indent    4T
*/

#ifndef _SIMULATOR_SERVICE_SHOW_MODE_H_
#define _SIMULATOR_SERVICE_SHOW_MODE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/service/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_SERVICE_SIMULATION_NAMESPACE

//! ������ ��������
enum ShowMode
{
	SM_NoShow,    //!< ��� ��������
	SM_Animation, //!< � ���������
	SM_Monitor    //!< �������� � ������ �� �����
};

CLOSE_RDO_SERVICE_SIMULATION_NAMESPACE

#endif // _SIMULATOR_SERVICE_SHOW_MODE_H_
