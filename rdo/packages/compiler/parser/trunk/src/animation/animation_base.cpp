/*!
  \copyright (c) RDO-Team, 2012
  \file      animation_base.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     ������� ����� �������� �������� �������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_base.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOFRMCommandList::RDOFRMCommandList()
{
	RDOParser::s_parser()->insertFRMCommandList(this);
}

LPExpression RDOFRMCommandList::generateExpression(CREF(rdo::runtime::LPRDOCalc) pCalc, CREF(RDOParserSrcInfo) srcInfo)
{
	ASSERT(pCalc);

	LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__void>(srcInfo);
	ASSERT(pType);

	LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, srcInfo);
	ASSERT(pExpression);

	return pExpression;
}

CLOSE_RDO_PARSER_NAMESPACE
