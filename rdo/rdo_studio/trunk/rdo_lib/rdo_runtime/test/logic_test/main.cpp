/*
 * copyright: (c) RDO-Team, 2009
 * filename : main.cpp
 * author   : ������ ������
 * date     : 28.09.09
 * bref     : ���� �����
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
#include <vector>
#include <algorithm>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdodebug.h"
// ===============================================================================

class MyOperation
{
public:
	MyOperation(ruint dummy)
		: m_dummy(dummy)
	{}
	rbool operator< (CREF(MyOperation) opr) const
	{
		return m_dummy < opr.m_dummy;
	}

private:
	ruint m_dummy;
};

template <class T, class C = std::vector<T> >
class Container
{
public:
	typedef T                             Item;
	typedef C                             List;
	typedef typename List::iterator       Iterator;
	typedef typename List::const_iterator CIterator;

	Iterator  begin()       { return m_list.begin(); }
	Iterator  end  ()       { return m_list.end  (); }
	CIterator begin() const { return m_list.begin(); }
	CIterator end  () const { return m_list.end  (); }
	rbool     empty() const { return m_list.empty(); }

	REF(Container) operator() (CREF(Item) item)
	{
		m_list.push_back(item);
		return *this;
	}

private:
	List m_list;
};

class OrderFIFO
{
public:
	template <class Container>
	static void sort(REF(Container) container)
	{}
};

class OrderLIFO
{
public:
	template <class Container>
	static void sort(REF(Container) container)
	{
		std::reverse(container.begin(), container.end());
	}
};

class OrderPrior
{
public:
	template <class Container>
	static void sort(REF(Container) container)
	{
		std::sort(container.begin(), container.end());
	}
};

template <class Order>
class Logic
{
public:
	Logic()
	{
		m_�ontainer(1)(2)(4)(3);
	}
	rbool checkOperation()
	{
		Order::sort(m_�ontainer);
		return true;
	}

private:
	typedef MyOperation              Item;
	typedef Container<Item>          Container;

	Container  m_�ontainer;
};

void main()
{
	{
		Logic<OrderFIFO> logic;
		logic.checkOperation();
		int i = 1;
	}
	{
		Logic<OrderLIFO> logic;
		logic.checkOperation();
		int i = 1;
	}
	{
		Logic<OrderPrior> logic;
		logic.checkOperation();
		int i = 1;
	}
}
