/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoparser_rdo.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"

#include "kernel/rdokernel.h"

#include "repository/rdorepository.h"

#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity_i.h"

#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/rdopat.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserRDOItem
// --------------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(rdoModelObjects::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun, StreamFrom from)
	: RDOParserItem(type, parser_fun, error_fun, lexer_fun, from)
	, m_pLexer(NULL)
{}

RDOParserRDOItem::~RDOParserRDOItem()
{
	if (m_pLexer)
	{
		delete m_pLexer;
		m_pLexer = NULL;
	}
}

void RDOParserRDOItem::parse(CREF(LPRDOParser) pParser)
{
	ASSERT(pParser);

	rdo::binarystream in_stream;
	switch (m_from)
	{
	case sf_repository:
		{
			rdo::repository::RDOThreadRepository::FileData fileData(m_type, in_stream);
			kernel->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
			break;
		}
	case sf_editor:
		{
			rdo::repository::RDOThreadRepository::FileData fileData(m_type, in_stream);
			kernel->sendMessage(kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &fileData);
			break;
		}
	}
	if (in_stream.good())
	{
		parse(pParser, in_stream);
	}
}

void RDOParserRDOItem::parse(CREF(LPRDOParser) pParser, REF(std::istream) in_stream)
{
	ASSERT(pParser  );
	ASSERT(!m_pLexer);

	std::ostringstream out_stream;
	m_pLexer = getLexer(pParser, &in_stream, &out_stream);

	if (m_pLexer)
	{
		if (m_parser_fun)
		{
			m_parser_fun(m_pLexer);
		}
		delete m_pLexer;
		m_pLexer = NULL;
	}
}

PTR(RDOLexer) RDOParserRDOItem::getLexer(CREF(LPRDOParser) pParser, PTR(std::istream) in_stream, PTR(std::ostream) out_stream)
{
	ASSERT(pParser);
	return new RDOLexer(pParser, in_stream, out_stream);
}

ruint RDOParserRDOItem::lexer_loc_line()
{
	if (m_pLexer)
	{
		return m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_line : m_pLexer->lineno();
	}
	else
	{
		return ruint(rdo::runtime::RDOSrcInfo::Position::UNDEFINE_LINE);
	}
}

