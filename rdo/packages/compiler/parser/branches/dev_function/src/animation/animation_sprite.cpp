/*!
  \copyright (c) RDO-Team, 2012
  \file      animation_sprite.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      14.12.2012
  \brief     ������ �������� � �������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/src/animation/animation_sprite.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOFRMSprite::RDOFRMSprite(CREF(RDOParserSrcInfo) src_info)
	: RDOFRMCommandList()
{
	m_pFunction = rdo::Factory<Function>::create(
		rdo::Factory<TypeInfo>::delegate<RDOType__void>(src_info),
		src_info
	);
	ASSERT(m_pFunction);

	m_pSprite = rdo::Factory<rdo::runtime::RDOFRMSprite>::create(m_pFunction->src_info());
	ASSERT(m_pSprite)

	RDOParser::s_parser()->insertFRMSprite(this);

	m_pFunction->pushContext();
}

RDOFRMSprite::~RDOFRMSprite()
{}

CREF(tstring) RDOFRMSprite::name() const
{
	return m_pFunction->src_text();
}

rdo::runtime::LPRDOFRMSprite RDOFRMSprite::list() const
{
	return m_pSprite;
}

LPExpression RDOFRMSprite::expression() const
{
	return m_pFunction->expression();
}

void RDOFRMSprite::end()
{
	m_pFunction->popContext();
	RDOFRMCommandList::end();
	m_pSprite->setSpriteCalc(expression()->calc());
}

CLOSE_RDO_PARSER_NAMESPACE
