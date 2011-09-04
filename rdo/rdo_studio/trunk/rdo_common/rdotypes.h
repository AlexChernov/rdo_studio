/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotypes.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      18.04.2009
  \brief     ���������� ������� �����
  \indent    4T
*/

#ifndef _LIB_COMMON_RDOTYPES_H_
#define _LIB_COMMON_RDOTYPES_H_

// ----------------------------------------------------------------------- INCLUDES
#ifdef WIN32
#include <tchar.h>
#else // not WIN32
#define _T(x)       x
#endif // WIN32

#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! ��� �������
typedef  char                       tchar;

//! ��� ������
typedef  std::basic_string<tchar>   tstring;

//! ��������� ���
typedef  bool                       rbool;

//! ����� �������� ���
typedef  signed int                 rsint;

//! ����� ����������� ���
typedef  unsigned int               ruint;

//! ����� �������� 64-� ������ ���
typedef  signed long long int       rsint64;

//! ����� ����������� 64-� ������ ���
typedef  unsigned long long int     ruint64;

//! ��� �����
typedef  unsigned char              rbyte;

#endif // _LIB_COMMON_RDOTYPES_H_
