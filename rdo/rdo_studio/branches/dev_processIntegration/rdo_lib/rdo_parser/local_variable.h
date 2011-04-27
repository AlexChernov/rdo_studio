/*
 * copyright: (c) RDO-Team, 2010
 * filename : local_variable.h
 * author   : ������ ������
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _LOCAL_VARIABLE_H_
#define _LOCAL_VARIABLE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/expression.h"
#include "rdo_lib/rdo_parser/type/type_param.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- LocalVariable
// ----------------------------------------------------------------------------
OBJECT(LocalVariable)
{
DECLARE_FACTORY(LocalVariable);
public:
	CREF(RDOValue) getValue     () const;
	LPExpression   getExpression() const;
	LPRDOTypeParam getParam     () const;

private:
	LocalVariable(CREF(RDOValue) value, CREF(LPExpression) pExpression, CREF(LPRDOTypeParam) pParam);

	RDOValue       m_value;
	LPExpression   m_pExpression;
	LPRDOTypeParam m_pParam;
};

// ----------------------------------------------------------------------------
// ---------- LocalVariableList
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- LocalVariableListStack
// ----------------------------------------------------------------------------
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

#endif //! _LOCAL_VARIABLE_H_
