/*!
  \copyright (c) RDO-Team, 2012
  \file      local_variable_context.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.12.2012
  \brief     �������� ������������� ������ ��������� ����������: integer i, j;
  \indent    4T
*/

#ifndef _SIMULATOR_COMPILER_PARSER_FUNCTION_LOCAL_VARIABLE_CONTEXT_H_
#define _SIMULATOR_COMPILER_PARSER_FUNCTION_LOCAL_VARIABLE_CONTEXT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_locvar.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/local_variable.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- IContextLocalVariableManager
// --------------------------------------------------------------------------------
OBJECT_INTERFACE(IContextLocalVariableManager)
{
DECLARE_FACTORY(IContextLocalVariableManager)
public:
	virtual void pushLocalVariableContext() = 0;
	virtual void popLocalVariableContext () = 0;

protected:
	IContextLocalVariableManager()
	{}
	virtual ~IContextLocalVariableManager()
	{}
};

#define DECLARE_IContextLocalVariableManager \
	void pushLocalVariableContext();         \
	void popLocalVariableContext ();

// --------------------------------------------------------------------------------
// -------------------- ContextLocalVariable
// --------------------------------------------------------------------------------
class ContextLocalVariable: public Context
{
DECLARE_FACTORY(ContextLocalVariable)
public:
	void pushLocalVariable(const LPLocalVariable& pLocalVariable);
	CREF(rdo::runtime::LPRDOCalcLocalVariableList) calc() const;

private:
	typedef  boost::function<void (const LPLocalVariable&)>  OnPushLocalVariable;

	ContextLocalVariable(const OnPushLocalVariable& onPushLocalVariable);
	virtual ~ContextLocalVariable();

	OnPushLocalVariable                       m_onPushLocalVariable;
	rdo::runtime::LPRDOCalcLocalVariableList  m_pCalcLocalVariableList;
};
DECLARE_POINTER(ContextLocalVariable);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _SIMULATOR_COMPILER_PARSER_FUNCTION_LOCAL_VARIABLE_CONTEXT_H_
