/*!
  \copyright (c) RDO-Team, 2011
  \file      expression.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      07.03.2011
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_EXPRESSION_H_
#define _RDOPARSER_EXPRESSION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/type/info.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/runtime/rdocalc.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Expression
// --------------------------------------------------------------------------------
class ExpressionStatement;
OBJECT(Expression)
{
DECLARE_FACTORY(Expression);
friend class ExpressionStatement;
public:
	CREF(LPTypeInfo)             typeInfo  () const;
	CREF(rdoRuntime::LPRDOCalc)  calc      () const;
	CREF(rdoRuntime::RDOSrcInfo) src_info  () const;
	void                         setSrcInfo(CREF(rdoRuntime::RDOSrcInfo) src_info);

	LPRDOValue                   constant  () const;

private:
	Expression(CREF(LPTypeInfo) pType, CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(rdoRuntime::RDOSrcInfo) src_info);
	Expression(CREF(LPRDOValue) pValue);
	Expression(CREF(LPExpression) pExpression);
	virtual ~Expression();

	LPTypeInfo             m_pType;
	LPRDOValue             m_pValue; //! < ������������ ��� m_pValue ��� m_pCalc
	rdoRuntime::LPRDOCalc  m_pCalc;  //! < ����� �������� �������: ������� ������������ ����
};

// --------------------------------------------------------------------------------
// -------------------- ExpressionStatement
// --------------------------------------------------------------------------------
CLASS(ExpressionStatement): INSTANCE_OF(Expression)
{
DECLARE_FACTORY(ExpressionStatement);
public:
	rbool getReturn();

private:
	ExpressionStatement(CREF(LPTypeInfo) pType, CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(rdoRuntime::RDOSrcInfo) src_info);
	ExpressionStatement(CREF(LPExpression) pExpression);
	virtual ~ExpressionStatement();

	rbool m_returnFlag;
};
DECLARE_POINTER(ExpressionStatement);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_EXPRESSION_H_
