/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     ���� �������� ������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#define BOOST_TEST_MODULE RDORuntime_Fuzzy_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_fuzzy.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

BOOST_AUTO_TEST_SUITE(RDORuntime_Fuzzy_Test)

BOOST_AUTO_TEST_CASE(DefineAreaTest)
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPDefineArea pDefineAreaEmpty = rdo::Factory<DefineArea>::create();
	BOOST_CHECK(pDefineAreaEmpty);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(1.0, 5.0);
	BOOST_CHECK(pDefineArea);
}
BOOST_AUTO_TEST_CASE(FuzzySetTest)
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOFuzzyType pType = rdo::Factory<RDOFuzzyType>::create();
	BOOST_CHECK(pType);

	LPFuzzySet pInfinityFuzzySet = rdo::Factory<FuzzySet>::create(pType);
	BOOST_CHECK(pInfinityFuzzySet);

	LPFuzzySet pCopyFuzzySet = rdo::Factory<FuzzySet>::create(pInfinityFuzzySet);
	BOOST_CHECK(pCopyFuzzySet);

	LPDefineArea pDefineArea = rdo::Factory<DefineArea>::create(0.0, 100.0);
	BOOST_CHECK(pDefineArea);

	LPFuzzySet pFuzzySet = rdo::Factory<FuzzySet>::create(pType, *pDefineArea);
	BOOST_CHECK(pFuzzySet);

	FuzzySet::FuzzySetDefinition pSetDefinition;
	typedef FuzzySet::FuzzySetDefinition::iterator It;
	typedef std::pair<It,rbool> Ins;

	Ins insVal1 = pSetDefinition.insert(std::pair<RDOValue,double>(5,0.5));
	BOOST_CHECK(insVal1.second);

	Ins insVal2 = pSetDefinition.insert(std::pair<RDOValue,double>(1,0.1));
	BOOST_CHECK(insVal2.second);

	Ins insVal3 = pSetDefinition.insert(std::pair<RDOValue,double>(2,0.2));
	BOOST_CHECK(insVal3.second);

	Ins insVal4 = pSetDefinition.insert(std::pair<RDOValue,double>(3,1.0));
	BOOST_CHECK(insVal4.second);

	Ins insVal5 = pSetDefinition.insert(std::pair<RDOValue,double>(6,0.4));
	BOOST_CHECK(insVal5.second);

	Ins insVal6 = pSetDefinition.insert(std::pair<RDOValue,double>(7,0.2));
	BOOST_CHECK(insVal6.second);

	Ins insVal7 = pSetDefinition.insert(std::pair<RDOValue,double>(9,0.1));
	BOOST_CHECK(insVal7.second);

	Ins insVal8 = pSetDefinition.insert(std::pair<RDOValue,double>(9,0.5));
	BOOST_CHECK(!insVal8.second);

	LPFuzzySet pSet = rdo::Factory<FuzzySet>::create(pSetDefinition);

	tstring stringPresentation = pSet->getAsString();
	std::cout << stringPresentation << std::endl;
//	BOOST_CHECK(stringPresentation == _T("<1,0.1> <2,0.2> <3,1.0> <5,0.5> <6,0.4> <7,0.2> <9,0.1>"));
}



BOOST_AUTO_TEST_SUITE_END() // RDORuntime_Fuzzy_Test
CLOSE_RDO_RUNTIME_NAMESPACE