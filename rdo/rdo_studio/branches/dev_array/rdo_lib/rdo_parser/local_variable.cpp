/*
 * copyright: (c) RDO-Team, 2010
 * filename : local_variable.cpp
 * author   : ������ ������
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_parser/rdoparser_error.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

LocalVariable::LocalVariable(CREF(RDOValue) value, CREF(LPRDOFUNArithm) pArithm)
	: m_value  (value  )
	, m_pArithm(pArithm)
{}

CREF(RDOValue) LocalVariable::getValue() const
{
	return m_value;
}

LocalVariableList::LocalVariableList()
{}

void LocalVariableList::append(CREF(LPLocalVariable) pVariable)
{
	ASSERT(pVariable);

	STL_FOR_ALL_CONST(VariableList, m_variableList, it)
	{
		if ((*it)->getValue()->getIdentificator() == pVariable->getValue()->getIdentificator())
		{
			rdoParse::g_error().push_only(pVariable->getValue().src_info(), rdo::format(_T("���������� %s ��� ���������"), pVariable->getValue()->getIdentificator().c_str()));
			rdoParse::g_error().push_only((*it)->getValue().src_info(),     _T("��. ������ ��������"));
			rdoParse::g_error().push_done();
		}
	}

	m_variableList.push_back(pVariable);
}

CLOSE_RDO_PARSER_NAMESPACE
