/*
 * copyright: (c) RDO-Team, 2010
 * filename : stack.cpp
 * author   : ������ ������
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/stack.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

ContextStack::ContextStack()
{}

ContextStack::~ContextStack()
{
	while (!m_container.empty())
		pop();
}

void ContextStack::push(CREF(LPContext) pContext)
{
	m_container.push_back(pContext);
}

void ContextStack::pop()
{
	m_container.pop_back();
}

LPContext ContextStack::top() const
{
	ASSERT(!m_container.empty());
	return m_container.back();
}

LPContext ContextStack::global() const
{
	ASSERT(!m_container.empty());
	return m_container.front();
}

CLOSE_RDO_PARSER_NAMESPACE
