/*!
  \copyright (c) RDO-Team, 2011
  \file      factory.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      21.09.2011
  \brief     
  \indent    4T
*/

#ifndef _UTILS_SMART_PTR_FACTORY_H_
#define _UTILS_SMART_PTR_FACTORY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/namespace.h"
#include "utils/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

template <class T>
class intrusive_ptr;

template <class T>
class RefCounter;

//! �������, ������������ ��� �������� � �������� ��������
//! \tparam T - ��� ������������ �������
template <class T>
class Factory
{
friend class intrusive_ptr<T>;
friend class RefCounter<T>;
public:
	//! ������� ������ ������ �
	//! \result ����� ��������� �� ������
	static intrusive_ptr<T> create();

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1>
	static intrusive_ptr<T> create(CREF(P1) p1);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \param p3 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2, typename P3>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \param p3 - �������� ������������
	//! \param p4 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2, typename P3, typename P4>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \param p3 - �������� ������������
	//! \param p4 - �������� ������������
	//! \param p5 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \param p3 - �������� ������������
	//! \param p4 - �������� ������������
	//! \param p5 - �������� ������������
	//! \param p6 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \param p3 - �������� ������������
	//! \param p4 - �������� ������������
	//! \param p5 - �������� ������������
	//! \param p6 - �������� ������������
	//! \param p7 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \param p3 - �������� ������������
	//! \param p4 - �������� ������������
	//! \param p5 - �������� ������������
	//! \param p6 - �������� ������������
	//! \param p7 - �������� ������������
	//! \param p8 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7, CREF(P8) p8);

	//! ������� ������ ������ �
	//! \param p1 - �������� ������������
	//! \param p2 - �������� ������������
	//! \param p3 - �������� ������������
	//! \param p4 - �������� ������������
	//! \param p5 - �������� ������������
	//! \param p6 - �������� ������������
	//! \param p7 - �������� ������������
	//! \param p8 - �������� ������������
	//! \param p9 - �������� ������������
	//! \result ����� ��������� �� ������
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static intrusive_ptr<T> create(CREF(P1) p1, CREF(P2) p2, CREF(P3) p3, CREF(P4) p4, CREF(P5) p5, CREF(P6) p6, CREF(P7) p7, CREF(P8) p8, CREF(P9) p9);

private:
	static void destroy(PTR(T) object);
};

#define DECLARE_FACTORY(TYPE) friend class rdo::Factory<TYPE>;

CLOSE_RDO_NAMESPACE

#endif // _UTILS_SMART_PTR_FACTORY_H_
