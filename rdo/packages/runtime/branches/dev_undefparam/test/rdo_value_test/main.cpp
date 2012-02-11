/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      13.07.2009
  \brief     ���� ��� RDOValue
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <boost/regex.hpp>
#define BOOST_TEST_MODULE RDOValue_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDOValue_Test)

BOOST_AUTO_TEST_CASE(RDOValue_String)
{
	const tstring str1 = _T("qqq");
	rdoRuntime::RDOValue value1(str1);
	BOOST_CHECK(value1.getString  () == str1);
	BOOST_CHECK(value1.getAsString() == str1);

	rdoRuntime::RDOValue value2 = value1;
	BOOST_CHECK(value2.getString  () == str1);
	BOOST_CHECK(value2.getAsString() == str1);
	BOOST_CHECK(value2 == value1);

	const tstring str2 = _T("aaa");
	value2 += str2;
	BOOST_CHECK(value2.getString  () == str1 + str2);
	BOOST_CHECK(value2.getAsString() == str1 + str2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint)
{
	const rsint int1 = 30;
	rdoRuntime::RDOValue value1(int1);
	BOOST_CHECK(value1.getInt() == int1);

	rdoRuntime::RDOValue value2 = value1;
	BOOST_CHECK(value2 == int1);
	BOOST_CHECK(value2 == value1);

	const rsint int2 = 20;
	value1 += int2;
	BOOST_CHECK(value1 == int1 + int2);

	const rsint int3 = 10;
	value1 -= int3;
	BOOST_CHECK(value1 == int1 + int2 - int3);
	
	const rsint int4 = 2;
	value2 = value1;
	value2 *= int4;
	BOOST_CHECK(value2 == value1 * int4);

	const rsint int5 = 5;
	value2 = value1;
	value2 /= int5;
	BOOST_CHECK(value2 == value1 / int5);
	
	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);	
	value2 ++;
	BOOST_CHECK(value2 == value1);

	value1 = int1;
	value2 = int2;
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 > value2);	
	BOOST_CHECK(value2 < value1);
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
	value2 = int1;
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);

}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint)
{
	const ruint int1 = 30;
	rdoRuntime::RDOValue value1(int1);
	BOOST_CHECK(value1 == int1);

	rdoRuntime::RDOValue value2 = value1;
	BOOST_CHECK(value2 == int1);
	BOOST_CHECK(value2 == value1);

	const ruint int2 = 20;
	value1 += int2;
	BOOST_CHECK(value1 == int1 + int2);

	const ruint int3 = 10;
	value1 -= int3;
	BOOST_CHECK(value1 == int1 + int2 - int3);

	const ruint int4 = 2;
	value2 = value1;
	value2 *= int4;
	BOOST_CHECK(value2 == value1 * int4);

	const ruint int5 = 5;
	value2 = value1;
	value2 /= int5;
	BOOST_CHECK(value2 == value1 / int5);

	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);	
	value2 ++;
	BOOST_CHECK(value2 == value1);	

	value1 = int1;
	value2 = int2;
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 > value2);	
	BOOST_CHECK(value2 < value1);
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
	value2 = int1;
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double)
{
	const double doub1 = 30.2;
	rdoRuntime::RDOValue value1(doub1);
	BOOST_CHECK(value1 == doub1);

	rdoRuntime::RDOValue value2 = value1;
	BOOST_CHECK(value2 == doub1);
	BOOST_CHECK(value2 == value1);

	const double doub2 = 20.5;
	value1 += doub2;
	BOOST_CHECK(value1 == doub1 + doub2);

	const double doub3 = 10.3;
	value1 -= doub3;
	BOOST_CHECK(value1 == doub1 + doub2 - doub3);

	const double doub4 = 2.5;
	value2 = value1;
	value2 *= doub4;
	BOOST_CHECK(value2 == value1 * doub4);

	const double doub5 = 5;
	value2 = value1;
	value2 /= doub5;
	BOOST_CHECK(value2 == value1 / doub5);

	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);	
	value2 ++;
	BOOST_CHECK(value2 == value1);	

	value1 = doub1;
	value2 = doub2;
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 > value2);	
	BOOST_CHECK(value2 < value1);
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
	value2 = doub1;
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Bool)
{
	rbool bool1 = true;
	rbool bool2 = false;
	rbool bool3 = true;
	rbool bool4 = false;

	rdoRuntime::RDOValue value1(bool1);
	rdoRuntime::RDOValue value2(bool2);
	rdoRuntime::RDOValue value3(bool3);
	rdoRuntime::RDOValue value4(bool4);

	BOOST_CHECK(value1.getBool());
	BOOST_CHECK(!value2);
	BOOST_CHECK(value2 == value4);
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(!(value1 == value2));
	BOOST_CHECK(value1 && value3);
	BOOST_CHECK(!(value2 && value3));
	BOOST_CHECK(value1 || value2);
	BOOST_CHECK(!(value2 || value4));	
}

BOOST_AUTO_TEST_CASE(RDOValue_Char)
{
	tchar ch1 = 'a';
	tchar ch2 = 'b';
	rdoRuntime::RDOValue value1 = ch1;
	rdoRuntime::RDOValue value2 = ch2;
	BOOST_CHECK(value1 == ch1);
	BOOST_CHECK(value1 < value2);
	BOOST_CHECK(value2 > value1);
	BOOST_CHECK(value1 != value2);
	value1 = value2;
	BOOST_CHECK(value1 == value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Enum)
{
	using namespace rdoRuntime;

	LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
	BOOST_CHECK(pEnum);
	pEnum->empty();	
}
/*
BOOST_AUTO_TEST_CASE(RDOValue_Identificator)
{
	
}
*/

BOOST_AUTO_TEST_CASE(RDOValue_Resource)
{
	using namespace rdoRuntime;

	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOResourceType pResourceType = rdo::Factory<RDOResourceType>::create(0);
	BOOST_CHECK(pResourceType);
	LPIResourceType pResourceFactory = pResourceType.interface_cast<IResourceType>();
	BOOST_CHECK(pResourceFactory);

	std::vector<RDOValue> paramList;
	paramList.push_back(RDOValue(1      ));
	paramList.push_back(RDOValue(2.2    ));
	paramList.push_back(RDOValue(_T("3")));

	LPRDOResource pResource = pResourceFactory->createRes(pRuntime, pRuntime->getResourceId(), paramList, true, true);
	BOOST_CHECK(pResource);

	RDOValue value1(pResourceType, pResource);

	LPRDOResourceType pType = value1.type().object_dynamic_cast<RDOResourceType>();
	BOOST_CHECK(pType);

	LPRDOResource pResource1 = value1.getPointer<RDOResource>();

	pRuntime   = NULL;
	value1     = RDOValue();
	pResource1 = NULL;

	BOOST_CHECK(pResource.owner());

	pResource = NULL;
}

BOOST_AUTO_TEST_SUITE_END() //RDOValue_Resource
