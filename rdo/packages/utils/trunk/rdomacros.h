/*!
  \copyright (c) RDO-Team, 2011
  \file      rdomacros.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     ���������� ������� ��������
  \indent    4T
*/

#ifndef _LIB_COMMON_MACROS_H_
#define _LIB_COMMON_MACROS_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/typeof/typeof.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

//! ������ �� A
#define   REF(A)       A&
//! ����������� ������ �� A
#define  CREF(A) const A&
//! ��������� �� �
#define   PTR(A)       A*
//! ����������� ��������� �� �
#define  CPTR(A) const A*
//! ����������� ��������� �� ����������� ������ ���� �
#define CPTRC(A) const A* const

//! ���������� ������
#define  CLASS(A)            class A
//! ���������� ���������
#define STRUCT(A)            struct A
//! ���������� ����������
#define S_INTERFACE(A)       struct A
//! ������������
#define IS  ,
//! ������������
#define AND ,
//! ���������� ���������� A
#define IMPLEMENTATION_OF(A) public A
//! ������ ���� A
#define INSTANCE_OF(A)       public A
//! �����-������� �� B
#define  CLASS_PARENT_OF(A, B)  CLASS(A): IMPLEMENTATION_OF(B)
//! ���������-������� �� B
#define STRUCT_PARENT_OF(A, B) STRUCT(A): IMPLEMENTATION_OF(B)

//! ������������ ����������
#define STL_FOR_ALL(CONTAINER, IT)                       \
typedef BOOST_TYPEOF(CONTAINER) CONTAINER_TYPE_FOR_##IT; \
for (CONTAINER_TYPE_FOR_##IT::iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! ������������ ������������ ����������
#define STL_FOR_ALL_CONST(CONTAINER, IT)                 \
typedef BOOST_TYPEOF(CONTAINER) CONTAINER_TYPE_FOR_##IT; \
for (CONTAINER_TYPE_FOR_##IT::const_iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! �������������� ����������� ���� ��� ����������
#define AUTO BOOST_AUTO

#ifdef COMPILER_VISUAL_STUDIO
	#define NO_V_TABLE __declspec(novtable)
#else
	#define NO_V_TABLE
#endif

#endif // _LIB_COMMON_MACROS_H_
