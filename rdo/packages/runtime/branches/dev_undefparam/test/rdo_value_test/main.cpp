/*!
  \copyright (c) RDO-Team, 2009-2012
  \file      main.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \authors   ������� ������� (robot.xet@gmail.com)
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

using namespace rdoRuntime;

BOOST_AUTO_TEST_SUITE(RDOValue_Test)

void testing(RDOValue value1, RDOValue value2)
{
	bool flag = false;
	try
	{
		value1 += value2;
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		value1 -= value2;
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		value1 *= value2;
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		value1 /= value2;
	}
	catch (CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		if (value1 > value2)
		{}
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		if(value1 <  value2);
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		if(value1 >= value2);
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		if(value1 <= value2);
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
	flag = false;
	try
	{
		if(value1 == value2);
	}
	catch(CREF(RDOValueException))
	{
		flag = true;
	}
	BOOST_CHECK(flag);
}

void compare(RDOValue value1, RDOValue value2)
{
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 <  value2);
	BOOST_CHECK(value2 >  value1);
	BOOST_CHECK(value1 <= value2);
	BOOST_CHECK(value2 >= value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_String)
{
	const tstring str1 = _T("qqq");
	RDOValue value1(str1);
	BOOST_CHECK(value1.getString  () == str1);
	BOOST_CHECK(value1.getAsString() == str1);

	RDOValue value2 = value1;
	BOOST_CHECK(value2.getString  () == str1);
	BOOST_CHECK(value2.getAsString() == str1);
	BOOST_CHECK(value2 == value1);

	const tstring str2 = _T("aaa");
	value2 += str2;
	BOOST_CHECK(value2.getString  () == str1 + str2);
	BOOST_CHECK(value2.getAsString() == str1 + str2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Arifmethic)
{
	const rsint val1 = 30;
	RDOValue value1(val1);
	BOOST_CHECK(value1.getInt      () == val1);
	BOOST_CHECK(value1.getUInt     () == val1);
	BOOST_CHECK(value1.getDouble   () == val1);
	BOOST_CHECK(value1.getAsString () == "30");
	BOOST_CHECK(value1.getEnumAsInt() == 30  );

	RDOValue value2 = value1;
	BOOST_CHECK(value2 == val1  );
	BOOST_CHECK(value2 == value1);

	const rsint val2 = 20;
	value1 += val2;
	BOOST_CHECK(value1 == val1 + val2);
	value1 = val1;
	value1 = value1 + val2;
	BOOST_CHECK(value1 == val1 + val2);

	const rsint val3 = 10;
	value1 -= val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);
	value1 += val3;
	value1 = value1 - val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);
	
	const rsint val4 = 2;
	value2 = value1;
	value2 *= val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);
	value2 /= val4;
	value2 = value2 * val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);

	const rsint val5 = 5;
	value2 = value1;
	value2 /= val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));
	value2 *= val5;
	value2 = value2 / val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));
	
	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);
	value2 ++;
	BOOST_CHECK(value2 == value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Compare)
{
	const rsint val1 = 30;
	const rsint val2 = 20;
	RDOValue value1(val1);
	RDOValue value2(val2);
	
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 >  value2);
	BOOST_CHECK(value2 <  value1);
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
	value2 = val1;
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Arifmethic)
{
	const ruint val1 = 30;
	RDOValue value1(val1);
	BOOST_CHECK(value1 == val1);
	BOOST_CHECK(value1.getInt      () == val1);
	BOOST_CHECK(value1.getUInt     () == val1);
	BOOST_CHECK(value1.getDouble   () == val1);
	BOOST_CHECK(value1.getAsString () == "30");
	BOOST_CHECK(value1.getEnumAsInt() == 30  );

	RDOValue value2 = value1;
	BOOST_CHECK(value2 == val1  );
	BOOST_CHECK(value2 == value1);

	const ruint val2 = 20;
	value1 += val2;
	BOOST_CHECK(value1 == val1 + val2);
	value1 = val1;
	value1 = value1 + val2;
	BOOST_CHECK(value1 == val1 + val2);

	const ruint val3 = 10;
	value1 -= val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);
	value1 += val3;
	value1 = value1 - val3;
	BOOST_CHECK(value1 == val1 + val2 - val3);

	const ruint val4 = 2;
	value2 = value1;
	value2 *= val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);
	value2 /= val4;
	value2 = value2 * val4;
	BOOST_CHECK(value2 == (val1 + val2 - val3) * val4);

	const ruint val5 = 5;
	value2 = value1;
	value2 /= val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));
	value2 *= val5;
	value2 = value2 / val5;
	BOOST_CHECK(value2 == ((val1 + val2 - val3) / val5));

	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);
	value2 ++;
	BOOST_CHECK(value2 == value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Compare)
{
	const ruint val1 = 30;
	const ruint val2 = 20;
	RDOValue value1(val1);
	RDOValue value2(val2);

	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 >  value2);
	BOOST_CHECK(value2 <  value1);
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
	value2 = val1;
	BOOST_CHECK(value1 >= value2);
	BOOST_CHECK(value2 <= value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_Arifmethic)
{
	const double doub1 = 30.2;
	RDOValue value1(doub1);
	BOOST_CHECK(value1 == doub1);
	BOOST_CHECK(value1.getInt      () == 30    );
	BOOST_CHECK(value1.getUInt     () == 30    );
	BOOST_CHECK(value1.getDouble   () == doub1 );
	BOOST_CHECK(value1.getAsString () == "30.2");
	BOOST_CHECK(value1.getEnumAsInt() == 30    );

	RDOValue value2 = value1;
	BOOST_CHECK(value2 == doub1);
	BOOST_CHECK(value2 == value1);

	const double doub2 = 20.5;
	value1 += doub2;
	BOOST_CHECK(value1 == doub1 + doub2);
	value1 = doub1;
	value1 = value1 + doub2;
	BOOST_CHECK(value1 == doub1 + doub2);

	const double doub3 = 10.3;
	value1 -= doub3;
	BOOST_CHECK(value1 == doub1 + doub2 - doub3);
	value1 += doub3;
	value1 = value1 - doub3;
	BOOST_CHECK(value1 == doub1 + doub2 - doub3);

	const double doub4 = 2.5;
	value2 = value1;
	value2 *= doub4;
	BOOST_CHECK(value2 == (doub1 + doub2 - doub3) * doub4);
	value2 /= doub4;
	value2 = value2 * doub4;
	BOOST_CHECK(value2 == (doub1 + doub2 - doub3) * doub4);

	const double doub5 = 5;
	value2 = value1;
	value2 /= doub5;
	BOOST_CHECK(value2 == ((doub1 + doub2 - doub3) / doub5));
	value2 *= doub5;
	value2 = value2 / doub5;
	BOOST_CHECK(value2 == ((doub1 + doub2 - doub3) / doub5));

	value2 = value1;
	value2 --;
	BOOST_CHECK(value2 == value1 - 1);
	value2 ++;
	BOOST_CHECK(value2 == value1);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_Compare)
{
	const double doub1 = 30.2;
	const double doub2 = 20.5;
	RDOValue value1(doub1);
	RDOValue value2(doub2);
	
	BOOST_CHECK(value1 != value2);
	BOOST_CHECK(value1 >  value2);
	BOOST_CHECK(value2 <  value1);
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

	RDOValue value1(bool1);
	RDOValue value2(bool2);
	RDOValue value3(bool3);
	RDOValue value4(bool4);

	BOOST_CHECK(value1.getBool     ()     );
	BOOST_CHECK(value1.getInt      () == 1);
	BOOST_CHECK(value1.getUInt     () == 1);
	BOOST_CHECK(value1.getEnumAsInt() == 1);
	BOOST_CHECK(value1.getDouble   () == 1);
	BOOST_CHECK(!value2);
	BOOST_CHECK(  value2 == value4 );
	BOOST_CHECK(  value1 != value2 );
	BOOST_CHECK(!(value1 == value2));
	BOOST_CHECK(  value1 && value3 );
	BOOST_CHECK(!(value2 && value3));
	BOOST_CHECK(  value1 || value2 );
	BOOST_CHECK(!(value2 || value4));
}

BOOST_AUTO_TEST_CASE(RDOValue_Char)
{
	tchar ch1 = 'a';
	tchar ch2 = 'b';
	RDOValue value1 = ch1;
	RDOValue value2 = ch2;
	BOOST_CHECK(value1 == ch1   );
	BOOST_CHECK(value1 <  value2);
	BOOST_CHECK(value2 >  value1);
	BOOST_CHECK(value1 != value2);
	value1 = value2;
	BOOST_CHECK(value1 == value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Enum)
{
	LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
	BOOST_CHECK(pEnum);
	BOOST_CHECK(pEnum->empty());
	pEnum->add(_T("test0"));
	pEnum->add(_T("test1"));
	pEnum->add(_T("test2"));
	pEnum->add(_T("test3"));
	BOOST_CHECK(pEnum->findEnum(_T("test1")) == 1);
	BOOST_CHECK(pEnum->exist   (_T("test3"))     );

	RDOValue value(pEnum);
	BOOST_CHECK(value.typeID      () == RDOType::t_enum);
	BOOST_CHECK(value.getEnum     () == pEnum          );
	BOOST_CHECK(value.getInt      () == 0              );
	BOOST_CHECK(value.getDouble   () == 0              );
	BOOST_CHECK(value.getEnumAsInt() == 0              );
	BOOST_CHECK(!value.getAsBool  ()                   );
	BOOST_CHECK(value.getAsString () == "test0"        );

	RDOValue value1(pEnum, "test2");
	BOOST_CHECK(value1.typeID      () == RDOType::t_enum);
	BOOST_CHECK(value1.getEnum     () == pEnum          );
	BOOST_CHECK(value1.getInt      () == 2              );
	BOOST_CHECK(value1.getDouble   () == 2              );
	BOOST_CHECK(value1.getEnumAsInt() == 2              );
	BOOST_CHECK(value1.getAsBool   ()                   );
	BOOST_CHECK(value1.getAsString () == "test2"        );

	RDOValue value2(pEnum, 2);
	BOOST_CHECK(value2.typeID      () == RDOType::t_enum);
	BOOST_CHECK(value2.getEnum     () == pEnum          );
	BOOST_CHECK(value2.getInt      () == 2              );
	BOOST_CHECK(value2.getDouble   () == 2              );
	BOOST_CHECK(value2.getEnumAsInt() == 2              );
	BOOST_CHECK(value2.getAsBool   ()                   );
	BOOST_CHECK(value2.getAsString () == "test2"        );
	
}

template <class T1, class T2>
void compareValue(CREF(T1) param1, CREF(T2) param2, CREF(T2) param3, CREF(RDOType::TypeID) type1, CREF(RDOType::TypeID) type2)
{
	T1 val1 = param1;
	T2 val2 = param2;
	RDOValue value1(val1);
	RDOValue value2(val2);

	compare(value1, value2);

	T2 val3 = param3;
	value2 = val3;
	BOOST_CHECK(value1 == value2);
	value1 = value2;
	BOOST_CHECK(value1 == value2);

	value1 = val1;
	value2 = val2;
	value1 += value2;
	BOOST_CHECK(value1          == T1(param1 + param2));
	BOOST_CHECK(value1.typeID() == type1);
	value1 -= value2;
	BOOST_CHECK(value1 == param1);
	value1 *= value2;
	BOOST_CHECK(value1 == param1 * param2);
	value1 /= value2;
	BOOST_CHECK(value1 == param1);

	BOOST_CHECK(value1.typeID() == type2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Ruint)
{
	compareValue<rsint, ruint>(10, 15, 10, RDOType::t_int, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Rsint)
{
	compareValue<ruint, rsint>(10, 15, 10, RDOType::t_int, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_Double)
{
	compareValue<rsint, double>(10, 15.2, 10, RDOType::t_int, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_rsint)
{
	compareValue<double, rsint>(10.2, 15, 15, RDOType::t_real, RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_Double)
{
	ruint  val1 = 10  ;
	double val2 = 15.2;
	RDOValue value1(val1);
	RDOValue value2(val2);

	compare(value1, value2);

	double val3 = 10;
	value2 = val3;
	BOOST_CHECK(value1 == value2);
	value1 = value2;
	BOOST_CHECK(value1 == value2);

	value1 = val1;
	value2 = val2;
	value1 += value2;
	BOOST_CHECK(value1          == 25            );
	BOOST_CHECK(value1.typeID() == RDOType::t_int);
	value1 -= value2;
	BOOST_CHECK(value1 == 10 );
	value1 *= value2;
	BOOST_CHECK(value1 == 152);
	value1 /= value2;
	BOOST_CHECK(value1 == 10 );

	BOOST_CHECK(value1.typeID() == RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_ruint)
{
	double val1 = 10.2;
	ruint  val2 = 15  ;
	RDOValue value1(val1);
	RDOValue value2(val2);

	compare(value1, value2);

	double val3 = 15;
	value1 = val3;
	BOOST_CHECK(value1 == value2);
	value1 = value2;
	BOOST_CHECK(value1 == value2);

	value1 = val1;
	value2 = val2;
	value1 += value2;
	BOOST_CHECK(value1          == 25.2           );
	BOOST_CHECK(value1.typeID() == RDOType::t_real);
	value1 -= value2;
	BOOST_CHECK(value1 == 10.2 );
	value1 *= value2;
	BOOST_CHECK(value1 == 153  );
	value1 /= value2;
	BOOST_CHECK(value1 == 10.2 );

	BOOST_CHECK(value1.typeID() == RDOType::t_real);
}

BOOST_AUTO_TEST_CASE(RDOValue_Rsint_String)
{
	rsint   val1 = 10       ;
	tstring str1 = _T("abc");
	RDOValue value1(val1);
	RDOValue value2(str1);

	testing(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Ruint_String)
{
	ruint   val1 = 10       ;
	tstring str1 = _T("abc");
	RDOValue value1(val1);
	RDOValue value2(str1);

	testing(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Double_String)
{
	double  val1 = 10       ;
	tstring str1 = _T("abc");
	RDOValue value1(val1);
	RDOValue value2(str1);

	testing(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Char_Rsint)
{
	rsint  val1 = 10     ;
	tchar  ch1  = _T('a');
	RDOValue value1(val1);
	RDOValue value2(ch1 );
	
	value1 += value2;
	BOOST_CHECK(value1 == val1 + ch1);
	value1 -= value2;
	BOOST_CHECK(value1 == val1      );
	value1 *= value2;
	BOOST_CHECK(value1 == val1 * ch1);
	value1 /= value2;
	BOOST_CHECK(value1 == val1      );

	compare(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Char_Ruint)
{
	ruint  val1 = 10     ;
	tchar  ch1  = _T('a');
	RDOValue value1(val1);
	RDOValue value2(ch1 );

	value1 += value2;
	BOOST_CHECK(value1 == val1 + ch1);
	value1 -= value2;
	BOOST_CHECK(value1 == val1      );
	value1 *= value2;
	BOOST_CHECK(value1 == val1 * ch1);
	value1 /= value2;
	BOOST_CHECK(value1 == val1      );

	compare(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Char_Double)
{
	double val1 = 10     ;
	tchar  ch1  = _T('a');
	RDOValue value1(val1);
	RDOValue value2(ch1 );

	value1 += value2;
	BOOST_CHECK(value1 == val1 + ch1);
	value1 -= value2;
	BOOST_CHECK(value1 == val1      );
	value1 *= value2;
	BOOST_CHECK(value1 == val1 * ch1);
	value1 /= value2;
	BOOST_CHECK(value1 == val1      );

	compare(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_String_Char)
{
	tstring  str1 = _T("abc");
	tchar    ch1  = _T('a'  );
	RDOValue value1(str1);
	RDOValue value2(ch1 );
	
	testing(value1, value2);
}

BOOST_AUTO_TEST_CASE(RDOValue_Identificator)
{
	tstring str = _T("abc");
	RDOValue value1(str, g_identificator);
	BOOST_CHECK(value1.typeID() == RDOType::t_identificator);
	tstring iden = value1.getIdentificator();
	BOOST_CHECK(iden == str);
	iden = value1.getAsString();
	BOOST_CHECK(iden == str);

	tstring str2 =_T("dba");
	RDOValue value2(str2, g_identificator);
	BOOST_CHECK(value1 != value2);

	RDOValue value3(str, g_identificator);
	BOOST_CHECK(value1 == value3);
}

BOOST_AUTO_TEST_CASE(RDOValue_Undefined)
{
	rsint val1 = 10;
	RDOValue value1(val1);
	BOOST_CHECK(value1);
	value1.setUndefined(1);
	double undefined = value1.getUndefined();
	BOOST_CHECK(undefined == 1  );
}

BOOST_AUTO_TEST_CASE(RDOValue_Resource)
{
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