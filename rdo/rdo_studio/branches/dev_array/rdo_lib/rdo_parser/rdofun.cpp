/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdofun.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_runtime/rdo_random_distribution.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/calc/arithm.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int funlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void funerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOFUNDoubleToIntByResult
// ----------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::roundCalc()
{
	STL_FOR_ALL(m_intOrDouble, it)
	{
		(*it)->needRound();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNBase
// ----------------------------------------------------------------------------
RDOFUNBase::RDOFUNBase(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{}

RDOFUNBase::RDOFUNBase(CREF(LPExpression) pExpression)
	: m_pExpression(pExpression)
{
	ASSERT(m_pExpression);
}

CREF(LPExpression) RDOFUNBase::expression() const
{
	ASSERT(m_pExpression);
	return m_pExpression;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic(CREF(LPRDOFUNArithm) pArithm)
	: RDOFUNBase(pArithm->expression())
{
	if (pArithm->typeID() != rdoRuntime::RDOType::t_bool)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("��������� ���������� ���������, �������: %s"), src_text().c_str()));
	}
}

RDOFUNLogic::RDOFUNLogic(CREF(LPExpression) pExpression, rbool hideWarning)
	: RDOFUNBase(pExpression)
{
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcFileType(src_filetype());
	m_pExpression->setSrcInfo(src_info);
	if (!hideWarning)
	{
		rdoRuntime::LPRDOCalcConst pConstCalc = m_pExpression->calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		if (pConstCalc)
		{
			if (pConstCalc->getValue().getAsBool())
			{
				RDOParser::s_parser()->error().warning(pConstCalc->src_info(), rdo::format(_T("���������� ��������� ������ �������: %s"), pConstCalc->src_text().c_str()));
			}
			else
			{
				RDOParser::s_parser()->error().warning(pConstCalc->src_info(), rdo::format(_T("���������� ��������� ������ �����: %s"), pConstCalc->src_text().c_str()));
			}
		}
	}
}

RDOFUNLogic::~RDOFUNLogic()
{}

LPRDOFUNLogic RDOFUNLogic::generateTrue(CREF(RDOParserSrcInfo) src_info)
{
	LPExpression pExpression = rdo::Factory<Expression>::create(
		rdo::Factory<RDOType__bool>::create(),
		rdo::Factory<rdoRuntime::RDOCalcConst>::create(true),
		src_info
	);
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	return pLogic;
}

rdoRuntime::LPRDOCalc RDOFUNLogic::getCalc(rdoRuntime::RDOType::TypeID id)
{
	if (id != rdoRuntime::RDOType::t_real)
	{
		m_intOrDouble.roundCalc();
	}
	return m_pExpression->calc();
}

LPRDOFUNLogic RDOFUNLogic::createLogic(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	ASSERT(pCalc);

	LPExpression pExpression = rdo::Factory<Expression>::create(rdo::Factory<RDOType__bool>::create(), pCalc, pCalc->src_info());
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(pCalc->src_info());

	return pLogic;
}

template <class T>
LPRDOFUNLogic RDOFUNLogic::generateLogic(CREF(LPRDOFUNLogic) pSecond)
{
	ASSERT(pSecond);

	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
	ASSERT(pCalc);
	LPRDOFUNLogic pLogic = createLogic(pCalc);
	pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pLogic;
}

template <class T>
LPRDOFUNLogic RDOFUNLogic::generateLogic(CREF(RDOSrcInfo::Position) position)
{
	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
	ASSERT(pCalc);
	LPRDOFUNLogic pLogic = createLogic(pCalc);
	pLogic->m_intOrDouble.insert(m_intOrDouble);
	return pLogic;
}

LPRDOFUNLogic RDOFUNLogic::operator&& (CREF(LPRDOFUNLogic) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcAnd>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator|| (CREF(LPRDOFUNLogic) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcOr>(pSecond);
}

LPRDOFUNLogic RDOFUNLogic::operator_not(CREF(RDOSrcInfo::Position) position)
{
	return generateLogic<rdoRuntime::RDOCalcNot>(position);
}

void RDOFUNLogic::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNLogic::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcPos(position);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNLogic::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcText(value);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) error_pos)
{
	RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
{
	RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm(CREF(LPExpression) pExpression)
	: RDOFUNBase(pExpression)
{
	ASSERT(m_pExpression);
	setSrcInfo(m_pExpression->src_info());
}

RDOFUNArithm::~RDOFUNArithm()
{}

LPRDOFUNArithm RDOFUNArithm::generateByConst(CREF(RDOValue) value)
{
	ASSERT(value.constant());

	LPExpression pExpression = rdo::Factory<Expression>::create(
		value.type(),
		rdo::Factory<rdoRuntime::RDOCalcConst>::create(value.value()),
		value.src_info()
	);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::generateByIdentificator(CREF(RDOValue) value)
{
	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);

	pContext = pContext->find(value);
	ASSERT(pContext);

	LPExpression pExpression = pContext->create(value);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::generateByIdentificator(CREF(RDOValue) value1, CREF(RDOValue) value2)
{
	LPContext pContext = RDOParser::s_parser()->context();
	ASSERT(pContext);

	pContext = pContext->find(value1);
	ASSERT(pContext);

	LPExpression pExpression = pContext->create(value2);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	return pArithm;
}

void RDOFUNArithm::castType(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	try
	{
		expression()->type()->type_cast(pSecond->expression()->type(), pSecond->expression()->src_info(), expression()->src_info(), expression()->src_info());
	}
	catch (REF(RDOSyntaxException))
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(error.c_str(), type()->name().c_str(), pSecond->type()->name().c_str()));
	}
}

void RDOFUNArithm::castValue(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	try
	{
		LPRDOType pType = expression()->type()->type_cast(pSecond->expression()->type(), pSecond->expression()->src_info(), expression()->src_info(), expression()->src_info());
		ASSERT(pType);
		RDOValue constant = pSecond->expression()->constant();
		if (constant.defined())
		{
			constant = expression()->type()->value_cast(constant, expression()->src_info(), pSecond->expression()->src_info());
			pSecond->m_pExpression = rdo::Factory<Expression>::create(
				pType,
				rdo::Factory<rdoRuntime::RDOCalcConst>::create(constant.value()),
				constant.src_info()
			);
			ASSERT(pSecond->m_pExpression);
		}
	}
	catch (REF(RDOSyntaxException))
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(error.c_str(), expression()->src_info().src_text().c_str(), pSecond->expression()->src_info().src_text().c_str()));
	}
}

