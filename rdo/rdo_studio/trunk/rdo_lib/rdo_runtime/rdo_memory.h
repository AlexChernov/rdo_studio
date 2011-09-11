/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_memory.h
  \author    ������ ������
  \date      02.12.2010
  \brief     ������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_MEMORY_H_
#define _LIB_RUNTIME_MEMORY_H_

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDOMemory
  \brief     ������
*/
OBJECT(RDOMemory)
{
DECLARE_FACTORY(RDOMemory);
public:
	typedef std::map<tstring, RDOValue> LocalMemory;

	void     createVariable(CREF(tstring) name);
	RDOValue getVariable   (CREF(tstring) name) const;
	void     setVariable   (CREF(tstring) name, CREF(RDOValue) Variable);
	rbool    findVariable  (CREF(tstring) name) const;

private:
	RDOMemory();

	LocalMemory m_localMemory;
};

/*!
  \class     RDOMemoryStack
  \brief     ���� ������
*/
OBJECT(RDOMemoryStack)
{
DECLARE_FACTORY(RDOMemoryStack);
public:
	typedef std::list<LPRDOMemory> MemoryStack;

	void     push  (LPRDOMemory pMemory);
	void     pop   ();

	void     create(CREF(tstring) name);
	RDOValue get   (CREF(tstring) name) const;
	void     set   (CREF(tstring) name, CREF(RDOValue) variable);

private:
	RDOMemoryStack();

	MemoryStack m_pMemoryStack;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_MEMORY_H_
