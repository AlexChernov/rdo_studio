/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_rdo.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_rdo.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdosmr.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdoopr.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_common/rdostream.h"
#include "rdo_kernel/rdokernel.h"
#include "rdo_repository/rdorepository.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(PTR(RDOParser) parser, rdoModelObjects::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun, StreamFrom from)
	: RDOParserItem(parser, type, parser_fun, error_fun, lexer_fun, from)
	, m_lexer(NULL)
{}

RDOParserRDOItem::~RDOParserRDOItem()
{
	if (m_lexer)
	{
		delete m_lexer;
		m_lexer = NULL;
	}
}

void RDOParserRDOItem::parse()
{
	rdo::binarystream in_stream;
	switch (m_from)
	{
		case sf_repository:
			kernel->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &rdoRepository::RDOThreadRepository::FileData(m_type, in_stream));
			break;
		case sf_editor:
			kernel->sendMessage(kernel->studio(), RDOThread::RT_STUDIO_MODEL_GET_TEXT, &rdoRepository::RDOThreadRepository::FileData(m_type, in_stream));
			break;
	}
	if (in_stream.good())
	{
		parse(in_stream);
	}
}

void RDOParserRDOItem::parse(REF(std::istream) in_stream)
{
	if (m_lexer)
		delete m_lexer;

	std::ostringstream out_stream;
	m_lexer = getLexer(&in_stream, &out_stream);

	if (m_lexer && m_parser_fun)
		m_parser_fun(m_lexer);
}

PTR(RDOLexer) RDOParserRDOItem::getLexer(PTR(std::istream) in_stream, PTR(std::ostream) out_stream)
{
	return new RDOLexer(m_parser, in_stream, out_stream);
}

ruint RDOParserRDOItem::lexer_loc_line()
{
	if (m_lexer)
	{
		return m_lexer->m_lploc ? m_lexer->m_lploc->first_line : m_lexer->lineno();
	}
	else
	{
		return rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE;
	}
}

