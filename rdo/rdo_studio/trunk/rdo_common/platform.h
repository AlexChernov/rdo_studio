/*!
  \copyright (c) RDO-Team, 2011
  \file      platform.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      04.09.2011
  \brief     ����������� ��������� � �����������
  \indent    4T
*/

#ifndef _LIB_COMMON_PLATFORM_H_
#define _LIB_COMMON_PLATFORM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

#ifdef _MSC_VER
	#define COMPILER_VISUAL_STUDIO
#else
	#ifdef __GNUC__
		#define COMPILER_GCC
	#endif
#endif

#ifdef _WIN32
	#define OST_WINDOWS
	#ifdef _WIN64
		#define OST_WINDOWS_64
	#else
		#define OST_WINDOWS_32
	#endif
#elif
	#ifdef __linux__
		#define OST_LINUX
	#endif
#endif

#endif // _LIB_COMMON_PLATFORM_H_