template <class T>
rdoRuntime::LPRDOCalc RDOFUNArithm::generateCalc(CREF(rdoRuntime::RDOSrcInfo::Position) position, CREF(tstring) error)
{
	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcUnaryBase::generateCalc<T>(position, m_pExpression->calc());
	ASSERT(pCalc);
	return pCalc;
}

template <class T>
rdoRuntime::LPRDOCalc RDOFUNArithm::generateCalc(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	castType(pSecond, error);
	rdoRuntime::LPRDOCalc pCalc = rdoRuntime::RDOCalcBinaryBase::generateCalc<T>(m_pExpression->calc(), pSecond->m_pExpression->calc());
	ASSERT(pCalc);
	return pCalc;
}

template <class T>
LPRDOFUNArithm RDOFUNArithm::generateArithm(CREF(rdoRuntime::RDOSrcInfo::Position) position, CREF(tstring) error)
{
	rdoRuntime::LPRDOCalc pCalc = generateCalc<T>(position, error);
	ASSERT(pCalc);

	LPRDOType pType = getPreType(this);
	ASSERT(pType);

	LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, pCalc->src_info());
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->m_intOrDouble.insert(m_intOrDouble);
	return pArithm;
}

template <class T>
LPRDOFUNArithm RDOFUNArithm::generateArithm(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	rdoRuntime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
	ASSERT(pCalc);

	LPRDOType pType = getPreType(pSecond);
	ASSERT(pType);

	LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, pCalc->src_info());
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pArithm;
}

template <class T>
LPRDOFUNLogic RDOFUNArithm::generateLogic(CREF(LPRDOFUNArithm) pSecond, CREF(tstring) error)
{
	rdoRuntime::LPRDOCalc pCalc = generateCalc<T>(pSecond, error);
	ASSERT(pCalc);

	LPExpression pExpression = rdo::Factory<Expression>::create(rdo::Factory<RDOType__bool>::create(), pCalc, pCalc->src_info());
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(pCalc->src_info());
	pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pLogic;
}

rdoRuntime::RDOValue RDOFUNArithm::const_value() const
{
	rdoRuntime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	ASSERT(pCalc);
	return pCalc->getValue();
}

LPRDOType RDOFUNArithm::getPreType(CREF(LPRDOFUNArithm) pSecond)
{
	if (typeID() == rdoRuntime::RDOType::t_unknow)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("���������� ������ �������: �������������� �������� ��������������� ���������: %s"), src_text().c_str()));
	}
	if (pSecond->typeID() == rdoRuntime::RDOType::t_unknow)
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(_T("���������� ������ �������: �������������� �������� ��������������� ���������: %s"), pSecond->src_text().c_str()));
	}
	if (typeID() == rdoRuntime::RDOType::t_identificator)
	{
		rdoRuntime::LPRDOCalcConst pCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		ASSERT(pCalc);
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("����������� �������������: %s"), const_value().getIdentificator().c_str()));
	}
	if (pSecond->typeID() == rdoRuntime::RDOType::t_identificator)
	{
		RDOParser::s_parser()->error().error(pSecond->src_info(), rdo::format(_T("����������� �������������: %s"), pSecond->const_value().getIdentificator().c_str()));
	}

	//! TODO: ������� ��� ���������� src_info(), ��������� � �������� ������������
	return type()->type_cast(pSecond->type(), pSecond->src_info(), src_info(), src_info());
}