ruint RDOParserRDOItem::lexer_loc_pos()
{
	return m_pLexer && m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_pos : 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSS
// --------------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS(StreamFrom from)
	: RDOParserRDOItem(rdoModelObjects::RSS, rssparse, rsserror, rsslex, from)
{}

void RDOParserRSS::parse(CREF(LPRDOParser) pParser)
{
	ASSERT(pParser);
	pParser->setHaveKWResources   (false);
	pParser->setHaveKWResourcesEnd(false);
	RDOParserRDOItem::parse(pParser);
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserRSSPost
// --------------------------------------------------------------------------------
void RDOParserRSSPost::parse(CREF(LPRDOParser) pParser)
{
	ASSERT(pParser);

	//! � ������ ������������� �� ������ ��� ������� ������� �� ������� �� �����, � �� �� ������� ����� �������� �� RSS
#ifdef RDOSIM_COMPATIBLE
	STL_FOR_ALL_CONST(pParser->getRTPResType(), rtp_it)
	{
#endif
		STL_FOR_ALL_CONST(pParser->getRSSResources(), rss_it)
		{
#ifdef RDOSIM_COMPATIBLE
			if ((*rss_it)->getType() == *rtp_it)
			{
#endif
				rdo::runtime::LPRDOCalc calc = (*rss_it)->createCalc();
				pParser->runtime()->addInitCalc(calc);
#ifdef RDOSIM_COMPATIBLE
			}
#endif
		}
#ifdef RDOSIM_COMPATIBLE
	}
#endif
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserSMRPost
// --------------------------------------------------------------------------------
RDOParserSMRPost::RDOParserSMRPost()
	: RDOParserItem(rdoModelObjects::SMR, NULL, NULL, NULL)
{}

void RDOParserSMRPost::parse(CREF(LPRDOParser) pParser)
{
	ASSERT(pParser);

	//! ������������ �������, ��������� � SMR
	STL_FOR_ALL_CONST(pParser->getEvents(), eventIt)
	{
		LPRDOEvent pEvent = *eventIt;
		ASSERT(pEvent);

		rdo::runtime::LPRDOCalc pInitCalc = pEvent->getInitCalc();
		if (pInitCalc)
		{
			pParser->runtime()->addInitCalc(pInitCalc);
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserEVNPost
// --------------------------------------------------------------------------------
void RDOParserEVNPost::parse(CREF(LPRDOParser) pParser)
{
	ASSERT(pParser);

	//! ������� ���������� ��� ������������ �������
	STL_FOR_ALL_CONST(pParser->getEvents(), eventIt)
	{
		LPRDOEvent pEvent = *eventIt;

		LPRDOPATPattern pPattern = pParser->findPATPattern(pEvent->name());
		if (!pPattern)
		{
			STL_FOR_ALL_CONST(pEvent->getCalcList(), calcIt)
			{
				pParser->error().push_only((*calcIt)->srcInfo(), rdo::format(_T("������� ������������� ����������� �������: %s"), pEvent->name().c_str()));
			}
			pParser->error().push_done();
		}
		if (pPattern->getType() != RDOPATPattern::PT_Event)
		{
			STL_FOR_ALL_CONST(pEvent->getCalcList(), calcIt)
			{
				pParser->error().push_only((*calcIt)->srcInfo(), rdo::format(_T("������� %s �� �������� ��������: %s"), pEvent->name().c_str()));
			}
			pParser->error().push_done();
		}

		//! \todo ���������� �� ��������� getRegular()
		if (pEvent->getRegular())
		{
			LPIBaseOperation pRuntimeEvent = pPattern->getPatRuntime<rdo::runtime::RDOPatternEvent>()->createActivity(pParser->runtime()->m_pMetaLogic, pParser->runtime(), pEvent->name());
			ASSERT(pRuntimeEvent);
			pEvent->setRuntimeEvent(pRuntimeEvent);

			STL_FOR_ALL(pEvent->getCalcList(), calcIt)
			{
				(*calcIt)->setEvent(pRuntimeEvent);
			}

			LPIActivity pActivity = pRuntimeEvent;
			ASSERT(pActivity);
			STL_FOR_ALL_CONST(pEvent->getParamList()->getContainer(), paramIt)
			{
				LPRDOFUNArithm pParam = *paramIt;
				ASSERT(pParam);
				if (m_currParam < pPattern->m_paramList.size())
				{
					rdo::runtime::LPRDOCalc pSetParamCalc;
					LPRDOParam pPatternParam = pPattern->m_paramList[m_currParam];
					ASSERT(pPatternParam);
					if (pParam->typeInfo()->src_info().src_text() == _T("*"))
					{
						if (!pPatternParam->getDefault()->defined())
						{
							RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format(_T("��� �������� ��-��������� ��� ��������� '%s'"), pPatternParam->src_text().c_str()));
							RDOParser::s_parser()->error().push_only(pPatternParam->src_info(), rdo::format(_T("��. �������� '%s', ��� '%s'"), pPatternParam->src_text().c_str(), pPatternParam->getTypeInfo()->src_info().src_text().c_str()));
							RDOParser::s_parser()->error().push_done();
						}
						rdo::runtime::RDOValue val = pPatternParam->getDefault()->value();
						ASSERT(val);
						pSetParamCalc = rdo::Factory<rdo::runtime::RDOSetPatternParamCalc>::create(
							m_currParam,
							rdo::Factory<rdo::runtime::RDOCalcConst>::create(val)
						);
					}
					else
					{
						LPTypeInfo pTypeInfo = pPatternParam->getTypeInfo();
						ASSERT(pTypeInfo);
						rdo::runtime::LPRDOCalc pParamValueCalc = pParam->createCalc(pTypeInfo);
						ASSERT(pParamValueCalc);
						pSetParamCalc = rdo::Factory<rdo::runtime::RDOSetPatternParamCalc>::create(
							m_currParam,
							pParamValueCalc
						);
					}
					ASSERT(pSetParamCalc);
					pActivity->addParamCalc(pSetParamCalc);
					++m_currParam;
				}
				else
				{
					RDOParser::s_parser()->error().push_only(pParam->src_info(), rdo::format(_T("������� ����� ���������� ��� ������� '%s' ��� ������������ ������� '%s'"), pEvent->name().c_str(), pEvent->name().c_str()));
					RDOParser::s_parser()->error().push_done();
				}
			}
			m_currParam = 0;
		}
		else
		{
			NEVER_REACH_HERE; //������ ��� ������������ ������� ������ ���
		}
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOParserRTPPost
// --------------------------------------------------------------------------------
void RDOParserRTPPost::parse(CREF(LPRDOParser) pParser)
{
	ASSERT(pParser);

	STL_FOR_ALL_CONST(pParser->getRTPResTypes(), RTPResTypeIt)
	{
		// ����� ��������� ��� ������� � �������
		LPRDORTPResType pResType = *RTPResTypeIt;

		// ������� ��������������� ��� �������� � ��������
		pResType->end();
	}
}

CLOSE_RDO_PARSER_NAMESPACE
