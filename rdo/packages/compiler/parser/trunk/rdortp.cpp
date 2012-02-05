/*!
  \copyright (c) RDO-Team, 2011
  \file      rdortp.cpp
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
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSE_NAMESPACE

int rtplex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void rtperror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDORTPResType
// --------------------------------------------------------------------------------
RDORTPResType::RDORTPResType(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, rbool permanent)
	: RDOParserSrcInfo(src_info            )
	, m_number        (pParser->getRTP_id())
	, m_permanent     (permanent           )
{
	pParser->insertRTPResType(LPRDORTPResType(this));
}

RDORTPResType::~RDORTPResType()
{}

LPRDORSSResource RDORTPResType::createRes(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info)
{
	return rdo::Factory<RDORSSResource>::create(pParser, src_info, this);
}

void RDORTPResType::addParam(CREF(LPRDORTPParam) param)
{
	if (findRTPParam(param->name()))
	{
		RDOParser::s_parser()->error().error(param->src_info(), rdo::format("�������� ��� ����������: %s", param->name().c_str()));
	}
	m_params.push_back(param);
}

void RDORTPResType::addParam(CREF(tstring) param_name, rdoRuntime::RDOType::TypeID param_typeID)
{
	UNUSED(param_name  );
	UNUSED(param_typeID);
}

LPRDORTPParam RDORTPResType::findRTPParam(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? *it : LPRDORTPParam();
}

ruint RDORTPResType::getRTPParamNumber(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_params.begin(), m_params.end(), compareName<RDORTPParam>(paramName));
	return it != m_params.end() ? it - m_params.begin() : UNDEFINED_PARAM;
}

void RDORTPResType::writeModelStructure(REF(std::ostream) stream) const
{
	stream << getNumber() << " " << name() << " " << getParams().size() << std::endl;
	for (ruint i = 0; i < getParams().size(); i++)
	{
		stream << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure(stream);
	}
}

tstring RDORTPResType::name() const
{
	static tstring s_name;
	s_name = src_text();
	return s_name;
}

LPRDOType RDORTPResType::type_cast(CREF(LPRDOType) pFrom, CREF(RDOParserSrcInfo) from_src_info, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	UNUSED(from_src_info);

	switch (pFrom->typeID())
	{
	case rdoRuntime::RDOType::t_pointer:
		{	
			LPRDOType pThisRTPType(const_cast<PTR(RDORTPResType)>(this));

			//! ��� ���� � ��� �� ���
			if (pThisRTPType == pFrom)
				return pThisRTPType;

			//! ���� ������, �������� ������
			rdoParse::g_error().push_only(src_info,    _T("�������������� ����� ��������"));
			rdoParse::g_error().push_only(to_src_info, to_src_info.src_text());
			rdoParse::g_error().push_done();
			break;
		}
	default:
		{
			rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ��� �������, �������: %s"), from_src_info.src_text().c_str()));
			rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
			rdoParse::g_error().push_done();
			break;
		}
	}

	return LPRDOType(NULL);
}

LPRDOValue RDORTPResType::value_cast(CREF(LPRDOValue) pFrom, CREF(RDOParserSrcInfo) to_src_info, CREF(RDOParserSrcInfo) src_info) const
{
	ASSERT(pFrom);

	LPRDORTPResType pRTPResType = pFrom->typeInfo()->type().object_dynamic_cast<RDORTPResType>();
	if (pRTPResType)
	{
		LPRDOType pThisType = const_cast<PTR(RDORTPResType)>(this);

		//! ��� ���� � ��� �� ���
		if (pThisType == pRTPResType.object_parent_cast<RDOType>())
			return pFrom;

		//! ���� ������, �������� ������
		rdoParse::g_error().push_only(src_info,    _T("�������������� ����� ��������"));
		rdoParse::g_error().push_only(to_src_info,  rdo::format(  _T("���������: %s"), to_src_info.src_text().c_str()));
		rdoParse::g_error().push_only(src_info,  rdo::format(  _T("������: %s"), pFrom->src_text().c_str()));
		rdoParse::g_error().push_only(to_src_info, to_src_info.src_text());
		rdoParse::g_error().push_done();
	}
	rdoParse::g_error().push_only(src_info,    rdo::format(_T("��������� ������, �������: %s"), pFrom->src_text().c_str()));
	rdoParse::g_error().push_only(to_src_info, rdo::format(_T("��. ���: %s"), to_src_info.src_text().c_str()));
	rdoParse::g_error().push_done();

	return LPRDOValue(NULL);
}

rdoRuntime::LPRDOCalc RDORTPResType::calc_cast(CREF(rdoRuntime::LPRDOCalc) pCalc, CREF(LPRDOType) pType) const
{
	return RDOType::calc_cast(pCalc, pType);
}

rdoRuntime::RDOValue RDORTPResType::get_default() const
{
	NEVER_REACH_HERE;
	return rdoRuntime::RDOValue();
	//return rdoRuntime::RDOValue (pResourceType,pResource);
}

IContextFind::Result RDORTPResType::onSwitchContext(CREF(LPExpression) pSwitchExpression, CREF(LPRDOValue) pValue) const
{
	ASSERT(pSwitchExpression);
	ASSERT(pValue           );

	ruint parNumb = getRTPParamNumber(pValue->value().getIdentificator());
	if (parNumb == RDORTPResType::UNDEFINED_PARAM)
	{
		RDOParser::s_parser()->error().error(pValue->src_info(), rdo::format(_T("����������� �������� �������: %s"), pValue->value().getIdentificator().c_str()));
	}

	LPRDORTPParam pParam = findRTPParam(pValue->value().getIdentificator());
	ASSERT(pParam);

	LPExpression pExpression = rdo::Factory<Expression>::create(
		pParam->getTypeInfo(),
		rdo::Factory<rdoRuntime::RDOCalcGetResourceParam>::create(pSwitchExpression->calc(), parNumb),
		pValue->src_info()
	);
	ASSERT(pExpression);

	return IContextFind::Result(const_cast<PTR(RDORTPResType)>(this), pExpression, pValue);
}

/*
// --------------------------------------------------------------------------------
// -------------------- RDORTPFuzzyMembershiftFun - �-�� �������������� ��������� �����
// --------------------------------------------------------------------------------
RDORTPFuzzyMembershiftFun::RDORTPFuzzyMembershiftFun(CREF(LPRDOParser) pParser):
	RDOParserObject(pParser)
{
/*	for (ruint i = 0; i < m_points.size(); i++)
	{
//		double x = m_points[i]->getX();
	}

	Items::iterator it = m_points.begin();
	while (it != m_points.end())
	{
		double x = (*it)->getX();
		it++;
	}
}
// --------------------------------------------------------------------------------
// -------------------- RDORTPFuzzyTerm - �������� ������
// --------------------------------------------------------------------------------
RDORTPFuzzyTerm::RDORTPFuzzyTerm(CREF(LPRDOParser) pParser, CREF(RDOParserSrcInfo) src_info, PTR(RDORTPFuzzyMembershiftFun) pMembersfift_fun):
	RDOParserObject(pParser)
{

}*/

CLOSE_RDO_PARSE_NAMESPACE