LPRDOFUNArithm RDOFUNArithm::operator+ (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdoRuntime::RDOCalcPlus>(pSecond, _T("�� �� ���� � ������� %s � %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator- (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdoRuntime::RDOCalcMinus>(pSecond, _T("������ �� %s ������� %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator* (CREF(LPRDOFUNArithm) pSecond)
{
	return generateArithm<rdoRuntime::RDOCalcMult>(pSecond, _T("������ %s �������� �� %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator/ (CREF(LPRDOFUNArithm) pSecond)
{
	rdoRuntime::LPRDOCalc pCalc = generateCalc<rdoRuntime::RDOCalcDiv>(pSecond, _T("������ %s ��������� �� %s"));
	ASSERT(pCalc);
	LPRDOType pType = getPreType(pSecond);
	ASSERT(pType);

	//! TODO: ������� ������������� � ����� ��� �������. � ��� ������ � ���������� � �.�. ?
	//! �����: � ���������� ���� ������ ���� �����, ������ ��������� ���
	if (pType->type()->typeID() == rdoRuntime::RDOType::t_int)
	{
		rdoRuntime::LPRDOCalc pNewCalc_div = pCalc;
		pCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToIntByResult>::create(pNewCalc_div);
		pCalc->setSrcInfo(pNewCalc_div->src_info());
	}
	LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, pCalc->src_info());
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	if (pType->type()->typeID() == rdoRuntime::RDOType::t_int)
	{
		rdoRuntime::LPRDOCalcDoubleToIntByResult pResult = pCalc.object_static_cast<rdoRuntime::RDOCalcDoubleToIntByResult>();
		pArithm->m_intOrDouble.push_back(pResult);
	}
	pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pArithm;
}

LPRDOFUNArithm RDOFUNArithm::setEqual(CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("������ ������������ %s = %s"));
	return this;
}

LPRDOFUNArithm RDOFUNArithm::uminus(CREF(rdoRuntime::RDOSrcInfo::Position) position)
{
	return generateArithm<rdoRuntime::RDOCalcUMinus>(position, _T("������ ����� ������� ����� �� %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator< (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsLess>(pSecond, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator> (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsGreater>(pSecond, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator<= (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsLEQ>(pSecond, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator>= (CREF(LPRDOFUNArithm) pSecond)
{
	return generateLogic<rdoRuntime::RDOCalcIsGEQ>(pSecond, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator== (CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("������ ��������� %s == %s"));
	return generateLogic<rdoRuntime::RDOCalcIsEqual>(pSecond, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator!= (CREF(LPRDOFUNArithm) pSecond)
{
	castValue(pSecond, _T("������ ��������� %s <> %s"));
	return generateLogic<rdoRuntime::RDOCalcIsNotEqual>(pSecond, _T("������ ���������� %s � %s"));
}

void RDOFUNArithm::checkParamType(CREF(LPRDOTypeParam) pType)
{
	pType->type()->type_cast(type(), src_info(), pType->src_info(), src_info());
	rdoRuntime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	if (pConstCalc)
	{
		rdoRuntime::RDOValue value = pConstCalc->getValue();
		pType->value_cast(RDOValue(value, type(), src_info()));
	}
}

rdoRuntime::LPRDOCalc RDOFUNArithm::createCalc(CREF(LPRDOTypeParam) pForType)
{
	if (typeID() != rdoRuntime::RDOType::t_identificator)
	{
		if (pForType == NULL)
		{
			return m_pExpression->calc();
		}
		if (pForType->type()->typeID() != rdoRuntime::RDOType::t_int)
		{
			if (pForType->type()->typeID() == rdoRuntime::RDOType::t_enum)
			{
				m_intOrDouble.roundCalc();
			}
			return m_pExpression->calc();
		}
		else
		{
			m_intOrDouble.roundCalc();
			rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToInt>::create(m_pExpression->calc()->src_pos(), m_pExpression->calc());
			pCalc->setSrcInfo(src_info());
			return pCalc;
		}
	}
	else if (typeID() == rdoRuntime::RDOType::t_identificator && !pForType)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("����������� �������������: %s"), const_value().getAsString().c_str()));
	}

	if (!pForType)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("����������� ��� ���������"));
	}

	rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pForType->value_cast(RDOValue(src_info())).value());
	pNewCalc->setSrcInfo(src_info());
	return pNewCalc;
}

void RDOFUNArithm::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNArithm::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcPos(position);
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNArithm::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	rdoRuntime::RDOSrcInfo src_info = m_pExpression->src_info();
	src_info.setSrcText(this->src_info().src_text());
	m_pExpression->setSrcInfo(src_info);
}

void RDOFUNArithm::setSrcInfo(CREF(RDOParserSrcInfo) begin, CREF(tstring) delim, CREF(RDOParserSrcInfo) end)
{
	RDOParserSrcInfo::setSrcInfo(begin, delim, end);
}

void RDOFUNArithm::setSrcPos(CREF(YYLTYPE) error_pos)
{
	RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNArithm::setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
{
	RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, CREF(RDOValue) default)
	: RDOParam(src_info, pType, default)
{
	if (!getDefault().defined())
	{
		ASSERT(pType);
		RDOParser::s_parser()->error().error(pType->src_info(), _T("��������� ������ ����� ��������"));
	}

	m_number = RDOParser::s_parser()->getFUNCONST_id();
	RDOParser::s_parser()->insertFUNConstant(this);
}

RDOFUNConstant::~RDOFUNConstant()
{}

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
// ���������, � �������� ������� ����������
// ----------------------------------------------------------------------------
RDOFUNParams::RDOFUNParams()
	: RDOParserSrcInfo()
{}

RDOFUNParams::~RDOFUNParams()
{}

rdoRuntime::LPRDOCalc RDOFUNParams::getCalc(ruint paramID, CREF(LPRDOTypeParam) pType)
{
	ASSERT(paramID < m_paramList.size());
	rdoRuntime::LPRDOCalc pCalc = m_paramList[paramID]->createCalc(pType);
	ASSERT(pCalc);
	return pCalc;
}

void RDOFUNParams::addParameter(CREF(LPRDOFUNArithm) pParam)
{
	ASSERT(pParam);
	m_paramList.push_back(pParam);
}

LPRDOFUNArithm RDOFUNParams::createCall(CREF(tstring) funName)
{
	LPRDOFUNFunction pFunction = RDOParser::s_parser()->findFUNFunction(funName);
	if (!pFunction)
	{
		return createSeqCall(funName);
	}

	int nParams = pFunction->getParams().size();
	if (nParams != m_paramList.size())
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("�������� ���������� ���������� �������: %s"), funName.c_str()));
	}

	//rdoRuntime::LPRDOCalc pFuncCall = pFunction->getFunctionCalc();
	rdoRuntime::LPRDOCalcFunctionCall pFuncCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(pFunction->getFunctionCalc());
	pFunction->insertPostLinked(pFuncCall);
	pFuncCall->setSrcInfo(src_info());
	for (int i = 0; i < nParams; i++)
	{
		LPRDOTypeParam pFuncParam = pFunction->getParams()[i]->getType();
		LPRDOFUNArithm pArithm = m_paramList[i];
		ASSERT(pArithm);
		pArithm->checkParamType(pFuncParam);
		pFuncCall->addParameter(pFuncParam->type()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->type()));
	}

	LPExpression pExpression = rdo::Factory<Expression>::create(pFunction->getReturn()->getType()->type(), pFuncCall, src_info());
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info());
	return pArithm;
}

