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
#ifdef COMPILER_VISUAL_STUDIO
	#include <tchar.h>
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	#include <stdint.h>
	#define _T(x) x
#endif // COMPILER_GCC

#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! ��� �������
typedef  char                        tchar;

//! ��� ������
typedef  std::basic_string<tchar>    tstring;

//! ��������� ���
typedef  bool                        rbool;

//! ����� �������� ���
typedef  signed int                  rsint;

//! ����� ����������� ���
typedef  unsigned int                ruint;

//! ����� �������� 32-� ������ ���
#ifdef COMPILER_VISUAL_STUDIO
	typedef  long                    rsint32;
#endif
#ifdef COMPILER_GCC
	typedef  int32_t                 rsint32;
#endif

//! ����� ����������� 32-� ������ ���
#ifdef COMPILER_VISUAL_STUDIO
	typedef  unsigned long           ruint32;
#endif
#ifdef COMPILER_GCC
	typedef  uint32_t                ruint32;
#endif

//! ����� �������� 64-� ������ ���
#ifdef COMPILER_VISUAL_STUDIO
	typedef  signed long long int    rsint64;
#endif
#ifdef COMPILER_GCC
	typedef  int64_t                 rsint64;
#endif

//! ����� ����������� 64-� ������ ���
#ifdef COMPILER_VISUAL_STUDIO
	typedef  unsigned long long int  ruint64;
#endif
#ifdef COMPILER_GCC
	typedef  uint64_t                ruint64;
#endif

//! ��� �����
typedef  unsigned char               rbyte;

#endif // _UTILS_RDOTYPES_H_
