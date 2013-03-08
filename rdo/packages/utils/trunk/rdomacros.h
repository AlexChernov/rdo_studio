/*!
  \copyright (c) RDO-Team, 2011
  \file      rdomacros.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     ���������� ������� ��������
  \indent    4T
*/

#ifndef _UTILS_MACROS_H_
#define _UTILS_MACROS_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/typeof/typeof.hpp>
#include <boost/preprocessor/cat.hpp>
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
#define  CLASS(A)              class A
//! ���������� ���������
#define STRUCT(A)              struct A
//! ���������� ����������
#define S_INTERFACE(A)         struct A
//! ������������
#define IS  ,
//! ������������
#define AND ,
//! ���������� ���������� A
#define IMPLEMENTATION_OF(A)   public A
//! ������ ���� A
#define INSTANCE_OF(A)         public A
//! ������ ���� ����������� A
#define INSTANCE_VIRTUAL_OF(A) public virtual A
//! �����-������� �� B
#define CLASS_PARENT_OF(A, B)  CLASS(A): IMPLEMENTATION_OF(B)
//! ���������-������� �� B
#define STRUCT_PARENT_OF(A, B) STRUCT(A): IMPLEMENTATION_OF(B)

#ifdef UNUSED
	#undef UNUSED
#endif
//! ������ ��������� �������������� ����������� � �������������� ����������
#define UNUSED(A) ((void)A);

//! �������-���������� ���, ���������� �� ������ ������
#define RDO_UNIQUE_NAME(NAME) BOOST_PP_CAT(NAME, __LINE__)

//! ������������ ����������
#define STL_FOR_ALL(CONTAINER, IT)                                \
typedef BOOST_TYPEOF(CONTAINER) RDO_UNIQUE_NAME(CONTAINER_TYPE_); \
for (RDO_UNIQUE_NAME(CONTAINER_TYPE_)::iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! ������������ ������������ ����������
#define STL_FOR_ALL_CONST(CONTAINER, IT)                          \
typedef BOOST_TYPEOF(CONTAINER) RDO_UNIQUE_NAME(CONTAINER_TYPE_); \
for (RDO_UNIQUE_NAME(CONTAINER_TYPE_)::const_iterator IT = CONTAINER.begin(); IT != CONTAINER.end(); ++IT)

//! �������������� ����������� ���� ��� ����������
#define AUTO BOOST_AUTO

#ifdef COMPILER_VISUAL_STUDIO
	#define NO_V_TABLE   __declspec(novtable)
	#define FORCE_INLINE __forceinline
#else
	#define NO_V_TABLE
	#define FORCE_INLINE inline
#endif

#endif // _UTILS_MACROS_H_