LPRDOFUNArithm RDOFUNParams::createSeqCall(CREF(tstring) seqName)
{
	LPRDOFUNSequence pSequence = RDOParser::s_parser()->findFUNSequence(seqName);
	if (!pSequence)
	{
		RDOParser::s_parser()->error().error(m_funseqName, rdo::format(_T("�������������� ������� ��� ������������������: %s"), seqName.c_str()));
	}
	LPRDOFUNArithm pArithm = pSequence->createCallCalc(LPRDOFUNParams(this), src_info());
	ASSERT(pArithm);
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOParserSrcInfo(pHeader->src_info())
	, m_pHeader       (pHeader            )
	, m_seed          (seed               )
{
	RDOParser::s_parser()->insertFUNSequence(this);
}

RDOFUNSequence::~RDOFUNSequence()
{}

void RDOFUNSequence::initResult()
{
	initCalcSrcInfo();
	m_pNextCalc->m_res_real = true;
	switch (m_pHeader->getType()->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				m_pNextCalc->m_diap     = true;
				m_pNextCalc->m_diap_min = pRange->range()->getMin().value().getDouble();
				m_pNextCalc->m_diap_max = pRange->range()->getMax().value().getDouble();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			m_pNextCalc->m_res_real = true;
			LPRDOTypeRealRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				m_pNextCalc->m_diap     = true;
				m_pNextCalc->m_diap_min = pRange->range()->getMin().value().getDouble();
				m_pNextCalc->m_diap_max = pRange->range()->getMax().value().getDouble();
			}
			break;
		}
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	if (m_pInitCalc)
	{
		m_pInitCalc->setSrcInfo(m_pHeader->src_info());
		m_pInitCalc->setSrcText(_T("��������� ���� ������������������ ") + m_pInitCalc->src_text());
	}
	m_pNextCalc->setSrcInfo(m_pHeader->src_info());
}

// ----------------------------------------------------------------------------
// ---------- ������� ��������� �����
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("������������������ ���� uniform �� ����� ���������� �������� ������������� ����"));
	}
}

void RDOFUNSequenceUniform::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorUniform) pGenerator = new rdoRuntime::RandGeneratorUniform();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextUniform>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceUniform::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 2)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("��� ������������ ������ ������������� '%s' ����� ������� ��� ���������: ����������� � ������������ ������� ���������"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdoRuntime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPExpression pExpression = rdo::Factory<Expression>::create(m_pHeader->getType()->type(), pFuctionCall, pParamList->src_info());
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("���������� ������ �������"));
	}
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() != rdoRuntime::RDOType::t_int && m_pHeader->getType()->type()->typeID() != rdoRuntime::RDOType::t_real)
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("������������������ '%s' ����� ���������� �������� ������ ������ ��� ������������� ����"), src_text().c_str()));
	}
}

void RDOFUNSequenceExponential::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorExponential) pGenerator = new rdoRuntime::RandGeneratorExponential();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextExponential>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceExponential::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 1)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("��� ����������������� ������ ������������� '%s' ����� ������� ���� ��������: �������������� ��������"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

	pFuctionCall->addParameter(pArg1);

	LPExpression pExpression = rdo::Factory<Expression>::create(m_pHeader->getType()->type(), pFuctionCall, pParamList->src_info());
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("���������� ������ �������"));
	}
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("������������������ ���� normal �� ����� ���������� �������� ������������� ����"));
	}
}

