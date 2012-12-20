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
#include "simulator/compiler/parser/src/animation/animation_frame.h"
#include "simulator/compiler/parser/rdoparser.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

RDOFRMSprite::RDOFRMSprite(CREF(RDOParserSrcInfo) src_info)
	: RDOFRMCommandList()
	, Function(rdo::Factory<TypeInfo>::delegate<RDOType__void>(src_info), src_info)
{
	m_pSprite = rdo::Factory<rdo::runtime::RDOFRMSprite>::create(this->src_info());
	ASSERT(m_pSprite)
	RDOParser::s_parser()->insertFRMSprite(this);

	setCall(m_pSprite);

	pushContext();
}

CREF(tstring) RDOFRMSprite::name() const
{
	return src_info().src_text();
}

rdo::runtime::LPRDOFRMSprite RDOFRMSprite::list() const
{
	return m_pSprite;
}

void RDOFRMSprite::end()
{
	popContext();
}

Context::FindResult RDOFRMSprite::onFindContext(CREF(LPRDOValue) pValue) const
{
	return Function::onFindContext(pValue);
}

CLOSE_RDO_PARSER_NAMESPACE
