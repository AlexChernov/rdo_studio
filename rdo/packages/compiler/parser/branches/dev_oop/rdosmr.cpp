/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosmr.cpp
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
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdorss.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdoparser_rdo.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int smr_file_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void smr_file_error(PTR(char) message)
{
	UNUSED(message);
}

int smr_sim_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void smr_sim_error(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOSMR
// --------------------------------------------------------------------------------
RDOSMR::RDOSMR()
	: m_frameNumber   (1 )
	, m_showRate      (60)
	, m_runStartTime  (0 )
	, m_traceStartTime(rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME)
	, m_traceEndTime  (rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME)
	, m_showMode      (rdo::service::simulation::SM_NoShow           )
{}

void RDOSMR::setShowMode(rdo::service::simulation::ShowMode showMode)
{
	m_showMode = showMode;
}

void RDOSMR::setFrameNumber(int value, CREF(YYLTYPE) pos)
{
	if (value <= 0)
	{
		RDOParser::s_parser()->error().error(pos, _T("����� ����� ������ ���� ������ ����"));
	}
	if (RDOParser::s_parser()->runtime()->m_frameList.size() + 1 <= (ruint)value)
	{
		RDOParser::s_parser()->error().error(pos, rdo::format(_T("�������������� ����: %d"), value));
	}
	m_frameNumber = value;
}

void RDOSMR::setShowRate(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		RDOParser::s_parser()->error().error(pos, _T("������� ������ ���� ������ ����"));
	}
	m_showRate = value;
}

void RDOSMR::setRunStartTime(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		RDOParser::s_parser()->error().error(pos, _T("��������� ��������� ����� ������ ���� ������ ����"));
	}
	m_runStartTime = value;
}

void RDOSMR::setTraceStartTime(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		RDOParser::s_parser()->error().error(pos, _T("��������� ����� ����������� ������ ���� ������ ����"));
	}
	if (getTraceEndTime() != rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceEndTime() <= value)
	{
		RDOParser::s_parser()->error().push_only(pos, _T("��������� ����� ����������� ������ ���� ������ ���������"));
		RDOParser::s_parser()->error().push_only(m_traceEndTime_pos, _T("��. �������� ����� �����������"));
		RDOParser::s_parser()->error().push_done();
	}
	m_traceStartTime     = value;
	m_traceStartTime_pos = pos;
}

void RDOSMR::setTraceEndTime(double value, CREF(YYLTYPE) pos)
{
	if (value < 0)
	{
		RDOParser::s_parser()->error().error(pos, _T("�������� ����� ����������� ������ ���� ������ ����"));
	}
	if (getTraceStartTime() != rdo::runtime::RDOSimulatorTrace::UNDEFINE_TIME && getTraceStartTime() >= value)
	{
		RDOParser::s_parser()->error().push_only(pos, _T("�������� ����� ����������� ������ ���� ������ ����������"));
		RDOParser::s_parser()->error().push_only(m_traceStartTime_pos, _T("��. ��������� ����� �����������"));
		RDOParser::s_parser()->error().push_done();
	}
	m_traceEndTime     = value;
	m_traceEndTime_pos = pos;
}

void RDOSMR::setTerminateIf(REF(LPRDOFUNLogic) pLogic)
{
	if (m_pTerminateIf)
	{
		RDOParser::s_parser()->error().push_only(pLogic->src_info(), _T("Terminate_if ��� ���������"));
		RDOParser::s_parser()->error().push_only(m_pTerminateIf->src_info(), _T("��. ������ �����������"));
		RDOParser::s_parser()->error().push_done();
	}
	m_pTerminateIf = pLogic;
	RDOParser::s_parser()->runtime()->setTerminateIf(pLogic->getCalc());
}

