/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : ������ ������
 * date     : 14.07.2009
 * bref     : ���� ����� ����������
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <list>
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdosmart_ptr.h"
#include "rdo_common/rdosmart_ptr_wrapper.h"
// ===============================================================================

class MyClass
{
DECLARE_FACTORY(MyClass)
public:
	ruint m_i;

private:
	MyClass()
		: m_i(2)
	{}

	~MyClass()
	{}
};

class MyClass2
{
DECLARE_FACTORY(MyClass2)
public:
	ruint m_i1;
	ruint m_i2;
	ruint m_i3;
	ruint m_i4;
	ruint m_i5;

private:
	MyClass2()
		: m_i1(2)
	{}

	~MyClass2()
	{}
};

void main()
{
	ruint size1 = sizeof(MyClass );
	ruint size2 = sizeof(MyClass2);
	ruint size3 = sizeof(rdo::smart_ptr<MyClass>);
	ruint size4 = sizeof(rdo::smart_ptr<MyClass2>);
	{
		rdo::smart_ptr<MyClass> obj = rdo::Factory<MyClass>::create();
		obj->m_i = 10;
	}
	{
		rdo::smart_ptr<MyClass> sobj = rdo::Factory<MyClass>::create();
		int i = 1;
	}
	{
		std::list<rdo::LPISmartPtrWrapper> container;
		for (ruint i = 0; i < 100; i++)
		{
			rdo::smart_ptr<MyClass> obj1 = rdo::Factory<MyClass>::create();
			rdo::smart_ptr_wrapper<MyClass> obj1wr(obj1);
			container.push_back(&obj1wr);
		}
		int i = 1;
	}

	int i = 1;
}
