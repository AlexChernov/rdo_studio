/*
 * copyright: (c) RDO-Team, 2011
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
#include "rdo_lib/rdo_runtime/calc/std_fun.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
RDOParserRDOItem::RDOParserRDOItem(rdoModelObjectsConvertor::RDOFileTypeIn type, t_bison_parse_fun parser_fun, t_bison_error_fun error_fun, t_flex_lexer_fun lexer_fun)
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
	: RDOParserRDOItem(rdoModelObjectsConvertor::RSS_IN, cnv_rssparse, cnv_rsserror, cnv_rsslex)
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
	: RDOParserItem(rdoModelObjectsConvertor::RSS_IN, NULL, NULL, NULL)
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
	: RDOParserItem(rdoModelObjectsConvertor::FUN_IN, NULL, NULL, NULL)
{
	m_needStream = false;
}

double cotan(double value)
{
	return 1.0 / tan(value);
}

int intLocal(int value)
{
	return value;
}

double log2Local(double value)
{
	return log(value) / log(2.0);
}

double logNLocal(double value1, double value2)
{
	return log(value1) / log(value2);
}

template <class T>
T maxLocal(T value1, T value2)
{
	return value1 > value2 ? value1 : value2;
}

template <class T>
T minLocal(T value1, T value2)
{
	return value1 < value2 ? value1 : value2;
}

double modf(double value)
{
	double tmp;
	return ::modf(value, &tmp);
}

int roundLocal(double value)
{
	return (int)floor(value + 0.5);
}

void RDOParserSTDFUN::parse(PTR(Converter) pParser)
{
	ASSERT(pParser);

	typedef rdoRuntime::std_fun1<double, double>         StdFun_D_D;
	typedef rdoRuntime::std_fun2<double, double, double> StdFun_D_DD;
	typedef rdoRuntime::std_fun2<double, double, int>    StdFun_D_DI;
	typedef rdoRuntime::std_fun1<int,    int>            StdFun_I_I;
	typedef rdoRuntime::std_fun2<int,    int, int>       StdFun_I_II;
	typedef rdoRuntime::std_fun1<int,    double>         StdFun_I_D;

	typedef rdoRuntime::RDOFunCalcStd<StdFun_D_D>  Function_D_D;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_D_DD> Function_D_DD;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_D_DI> Function_D_DI;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_I_I>  Function_I_I;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_I_II> Function_I_II;
	typedef rdoRuntime::RDOFunCalcStd<StdFun_I_D>  Function_I_D;

	LPRDOTypeParam intType     = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(),  RDOParserSrcInfo());
	LPRDOTypeParam realType    = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	LPRDOParam     pIntReturn  = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), intType );
	LPRDOParam     pRealReturn = rdo::Factory<RDOParam>::create(RDOParserSrcInfo(), realType);

	LPRDOFUNFunction fun   = rdo::Factory<RDOFUNFunction>::create(_T("Abs"), pRealReturn);
	LPRDOParam       param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(fabs));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcCos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(acos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcSin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(asin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ArcTan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(atan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Cos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Cotan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cotan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Exp"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(exp));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Floor"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(floor));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Frac"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(modf));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IAbs"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(static_cast<Function_I_I::function_type>(abs)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IMax"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(maxLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IMin"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(minLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Int"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(intLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("IntPower"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create(static_cast<Function_D_DI::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Ln"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Log10"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log10));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Log2"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log2Local));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("LogN"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(logNLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Max"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(maxLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Min"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(minLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Power"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(static_cast<Function_D_DD::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Round"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_D>::create(roundLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Sin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Sqrt"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sqrt));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("Tan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(tan));

	// -----------------------------------------
	// � ��� ��������� ����
	// -----------------------------------------
	fun   = rdo::Factory<RDOFUNFunction>::create(_T("abs"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(fabs));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arccos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(acos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arcsin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(asin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("arctan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(atan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("cos"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cos));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("cotan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(cotan));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("exp"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(exp));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("floor"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(floor));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("frac"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(modf));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("iabs"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(static_cast<Function_I_I::function_type>(abs)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("imax"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(maxLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("imin"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), intType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_II>::create(minLocal<int>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("int"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_I>::create(intLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("intpower"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), intType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DI>::create(static_cast<Function_D_DI::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("ln"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("log10"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log10));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("log2"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(log2Local));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("logn"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(logNLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("max"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(maxLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("min"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(minLocal<double>));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("power"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	param = rdo::Factory<RDOParam>::create(_T("p2"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_DD>::create(static_cast<Function_D_DD::function_type>(pow)));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("round"), pIntReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_I_D>::create(roundLocal));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("sin"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sin));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("sqrt"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(sqrt));

	fun   = rdo::Factory<RDOFUNFunction>::create(_T("tan"), pRealReturn);
	param = rdo::Factory<RDOParam>::create(_T("p1"), realType);
	fun->add(param);
	fun->setFunctionCalc(rdo::Factory<Function_D_D>::create(tan));
}

CLOSE_RDO_CONVERTER_NAMESPACE