void RDOSMR::setConstValue(CREF(RDOParserSrcInfo) const_info, REF(LPRDOFUNArithm) pArithm)
{
	LPRDOFUNConstant pConstant = RDOParser::s_parser()->findFUNConstant(const_info.src_text());
	if (!pConstant)
	{
		RDOParser::s_parser()->error().error(const_info, rdo::format(_T("��������� '%s' �� �������"), const_info.src_text().c_str()));
	}
	ASSERT(pArithm);
	pArithm->checkParamType(pConstant->getTypeInfo());
	rdo::runtime::LPRDOCalc pCalc = pArithm->createCalc(pConstant->getTypeInfo());
	RDOParser::s_parser()->runtime()->addInitCalc(rdo::Factory<rdo::runtime::RDOCalcSetConst>::create(pConstant->getNumber(), pCalc));
	RDOParser::s_parser()->insertChanges(pConstant->src_text(), pArithm->src_text());
}

void RDOSMR::setResParValue(CREF(RDOParserSrcInfo) res_info, CREF(RDOParserSrcInfo) par_info, REF(LPRDOFUNArithm) pArithm)
{
	LPRDORSSResource pResource = RDOParser::s_parser()->findRSSResource(res_info.src_text());
	if (!pResource)
	{
		RDOParser::s_parser()->error().error(res_info.src_info(), rdo::format(_T("������ '%s' �� ������"), res_info.src_text().c_str()));
	}
	LPRDORTPParam pParam = pResource->getType()->findRTPParam(par_info.src_text());
	if (!pParam)
	{
		RDOParser::s_parser()->error().push_only(par_info.src_info(), rdo::format(_T("�������� '%s' �� ������"), par_info.src_text().c_str()));
		RDOParser::s_parser()->error().push_only(pResource->src_info(), _T("��. ������"));
		RDOParser::s_parser()->error().push_only(pResource->getType()->src_info(), _T("��. ��� �������"));
		RDOParser::s_parser()->error().push_done();
	}
	ASSERT(pArithm);
	pArithm->checkParamType(pParam->getTypeInfo());
	ruint                 parNumb = pResource->getType()->getRTPParamNumber(par_info.src_text());
	rdo::runtime::LPRDOCalc pCalc   = pArithm->createCalc(pParam->getTypeInfo());
	RDOParser::s_parser()->runtime()->addInitCalc(rdo::Factory<rdo::runtime::RDOSetResourceParamCalc>::create(pResource->getID(), parNumb, pCalc));
	RDOParser::s_parser()->insertChanges(res_info.src_text() + _T(".") + par_info.src_text(), pArithm->src_text());
}

void RDOSMR::setSeed(CREF(RDOParserSrcInfo) seq_info, int base)
{
	LPRDOFUNSequence pSequence = RDOParser::s_parser()->findFUNSequence(seq_info.src_text());
	if (!pSequence)
	{
		RDOParser::s_parser()->error().error(seq_info, rdo::format(_T("������������������ '%s' �� �������"), seq_info.src_text().c_str()));
	}
	pSequence->getInitCalc()->setBase(base);
	RDOParser::s_parser()->insertChanges(pSequence->src_text() + _T(".Seed"), rdo::format(_T("%d"), base));
}

void RDOSMR::insertBreakPoint(CREF(RDOParserSrcInfo) src_info, REF(LPRDOFUNLogic) pLogic)
{
	STL_FOR_ALL_CONST(m_breakPointList, it)
	{
		if ((*it)->src_text() == src_info.src_text())
		{
			RDOParser::s_parser()->error().push_only(src_info, rdo::format(_T("����� �������� � ������ '%s' ��� ����������"), src_info.src_text().c_str()));
			RDOParser::s_parser()->error().push_only((*it)->src_info(), _T("��. ������ �����������"));
			RDOParser::s_parser()->error().push_done();
		}
	}
	LPBreakPoint pBreakPoint = rdo::Factory<BreakPoint>::create(src_info, pLogic);
	ASSERT(pBreakPoint);
	m_breakPointList.push_back(pBreakPoint);
}

RDOSMR::BreakPoint::BreakPoint(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic)
	: RDOParserSrcInfo(src_info)
{
	ASSERT(pLogic);
	RDOParser::s_parser()->runtime()->insertBreakPoint(src_text(), pLogic->getCalc());
}

CLOSE_RDO_PARSER_NAMESPACE
