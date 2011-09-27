/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      22.06.2008
  \brief     ������� ��� ������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_type.h"
#include "utils/smart_ptr/factory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/// @todo �������� ������!

/*!
  \def       DEFINE_ATOM_TYPE_DEFAULT_VALUE
  \brief     ������ ��� ����������� �������� �� ��������� ��� ������� (���������) ����� ������
*/
#define DEFINE_ATOM_TYPE_DEFAULT_VALUE(Type) rdo::intrusive_ptr<RDOType__##Type> g_##Type = rdo::Factory<RDOType__##Type>::create();

/*!
  \var       g_unknow
  \brief     �������� ���� unknow �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(unknow);

/*!
  \var       g_void
  \brief     �������� ���� void �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(void);

/*!
  \var       g_identificator
  \brief     �������� ���� identificator �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(identificator);

/*!
  \var       g_int
  \brief     �������� ���� int �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(int);

/*!
  \var       g_real
  \brief     �������� ���� real �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(real);

/*!
  \var       g_bool
  \brief     �������� ���� bool �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(bool);

/*!
  \var       g_string
  \brief     �������� ���� string �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(string);

/*!
  \var       g_arrayIterator
  \brief     �������� ���� arrayIterator �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(arrayIterator);

/*!
  \var       g_matrixIterator
  \brief     �������� ���� matrixIterator �� ���������
*/
DEFINE_ATOM_TYPE_DEFAULT_VALUE(matrixIterator);

CLOSE_RDO_RUNTIME_NAMESPACE
