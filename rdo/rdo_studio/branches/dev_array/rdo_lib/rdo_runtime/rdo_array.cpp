/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.cpp
 * author   : ������ ������
 * date     : 14.12.2009
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_array.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
RDOArrayValue::RDOArrayValue(CREF(RDOArrayType) type)
	: m_arrayType(&type)
{}

RDOArrayValue::RDOArrayValue(CREF(RDOArrayValue) value)
	: m_container (value.m_container)
	, m_arrayType (value.m_arrayType)
{}

RDOArrayValue::~RDOArrayValue()
{}

CREF(RDOArrayType) RDOArrayValue::type() const
{
	ASSERT(m_arrayType);
	return *m_arrayType;
}

void RDOArrayValue::insertItem(CREF(RDOValue) pArray)
{
	m_container.push_back(pArray);
}

RDOArrayValue::Container::iterator RDOArrayValue::containerBegin()
{
	return m_container.begin();
}

RDOArrayValue::Container::iterator RDOArrayValue::containerEnd()
{
	return m_container.end();
}

void RDOArrayValue::insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.insert(itr,itrFst,itrLst);
}

void RDOArrayValue::eraseItems(Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.erase(itrFst,itrLst);
}

tstring RDOArrayValue::getAsString() const
{
	tstring ArrayName = _T("[");
	STL_FOR_ALL_CONST(Container, m_container, it)
	{
		ArrayName += it->getAsString();
		if(it != --m_container.end())
			ArrayName += _T(", ");
	}
	return ArrayName += _T("]");
}

CREF(RDOValue) RDOArrayValue::operator[] (CREF(RDOValue) ind)
{
	return m_container[ind.getInt()];
}

// ----------------------------------------------------------------------------
// ---------- RDOArrayIterator
// ----------------------------------------------------------------------------
RDOArrayIterator::RDOArrayIterator(CREF(RDOArrayIterator) iterator)
	: m_iterator(iterator.m_iterator)
{}

RDOArrayIterator::RDOArrayIterator(CREF(Iterator) iterator)
	: m_iterator(iterator)
{}

RDOArrayIterator::Iterator RDOArrayIterator::getIterator() const
{
	return m_iterator;
}

RDOArrayIterator::Iterator RDOArrayIterator::operator+ (rsint num)
{
	return m_iterator + num;
}

RDOArrayIterator::Iterator RDOArrayIterator::operator- (rsint num)
{
	return m_iterator - num;
}

RDOValue RDOArrayIterator::getValue() const
{
	return *m_iterator;
}

rbool RDOArrayIterator::operator== (CREF(RDOArrayIterator) iterator) const
{
	return m_iterator == iterator.m_iterator;
}

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
RDOArrayType::RDOArrayType(PTR(RDORuntimeParent) parent, ArrayType pArrayType)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_array)
	, m_pArrayType    (pArrayType      )
{}

RDOArrayType::ArrayType RDOArrayType::getArrayType() const
{
	return m_pArrayType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
