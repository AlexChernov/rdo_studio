/*!
  \copyright (c) RDO-Team, 2010
  \file      rdo_memory.cpp
  \author    ������ ������
  \date      02.12.2010
  \brief     ������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "rdo_lib/rdo_runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_memory.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
// --------------------------------------------------------------------------------
// -------------------- RDOMemory
// --------------------------------------------------------------------------------
RDOMemory::RDOMemory()
{}

void RDOMemory::createVariable(CREF(tstring) name)
{
	std::pair<LocalMemory::iterator, rbool> result =
		m_localMemory.insert(LocalMemory::value_type(name, RDOValue()));

	ASSERT(result.second);
}

RDOValue RDOMemory::getVariable(CREF(tstring) name) const
{
	LocalMemory::const_iterator it = m_localMemory.find(name);
	ASSERT(it != m_localMemory.end());
	return it->second;
}

void RDOMemory::setVariable(CREF(tstring) name, CREF(RDOValue) variable)
{
	LocalMemory::iterator it = m_localMemory.find(name);
	ASSERT(it != m_localMemory.end());
	it->second = variable;
}

rbool RDOMemory::findVariable(CREF(tstring) name) const
{
	return m_localMemory.find(name) != m_localMemory.end();
}

// --------------------------------------------------------------------------------
// -------------------- RDOMemoryStack
// --------------------------------------------------------------------------------
RDOMemoryStack::RDOMemoryStack()
{}

void RDOMemoryStack::push(LPRDOMemory pMemory)
{
	m_pMemoryStack.push_back(pMemory);
}

void RDOMemoryStack::pop()
{
	ASSERT(!m_pMemoryStack.empty());

	m_pMemoryStack.pop_back();
}

void RDOMemoryStack::create(CREF(tstring) name)
{
	ASSERT(!m_pMemoryStack.empty());

	m_pMemoryStack.back()->createVariable(name);
}

RDOValue RDOMemoryStack::get(CREF(tstring) name) const
{
	ASSERT(!m_pMemoryStack.empty());

	MemoryStack::const_reverse_iterator stack_it = m_pMemoryStack.rbegin();
	while (stack_it != m_pMemoryStack.rend())
	{
		if((*stack_it)->findVariable(name)) break;
		else ++stack_it;
	};

	ASSERT(stack_it != m_pMemoryStack.rend());
	return (*stack_it)->getVariable(name);
}

void RDOMemoryStack::set(CREF(tstring) name, CREF(RDOValue) Variable)
{
	ASSERT(!m_pMemoryStack.empty());

	MemoryStack::reverse_iterator stack_it = m_pMemoryStack.rbegin();
	while (stack_it != m_pMemoryStack.rend())
	{
		if((*stack_it)->findVariable(name)) break;
		else ++stack_it;
	};

	ASSERT(stack_it != m_pMemoryStack.rend());
	(*stack_it)->setVariable(name, Variable);
}

CLOSE_RDO_RUNTIME_NAMESPACE
