/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_rdo.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdoparser_rdo.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdosmr.h"
#include "rdo_lib/rdo_converter/rdorss.h"
#include "rdo_lib/rdo_converter/rdortp.h"
#include "rdo_lib/rdo_converter/rdofun.h"
#include "rdo_lib/rdo_converter/rdosmr.h"
#include "rdo_lib/rdo_converter/rdopat.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_common/rdostream.h"
#include "rdo_kernel/rdokernel.h"
#include "rdo_repository/rdorepository.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(rdoModelObjectsConvertor::RDOFileType type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun)
	: RDOParserItem(type, parser_fun, error_fun, lexer_fun)
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

void RDOParserRDOItem::parse(PTR(Converter) pParser, REF(std::istream) streamIn)
{
	ASSERT(pParser);

	if (!streamIn.good())
		return;

	if (m_pLexer)
		delete m_pLexer;

	std::ostringstream streamOut;
	m_pLexer = getLexer(pParser, &streamIn, &streamOut);

	if (m_pLexer && m_parser_fun)
		m_parser_fun(m_pLexer);
}

PTR(RDOLexer) RDOParserRDOItem::getLexer(PTR(Converter) pParser, PTR(std::istream) streamIn, PTR(std::ostream) streamOut)
{
	ASSERT(pParser);
	return new RDOLexer(pParser, streamIn, streamOut);
}

ruint RDOParserRDOItem::lexer_loc_line()
{
	if (m_pLexer)
	{
		return m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_line : m_pLexer->lineno();
	}
	else
	{
		return rdoRuntime::RDOSrcInfo::Position::UNDEFINE_LINE;
	}
}

ruint RDOParserRDOItem::lexer_loc_pos()
{
	return m_pLexer && m_pLexer->m_lploc ? m_pLexer->m_lploc->m_first_pos : 0;
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
RDOParserRSS::RDOParserRSS()
	: RDOParserRDOItem(rdoModelObjectsConvertor::RSS, cnv_rssparse, cnv_rsserror, cnv_rsslex)
{}

void RDOParserRSS::parse(PTR(Converter) pParser, REF(std::istream) streamIn)
{
	ASSERT(pParser);
	pParser->setHaveKWResources   (false);
	pParser->setHaveKWResourcesEnd(false);
	RDOParserRDOItem::parse(pParser, streamIn);
}

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
RDOParserRSSPost::RDOParserRSSPost()
	: RDOParserItem(rdoModelObjectsConvertor::RSS, NULL, NULL, NULL)
{
	m_needStream = false;
}

void RDOParserRSSPost::parse(PTR(Converter) pParser)
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
				rdoRuntime::LPRDOCalc calc = (*rss_it)->createCalc();
				pParser->runtime()->addInitCalc(calc);
#ifdef RDOSIM_COMPATIBLE
			}
#endif
		}
#ifdef RDOSIM_COMPATIBLE
	}
#endif
}

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
RDOParserSTDFUN::RDOParserSTDFUN()
	: RDOParserItem(rdoModelObjectsConvertor::FUN, NULL, NULL, NULL)
{
	m_needStream = false;
}

void RDOParserSTDFUN::parse(PTR(Converter) pParser)
{
	ASSERT(pParser);

	LPRDOTypeParam intType  = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(),  RDOValue(), RDOParserSrcInfo());
	LPRDOTypeParam realType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOValue(), RDOParserSrcInfo());

	LPRDOFUNFunction      fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Abs"), realType);
	LPRDOFUNFunctionParam param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcAbs>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("ArcCos"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcCos>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("ArcSin"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcSin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("ArcTan"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcTan>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Cos"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCos>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Cotan"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCotan>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Exp"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcExp>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Floor"), intType );
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFloor>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Frac"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFrac>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("IAbs"), intType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIAbs>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("IMax"), intType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   intType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMax>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("IMin"), intType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   intType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Int"), intType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcInt>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("IntPower"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),       realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),       intType );
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIntPower>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Ln"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLn>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Log10"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog10>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Log2"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog2>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("LogN"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLogN>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Max"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMax>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Min"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Power"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcPower>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Round"), intType );
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcRound>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Sin"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Sqrt"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSqrt>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("Tan"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcTan>::create());

	// -----------------------------------------
	// � ��� ��������� ����
	// -----------------------------------------
	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("abs"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcAbs>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("arccos"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcCos>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("arcsin"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcSin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("arctan"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),     realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcArcTan>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("cos"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCos>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("cotan"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcCotan>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("exp"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcExp>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("floor"), intType );
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFloor>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("frac"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcFrac>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("iabs"), intType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIAbs>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("imax"), intType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   intType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMax>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("imin"), intType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   intType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),   intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIMin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("int"), intType );
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcInt>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("intpower"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),       realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),       intType );
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcIntPower>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("ln"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLn>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("log10"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog10>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("log2"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLog2>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("logn"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcLogN>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("max"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMax>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("min"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcMin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("power"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p2"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcPower>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("round"), intType );
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),    realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcRound>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("sin"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSin>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("sqrt"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),   realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcSqrt>::create());

	fun   = rdo::Factory<RDOFUNFunction>::create     (_T("tan"), realType);
	param = rdo::Factory<RDOFUNFunctionParam>::create(_T("p1"),  realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<rdoRuntime::RDOFunCalcTan>::create());
}

CLOSE_RDO_CONVERTER_NAMESPACE
