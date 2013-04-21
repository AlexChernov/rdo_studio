/*!
  \copyright (c) RDO-Team, 2012
  \file      simulator/compiler/parser/rdo_logic_base.cpp
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Клеванец Игорь (impus@hotbox.ru)
  \date      5.02.2012
  \brief     Базовый класс для точек принятия решений
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdo_logic_base.h"
#include "simulator/runtime/rdo_priority.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOLogicBase
// --------------------------------------------------------------------------------
RDOLogicBase::RDOLogicBase(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
	, m_multithreading(false   )
{}

RDOLogicBase::~RDOLogicBase()
{}

CREF(tstring) RDOLogicBase::name() const
{
	return src_info().src_text();
}

rbool RDOLogicBase::setPrior(REF(LPRDOFUNArithm) pPrior)
{
	LPIPriority pPriority = m_pRuntimeLogic;
	if (pPriority)
	{
		return pPriority->setPrior(pPrior->createCalc());
	}
	return false;
}

rbool RDOLogicBase::getMultithreading() const
{
	return m_multithreading;
}

void RDOLogicBase::setMultithreading(rbool multithreading)
{
	m_multithreading = multithreading;
}

void RDOLogicBase::setCondition(CREF(LPRDOFUNLogic) pConditon)
{
	m_pConditon = pConditon;
}

LPRDOFUNLogic RDOLogicBase::getConditon() const
{
	return m_pConditon;
}

LPILogic RDOLogicBase::getLogic() const
{
	return m_pRuntimeLogic;
}

void RDOLogicBase::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
	m_pRuntimeLogic->setMultithreading(m_multithreading);
	RDOParser::s_parser()->contextStack()->pop<RDOLogicBase>();
}

CLOSE_RDO_PARSER_NAMESPACE
