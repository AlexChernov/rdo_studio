/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotypes.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      18.04.2009
  \brief     ���������� ������� �����
  \indent    4T
*/

#ifndef _UTILS_RDOTYPES_H_
#define _UTILS_RDOTYPES_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef OST_WINDOWS
	#include <tchar.h>
#else
	#include <stdint.h>
	#define _T(x) x
#endif

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

//! ����� �������� 32-� ������ ���
#ifdef OST_WINDOWS
	typedef  long                   rsint32;
#else
	typedef  int32_t                rsint32;
#endif

//! ����� ����������� 32-� ������ ���
#ifdef OST_WINDOWS
	typedef  unsigned long          ruint32;
#else
	typedef  uint32_t               ruint32;
#endif

//! ����� �������� 64-� ������ ���
typedef  signed long long int       rsint64;

//! ����� ����������� 64-� ������ ���
typedef  unsigned long long int     ruint64;

//! ��� �����
typedef  unsigned char              rbyte;

#endif // _UTILS_RDOTYPES_H_
