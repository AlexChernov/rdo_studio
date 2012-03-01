/*!
  \copyright (c) RDO-Team, 2011
  \file      local_variable.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      16.10.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/local_variable.h"
#include "simulator/compiler/parser/rdoparser_error.h"
#include "simulator/runtime/rdo_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- LocalVariable
// --------------------------------------------------------------------------------
LocalVariable::LocalVariable(CREF(LPRDOValue) pValue, CREF(LPExpression) pExpression, CREF(LPTypeInfo) pType)
	: m_pValue     (pValue     )
	, m_pExpression(pExpression)
	, m_pType      (pType      )
{
	ASSERT(m_pValue);
	ASSERT(m_pExpression);
	ASSERT(m_pType);

	m_pDefaultValue = m_pType->type()->get_default();

	if(m_pDefaultValue.type()->typeID() == rdoRuntime::RDOType::t_unknow)
	{
		rdoParser::g_error().error(pValue->src_info(), _T("� ������� ���� ��� �������� �����������."));
	}
}

CREF(LPRDOValue) LocalVariable::getValue() const
{
	return m_pValue;
}

CREF(LPExpression) LocalVariable::getExpression() const
{
	return m_pExpression;
}

CREF(LPTypeInfo) LocalVariable::getTypeInfo() const
{
	return m_pType;
}

CREF(rdoRuntime::RDOValue) LocalVariable::getDefaultValue () const
{
	return m_pDefaultValue;
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableList
// --------------------------------------------------------------------------------
LocalVariableList::LocalVariableList()
{}

void LocalVariableList::append(CREF(LPLocalVariable) pVariable)
{
	ASSERT(pVariable);

	STL_FOR_ALL_CONST(m_variableList, it)
	{
		if ((*it)->getValue()->value().getIdentificator() == pVariable->getValue()->value().getIdentificator())
		{
			rdoParser::g_error().push_only(pVariable->getValue()->src_info(), rdo::format(_T("���������� %s ��� ���������"), pVariable->getValue()->value().getIdentificator().c_str()));
			rdoParser::g_error().push_only((*it)->getValue()->src_info(),     _T("��. ������ ��������"));
			rdoParser::g_error().push_done();
		}
	}

	m_variableList.push_back(pVariable);
}

LPLocalVariable LocalVariableList::findLocalVariable(CREF(tstring) name) const
{
	STL_FOR_ALL_CONST(m_variableList, it)
	{
		if ((*it)->getValue()->value().getIdentificator() == name)
		{
			return *it;
		}
	}
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- LocalVariableListStack
// --------------------------------------------------------------------------------
LocalVariableListStack::LocalVariableListStack()
{}

void LocalVariableListStack::push(CREF(LPLocalVariableList) pVariableList)
{
	m_pVariableListStack.push_back(pVariableList);
}

void LocalVariableListStack::pop()
{
	ASSERT(!m_pVariableListStack.empty());

	m_pVariableListStack.pop_back();
}

void LocalVariableListStack::append(CREF(LPLocalVariable) pVariable)
{
	ASSERT(!m_pVariableListStack.empty());

	m_pVariableListStack.back()->append(pVariable);
}

LPLocalVariable LocalVariableListStack::findLocalVariable(CREF(tstring) name) const
{
	STL_FOR_ALL_CONST(m_pVariableListStack, it)
	{
		if ((*it)->findLocalVariable(name))
		{
			return (*it)->findLocalVariable(name);
		}
	}
	return NULL;
}

CLOSE_RDO_PARSER_NAMESPACE
