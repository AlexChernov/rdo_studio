/*!
  \copyright (c) RDO-Team, 2011
  \file      animation_frame.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      ����� �������� � �������
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int frmlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void frmerror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
RDOFRMFrame::RDOFRMFrame(CREF(RDOParserSrcInfo) srcInfo)
	: RDOFRMCommandList()
{
	m_pFrame = rdo::Factory<rdo::runtime::RDOFRMFrame>::create(srcInfo);
	ASSERT(m_pFrame)
	RDOParser::s_parser()->runtime()->addRuntimeFrame(m_pFrame);
	RDOParser::s_parser()->insertFRMFrame(this);
	RDOParser::s_parser()->contextStack()->push(this);

	m_pContextMemory = rdo::Factory<ContextMemory>::create();
	ASSERT(m_pContextMemory);
	RDOParser::s_parser()->contextStack()->push(m_pContextMemory);

	ContextMemory::push();
}

RDOFRMFrame::~RDOFRMFrame()
{}

CREF(tstring) RDOFRMFrame::name() const
{
	return src_info().src_text();
}

CREF(rdo::runtime::LPRDOFRMFrame) RDOFRMFrame::frame() const
{
	return m_pFrame;
}

rdo::runtime::LPRDOFRMSprite RDOFRMFrame::list() const
{
	return m_pFrame;
}

Context::FindResult RDOFRMFrame::onFindContext(CREF(LPRDOValue) pValue) const
{
	ASSERT(pValue);

	tstring name = pValue->value().getIdentificator();
	LPRDOFRMSprite pSprite = RDOParser::s_parser()->findFRMSprite(name);
	if (pSprite)
	{
		return Context::FindResult(const_cast<PTR(RDOFRMFrame)>(this), pSprite->expression(), pValue);
	}

	return Context::FindResult();
}

void RDOFRMFrame::end()
{
	ContextMemory::pop();
	RDOParser::s_parser()->contextStack()->pop<ContextMemory>();
	RDOParser::s_parser()->contextStack()->pop<RDOFRMFrame>();
}

CLOSE_RDO_PARSER_NAMESPACE
