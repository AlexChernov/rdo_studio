/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorss.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "simulator/runtime/rdocalc.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_rsslex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_rsserror(PTR(char) message)
{}

// --------------------------------------------------------------------------------
// -------------------- RDORSSResource
// --------------------------------------------------------------------------------
RDORSSResource::RDORSSResource(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id)
	: RDOParserSrcInfo(src_info                                      )
	, m_pResType      (pResType                                      )
	, m_id            (id == UNDEFINED_ID ? pParser->getRSS_id() : id)
	, trace           (false                                         )
{
	ASSERT(m_pResType);
	pParser->insertRSSResource(LPRDORSSResource(this));
	m_currParam = m_pResType->getParams().begin();
}

void RDORSSResource::writeModelStructure(REF(std::ostream) stream) const
{
	stream << (getID() + 1) << _T(" ") << name() << _T(" ") << getType()->getNumber() << std::endl;
}

void RDORSSResource::addParam(CREF(RDOValue) param)
{
	if (m_currParam == getType()->getParams().end())
	{
		Converter::s_converter()->error().push_only(param.src_info(), _T("������� ����� ����������"));
		Converter::s_converter()->error().push_only(getType()->src_info(), _T("��. ��� �������"));
		Converter::s_converter()->error().push_done();
	}
	try
	{
		if (param->getAsString() == _T("*"))
		{
			if (!(*m_currParam)->getDefault().defined())
			{
				Converter::s_converter()->error().push_only(param.src_info(), _T("���������� ������������ '*', �.�. ����������� �������� ��-���������"));
				Converter::s_converter()->error().push_only((*m_currParam)->getType()->src_info(), _T("��. �������� ���������"));
				Converter::s_converter()->error().push_done();
			}
			m_paramList.push_back(Param((*m_currParam)->getDefault()));
			m_currParam++;
		}
		else
		{
			m_paramList.push_back(Param((*m_currParam)->getType()->value_cast(param)));
			m_currParam++;
		}
	}
	catch(REF(RDOSyntaxException))
	{
		Converter::s_converter()->error().modify(rdo::format(_T("��� ��������� '%s': "), (*m_currParam)->name().c_str()));
	}
}

bool RDORSSResource::defined() const
{
	return m_currParam == getType()->getParams().end();
}

rdoRuntime::LPRDOCalc RDORSSResource::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(params(), it)
	{
		paramList.push_back(it->param().value());
	}

	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcCreateNumberedResource>::create(getType()->getNumber(), getTrace(), paramList, getID(), getType()->isPermanent());
	calc->setSrcInfo(src_info());
	calc->setSrcText(_T("�������� ������� ") + calc->src_text());
	return calc;
}

// --------------------------------------------------------------------------------
// -------------------- RDOPROCResource
// --------------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource(PTR(Converter) pParser, CREF(RDOParserSrcInfo) src_info, CREF(LPRDORTPResType) pResType, int id)
	: RDORSSResource(pParser, src_info, pResType, id)
{}

rdoRuntime::LPRDOCalc RDOPROCResource::createCalc() const
{
	std::vector<rdoRuntime::RDOValue> paramList;
	STL_FOR_ALL_CONST(params(), it)
	{
		paramList.push_back(it->param().value());
	}

	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcCreateProcessResource>::create(getType()->getNumber(), getTrace(), paramList, getID(), getType()->isPermanent());
	calc->setSrcInfo(src_info());
	calc->setSrcText(_T("�������� ������� ") + calc->src_text());
	return calc;
}

CLOSE_RDO_CONVERTER_NAMESPACE