ruint RDOParserRDOItem::lexer_loc_pos()
{
	return m_lexer && m_lexer->m_lploc ? m_lexer->m_lploc->first_column : 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS(PTR(RDOParser) parser, StreamFrom from)
	: RDOParserRDOItem(parser, rdoModelObjects::RSS, rssparse, rsserror, rsslex, from)
{}

void RDOParserRSS::parse()
{
	m_parser->setHaveKWResources   (false);
	m_parser->setHaveKWResourcesEnd(false);
	RDOParserRDOItem::parse();
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
void RDOParserRSSPost::parse()
{
	// � ������ ������������� �� ������ ��� ������� ������� �� ������� �� �����, � �� �� ������� ����� �������� �� RSS
#ifdef RDOSIM_COMPATIBLE
	STL_FOR_ALL_CONST(RDOParser::RTPResTypeList, m_parser->getRTPResType(), rtp_it)
	{
#endif
		STL_FOR_ALL_CONST(RDOParser::RSSResourceList, m_parser->getRSSResources(), rss_it)
		{
#ifdef RDOSIM_COMPATIBLE
			if ((*rss_it)->getType() == *rtp_it)
			{
#endif
				rdoRuntime::LPRDOCalc calc = (*rss_it)->createCalc();
				m_parser->runtime()->addInitCalc(calc);
#ifdef RDOSIM_COMPATIBLE
			}
#endif
		}
#ifdef RDOSIM_COMPATIBLE
	}
#endif
}

// ----------------------------------------------------------------------------
// ---------- RDOParserPATPost
// ----------------------------------------------------------------------------
void RDOParserPATPost::parse()
{
	//! ������� ���������� ��� ������������ �������
	STL_FOR_ALL_CONST(RDOParser::EventList, m_parser->getEvents(), eventIt)
	{
		LPRDOEvent pEvent = *eventIt;

		CPTR(RDOPATPattern) pPattern = m_parser->findPATPattern(pEvent->name());
		if (!pPattern)
		{
			STL_FOR_ALL_CONST(RDOEvent::CalcList, pEvent->getCalcList(), calcIt)
			{
				m_parser->error().push_only((*calcIt)->src_info(), rdo::format(_T("������� ������������� ����������� �������: %s"), pEvent->name().c_str()));
			}
			m_parser->error().push_done();
		}
		if (pPattern->getType() != RDOPATPattern::PT_Event)
		{
			STL_FOR_ALL_CONST(RDOEvent::CalcList, pEvent->getCalcList(), calcIt)
			{
				m_parser->error().push_only((*calcIt)->src_info(), rdo::format(_T("������� %s �� �������� ��������: %s"), pEvent->name().c_str()));
			}
			m_parser->error().push_done();
		}

		LPIBaseOperation pRuntimeEvent = static_cast<PTR(rdoRuntime::RDOPatternEvent)>(pPattern->getPatRuntime())->createActivity(m_parser->runtime()->m_metaLogic, m_parser->runtime(), pEvent->name());
		ASSERT(pRuntimeEvent);
		pEvent->setRuntimeEvent(pRuntimeEvent);

		STL_FOR_ALL(RDOEvent::CalcList, pEvent->getCalcList(), calcIt)
		{
			(*calcIt)->setEvent(pRuntimeEvent);
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
void RDOParserSTDFUN::parse()
{
	LPRDOTypeParam intType  = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create().object_cast<RDOType>(),  RDOValue(), RDOParserSrcInfo());
	LPRDOTypeParam realType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create().object_cast<RDOType>(), RDOValue(), RDOParserSrcInfo());

	PTR(RDOFUNFunction)      fun   = new RDOFUNFunction     (m_parser, _T("Abs"), realType);
	PTR(RDOFUNFunctionParam) param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcAbs>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("ArcCos"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcCos>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("ArcSin"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcSin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("ArcTan"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcTan>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Cos"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCos>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Cotan"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCotan>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Exp"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcExp>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Floor"), intType );
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFloor>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Frac"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFrac>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("IAbs"), intType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIAbs>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("IMax"), intType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMax>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("IMin"), intType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Int"), intType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcInt>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("IntPower"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),       realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),       intType );
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIntPower>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Ln"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLn>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Log10"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog10>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Log2"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog2>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("LogN"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLogN>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Max"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMax>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Min"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Power"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcPower>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Round"), intType );
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcRound>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Sin"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Sqrt"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSqrt>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("Tan"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcTan>::create().object_cast<rdoRuntime::RDOFunCalc>());

	// -----------------------------------------
	// � ��� ��������� ����
	// -----------------------------------------
	fun   = new RDOFUNFunction     (m_parser, _T("abs"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcAbs>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("arccos"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcCos>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("arcsin"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcSin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("arctan"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcTan>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("cos"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCos>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("cotan"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCotan>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("exp"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcExp>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("floor"), intType );
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFloor>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("frac"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFrac>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("iabs"), intType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIAbs>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("imax"), intType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMax>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("imin"), intType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   intType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("int"), intType );
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcInt>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("intpower"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),       realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),       intType );
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIntPower>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("ln"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLn>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("log10"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog10>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("log2"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog2>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("logn"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLogN>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("max"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMax>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("min"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("power"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	param = new RDOFUNFunctionParam(fun,      _T("p2"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcPower>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("round"), intType );
	param = new RDOFUNFunctionParam(fun,      _T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcRound>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("sin"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSin>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("sqrt"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSqrt>::create().object_cast<rdoRuntime::RDOFunCalc>());

	fun   = new RDOFUNFunction     (m_parser, _T("tan"), realType);
	param = new RDOFUNFunctionParam(fun,      _T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcTan>::create().object_cast<rdoRuntime::RDOFunCalc>());
}

CLOSE_RDO_PARSER_NAMESPACE
