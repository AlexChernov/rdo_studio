/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopmd.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_parser/rdopmd.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int pmdlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void pmderror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
RDOPMDPokaz::RDOPMDPokaz(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{
	LPRDOPMDPokaz pPokaz = RDOParser::s_parser()->findPMDPokaz(src_text());
	if (pPokaz)
	{
		RDOParser::s_parser()->error().push_only(this->src_info(), rdo::format(_T("���������� '%s' ��� ����������"), src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pPokaz->src_info(), _T("��. ������ �����������"));
		RDOParser::s_parser()->error().push_done();
	}
}

RDOPMDPokaz::~RDOPMDPokaz()
{}

void RDOPMDPokaz::endOfCreation(CREF(LPIPokaz) pPokaz)
{
	m_pPokaz = pPokaz;
	LPITrace trace = m_pPokaz;
	if (trace)
	{
		trace->setTraceID(RDOParser::s_parser()->getPMD_id());
	}
	RDOParser::s_parser()->insertPMDPokaz(this);
	//! [TODO]: ��������� � ����������� rdoRuntime::RDOPMDPokaz
	RDOParser::s_parser()->runtime()->addRuntimePokaz(m_pPokaz);
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
RDOPMDWatchPar::RDOPMDWatchPar(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_src_info, CREF(RDOParserSrcInfo) par_src_info)
	: RDOPMDPokaz(src_info)
{
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(res_src_info.src_text());
	if (!pResource)
	{
		RDOParser::s_parser()->error().error(res_src_info, rdo::format(_T("������ '%s' �� ������"), res_src_info.src_text().c_str()));
	}
/*
	if (!pResource->getType()->isPermanent())
	{
		RDOParser::s_parser()->error().push_only(res_src_info, _T("��������� (watch_par) ����� ������ �� ���������� ����������� �������"));
		RDOParser::s_parser()->error().push_only(pResource->getType()->src_info(), _T("��. ��� �������"));
		RDOParser::s_parser()->error().push_done();
	}
*/
	LPRDORTPParam pParam = pResource->getType()->findRTPParam(par_src_info.src_text());
	if (!pParam)
	{
		RDOParser::s_parser()->error().push_only(par_src_info, rdo::format(_T("�������� '%s' �� ������"), par_src_info.src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pResource->src_info(), _T("��. ������"));
		RDOParser::s_parser()->error().push_only(pResource->getType()->src_info(), _T("��. ��� �������"));
		RDOParser::s_parser()->error().push_done();
	}
	rdoRuntime::RDOType::TypeID typeID = pParam->getType()->type()->typeID();
	if (typeID != rdoRuntime::RDOType::t_int && typeID != rdoRuntime::RDOType::t_real)
	{
		RDOParser::s_parser()->error().push_only(par_src_info, _T("��������� ����� ������ �� ���������� ������ ��� ������������� ����"));
		RDOParser::s_parser()->error().push_only(pParam->getType()->src_info(), _T("��. ��� ���������"));
		RDOParser::s_parser()->error().push_done();
	}
	endOfCreation(F(rdoRuntime::RDOPMDWatchPar)::create(RDOParser::s_parser()->runtime(), src_text(), trace, res_src_info.src_text(), par_src_info.src_text(), pResource->getID(), pResource->getType()->getRTPParamNumber(par_src_info.src_text())));
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
RDOPMDWatchState::RDOPMDWatchState(CREF(RDOParserSrcInfo) src_info, rbool trace, LPRDOFUNLogic pLogic)
	: RDOPMDPokaz(src_info)
{
	endOfCreation(F(rdoRuntime::RDOPMDWatchState)::create(RDOParser::s_parser()->runtime(), src_text(), trace, pLogic->getCalc()));
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchTemp
// ----------------------------------------------------------------------------
RDOPMDWatchTemp::RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDPokaz(src_info)
{
	LPRDORTPResType pResType = RDOParser::s_parser()->findRTPResType(res_type_src_info.src_text());
	if (!pResType)
	{
		RDOParser::s_parser()->error().error(res_type_src_info, rdo::format(_T("��� ������� '%s' �� ������"), res_type_src_info.src_text().c_str()));
	}
	if (!pResType->isTemporary())
	{
		RDOParser::s_parser()->error().push_only(res_type_src_info, _T("���������� �������� ���������� �� ��������� �������� (temporary)"));
		RDOParser::s_parser()->error().push_only(pResType->src_info(), _T("��. ��� �������"));
		RDOParser::s_parser()->error().push_done();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
RDOPMDWatchQuant::RDOPMDWatchQuant(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{
	LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(pGroupLogic);
	endOfCreation(F(rdoRuntime::RDOPMDWatchQuant)::create(RDOParser::s_parser()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchQuant::setLogic(REF(LPRDOFUNLogic) pLogic)
{
	LPIPokazWatchQuant pQuant = m_pPokaz;
	ASSERT(pQuant);
	pQuant->setLogicCalc(pLogic->getCalc());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchQuant::setLogicNoCheck()
{
	LPIPokazWatchQuant pQuant = m_pPokaz;
	ASSERT(pQuant);
	pQuant->setLogicCalc(rdo::Factory<rdoRuntime::RDOCalcConst>::create(1));
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
RDOPMDWatchValue::RDOPMDWatchValue(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info)
	: RDOPMDWatchTemp(src_info, res_type_src_info)
{
	LPRDOFUNGroupLogic pGroupLogic = rdo::Factory<RDOFUNGroupLogic>::create(RDOFUNGroupLogic::fgt_unknow, RDOParserSrcInfo(res_type_src_info.src_text()));
	ASSERT(pGroupLogic);
	endOfCreation(F(rdoRuntime::RDOPMDWatchValue)::create(RDOParser::s_parser()->runtime(), src_text(), trace, res_type_src_info.src_text(), pGroupLogic->getResType()->getNumber()));
}

void RDOPMDWatchValue::setLogic(REF(LPRDOFUNLogic) pLogic, REF(LPRDOFUNArithm) pArithm)
{
	LPIPokazWatchValue pWatch = m_pPokaz;
	ASSERT(pWatch);
	pWatch->setLogicCalc (pLogic->getCalc()    );
	pWatch->setArithmCalc(pArithm->createCalc());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

void RDOPMDWatchValue::setLogicNoCheck(REF(LPRDOFUNArithm) pArithm)
{
	LPIPokazWatchValue pWatch = m_pPokaz;
	ASSERT(pWatch);
	pWatch->setLogicCalc (rdo::Factory<rdoRuntime::RDOCalcConst>::create(1));
	pWatch->setArithmCalc(pArithm->createCalc());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
}

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
RDOPMDGetValue::RDOPMDGetValue(CREF(RDOParserSrcInfo) src_info, LPRDOFUNArithm pArithm)
	: RDOPMDPokaz(src_info)
{
	endOfCreation(F(rdoRuntime::RDOPMDGetValue)::create(RDOParser::s_parser()->runtime(), src_text(), pArithm->createCalc()));
}

CLOSE_RDO_PARSER_NAMESPACE
