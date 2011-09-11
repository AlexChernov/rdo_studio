/*!
  \copyright (c) RDO-Team, 2011
  \file      static_assert.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      08.03.2010
  \brief     ������� �� ������ CompileTimeError �� <a href="http://lib.mexmat.ru/books/7498">�������������</a>
  \indent    4T
*/

#ifndef _LIB_COMMON_STATIC_ASSERT_H_
#define _LIB_COMMON_STATIC_ASSERT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

//! \struct  rdo::CompileTimeError
//! \brief   ��������� ����� ��� ��������� ������ ����������
//! \details ������������ ����� ������ \ref STATIC_ASSERT
//! \tparam  rbool - ���� \b true, ������ ���, ���� \b false - ������ ����
template <rbool>
struct CompileTimeError;

//! ������������� ���������� ������ \ref rdo::CompileTimeError ��� �������� �������� \b ���������� ������ ����������
template <>
struct CompileTimeError<true> {};

//! ����������� ASSERT �� ����� ����������, ����� ���������� ������� �� ����� �������, �.�. ��������� ������ ����������. ������:
//! \code
//! STATIC_ASSERT(Some_error);
//! \endcode
//! \param expr - ����� ������, ����������� ��� ��������
#define STATIC_ASSERT(expr)                 \
	rdo::CompileTimeError<((expr) != 0)>();

CLOSE_RDO_NAMESPACE

#endif // _LIB_COMMON_STATIC_ASSERT_H_
