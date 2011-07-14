/*
 * copyright: (c) RDO-Team, 2011
 * filename : main.cpp
 * author   : ������ ������, Evgeny Proydakov
 * date     : 16.06.2011
 * bref     : Test for rdo_common interface
 * indent   : 4T
 */
 
// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <iostream>
#define BOOST_TEST_MODULE test_rdo_interface
#include <boost/test/included/unit_test.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdointerface.h"
// ===============================================================================

class IMy1
{
public:
	virtual void fun1() = 0;
};

class IMy2
{
public:
	virtual void fun2() = 0;
};

class IMy3
{
public:
	virtual void fun3() = 0;
};

class IMy4
{
public:
	virtual void fun4() = 0;
};

INTERFACE_REGISTRATOR(IMy1, 1);
INTERFACE_REGISTRATOR(IMy2, 2);
INTERFACE_REGISTRATOR(IMy3, 3);
INTERFACE_REGISTRATOR(IMy4, 4);

class MyClass: public rdo::IGetUnknown, public IMy1, public IMy2, public IInit
{
	QUERY_INTERFACE_BEGIN
		QUERY_INTERFACE(IMy1)
		QUERY_INTERFACE(IMy2)
		QUERY_INTERFACE(IInit)
		QUERY_INTERFACE_END

protected:
	MyClass(int i)
		: m_i(i)
	{
		std::cout << "MyClass(int " << i << ");" << std::endl;
	}
	~MyClass()
	{
		std::cout << "~MyClass(): " << m_i << std::endl;
	}
	rbool init()
	{
		return true;
	}

protected:
	int m_i;

private:
	void fun1()
	{
		std::cout << "void fun1(): " << m_i << std::endl;
	}
	void fun2()
	{
		std::cout << "void fun2(): " << m_i << std::endl;
	}
};

void fun(CREF(LPIMy1) my1)
{
	ASSERT(my1);
}

class MyClass2: public MyClass, public IMy3
{
	DEFINE_IFACTORY(MyClass2);
	QUERY_INTERFACE_BEGIN
		QUERY_INTERFACE_PARENT(MyClass)
		QUERY_INTERFACE(IMy3)
		QUERY_INTERFACE_END

private:
	MyClass2(int i): MyClass(i)
	{
		std::cout << "MyClass2(int " << i << ");" << std::endl;
	}
	~MyClass2()
	{
		std::cout << "~MyClass2()" << std::endl;
	}
	void fun3()
	{
		fun(this);

		LPIMy1 int1 = this;
		ASSERT(int1)
			std::cout << "void fun3(): " << m_i << std::endl;
	}
};

class MyClass3: public IMy3, public rdo::IGetUnknown
{
	DEFINE_IFACTORY(MyClass3);
	QUERY_INTERFACE_BEGIN
		QUERY_INTERFACE(IMy3)
		QUERY_INTERFACE_END

protected:
	MyClass3(int i)
		: m_i(i)
	{
		std::cout << "MyClass3(int " << i << ");" << std::endl;
	}
	~MyClass3()
	{
		std::cout << "~MyClass3(): " << m_i << std::endl;
	}

protected:
	int m_i;

private:
	void fun3()
	{
		std::cout << "void fun3(): " << m_i << std::endl;
	}
};

typedef rdo::Interface<IMy3>     MyInterface;
typedef std::vector<MyInterface> MyInterfaceList;

BOOST_AUTO_TEST_SUITE(test_rdo_interface)

BOOST_AUTO_TEST_CASE(checking_on_the_same_operator)
{
	ruint initValue = 2;
	rdo::UnknownPointer smptr = F(MyClass2)::create(initValue);
	BOOST_REQUIRE(smptr);

	///  TODO ��� ������ ���� ����� ���� ? ��� ���� ��  ///
	smptr.query_cast<IMy1>()->fun1();
	smptr.query_cast<IMy3>()->fun3();

	MyInterface imy3 = smptr;
	rdo::UnknownPointer smptr2;
	smptr2 = F(MyClass2)::create(2);
	///////////////////////////////////////////////////////
	
	rdo::UnknownPointer smptr2_2 = smptr2;
	BOOST_CHECK(smptr2_2 == smptr);
	BOOST_CHECK(smptr2_2 == smptr2);

	rdo::Interface<IMy1> int1_1 = smptr;
	rdo::Interface<IMy1> int2_1 = smptr2;
	rdo::Interface<IMy1> int1_2 = smptr;
	BOOST_CHECK(int1_1 == int2_1);
	BOOST_CHECK(int1_1 == int1_2);
}

// ��� ������ ��������� ���� ���� ???
// ���� ����������� �� ��������: ***
BOOST_AUTO_TEST_CASE(test_rdo_interface_2)
{
	MyInterfaceList list;

	ruint initValue = 2;

	rdo::UnknownPointer smptr = F(MyClass2)::create(initValue);
	rdo::UnknownPointer smptr2 = F(MyClass2)::create(initValue * 2);

	list.push_back(F(MyClass3)::create(3));
	list.push_back(smptr);
	list.push_back(smptr2);
	
	MyInterfaceList::const_iterator endIt = list.end();
	for(MyInterfaceList::iterator it = list.begin(); it != endIt; ++it)
	{
		///  TODO ��� ������ ���� ����� ���� ? ��� ���� ��  ///
		rdo::Interface<IMy1> ptr1;
		ptr1 = *it;
		if (ptr1)
			ptr1->fun1();

		rdo::Interface<IMy2> ptr2 = *it;
		if (ptr2)
			ptr2->fun2();

		rdo::Interface<IMy3> ptr3 = *it;
		if (ptr3)
			ptr3->fun3();
		///////////////////////////////////////////////////////
	}
}

BOOST_AUTO_TEST_SUITE_END()