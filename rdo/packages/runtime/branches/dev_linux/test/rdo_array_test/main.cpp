/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   ������ ������
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      13.12.2009
  \brief     ���� ��������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <stdio.h>
#define BOOST_TEST_MODULE RDORuntime_Array_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
#include "simulator/runtime/rdo_array.h"
#include "simulator/runtime/rdo_type.h"
// --------------------------------------------------------------------------------

const ruint KILLER_ITERATION = 6;

BOOST_AUTO_TEST_SUITE(RDORuntime_Array_Test)

BOOST_AUTO_TEST_CASE(arrayTestCreate)
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	BOOST_CHECK(arrayVal1.getAsString() == _T("[1, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(arrayTestInsert)
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	arrayVal1.insert(arrayVal1.begin() + rdoRuntime::RDOValue(1), arrayVal2.begin(), arrayVal2.end());
	
	BOOST_CHECK(arrayVal1.getAsString() == _T("[1, 4, 5, 6, 2, 3]"));
}

BOOST_AUTO_TEST_CASE(arrayTestErase)
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	arrayVal1.erase(arrayVal1.begin() + rdoRuntime::RDOValue(1), arrayVal1.begin() + rdoRuntime::RDOValue(3));
	
	BOOST_CHECK(arrayVal1.getAsString() == _T("[1]"));
}

BOOST_AUTO_TEST_CASE(arrayTestPPOperator)
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	ruint killer_cycle = KILLER_ITERATION;
	tstring itStr = _T("");
	for (rdoRuntime::RDOValue it = arrayVal1.begin(); it != arrayVal1.end()  && killer_cycle; ++it)
	{
		itStr += it.getAsString();
		--killer_cycle;
	}
	BOOST_CHECK(itStr == _T("123"));
	BOOST_CHECK(killer_cycle); // infinite loop error
}

BOOST_AUTO_TEST_CASE(arrayTestOperatorPP)
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	ruint killer_cycle = KILLER_ITERATION;
	tstring itStr = _T("");
	for (rdoRuntime::RDOValue it = arrayVal1.begin(); it != arrayVal1.end()  && killer_cycle; it++)
	{
		itStr += it.getAsString();
		--killer_cycle;
	}
	BOOST_CHECK(itStr == _T("123"));
	BOOST_CHECK(killer_cycle);  // infinite loop error
}

BOOST_AUTO_TEST_CASE(arrayTestMMOperator)
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	ruint killer_cycle = KILLER_ITERATION;
	tstring itStr = _T("");
	rdoRuntime::RDOValue it1 = arrayVal2.end();
	do 
	{
		--killer_cycle;

		--it1;
		itStr += it1.getAsString();
	} while (it1 != arrayVal2.begin() && killer_cycle);

	BOOST_CHECK(itStr == _T("654"));
	BOOST_CHECK(killer_cycle);  // infinite loop error
}

BOOST_AUTO_TEST_CASE(arrayTestOperatorMM)
{
	#include "simulator/runtime/test/rdo_array_test/arrayCreate.inl"

	ruint killer_cycle = KILLER_ITERATION;
	tstring itStr = _T("");
	rdoRuntime::RDOValue it1 = arrayVal2.end();
	do 
	{
		--killer_cycle;

		it1--;
		itStr += it1.getAsString();
	} while (it1 != arrayVal2.begin() && killer_cycle);

	BOOST_CHECK(itStr == _T("654"));
	BOOST_CHECK(killer_cycle);  // infinite loop error
}

BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Array_Test
