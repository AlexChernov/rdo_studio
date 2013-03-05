/*!
  \copyright (c) RDO-Team, 2009-2012
  \file      main.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \authors   ������� ������� (robot.xet@gmail.com)
  \authors   �������� ����� (cerevra@gmail.com)
  \date      05.03.2013
  \brief     ����-���� ��� RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/bind.hpp>
#define BOOST_TEST_MODULE RDOValue_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

RDOValue foo()
{
	LPRDOEnumType pRDOEnumType = rdo::Factory<RDOEnumType>::create();

	pRDOEnumType->add(_T("first_record"));
	pRDOEnumType->add(_T("second_record"));

	RDOValue tempObject = RDOValue(pRDOEnumType,_T("second_record"));
	return tempObject;
}

BOOST_AUTO_TEST_SUITE(RDOValue_Test)

BOOST_AUTO_TEST_CASE(RDOValue_String)
{
	RDOValue a = foo();
	int i = 1;
}


BOOST_AUTO_TEST_SUITE_END() // RDOValue_Test

CLOSE_RDO_RUNTIME_NAMESPACE
