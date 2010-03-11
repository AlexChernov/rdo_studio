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

protected:
	MyClass()
		: m_i(2)
	{}

	~MyClass()
	{}
};
DECLARE_POINTER(MyClass);

class MyClass2: public MyClass
{
DECLARE_FACTORY(MyClass2)
public:
	ruint m_i1;
	ruint m_i2;
	ruint m_i3;
	ruint m_i4;
	ruint m_i5;

	rbool operator== (CREF(MyClass2) class2)
	{
		return m_i1 == class2.m_i1;
	}

private:
	MyClass2()
		: m_i1(2)
	{}

	~MyClass2()
	{}
};
DECLARE_POINTER(MyClass2);

PREDECLARE_POINTER(MyClass3);
class MyClass3: public rdo::smart_ptr_counter_reference
{
DECLARE_FACTORY(MyClass3)
public:
	ruint m_i1;

	rbool operator== (CREF(MyClass3) class3)
	{
		return m_i1 == class3.m_i1;
	}

	LPMyClass3 getThis()
	{
		return this;
	}

private:
	MyClass3()
		: m_i1(2)
	{}
	~MyClass3()
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
		std::list<rdo::LPISmartPtrWrapper> container;
		for (ruint i = 0; i < 100; i++)
		{
			rdo::smart_ptr<MyClass> obj1 = rdo::Factory<MyClass>::create();
			rdo::smart_ptr_wrapper<MyClass> obj1wr(obj1);
			container.push_back(&obj1wr);
		}
		int i = 1;
	}
	{
		LPMyClass2 obj21 = rdo::Factory<MyClass2>::create();
		LPMyClass  obj11 = obj21;
		LPMyClass2 obj22 = obj11.cast<MyClass2>();
		LPMyClass2 obj23 = obj11.lp_cast<LPMyClass2>();
//		LPMyClass3 obj31 = obj21.cast<MyClass3>(); //! �� ������ ���������������
		int i = 1;
	}
	{
		LPMyClass3 obj31 = rdo::Factory<MyClass3>::create();
		LPMyClass3 obj32 = obj31->getThis();
		int i = 1;
	}
	{
		LPMyClass2 obj21 = rdo::Factory<MyClass2>::create();
		LPMyClass2 obj22 = obj21;
		LPMyClass2 obj23 = rdo::Factory<MyClass2>::create();
		rbool flag1 = obj21 == obj22;
		rbool flag2 = obj21 == obj23;
		ASSERT( flag1);
		ASSERT(!flag2);
		LPMyClass3 obj31 = rdo::Factory<MyClass3>::create();
		LPMyClass3 obj32 = obj31->getThis();
		LPMyClass3 obj33 = obj31->getThis();
		LPMyClass3 obj34 = rdo::Factory<MyClass3>::create();
		rbool flag3 = obj31 == obj32 && obj32 == obj33;
		rbool flag4 = obj31 == obj34;
		ASSERT( flag3);
		ASSERT(!flag4);
	}
	{
		LPMyClass2 obj21 = rdo::Factory<MyClass2>::create();
		LPMyClass2 obj22 = obj21;
		LPMyClass2 obj23 = rdo::Factory<MyClass2>::create();
		rbool flag1 = obj21.compare(obj22);
		rbool flag2 = obj21.compare(obj23);
		ASSERT(flag1);
		ASSERT(flag2);
		obj23->m_i1 = 10;
		rbool flag3 = obj21.compare(obj23);
		ASSERT(!flag3);
		LPMyClass3 obj31 = rdo::Factory<MyClass3>::create();
		LPMyClass3 obj32 = obj31->getThis();
		LPMyClass3 obj33 = obj31->getThis();
		LPMyClass3 obj34 = rdo::Factory<MyClass3>::create();
		rbool flag4 = obj31.compare(obj32) && obj32.compare(obj33);
		rbool flag5 = obj31.compare(obj34);
		ASSERT(flag4);
		ASSERT(flag5);
		obj34->m_i1 = 10;
		rbool flag6 = obj21.compare(obj23);
		ASSERT(!flag6);
	}
	int i = 1;
}
