/*!
  \copyright (c) RDO-Team, 2011
  \file      local_variable.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

#ifndef _LOCAL_VARIABLE_H_
#define _LOCAL_VARIABLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/expression.h"
#include "simulator/compiler/parser/type/info.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- LocalVariable
// --------------------------------------------------------------------------------
OBJECT(LocalVariable)
{
DECLARE_FACTORY(LocalVariable);
public:
	CREF(LPRDOValue)   getValue     () const;
	CREF(LPExpression) getExpression() const;
	CREF(LPTypeInfo)   getTypeInfo  () const;

private:
	LocalVariable(CREF(LPRDOValue) pValue, CREF(LPExpression) pExpression, CREF(LPTypeInfo) pType);

	LPRDOValue    m_pValue;
	LPExpression  m_pExpression;
	LPTypeInfo    m_pType;
};

// --------------------------------------------------------------------------------
// -------------------- LocalVariableList
// --------------------------------------------------------------------------------
OBJECT(LocalVariableList)
{
DECLARE_FACTORY(LocalVariableList);
public:
	typedef std::list<LPLocalVariable> VariableList;

	void            append           (CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(tstring)         paramName) const;

private:
	LocalVariableList();

	VariableList m_variableList;
};

// --------------------------------------------------------------------------------
// -------------------- LocalVariableListStack
// --------------------------------------------------------------------------------
OBJECT(LocalVariableListStack)
{
	DECLARE_FACTORY(LocalVariableListStack);
public:
	typedef std::list<LPLocalVariableList> VariableListStack;

	void push(CREF(LPLocalVariableList) pVariableList);
	void pop ();

	void            append           (CREF(LPLocalVariable) pVariable);
	LPLocalVariable findLocalVariable(CREF(tstring)         paramName) const;

private:
	LocalVariableListStack();

	VariableListStack m_pVariableListStack;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _LOCAL_VARIABLE_H_
