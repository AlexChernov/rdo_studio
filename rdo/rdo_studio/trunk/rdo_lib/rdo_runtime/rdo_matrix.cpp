/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_matrix.cpp
 * author   : ������ ������
 * date     : 01.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_matrix.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOMatrixValue
// ----------------------------------------------------------------------------
RDOMatrixValue::RDOMatrixValue(CREF(RDOMatrixType) type)
	: m_matrixType(&type)
{}

RDOMatrixValue::RDOMatrixValue(CREF(RDOMatrixValue) value)
	: m_container (value.m_container)
	, m_matrixType (value.m_matrixType)
{}

RDOMatrixValue::~RDOMatrixValue()
{}

CREF(RDOMatrixType) RDOMatrixValue::type() const
{
	ASSERT(m_matrixType);
	return *m_matrixType;
}

void RDOMatrixValue::insertItem(CREF(RDOValue) pMatrix)
{
	m_container.push_back(pMatrix);
}

RDOMatrixValue::Container::iterator RDOMatrixValue::containerBegin()
{
	return m_container.begin();
}

RDOMatrixValue::Container::iterator RDOMatrixValue::containerEnd()
{
	return m_container.end();
}

void RDOMatrixValue::insertItems(Container::iterator itr, Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.insert(itr,itrFst,itrLst);
}

void RDOMatrixValue::eraseItems(Container::iterator itrFst, Container::iterator itrLst)
{
	m_container.erase(itrFst,itrLst);
}

tstring RDOMatrixValue::getAsString() const
{
	tstring MatrixName = _T("[");
	STL_FOR_ALL_CONST(Container, m_container, it)
	{
		MatrixName += it->getAsString();
		if(it != --m_container.end())
			MatrixName += _T(", ");
	}
	return MatrixName += _T("]");
}

CREF(RDOValue) RDOMatrixValue::operator[] (CREF(RDOValue) ind)
{
	return m_container[ind.getInt()];
}

// ----------------------------------------------------------------------------
// ---------- RDOMatrixIterator
// ----------------------------------------------------------------------------
RDOMatrixIterator::RDOMatrixIterator(CREF(RDOMatrixIterator) iterator)
	: m_iterator(iterator.m_iterator)
{}

RDOMatrixIterator::RDOMatrixIterator(CREF(Iterator) iterator)
	: m_iterator(iterator)
{}

RDOMatrixIterator::Iterator RDOMatrixIterator::getIterator() const
{
	return m_iterator;
}

RDOMatrixIterator::Iterator RDOMatrixIterator::operator+ (rsint num)
{
	return m_iterator + num;
}

RDOMatrixIterator::Iterator RDOMatrixIterator::operator- (rsint num)
{
	return m_iterator - num;
}

RDOValue RDOMatrixIterator::getValue() const
{
	return *m_iterator;
}

rbool RDOMatrixIterator::operator== (CREF(RDOMatrixIterator) iterator) const
{
	return m_iterator == iterator.m_iterator;
}

// ----------------------------------------------------------------------------
// ---------- RDOMatrixType
// ----------------------------------------------------------------------------
RDOMatrixType::RDOMatrixType(PTR(RDORuntimeParent) parent, MatrixType pMatrixType)
	: RDORuntimeObject(parent          )
	, RDOType         (RDOType::t_matrix)
	, m_pMatrixType    (pMatrixType      )
{}

RDOMatrixType::MatrixType RDOMatrixType::getMatrixType() const
{
	return m_pMatrixType;
}

CLOSE_RDO_RUNTIME_NAMESPACE