void RDOFUNSequenceNormal::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorNormal) pGenerator = new rdoRuntime::RandGeneratorNormal();
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextNormal>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceNormal::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 2)
	{
		RDOParser::s_parser()->error().error(seq_src_info, rdo::format(_T("��� ����������� ������ ������������� '%s' ����� ������� ��� ���������: �������������� �������� � ������� �������������� ����������"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdoRuntime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPExpression pExpression = rdo::Factory<Expression>::create(m_pHeader->getType()->type(), pFuctionCall, pParamList->src_info());
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdoRuntime::RDOType::t_enum)
	{
		RDOParser::s_parser()->error().error(src_info(), _T("���������� ������ �������"));
	}
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- �����������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
RDOFUNSequenceByHist::RDOFUNSequenceByHist(CREF(LPRDOFUNSequenceByHistHeader) pHeader)
	: RDOFUNSequence(pHeader->getHeader(), pHeader->getSeed())
{}

LPRDOFUNArithm RDOFUNSequenceByHist::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList().size() != 0)
	{
		RDOParser::s_parser()->error().error(src_info, rdo::format(_T("����������� '%s' ������ ���������� ��� ����������"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPExpression pExpression = rdo::Factory<Expression>::create(m_pHeader->getType()->type(), pFuctionCall, pParamList->src_info());
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
RDOFUNSequenceByHistReal::RDOFUNSequenceByHistReal(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq)
	: RDOFUNSequenceByHist(pHeader)
{
	addReal(from, to, freq);
}

void RDOFUNSequenceByHistReal::addReal(CREF(RDOValue) from, CREF(RDOValue) to, CREF(RDOValue) freq)
{
	if (!m_to.empty() && from.value() != m_to.back())
	{
		RDOParser::s_parser()->error().error(from, _T("������ ��������� ������ ��������� � ���������� �����������"));
	}
	if (from.value() > to.value())
	{
		RDOParser::s_parser()->error().error(to, _T("������ ��������� ������ ���� ������ ��� �����"));
	}
	if (freq.value() == 0)
	{
		RDOParser::s_parser()->error().warning(freq, _T("������������� ����������� ������ ���� ������ ����"));
	}
	else if (freq.value() < 0)
	{
		RDOParser::s_parser()->error().error(freq, _T("������������� ����������� ������ ���� ������ ����"));
	}
	m_pHeader->getType()->value_cast(from);
	m_pHeader->getType()->value_cast(to  );
	m_from.push_back(from.value());
	m_to  .push_back(to  .value());
	m_freq.push_back(freq.value());
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorByHistReal) pGenerator = new rdoRuntime::RandGeneratorByHistReal();
	int size = m_from.size();
	for (int i = 0; i < size; i++)
	{
		pGenerator->addValues(m_from[i].getDouble(), m_to[i].getDouble(), m_freq[i].getDouble());
	}

	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
	initResult();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
RDOFUNSequenceByHistEnum::RDOFUNSequenceByHistEnum(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(RDOValue) value, CREF(RDOValue) freq)
	: RDOFUNSequenceByHist(pHeader)
{
	addEnum(value, freq);
}

void RDOFUNSequenceByHistEnum::addEnum(CREF(RDOValue) value, CREF(RDOValue) freq)
{
	if (freq.value() == 0)
	{
		RDOParser::s_parser()->error().warning(freq, _T("������������� ����������� ������ ���� ������ ����"));
	}
	else if (freq.value() < 0)
	{
		RDOParser::s_parser()->error().error(freq, _T("������������� ����������� ������ ���� ������ ����"));
	}
	m_pHeader->getType()->value_cast(value);
	rdoRuntime::RDOValue enum_id = m_pHeader->getType()->value_cast(value).value();
	if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
	{
		RDOParser::s_parser()->error().error(value, rdo::format(_T("������������ �������� ���������� ������: %s"), value->getIdentificator().c_str()));
	}
	m_values.push_back(enum_id);
	m_freq  .push_back(freq.value());
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorByHistEnum) pGenerator = new rdoRuntime::RandGeneratorByHistEnum();
	ruint size = m_values.size();
	for (ruint i = 0; i < size; i++)
	{
		pGenerator->addValues(m_values[i], m_freq[i].getDouble());
	}
	m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	RDOParser::s_parser()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- �������� ��������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
LPRDOFUNArithm RDOFUNSequenceEnumerative::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList().size() != 0)
	{
		RDOParser::s_parser()->error().error(src_info, rdo::format(_T("������������ '%s' ������ ���������� ��� ����������"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPExpression pExpression = rdo::Factory<Expression>::create(m_pHeader->getType()->type(), pFuctionCall, pParamList->src_info());
	ASSERT(pExpression);
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
	PTR(rdoRuntime::RandGeneratorEnumerative) pGenerator = new rdoRuntime::RandGeneratorEnumerative();
	for (ruint i = 0; i < m_valueList.size(); i++)
	{
		pGenerator->addValue(m_valueList[i].value());
	}
	m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- �������
// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElement
// ----------------------------------------------------------------------------
RDOFUNFunctionListElement::RDOFUNFunctionListElement(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{}

RDOFUNFunctionListElement::~RDOFUNFunctionListElement()
{}

rdoRuntime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(CREF(LPRDOTypeParam) pRetType, CREF(rdoRuntime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = createResultCalc(pRetType, src_pos);
	return rdo::Factory<rdoRuntime::RDOCalcIsEqual>::create(pFuncParam, pCalcConst);
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementIdentif
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementIdentif::RDOFUNFunctionListElementIdentif(CREF(RDOParserSrcInfo) src_info)
	: RDOFUNFunctionListElement(src_info)
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(RDOParserSrcInfo(src_pos.getPosAsYY(), src_text()))).value());
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementReal
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementReal::RDOFUNFunctionListElementReal(CREF(YYLTYPE) position, double value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%f"), value)))
	, m_value(value)
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(m_value, rdo::Factory<RDOType__real>::create(), src_pos)).value());
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementInt
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementInt::RDOFUNFunctionListElementInt(CREF(YYLTYPE) position, int value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%d"), value)))
	, m_value(value)
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdoRuntime::LPRDOCalcConst pCalcConst = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pRetType->value_cast(RDOValue(m_value, rdo::Factory<RDOType__int>::create(), src_pos)).value());
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementEq
// ----------------------------------------------------------------------------
RDOFUNFunctionListElementEq::RDOFUNFunctionListElementEq(CREF(YYLTYPE) position)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, _T("=")))
{}

rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	RDOParser::s_parser()->error().error(src_pos, _T("���������� ������ �������: RDOFUNFunctionListElementEq::createResultCalc"));
	NEVER_REACH_HERE;
	return NULL;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNCalculateIf
// ----------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf(CREF(LPRDOFUNLogic) pCondition, CREF(LPRDOFUNArithm) pAction)
	: m_pCondition(pCondition)
	, m_pAction   (pAction   )
{
	m_pAction->checkParamType(RDOParser::s_parser()->getLastFUNFunction()->getReturn()->getType());
}

RDOFUNCalculateIf::~RDOFUNCalculateIf()
{}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(src_info)
	, m_pReturn       (pReturn )
{

	LPContextPattern pContext = rdo::Factory<ContextPattern>::create();
	RDOParser::s_parser()->insertFUNFunction(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

RDOFUNFunction::RDOFUNFunction(CREF(tstring) name, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(name   )
	, m_pReturn       (pReturn)
{
	RDOParser::s_parser()->insertFUNFunction(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

void RDOFUNFunction::end()
{
	RDOParser::s_parser()->contextStack()->pop();
}

LPContext RDOFUNFunction::onFindContext(CREF(RDOValue) value) const
{
	//! ���������
	LPRDOParam pParam = findFUNFunctionParam(value->getIdentificator());
	if (pParam)
	{
		return const_cast<PTR(RDOFUNFunction)>(this);
	}

	return NULL;
}

LPExpression RDOFUNFunction::onCreateExpression(CREF(RDOValue) value)
{
	//! ���������
	LPRDOParam pParam = findFUNFunctionParam(value->getIdentificator());
	if (pParam)
	{
		LPExpression pExpression = rdo::Factory<Expression>::create(
			pParam->getType()->type(),
			rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(findFUNFunctionParamNum(value->getIdentificator()), pParam->src_info()),
			value.src_info()
		);
		ASSERT(pExpression);
		return pExpression;
	}

	NEVER_REACH_HERE;
	return NULL;
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::setFunctionCalc(CREF(rdoRuntime::LPRDOFunCalc) pCalc)
{
	m_pFunctionCalc = pCalc;
	if (m_pFunctionCalc->src_empty())
	{
		m_pFunctionCalc->setSrcInfo(src_info());
	}
	STL_FOR_ALL(m_postLinkedList, it)
	{
		(*it)->setFunctionCalc(getFunctionCalc());
	}
}

LPRDOParam RDOFUNFunction::findFUNFunctionParam(CREF(tstring) paramName) const 
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? *it : NULL;
}

int RDOFUNFunction::findFUNFunctionParamNum(CREF(tstring) paramName) const
{
	ParamList::const_iterator it = std::find_if(m_paramList.begin(), m_paramList.end(), compareName<RDOParam>(paramName));
	return it != m_paramList.end() ? it - m_paramList.begin() : -1;
}

void RDOFUNFunction::add(CREF(LPRDOParam) pParam)
{ 
	LPRDOParam pParamPrev = findFUNFunctionParam(pParam->name());
	if (pParamPrev)
	{
		RDOParser::s_parser()->error().push_only(pParam->src_info(), rdo::format(_T("�������� ��� ����������: %s"), pParam->name().c_str()));
		RDOParser::s_parser()->error().push_only(pParamPrev->src_info(), _T("��. ������ �����������"));
		RDOParser::s_parser()->error().push_done();
	}
	m_paramList.push_back(pParam); 
}

void RDOFUNFunction::add(CREF(LPRDOFUNFunctionListElement) pParam)
{
	m_elementList.push_back(pParam); 
}

void RDOFUNFunction::add(CREF(LPRDOFUNCalculateIf) pCalculateIf)
{
	m_calculateIfList.push_back(pCalculateIf);
}

void RDOFUNFunction::createListCalc()
{
	if (!m_pReturn->getDefault().defined())
	{
		RDOParser::s_parser()->error().error(m_pReturn->src_info(), rdo::format(_T("������� '%s' ������ ����� �������� ��-���������"), name().c_str()));
	}
	if (m_paramList.empty())
	{
		RDOParser::s_parser()->error().error(src_info(), rdo::format(_T("������� '%s' ������ ����� ���������, �.�. � ��� list"), name().c_str()));
	}
	if (m_elementList.empty())
	{
		RDOParser::s_parser()->error().warning(src_info(), rdo::format(_T("������� '%s' �� �������� ������"), name().c_str()));
	}

	rdoRuntime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pReturn->getDefault().value());
	ASSERT(pCalcDefault);
	pCalcDefault->setSrcInfo(m_pReturn->getDefault().src_info());
	rdoRuntime::LPRDOFunListCalc pFunListCalc = rdo::Factory<rdoRuntime::RDOFunListCalc>::create(pCalcDefault);
	ASSERT(pFunListCalc);
	pFunListCalc->setSrcInfo(src_info());
	ElementList::const_iterator elem_it = m_elementList.begin();
	while (elem_it != m_elementList.end())
	{
		rdoRuntime::LPRDOCalc pCaseCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(true);
		ASSERT(pCaseCalc);
		pCaseCalc->setSrcInfo((*elem_it)->src_info());
		int currParamNumber = 0;
		ParamList::const_iterator param_it = m_paramList.begin();
		while (param_it != m_paramList.end())
		{
			LPRDOParam pParam = *param_it;
			if (elem_it == m_elementList.end())
			{
				--elem_it;
				RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("��������� �������� ��� ��������� '%s'"), pParam->name().c_str()));
			}
			LPRDOFUNFunctionListElement pListElement = *elem_it;
			ASSERT(pListElement);
			if (pListElement->isEquivalence())
			{
				if ((param_it + 1) == m_paramList.end())
				{
					RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format(_T("����� ������ ��������� ��������� �������� ��� ���������: %s"),pParam->name().c_str()));
				}
				else
				{
					tstring str = (*param_it)->src_text();
					++param_it;
					while (param_it != m_paramList.end())
					{
						str += _T(", ");
						str += (*param_it)->src_text();
						++param_it;
					}
					RDOParser::s_parser()->error().error(pListElement->src_info(), rdo::format(_T("����� ������ ��������� ��������� �������� ��� ����������: %s"), str.c_str()));
				}
			}
			rdoRuntime::LPRDOCalcFuncParam pFuncParam   = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(currParamNumber, pParam->src_info());
			ASSERT(pFuncParam);
			rdoRuntime::LPRDOCalcIsEqual   pCompareCalc = pListElement->createIsEqualCalc(pParam->getType(), pFuncParam, pListElement->src_info());
			ASSERT(pCompareCalc);
			rdoRuntime::LPRDOCalc          pAndCalc     = rdo::Factory<rdoRuntime::RDOCalcAnd>::create(pCaseCalc, pCompareCalc);
			ASSERT(pAndCalc);
			pCaseCalc = pAndCalc;
			++elem_it;
			++param_it;
			++currParamNumber;
		}
		if (elem_it == m_elementList.end())
		{
			--elem_it;
			--param_it;
			RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("����� �������� ��� ��������� '%s' ��������� ���� ���������"), (*param_it)->name().c_str()));
		}
		if (!(*elem_it)->isEquivalence())
		{
			RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("������� ����� ��������, ������ '%s' ��������� ���� ���������"), (*elem_it)->src_text().c_str()));
		}
		++elem_it;
		if (elem_it == m_elementList.end())
		{
			--elem_it;
			RDOParser::s_parser()->error().error((*elem_it)->src_info(), rdo::format(_T("����� ����� ��������� ��������� �������� ������� '%s'"), name().c_str()));
		}
		rdoRuntime::LPRDOCalcConst pResultCalc = (*elem_it)->createResultCalc(m_pReturn->getType(), (*elem_it)->src_info());
		pFunListCalc->addCase(pCaseCalc, pResultCalc);
		++elem_it;
	}
	setFunctionCalc(pFunListCalc);
	end();
}

void RDOFUNFunction::createTableCalc(CREF(YYLTYPE) elements_pos)
{
	ElementList::const_iterator it = m_elementList.begin();
	while (it != m_elementList.end())
	{
		if ((*it)->isEquivalence())
		{
			RDOParser::s_parser()->error().error((*it)->src_info(), _T("������ '=' ���������� � ��������� �������"));
		}
		++it;
	}
	int param_cnt = m_paramList.size();
	int range     = 1;
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(0);
	ASSERT(pCalc);
	pCalc->setSrcInfo(src_info());
	pCalc->setSrcText(_T("0"));
	for (int currParam = 0; currParam < param_cnt; currParam++)
	{
		LPRDOParam pFunctionParam = m_paramList.at(currParam);
		ASSERT(pFunctionParam);
		rdoRuntime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(currParam, pFunctionParam->src_info());
		ASSERT(pFuncParam);
		rdoRuntime::LPRDOCalc pValue2 = pFuncParam;
		if (pFunctionParam->getType()->type()->typeID() != rdoRuntime::RDOType::t_enum)
		{
			rdoRuntime::LPRDOCalcConst pCalcConst1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			pCalcConst1->setSrcInfo(pFunctionParam->src_info());
			pValue2 = rdo::Factory<rdoRuntime::RDOCalcMinus>::create(pValue2, pCalcConst1);
		}
		rdoRuntime::LPRDOCalcConst pCalcConst2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(range);
		ASSERT(pCalcConst2);
		pCalcConst2->setSrcInfo(pFunctionParam->src_info());
		rdoRuntime::LPRDOCalcMult pCalcMult = rdo::Factory<rdoRuntime::RDOCalcMultEnumSafe>::create(pCalcConst2, pValue2);
		rdoRuntime::LPRDOCalcPlus pCalcAdd  = rdo::Factory<rdoRuntime::RDOCalcPlusEnumSafe>::create(pCalcMult, pCalc);

		rbool found = false;
		switch (pFunctionParam->getType()->type()->typeID())
		{
		case rdoRuntime::RDOType::t_int:
			if (dynamic_cast<PTR(RDOTypeIntRange)>(pFunctionParam->getType()->type().get()))
			{
				LPRDOTypeIntRange pRange = pFunctionParam->getType()->type().object_static_cast<RDOTypeIntRange>();
				if (pRange->range()->getMin().value().getInt() != 1)
				{
					RDOParser::s_parser()->error().error(pRange->range()->src_info(), rdo::format(_T("����������� �������� ��������� ������ ���� 1, ������� �������� [%d..%d]"), pRange->range()->getMin().value().getInt(), pRange->range()->getMax().value().getInt()));
				}
				range *= pRange->range()->getMax().value().getInt() - pRange->range()->getMin().value().getInt() + 1;
			}
			else
			{
				RDOParser::s_parser()->error().error(pFunctionParam->getType()->src_info(), _T("��� ��������� ��������� ������� ������ ���� ����� ���������� ��������"));
			}
			break;
		case rdoRuntime::RDOType::t_enum:
			range *= pFunctionParam->getType()->type().object_static_cast<RDOEnumType>()->getEnums()->getValues().size();
			break;
		default:
			RDOParser::s_parser()->error().error(pFunctionParam->getType()->src_info(), _T("�������� ��������� ������� ����� ���� ������ ��� ������������� ����"));
		}
		pCalc = pCalcAdd;
	}

	if (m_elementList.size() != range)
	{
		RDOParser::s_parser()->error().error(elements_pos, rdo::format(_T("�������� ���������� ��������� ��������� ������� '%s': %d, ��������� %d"), name().c_str(), m_elementList.size(), range));
	}

	rdoRuntime::LPRDOFuncTableCalc pFuncTableCalc = rdo::Factory<rdoRuntime::RDOFuncTableCalc>::create(pCalc);
	ASSERT(pFuncTableCalc);
	pFuncTableCalc->setSrcInfo(src_info());
	for (int currElem = 0; currElem < range; currElem++)
	{
		LPRDOFUNFunctionListElement pListElement = m_elementList.at(currElem);
		ASSERT(pListElement);
		rdoRuntime::LPRDOCalcConst  pResultCalc  = pListElement->createResultCalc(m_pReturn->getType(), pListElement->src_info());
		ASSERT(pResultCalc);
		pFuncTableCalc->addResultCalc(pResultCalc);
	}
	setFunctionCalc(pFuncTableCalc);
	end();
}

void RDOFUNFunction::createAlgorithmicCalc(CREF(RDOParserSrcInfo) body_src_info)
{
	if(!getFunctionCalc()->isReturn()) RDOParser::s_parser()->error().error(body_src_info, rdo::format(_T("�� �������������� ����������� �������� �������")));
	end();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup(CREF(RDOParserSrcInfo) res_info)
	: RDOParserSrcInfo(res_info)
{
	init(res_info);
}

RDOFUNGroup::~RDOFUNGroup()
{}

void RDOFUNGroup::init(CREF(RDOParserSrcInfo) res_info)
{
	m_pResType = RDOParser::s_parser()->findRTPResType(res_info.src_text());
	if (!m_pResType)
	{
		RDOParser::s_parser()->error().error(res_info, rdo::format(_T("����������� ��� �������: %s"), res_info.src_text().c_str()));
	}
	RDOParser::s_parser()->insertFUNGroup(this);
	RDOParser::s_parser()->contextStack()->push(this);
}

void RDOFUNGroup::end()
{
	RDOParser::s_parser()->contextStack()->pop();
}

LPContext RDOFUNGroup::onFindContext(CREF(RDOValue) value) const
{
	if (getResType()->name() == value->getIdentificator())
	{
		return const_cast<PTR(RDOFUNGroup)>(this);
	}

	return NULL;
}

LPExpression RDOFUNGroup::onCreateExpression(CREF(RDOValue) value)
{
	//! ������ ������ ��������� �������
	ruint parNumb = getResType()->getRTPParamNumber(value->getIdentificator());
	if (parNumb == RDORTPResType::UNDEFINED_PARAM)
	{
		RDOParser::s_parser()->error().error(value.src_info(), rdo::format(_T("����������� �������� �������: %s"), value->getIdentificator().c_str()));
	}

	LPExpression pExpression = rdo::Factory<Expression>::create(
		getResType()->findRTPParam(value->getIdentificator())->getType()->type(),
		rdo::Factory<rdoRuntime::RDOCalcGetGroupResParam>::create(parNumb),
		value.src_info()
	);
	ASSERT(pExpression);
	return pExpression;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
RDOFUNGroupLogic::RDOFUNGroupLogic(FunGroupType funType, CREF(RDOParserSrcInfo) res_info)
	: RDOFUNGroup(res_info)
	, m_funType  (funType )
{}

LPRDOFUNLogic RDOFUNGroupLogic::createFunLogic(REF(LPRDOFUNLogic) pCondition)
{
	rdoRuntime::LPRDOFunCalcGroup pCalc;
	switch (m_funType)
	{
	case fgt_exist    : setSrcText(_T("Exist(")     + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notexist : setSrcText(_T("NotExist(")  + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_forall   : setSrcText(_T("ForAll(")    + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notforall: setSrcText(_T("NotForAll(") + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(src_info(), _T("���������� ������: �������������� ��� �������"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(rdo::Factory<RDOType__bool>::create(), pCalc, pCalc->src_info());
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(src_info());
	return pLogic;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
RDOFUNSelect::RDOFUNSelect(CREF(RDOParserSrcInfo) res_info)
	: RDOFUNGroup(RDOParserSrcInfo(res_info.src_text()))
{}

//! ��� Select ��� ������� �� ���� � �������
void RDOFUNSelect::initSelect(LPRDOFUNLogic pCondition)
{
	m_pCalcSelect = rdo::Factory<rdoRuntime::RDOFunCalcSelect>::create(getResType()->getNumber(), pCondition->getCalc());
	m_pCalcSelect->setSrcInfo(pCondition->src_info());
}

//! ��������� ������� ��� �������� Select'�
LPRDOFUNLogic RDOFUNSelect::createFunSelectGroup(RDOFUNGroupLogic::FunGroupType funType, REF(LPRDOFUNLogic) pCondition)
{
	ASSERT(pCondition);
	rdoRuntime::LPRDOFunCalcSelectBase pCalc;
	switch (funType)
	{
	case RDOFUNGroupLogic::fgt_exist    : setSrcText(src_text() + _T(".Exist(")     + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectExist    >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notexist : setSrcText(src_text() + _T(".NotExist(")  + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotExist >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_forall   : setSrcText(src_text() + _T(".ForAll(")    + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectForAll   >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notforall: setSrcText(src_text() + _T(".NotForAll(") + pCondition->src_text() + _T(")")); pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc()); break;
	default: RDOParser::s_parser()->error().error(pCondition->src_info(), _T("���������� ������: ����������� ����� ��� ������ ��������"));
	}
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(rdo::Factory<RDOType__bool>::create(), pCalc, pCalc->src_info());
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(src_info());
	return pLogic;
}

LPRDOFUNLogic RDOFUNSelect::createFunSelectEmpty(CREF(RDOParserSrcInfo) empty_info)
{
	setSrcText(src_text() + _T(".") + empty_info.src_text());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect);
	ASSERT(pCalc);
	pCalc->setSrcInfo(src_info());

	LPExpression pExpression = rdo::Factory<Expression>::create(rdo::Factory<RDOType__bool>::create(), pCalc, pCalc->src_info());
	ASSERT(pExpression);

	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pExpression, false);
	ASSERT(pLogic);

	pLogic->setSrcInfo(empty_info);
	return pLogic;
}

LPRDOFUNArithm RDOFUNSelect::createFunSelectSize(CREF(RDOParserSrcInfo) size_info)
{
	setSrcText(src_text() + _T(".") + size_info.src_text());
	RDOParser::s_parser()->getFUNGroupStack().pop_back();
	end();

	LPExpression pExpression = rdo::Factory<Expression>::create(rdo::Factory<RDOType__int>::create(), rdo::Factory<rdoRuntime::RDOFunCalcSelectSize>::create(m_pCalcSelect), size_info);
	ASSERT(pExpression);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
	ASSERT(pArithm);

	pArithm->setSrcInfo(size_info);
	return pArithm;
}

CLOSE_RDO_PARSER_NAMESPACE
