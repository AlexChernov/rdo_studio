/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofun.cpp
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
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdo_type_range.h"
#include "simulator/runtime/rdo_random_distribution.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdoframe.h"
#include "simulator/runtime/calc/calc_process.h"
#include "simulator/runtime/calc/calc_pattern.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
#include "simulator/runtime/calc/function/calc_function_system.h"
#include "simulator/runtime/calc/operation/calc_arithm.h"
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

int cnv_funlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_funerror(PTR(char) message)
{
	UNUSED(message);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNDoubleToIntByResult
// --------------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::roundCalc()
{
	STL_FOR_ALL(m_intOrDouble, it)
	{
		(*it)->needRound();
	}
}

// --------------------------------------------------------------------------------
// -------------------- ����� �������� ��� ��������� ���������� � �������������� ���������
// --------------------------------------------------------------------------------
#define CREATE_CALC(CALC, OPR) \
rdo::runtime::LPRDOCalcConst pConstCalc1 = m_pCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>(); \
rdo::runtime::LPRDOCalcConst pConstCalc2 = pSecond->m_pCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>(); \
rdo::runtime::LPRDOCalc pNewCalc; \
if (pConstCalc1 && pConstCalc2) \
{ \
	pNewCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pConstCalc1->getValue() OPR pConstCalc2->getValue()); \
	pNewCalc->setSrcInfo(rdo::runtime::RDOCalc##CALC::getStaticSrcInfo(pConstCalc1, pConstCalc2)); \
} \
else \
{ \
	pNewCalc = rdo::Factory<rdo::runtime::RDOCalc##CALC>::create(m_pCalc, pSecond->m_pCalc); \
}

#define RETURN_LOGIC() \
LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false); \
pLogic->setSrcInfo(pNewCalc->srcInfo()); \
pLogic->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble); \
return pLogic;

#define GENERATE_LOGIC(CALC, OPR) \
CREATE_CALC(CALC, OPR); \
RETURN_LOGIC();

#define CAST_ARITHM_VALUE(OPR, ERROR) \
try \
{ \
	if (beforeCastValue(pSecond) == CR_CONTINUE) \
	{ \
		value()->value() OPR pSecond->value()->value(); \
	} \
} \
catch (REF(rdo::runtime::RDOValueException)) \
{ \
	Converter::s_converter()->error().error(pSecond->src_info(), rdo::format(ERROR, type()->name().c_str(), pSecond->type()->name().c_str())); \
}

#define GET_ARITHM_PRE_TYPE() \
LPRDOType pNewType = getPreType(pSecond);

#define GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
CAST_ARITHM_VALUE(OPR, ERROR) \
GET_ARITHM_PRE_TYPE() \
CREATE_CALC(CALC, OPR)

#define RETURN_ARITHM() \
LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(rdo::Factory<RDOValue>::create(pNewType, pNewCalc->srcInfo()), pNewCalc); \
pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble); \
return pArithm;

#define GENERATE_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_ARITHM()

#define GENERATE_LOGIC_FROM_ARITHM(CALC, OPR, ERROR) \
GENERATE_ARITHM_CALC(CALC, OPR, ERROR) \
RETURN_LOGIC()

// --------------------------------------------------------------------------------
// -------------------- RDOFUNLogic
// --------------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic(CREF(LPRDOFUNArithm) pArithm)
	: RDOParserSrcInfo(pArithm->src_info())
{
	switch (pArithm->typeID())
	{
	case rdo::runtime::RDOType::t_bool: m_pCalc = pArithm->calc(); break;
	}
	if (!m_pCalc)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("��������� ���������� ���������, �������: %s"), src_text().c_str()));
	}
}

RDOFUNLogic::RDOFUNLogic(CREF(rdo::runtime::LPRDOCalc) pCalc, rbool hideWarning)
	: RDOParserSrcInfo(     )
	, m_pCalc         (pCalc)
{
	if (m_pCalc)
	{
		rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcFileType(src_filetype());
		m_pCalc->setSrcInfo(srcInfo);
	}
	if (!hideWarning)
	{
		rdo::runtime::LPRDOCalcConst pConstCalc = m_pCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>();
		if (pConstCalc)
		{
			if (pConstCalc->calcValue(Converter::s_converter()->runtime()).getAsBool())
			{
				Converter::s_converter()->error().warning(pConstCalc->srcInfo(), rdo::format(_T("���������� ��������� ������ �������: %s"), pConstCalc->srcInfo().src_text().c_str()));
			}
			else
			{
				Converter::s_converter()->error().warning(pConstCalc->srcInfo(), rdo::format(_T("���������� ��������� ������ �����: %s"), pConstCalc->srcInfo().src_text().c_str()));
			}
		}
	}
}

RDOFUNLogic::~RDOFUNLogic()
{}

rdo::runtime::LPRDOCalc RDOFUNLogic::getCalc(rdo::runtime::RDOType::TypeID id)
{
	if (id != rdo::runtime::RDOType::t_real)
	{
		m_intOrDouble.roundCalc();
	}
	return m_pCalc;
}

LPRDOFUNLogic RDOFUNLogic::operator&& (CREF(LPRDOFUNLogic) pSecond)
{
	GENERATE_LOGIC(And, &&);
}

LPRDOFUNLogic RDOFUNLogic::operator|| (CREF(LPRDOFUNLogic) pSecond)
{
	GENERATE_LOGIC(Or, ||);
}

LPRDOFUNLogic RDOFUNLogic::operator_not()
{
	rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcNot>::create(m_pCalc->srcInfo().src_pos(), m_pCalc);
	LPRDOFUNLogic         pLogic   = rdo::Factory<RDOFUNLogic>::create(pNewCalc, false);
	pLogic->setSrcInfo(pNewCalc->srcInfo());
	pLogic->m_intOrDouble.insert(m_intOrDouble);
	return pLogic;
}

void RDOFUNLogic::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	if (m_pCalc)
	{
		m_pCalc->setSrcInfo(src_info);
	}
}

void RDOFUNLogic::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	if (m_pCalc)
	{
		rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcPos(position);
		m_pCalc->setSrcInfo(srcInfo);
	}
}

void RDOFUNLogic::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	if (m_pCalc)
	{
		rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcText(value);
		m_pCalc->setSrcInfo(srcInfo);
	}
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) error_pos)
{
	RDOParserSrcInfo::setSrcPos(error_pos);
}

void RDOFUNLogic::setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
{
	RDOParserSrcInfo::setSrcPos(pos_begin, pos_end);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNArithm
// --------------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm(CREF(LPRDOValue) pValue, CREF(rdo::runtime::LPRDOCalc) pCalc)
	: m_pValue(pValue)
	, m_pCalc (pCalc )
{
	setSrcInfo(m_pValue->src_info());
}

RDOFUNArithm::RDOFUNArithm(CREF(LPRDOValue) pValue)
{
	init(pValue);
}

RDOFUNArithm::RDOFUNArithm(CREF(LPRDOValue) pResName, CREF(LPRDOValue) pParName)
{
	init(pResName, pParName);
}

RDOFUNArithm::~RDOFUNArithm()
{}

void RDOFUNArithm::init(CREF(LPRDOValue) pValue)
{
	setSrcInfo(pValue->src_info());

	if (pValue->constant())
	{
		m_pValue = rdo::Factory<RDOValue>::create(pValue);
		m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pValue->value());
		m_pCalc->setSrcInfo(src_info());
		return;
	}

	if (pValue->value().getIdentificator() == _T("Time_now") || pValue->value().getIdentificator() == _T("time_now") || pValue->value().getIdentificator() == _T("���������_�����") || pValue->value().getIdentificator() == _T("���������_�����"))
	{
		m_pValue = rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__real>::create().object_parent_cast<RDOType>());
		m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetTimeNow>::create();
		m_pCalc->setSrcInfo(src_info());
		return;
	}
	else if (pValue->value().getIdentificator() == _T("Terminate_counter") || pValue->value().getIdentificator() == _T("terminate_counter"))
	{
		m_pValue = rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__int>::create().object_parent_cast<RDOType>());
		m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetTermNow>::create();
		m_pCalc->setSrcInfo(src_info());
		return;
	}
	else if (pValue->value().getIdentificator() == _T("Seconds") || pValue->value().getIdentificator() == _T("seconds"))
	{
		m_pValue = rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__real>::create().object_parent_cast<RDOType>());
		m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetSeconds>::create();
		m_pCalc->setSrcInfo(src_info());
		return;
	}

	//! ��������, ��� ��� �������� ������������� ����, ������ ���� � ���� �������� ����� ����������� � ������
	//! ������������ �����, ������� ����� ������ �� ��� ������� - ������
	{ErrorBlockMonicker errorBlockMonicker;
		CREF(Converter::PreCastTypeList) typeList = Converter::s_converter()->getPreCastTypeList();
		STL_FOR_ALL_CONST(typeList, it)
		{
			LPRDOValue pTryCastValue = (*it)->value_cast(pValue);
			if (pTryCastValue->defined())
			{
				m_pValue = rdo::Factory<RDOValue>::create(pValue);
				return;
			}
		}
	}

	//! ���� �������� ������������ �������
	if (Converter::s_converter()->getFileToParse() == rdo::converter::smr2rdox::PAT_IN)
	{
		LPRDOPATPattern pPattern = Converter::s_converter()->getLastPATPattern();
		if (pPattern && pPattern->m_pCurrRelRes)
		{
			LPRDORTPParam pParam = pPattern->m_pCurrRelRes->getType()->findRTPParam(pValue->value().getIdentificator());
			if (pParam)
			{
				LPRDOValue pParamName = rdo::Factory<RDOValue>::create(pPattern->m_pCurrRelRes->m_bodySrcInfo);
				init(pParamName, pValue);
				return;
			}
		}
	}

	//! ���� ��������� �������� ��� ������� �� �����
	LPRDOParam pFunctionParam;
	switch (Converter::s_converter()->getFileToParse())
	{
	case rdo::converter::smr2rdox::PAT_IN: pFunctionParam = Converter::s_converter()->getLastPATPattern ()->findPATPatternParam (pValue->value().getIdentificator()); break;
	case rdo::converter::smr2rdox::FUN_IN: pFunctionParam = Converter::s_converter()->getLastFUNFunction()->findFUNFunctionParam(pValue->value().getIdentificator()); break;
	}

	//! ���� ��������� �� �����
	LPRDOFUNConstant pConstant = Converter::s_converter()->findFUNConstant(pValue->value().getIdentificator());

	if (pConstant && pFunctionParam)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("��� ��������� ������� ��������� � ������ ���������: %s"), pValue->value().getIdentificator().c_str()));
	}

	if (pConstant)
	{
		m_pValue = rdo::Factory<RDOValue>::create(pConstant->getType()->type());
		m_pCalc  = rdo::Factory<rdo::runtime::RDOCalcGetConst>::create(pConstant->getNumber());
		m_pCalc->setSrcInfo(src_info());
		return;
	}

	//! ���� ������������������ �� �����
	LPRDOFUNSequence pSequence = Converter::s_converter()->findFUNSequence(pValue->value().getIdentificator());
	if (pSequence && pFunctionParam)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("��� ��������� ������� ��������� � ������ ������������������: %s"), pValue->value().getIdentificator().c_str()));
	}

	if (pSequence)
	{
		LPRDOFUNParams pParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pParams);
		LPRDOFUNArithm pArithm = pParams->createSeqCall(pValue->value().getIdentificator());
		ASSERT(pArithm);
		pArithm->setSrcInfo(src_info());
		m_pValue =rdo::Factory<RDOValue>::create(pArithm->value());
		m_pCalc  = pArithm->m_pCalc;
		return;
	}

	if (pFunctionParam)
	{
		//! ��� ��������
		m_pValue = rdo::Factory<RDOValue>::create(pFunctionParam->getType()->type());
		switch (Converter::s_converter()->getFileToParse())
		{
		case rdo::converter::smr2rdox::PAT_IN: m_pCalc = rdo::Factory<rdo::runtime::RDOCalcPatParam> ::create(Converter::s_converter()->getLastPATPattern ()->findPATPatternParamNum (pValue->value().getIdentificator())); break;
		case rdo::converter::smr2rdox::FUN_IN: m_pCalc = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(Converter::s_converter()->getLastFUNFunction()->findFUNFunctionParamNum(pValue->value().getIdentificator()), pFunctionParam->src_info()); break;
		}
		if (m_pCalc)
		{
			m_pCalc->setSrcInfo(src_info());
		}
		return;
	}

	Converter::s_converter()->error().error(pValue->src_info(), rdo::format(_T("����������� �������������: %s"), pValue->value().getIdentificator().c_str()));
}

void RDOFUNArithm::init(CREF(LPRDOValue) pResName, CREF(LPRDOValue) pParName)
{
	setSrcInfo(pResName->src_info(), _T("."), pParName->src_info());
	LPRDORSSResource pResource = Converter::s_converter()->findRSSResource(pResName->value().getIdentificator());
	if (pResource)
	{
		//! ��� ������ � �������� RSS
		ruint parNumb = pResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
		if (parNumb == RDORTPResType::UNDEFINED_PARAM)
		{
			Converter::s_converter()->error().error(pParName->src_info(), rdo::format(_T("����������� �������� �������: %s"), pParName->value().getIdentificator().c_str()));
		}
		if (pResource->getType()->isPermanent())
		{
			m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
		}
		else if (pResource->getType()->isTemporary() && Converter::s_converter()->getFileToParse() == rdo::converter::smr2rdox::FRM_IN)
		{
			m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
		}
		else
		{
			Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("������ ������������ ��������� ������: %s"), pResName->value().getIdentificator().c_str()));
		}
		m_pCalc->setSrcInfo(src_info());
		m_pValue = rdo::Factory<RDOValue>::create(pResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
		return;
	}
	//! ��� �� ������, �� ��������, ������ ������ ��������� �������
	else if (!Converter::s_converter()->getFUNGroupStack().empty() && Converter::s_converter()->getFUNGroupStack().back()->getResType()->name() == pResName->value().getIdentificator())
	{
		//! ��� ������ ������ ��������� �������
		LPRDOFUNGroup pFUNGroup = Converter::s_converter()->getFUNGroupStack().back();
		ruint         parNumb   = pFUNGroup->getResType()->getRTPParamNumber(pParName->value().getIdentificator());
		if (parNumb == RDORTPResType::UNDEFINED_PARAM)
		{
			Converter::s_converter()->error().error(pParName->src_info(), rdo::format(_T("����������� �������� �������: %s"), pParName->value().getIdentificator().c_str()));
		}
		m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
		m_pCalc->setSrcInfo(src_info());
		m_pValue = rdo::Factory<RDOValue>::create(pFUNGroup->getResType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
		return;
	}
	else
	{
		//! ��������, ��� ����������� ������
		switch (Converter::s_converter()->getFileToParse())
		{
		case rdo::converter::smr2rdox::PAT_IN:
			if (Converter::s_converter()->getLastPATPattern() && Converter::s_converter()->getLastPATPattern()->findRelevantResource(pResName->value().getIdentificator()))
			{
				//! ��� ����������� ������ ���-�� � �������� (with_min-common-choice, $Time, $Body)
				LPRDOPATPattern       pPattern          = Converter::s_converter()->getLastPATPattern();
				LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(pResName->value().getIdentificator());
				if (!pPattern->m_pCurrRelRes)
				{
					//! ������ with_min-common-choice ��� $Time
					if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_NonExist || pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Create)
					{
						Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ �� ����� ���� �����������, �.�. �� ��� �� ����������: %s"), pRelevantResource->name().c_str()));
					}
				}
				else
				{
					//! ������ $Body
					//! ��������� ������������� ��������������������� ���.������� (pRelevantResource) � Choice from ������ ���.������� (pPattern->m_pCurrRelRes)
					if (pPattern->m_pCurrRelRes->isChoiceFromState())
					{
						if (!pRelevantResource->m_alreadyHaveConverter && !pRelevantResource->isDirect())
						{
							Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ �� ���������: %s. ��� ������ ������������ � �������� ������ ������ �������� �� ��� ������������ Choice from"), pRelevantResource->name().c_str()));
						}
						if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_NonExist)
						{
							Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ � ������ �������� �� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
						}
						if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Create)
						{
							Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����� ����� �������� (Create) ����������� ������ '%s' ����� ������������ ������ � �����������, �� �� � ������� ������"), pRelevantResource->name().c_str()));
						}
					}
					//! ��������� ������������� ���������� ���.������� ������ ���������� ������� ���.�������
					if (pRelevantResource->getType()->isTemporary())
					{
						//! � ���������� ������
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin)
						{
							if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ ������ ������������ �� ��� �������� (Create): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ ������ ������������ ����� �������� (Erase): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusBegin == rdo::runtime::RDOResource::CS_NonExist)
							{
								Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ �� ���������� � ���� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
							}
						}
						//! � ���������� �����
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd)
						{
							if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::CS_Create && !pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ ������ ������������ �� ��� �������� (Create): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::CS_Erase && pRelevantResource->m_alreadyHaveConverter)
							{
								Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ ������ ������������ ����� �������� (Erase): %s"), pRelevantResource->name().c_str()));
							}
							if (pRelevantResource->m_statusEnd == rdo::runtime::RDOResource::CS_NonExist)
							{
								Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������ �� ���������� � ���� ���������� (NonExist): %s"), pRelevantResource->name().c_str()));
							}
						}
					}
					//! ��������� ������������� ��� �� ������������������� (������ ��� Create) ��������� ���. ������� � ��� �� ����������
					LPRDORTPParam pParam = pPattern->m_pCurrRelRes->getType()->findRTPParam(pParName->value().getIdentificator());
					if (pParam && pPattern->m_pCurrRelRes->name() == pResName->value().getIdentificator())
					{
						//! � ���������� ������
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin && pPattern->m_pCurrRelRes->m_statusBegin == rdo::runtime::RDOResource::CS_Create)
						{
							if (!pPattern->m_pCurrRelRes->getParamSetList().find(pParName->value().getIdentificator()))
							{
								if (!pParam->getDefault()->defined())
								{
									Converter::s_converter()->error().error(pParName->src_info(), rdo::format(_T("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� �� ��������� � ���� �������"), pParName->value().getIdentificator().c_str()));
								}
							}
						}
						//! � ���������� �����
						if (pPattern->m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd && pPattern->m_pCurrRelRes->m_statusEnd == rdo::runtime::RDOResource::CS_Create)
						{
							if (!pPattern->m_pCurrRelRes->getParamSetList().find(pParName->value().getIdentificator()))
							{
								if (!pPattern->m_pCurrRelRes->getParamSetList().find(pParName->value().getIdentificator()))
								{
									if (!pParam->getDefault()->defined())
									{
										Converter::s_converter()->error().error(pParName->src_info(), rdo::format(_T("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� �� ��������� � ���� �������"), pParName->value().getIdentificator().c_str()));
									}
								}
							}
						}
					}
				}
				ruint parNumb = pRelevantResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
				if (parNumb == RDORTPResType::UNDEFINED_PARAM)
				{
					Converter::s_converter()->error().error(pParName->src_info(), rdo::format(_T("����������� �������� �������: %s"), pParName->value().getIdentificator().c_str()));
				}
				m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
				m_pCalc->setSrcInfo(src_info());
				m_pValue = rdo::Factory<RDOValue>::create(pRelevantResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
				return;
			}
			break;

		case rdo::converter::smr2rdox::DPT_IN:
			if (Converter::s_converter()->isCurrentDPTSearch() && Converter::s_converter()->getLastDPTSearch()->getLastActivity())
			{
				LPRDOPATPattern pPattern = Converter::s_converter()->getLastDPTSearch()->getLastActivity()->pattern();
				if (pPattern && pPattern->findRelevantResource(pResName->value().getIdentificator())) {
					//! ��� ������, ������� ������������ � DPT (condition, term_condition, evaluate_by, value before, value after)
					LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(pResName->value().getIdentificator());
					ruint                 parNumb           = pRelevantResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
					if (parNumb == RDORTPResType::UNDEFINED_PARAM)
					{
						Converter::s_converter()->error().error(pParName->src_info(), rdo::format(_T("����������� �������� �������: %s"), pParName->value().getIdentificator().c_str()));
					}
					m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
					m_pCalc->setSrcInfo(src_info());
					m_pValue = rdo::Factory<RDOValue>::create(pRelevantResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
					return;
				}
			}
			if (Converter::s_converter()->isCurrentDPTPrior() && Converter::s_converter()->getLastDPTPrior()->getLastActivity())
			{
				LPRDOPATPattern pPattern = Converter::s_converter()->getLastDPTPrior()->getLastActivity()->pattern();
				if (pPattern && pPattern->findRelevantResource(pResName->value().getIdentificator())) {
					//! ��� ������, ������� ������������ � ��������� ���������� ���������� DPTPrior
					LPRDORelevantResource pRelevantResource = pPattern->findRelevantResource(pResName->value().getIdentificator());
					ruint                 parNumb           = pRelevantResource->getType()->getRTPParamNumber(pParName->value().getIdentificator());
					if (parNumb == RDORTPResType::UNDEFINED_PARAM)
					{
						Converter::s_converter()->error().error(pParName->src_info(), rdo::format(_T("����������� �������� �������: %s"), pParName->value().getIdentificator().c_str()));
					}
					m_pCalc = rdo::Factory<rdo::runtime::RDOCalcNop>::create();
					m_pCalc->setSrcInfo(src_info());
					m_pValue = rdo::Factory<RDOValue>::create(pRelevantResource->getType()->findRTPParam(pParName->value().getIdentificator())->getType()->type());
					return;
				}
			}
			break;
		}
	}
	Converter::s_converter()->error().error(pResName->src_info(), rdo::format(_T("����������� ������: %s"), pResName->value().getIdentificator().c_str()));
}

RDOFUNArithm::CastResult RDOFUNArithm::beforeCastValue(LPRDOFUNArithm pSecond)
{
	if (typeID() == rdo::runtime::RDOType::t_enum && pSecond->typeID() == rdo::runtime::RDOType::t_identificator)
	{
		pSecond->m_pValue = rdo::Factory<RDOValue>::create(
			type()->value_cast(
				pSecond->value(),
				pSecond->src_info(),
				src_info()
			)->value(),
			enumType(),
			pSecond->m_pValue->src_info()
		);
		pSecond->m_pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pSecond->m_pValue->value());
		pSecond->m_pCalc->setSrcInfo(pSecond->src_info());
		return CR_DONE;
	}
	else if (typeID() == rdo::runtime::RDOType::t_identificator && pSecond->typeID() == rdo::runtime::RDOType::t_enum)
	{
		m_pValue = rdo::Factory<RDOValue>::create(
			pSecond->type()->value_cast(
				value(),
				src_info(),
				pSecond->src_info()
			)->value(),
			pSecond->enumType(),
			m_pValue->src_info()
		);
		m_pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pValue->value());
		m_pCalc->setSrcInfo(src_info());
		return CR_DONE;
	}
	return CR_CONTINUE;
}

LPRDOType RDOFUNArithm::getPreType(CREF(LPRDOFUNArithm) pSecond)
{
	if (typeID() == rdo::runtime::RDOType::t_unknow)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("���������� ������ �������: �������������� �������� ��������������� ���������: %s"), src_text().c_str()));
	}
	if (pSecond->typeID() == rdo::runtime::RDOType::t_unknow)
	{
		Converter::s_converter()->error().error(pSecond->src_info(), rdo::format(_T("���������� ������ �������: �������������� �������� ��������������� ���������: %s"), pSecond->src_text().c_str()));
	}
	if (typeID() == rdo::runtime::RDOType::t_identificator)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("����������� �������������: %s"), value()->value().getIdentificator().c_str()));
	}
	if (pSecond->typeID() == rdo::runtime::RDOType::t_identificator)
	{
		Converter::s_converter()->error().error(pSecond->src_info(), rdo::format(_T("����������� �������������: %s"), pSecond->value()->value().getIdentificator().c_str()));
	}

	/// @todo ������� ��� ���������� src_info(), ��������� � �������� ������������
	return type()->type_cast(pSecond->type(), pSecond->src_info(), src_info(), src_info());
}

LPRDOFUNArithm RDOFUNArithm::operator+ (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM(Plus, +, _T("�� �� ���� � ������� %s � %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator- (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM(Minus, -, _T("������ �� %s ������� %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator* (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM(Mult, *, _T("������ %s �������� �� %s"));
}

LPRDOFUNArithm RDOFUNArithm::operator/ (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_ARITHM_CALC(Div, /, _T("������ %s ��������� �� %s"));
	/// @todo ������� ������������� � ����� ��� �������. � ��� ������ � ���������� � �.�. ?
	//! �����: � ���������� ���� ������ ���� �����, ������ ��������� ���
	if (pNewType->type()->typeID() == rdo::runtime::RDOType::t_int)
	{
		rdo::runtime::LPRDOCalc pNewCalcDiv = pNewCalc;
		pNewCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToIntByResult>::create(pNewCalcDiv);
		pNewCalc->setSrcInfo(pNewCalcDiv->srcInfo());
	}
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(pNewType, pNewCalc->srcInfo()),
		pNewCalc
	);
	if (pNewType->type()->typeID() == rdo::runtime::RDOType::t_int)
	{
		rdo::runtime::LPRDOCalcDoubleToIntByResult pResult = pNewCalc.object_static_cast<rdo::runtime::RDOCalcDoubleToIntByResult>();
		pArithm->m_intOrDouble.push_back(pResult);
	}
	pArithm->m_intOrDouble.insert(m_intOrDouble, pSecond->m_intOrDouble);
	return pArithm;
}

LPRDOFUNLogic RDOFUNArithm::operator< (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsLess, <, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator> (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsGreater, >, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator<= (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsLEQ, <=, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator>= (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsGEQ, >, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator== (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsEqual, ==, _T("������ ���������� %s � %s"));
}

LPRDOFUNLogic RDOFUNArithm::operator!= (CREF(LPRDOFUNArithm) pSecond)
{
	GENERATE_LOGIC_FROM_ARITHM(IsNotEqual, !=, _T("������ ���������� %s � %s"));
}

void RDOFUNArithm::checkParamType(CREF(LPRDOTypeParam) pType)
{
	pType->type()->type_cast(type(), src_info(), pType->src_info(), src_info());
	rdo::runtime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdo::runtime::RDOCalcConst>();
	if (pConstCalc)
	{
		rdo::runtime::RDOValue value = pConstCalc->getValue();
		pType->value_cast(rdo::Factory<RDOValue>::create(value, type(), src_info()));
	}
}

rdo::runtime::LPRDOCalc RDOFUNArithm::createCalc(CREF(LPRDOTypeParam) pForType)
{
	if (typeID() != rdo::runtime::RDOType::t_identificator)
	{
		if (!pForType)
		{
			return m_pCalc;
		}
		if (pForType->type()->typeID() != rdo::runtime::RDOType::t_int)
		{
			if (pForType->type()->typeID() == rdo::runtime::RDOType::t_enum)
			{
				m_intOrDouble.roundCalc();
			}
			return m_pCalc;
		}
		else
		{
			m_intOrDouble.roundCalc();
			rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcDoubleToInt>::create(m_pCalc->srcInfo().src_pos(), m_pCalc);
			pNewCalc->setSrcInfo(src_info());
			return pNewCalc;
		}
	}
	else if (typeID() == rdo::runtime::RDOType::t_identificator && !pForType)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("����������� �������������: %s"), value()->value().getAsString().c_str()));
	}

	if (!pForType)
	{
		Converter::s_converter()->error().error(src_info(), _T("����������� ��� ���������"));
	}

	rdo::runtime::LPRDOCalc pNewCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(pForType->value_cast(rdo::Factory<RDOValue>::create(src_info()))->value());
	pNewCalc->setSrcInfo(src_info());
	return pNewCalc;
}

void RDOFUNArithm::setSrcInfo(CREF(RDOParserSrcInfo) src_info)
{
	RDOParserSrcInfo::setSrcInfo(src_info);
	if (m_pCalc)
	{
		m_pCalc->setSrcInfo(src_info);
	}
}

void RDOFUNArithm::setSrcPos(CREF(RDOSrcInfo::Position) position)
{
	RDOParserSrcInfo::setSrcPos(position);
	if (m_pCalc)
	{
		rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcPos(position);
		m_pCalc->setSrcInfo(srcInfo);
	}
}

void RDOFUNArithm::setSrcText(CREF(tstring) value)
{
	RDOParserSrcInfo::setSrcText(value);
	if (m_pCalc)
	{
		rdo::runtime::RDOSrcInfo srcInfo(m_pCalc->srcInfo());
		srcInfo.setSrcText(src_info().src_text());
		m_pCalc->setSrcInfo(srcInfo);
	}
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

// --------------------------------------------------------------------------------
// -------------------- RDOFUNConstant
// --------------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOTypeParam) pType, CREF(LPRDOValue) pDefault)
	: RDOParam(src_info, pType, pDefault)
{
	if (!getDefault() || !getDefault()->defined())
	{
		ASSERT(pType);
		Converter::s_converter()->error().error(pType->src_info(), _T("��������� ������ ����� ��������"));
	}

	m_number = Converter::s_converter()->getFUNCONST_id();
	Converter::s_converter()->insertFUNConstant(this);
}

RDOFUNConstant::~RDOFUNConstant()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNParams
// --------------------------------------------------------------------------------
// ���������, � �������� ������� ����������
// --------------------------------------------------------------------------------
RDOFUNParams::RDOFUNParams()
	: RDOParserSrcInfo()
{}

RDOFUNParams::~RDOFUNParams()
{}

rdo::runtime::LPRDOCalc RDOFUNParams::getCalc(ruint paramID, CREF(LPRDOTypeParam) pType)
{
	ASSERT(paramID < m_paramList.size());
	rdo::runtime::LPRDOCalc pCalc = m_paramList[paramID]->createCalc(pType);
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
	LPRDOFUNFunction pFunction = Converter::s_converter()->findFUNFunction(funName);
	if (!pFunction)
	{
		return createSeqCall(funName);
	}

	ruint nParams = pFunction->getParams().size();
	if (nParams != m_paramList.size())
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("�������� ���������� ���������� �������: %s"), funName.c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuncCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(pFunction->getFunctionCalc());
	pFunction->insertPostLinked(pFuncCall);
	pFuncCall->setSrcInfo(src_info());
	for (ruint i = 0; i < nParams; i++)
	{
		LPRDOTypeParam pFuncParam = pFunction->getParams()[i]->getType();
		LPRDOFUNArithm pArithm = m_paramList[i];
		ASSERT(pArithm);
		pArithm->checkParamType(pFuncParam);
		pFuncCall->addParameter(pFuncParam->type()->calc_cast(pArithm->createCalc(pFuncParam), pArithm->type()));
	}

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(pFunction->getReturn()->getType()->type(), src_pos()),
		pFuncCall.object_parent_cast<rdo::runtime::RDOCalc>()
	);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info());

	return pArithm;
}

LPRDOFUNArithm RDOFUNParams::createSeqCall(CREF(tstring) seqName)
{
	LPRDOFUNSequence pSequence = Converter::s_converter()->findFUNSequence(seqName);
	if (!pSequence)
	{
		Converter::s_converter()->error().error(m_funseqName, rdo::format(_T("�������������� ������� ��� ������������������: %s"), seqName.c_str()));
	}
	LPRDOFUNParams pThis(this);
	ASSERT(pThis);
	LPRDOFUNArithm pArithm = pSequence->createCallCalc(pThis, src_info());
	ASSERT(pArithm);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- ������������������
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequence
// --------------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOParserSrcInfo(pHeader->src_info())
	, m_pHeader       (pHeader            )
	, m_seed          (seed               )
{
	Converter::s_converter()->insertFUNSequence(this);
}

RDOFUNSequence::~RDOFUNSequence()
{}

void RDOFUNSequence::initResult()
{
	initCalcSrcInfo();
	m_pNextCalc->m_res_real = true;
	switch (m_pHeader->getType()->type()->typeID())
	{
		case rdo::runtime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				m_pNextCalc->m_range = rdo::runtime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
			}
			break;
		}
		case rdo::runtime::RDOType::t_real:
		{
			m_pNextCalc->m_res_real = true;
			LPRDOTypeRealRange pRange = m_pHeader->getType()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				m_pNextCalc->m_range = rdo::runtime::RDOCalcSeqNext::Range(pRange->range()->getMin()->value().getDouble(), pRange->range()->getMax()->value().getDouble());
			}
			break;
		}
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	if (m_pInitCalc)
	{
		rdo::runtime::RDOSrcInfo srcInfo(m_pHeader->src_info());
		srcInfo.setSrcText(_T("��������� ���� ������������������ ") + m_pHeader->src_info().src_text());
		m_pInitCalc->setSrcInfo(srcInfo);
	}
	m_pNextCalc->setSrcInfo(m_pHeader->src_info());
}

// --------------------------------------------------------------------------------
// -------------------- ������� ��������� �����
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceUniform
// --------------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("������������������ ���� uniform �� ����� ���������� �������� ������������� ����"));
	}
}

void RDOFUNSequenceUniform::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorUniform) pGenerator = new rdo::runtime::RandGeneratorUniform();
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextUniform>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceUniform::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 2)
	{
		Converter::s_converter()->error().error(seq_src_info, rdo::format(_T("��� ������������ ������ ������������� '%s' ����� ������� ��� ���������: ����������� � ������������ ������� ���������"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
		pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
	);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdo::runtime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("���������� ������ �������"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceExponential
// --------------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() != rdo::runtime::RDOType::t_int && m_pHeader->getType()->type()->typeID() != rdo::runtime::RDOType::t_real)
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("������������������ '%s' ����� ���������� �������� ������ ������ ��� ������������� ����"), src_text().c_str()));
	}
}

void RDOFUNSequenceExponential::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorExponential) pGenerator = new rdo::runtime::RandGeneratorExponential();
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextExponential>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceExponential::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 1)
	{
		Converter::s_converter()->error().error(seq_src_info, rdo::format(_T("��� ����������������� ������ ������������� '%s' ����� ������� ���� ��������: �������������� ��������"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);

	pFuctionCall->addParameter(pArg1);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
		pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
	);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdo::runtime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("���������� ������ �������"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceNormal
// --------------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal(CREF(LPRDOFUNSequenceHeader) pHeader, int seed)
	: RDOFUNSequence(pHeader, seed)
{
	if (m_pHeader->getType()->type()->typeID() == rdo::runtime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("������������������ ���� normal �� ����� ���������� �������� ������������� ����"));
	}
}

void RDOFUNSequenceNormal::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorNormal) pGenerator = new rdo::runtime::RandGeneratorNormal();
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextNormal>::create(pGenerator);
	initResult();
}

LPRDOFUNArithm RDOFUNSequenceNormal::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) seq_src_info) const
{
	if (pParamList->getParamList().size() != 2)
	{
		Converter::s_converter()->error().error(seq_src_info, rdo::format(_T("��� ����������� ������ ������������� '%s' ����� ������� ��� ���������: �������������� �������� � ������� �������������� ����������"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOTypeParam        pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo());
	rdo::runtime::LPRDOCalc pArg1 = pParamList->getCalc(0, pType);
	rdo::runtime::LPRDOCalc pArg2 = pParamList->getCalc(1, pType);

	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
		pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
	);
	ASSERT(pArithm);
	pArithm->setSrcInfo(seq_src_info);
	if (pArithm->typeID() == rdo::runtime::RDOType::t_enum)
	{
		Converter::s_converter()->error().error(src_info(), _T("���������� ������ �������"));
	}
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- �����������
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHist
// --------------------------------------------------------------------------------
RDOFUNSequenceByHist::RDOFUNSequenceByHist(CREF(LPRDOFUNSequenceByHistHeader) pHeader)
	: RDOFUNSequence(pHeader->getHeader(), pHeader->getSeed())
{}

LPRDOFUNArithm RDOFUNSequenceByHist::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList().size() != 0)
	{
		Converter::s_converter()->error().error(src_info, rdo::format(_T("����������� '%s' ������ ���������� ��� ����������"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
		pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
	);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistReal
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistReal::RDOFUNSequenceByHistReal(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(LPRDOValue) pFrom, CREF(LPRDOValue) pTo, CREF(LPRDOValue) pFreq)
	: RDOFUNSequenceByHist(pHeader)
{
	addReal(pFrom, pTo, pFreq);
}

void RDOFUNSequenceByHistReal::addReal(CREF(LPRDOValue) pFrom, CREF(LPRDOValue) pTo, CREF(LPRDOValue) pFreq)
{
	ASSERT(pFrom);
	ASSERT(pTo  );
	ASSERT(pFreq);

	if (!m_to.empty() && pFrom->value() != m_to.back())
	{
		Converter::s_converter()->error().error(pFrom->src_info(), _T("������ ��������� ������ ��������� � ���������� �����������"));
	}
	if (pFrom->value() > pTo->value())
	{
		Converter::s_converter()->error().error(pTo->src_info(), _T("������ ��������� ������ ���� ������ ��� �����"));
	}
	if (pFreq->value() == 0)
	{
		Converter::s_converter()->error().warning(pFreq->src_info(), _T("������������� ����������� ������ ���� ������ ����"));
	}
	else if (pFreq->value() < 0)
	{
		Converter::s_converter()->error().error(pFreq->src_info(), _T("������������� ����������� ������ ���� ������ ����"));
	}
	m_pHeader->getType()->value_cast(pFrom);
	m_pHeader->getType()->value_cast(pTo  );
	m_from.push_back(pFrom->value());
	m_to  .push_back(pTo  ->value());
	m_freq.push_back(pFreq->value());
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorByHistReal) pGenerator = new rdo::runtime::RandGeneratorByHistReal();
	int size = m_from.size();
	for (int i = 0; i < size; i++)
	{
		pGenerator->addValues(m_from[i].getDouble(), m_to[i].getDouble(), m_freq[i].getDouble());
	}

	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
	initResult();
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceByHistEnum
// --------------------------------------------------------------------------------
RDOFUNSequenceByHistEnum::RDOFUNSequenceByHistEnum(CREF(LPRDOFUNSequenceByHistHeader) pHeader, CREF(LPRDOValue) pValue, CREF(LPRDOValue) pFreq)
	: RDOFUNSequenceByHist(pHeader)
{
	addEnum(pValue, pFreq);
}

void RDOFUNSequenceByHistEnum::addEnum(CREF(LPRDOValue) pValue, CREF(LPRDOValue) pFreq)
{
	ASSERT(pValue);
	ASSERT(pFreq );

	if (pFreq->value() == 0)
	{
		Converter::s_converter()->error().warning(pFreq->src_info(), _T("������������� ����������� ������ ���� ������ ����"));
	}
	else if (pFreq->value() < 0)
	{
		Converter::s_converter()->error().error(pFreq->src_info(), _T("������������� ����������� ������ ���� ������ ����"));
	}
	m_pHeader->getType()->value_cast(pValue);
	rdo::runtime::RDOValue enum_id = m_pHeader->getType()->value_cast(pValue)->value();
	if (std::find(m_values.begin(), m_values.end(), enum_id) != m_values.end())
	{
		Converter::s_converter()->error().error(pValue->src_info(), rdo::format(_T("������������ �������� ���������� ������: %s"), pValue->value().getIdentificator().c_str()));
	}
	m_values.push_back(enum_id);
	m_freq  .push_back(pFreq->value());
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorByHistEnum) pGenerator = new rdo::runtime::RandGeneratorByHistEnum();
	ruint size = m_values.size();
	for (ruint i = 0; i < size; i++)
	{
		pGenerator->addValues(m_values[i], m_freq[i].getDouble());
	}
	m_pInitCalc = rdo::Factory<rdo::runtime::RDOCalcSeqInit>::create(m_seed, pGenerator);
	Converter::s_converter()->runtime()->addInitCalc(m_pInitCalc);
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- �������� ��������
// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
LPRDOFUNArithm RDOFUNSequenceEnumerative::createCallCalc(REF(LPRDOFUNParams) pParamList, CREF(RDOParserSrcInfo) src_info) const
{
	if (pParamList->getParamList().size() != 0)
	{
		Converter::s_converter()->error().error(src_info, rdo::format(_T("������������ '%s' ������ ���������� ��� ����������"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcFunctionCaller pFuctionCall = rdo::Factory<rdo::runtime::RDOCalcFunctionCaller>::create(m_pNextCalc);
	ASSERT(pFuctionCall);

	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(m_pHeader->getType()->type(), pParamList->src_pos()),
		pFuctionCall.object_parent_cast<rdo::runtime::RDOCalc>()
	);
	ASSERT(pArithm);
	pArithm->setSrcInfo(src_info);
	return pArithm;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSequenceEnumerative
// --------------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
	PTR(rdo::runtime::RandGeneratorEnumerative) pGenerator = new rdo::runtime::RandGeneratorEnumerative();
	for (ruint i = 0; i < m_valueList.size(); i++)
	{
		pGenerator->addValue(m_valueList[i]->value());
	}
	m_pNextCalc = rdo::Factory<rdo::runtime::RDOCalcSeqNextByHist>::create(pGenerator);
	initCalcSrcInfo();
}

// --------------------------------------------------------------------------------
// -------------------- �������
// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElement
// --------------------------------------------------------------------------------
RDOFUNFunctionListElement::RDOFUNFunctionListElement(CREF(RDOParserSrcInfo) src_info)
	: RDOParserSrcInfo(src_info)
{}

RDOFUNFunctionListElement::~RDOFUNFunctionListElement()
{}

rdo::runtime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(CREF(LPRDOTypeParam) pRetType, CREF(rdo::runtime::LPRDOCalcFuncParam) pFuncParam, CREF(RDOParserSrcInfo) src_pos) const
{
	rdo::runtime::LPRDOCalcConst pCalcConst = createResultCalc(pRetType, src_pos);
	return rdo::Factory<rdo::runtime::RDOCalcIsEqual>::create(pFuncParam, pCalcConst);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementIdentif
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementIdentif::RDOFUNFunctionListElementIdentif(CREF(RDOParserSrcInfo) src_info)
	: RDOFUNFunctionListElement(src_info)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
		pRetType->value_cast(
			rdo::Factory<RDOValue>::create(RDOParserSrcInfo(src_pos.getPosAsYY(), src_text()))
		)->value()
	);
	ASSERT(pCalcConst);
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementReal
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementReal::RDOFUNFunctionListElementReal(CREF(YYLTYPE) position, double value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%f"), value)))
	, m_value(value)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
		pRetType->value_cast(rdo::Factory<RDOValue>::create(m_value, rdo::Factory<RDOType__real>::create(), src_pos))->value()
	);
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementInt
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementInt::RDOFUNFunctionListElementInt(CREF(YYLTYPE) position, int value)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, rdo::format(_T("%d"), value)))
	, m_value(value)
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	rdo::runtime::LPRDOCalcConst pCalcConst = rdo::Factory<rdo::runtime::RDOCalcConst>::create(
		pRetType->value_cast(rdo::Factory<RDOValue>::create(m_value, rdo::Factory<RDOType__int>::create().object_parent_cast<RDOType>(), src_pos))->value()
	);
	pCalcConst->setSrcInfo(src_pos);
	return pCalcConst;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunctionListElementEq
// --------------------------------------------------------------------------------
RDOFUNFunctionListElementEq::RDOFUNFunctionListElementEq(CREF(YYLTYPE) position)
	: RDOFUNFunctionListElement(RDOParserSrcInfo(position, _T("=")))
{}

rdo::runtime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(CREF(LPRDOTypeParam) pRetType, CREF(RDOParserSrcInfo) src_pos) const
{
	UNUSED(pRetType);

	Converter::s_converter()->error().error(src_pos, _T("���������� ������ �������: RDOFUNFunctionListElementEq::createResultCalc"));
	NEVER_REACH_HERE;
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNCalculateIf
// --------------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf(CREF(LPRDOFUNLogic) pCondition, CREF(LPRDOFUNArithm) pAction)
	: m_pCondition(pCondition)
	, m_pAction   (pAction   )
{
	m_pAction->checkParamType(Converter::s_converter()->getLastFUNFunction()->getReturn()->getType());
}

RDOFUNCalculateIf::~RDOFUNCalculateIf()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNFunction
// --------------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction(CREF(RDOParserSrcInfo) src_info, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(src_info)
	, m_pReturn       (pReturn )
{
	Converter::s_converter()->insertFUNFunction(this);
}

RDOFUNFunction::RDOFUNFunction(CREF(tstring) name, CREF(LPRDOParam) pReturn)
	: RDOParserSrcInfo(name   )
	, m_pReturn       (pReturn)
{
	Converter::s_converter()->insertFUNFunction(this);
}

RDOFUNFunction::~RDOFUNFunction()
{}

void RDOFUNFunction::setFunctionCalc(CREF(rdo::runtime::LPRDOFunCalc) pCalc)
{
	m_pFunctionCalc = pCalc;
	if (m_pFunctionCalc->srcInfo().src_empty())
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
	return it != m_paramList.end() ? *it : LPRDOParam(NULL);
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
		Converter::s_converter()->error().push_only(pParam->src_info(), rdo::format(_T("�������� ��� ����������: %s"), pParam->name().c_str()));
		Converter::s_converter()->error().push_only(pParamPrev->src_info(), _T("��. ������ �����������"));
		Converter::s_converter()->error().push_done();
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
	if (!m_pReturn->getDefault()->defined())
	{
		Converter::s_converter()->error().error(m_pReturn->src_info(), rdo::format(_T("������� '%s' ������ ����� �������� �� ���������"), name().c_str()));
	}
	if (m_paramList.empty())
	{
		Converter::s_converter()->error().error(src_info(), rdo::format(_T("������� '%s' ������ ����� ���������, �.�. � ��� list"), name().c_str()));
	}
	if (m_elementList.empty())
	{
		Converter::s_converter()->error().warning(src_info(), rdo::format(_T("������� '%s' �� �������� ������"), name().c_str()));
	}

	rdo::runtime::LPRDOCalcConst pCalcDefault = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getDefault()->value());
	ASSERT(pCalcDefault);
	pCalcDefault->setSrcInfo(m_pReturn->getDefault()->src_info());
	rdo::runtime::LPRDOFunListCalc pFunListCalc = rdo::Factory<rdo::runtime::RDOFunListCalc>::create(pCalcDefault);
	ASSERT(pFunListCalc);
	pFunListCalc->setSrcInfo(src_info());
	ElementList::const_iterator elem_it = m_elementList.begin();
	while (elem_it != m_elementList.end())
	{
		rdo::runtime::LPRDOCalc pCaseCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(true);
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
				Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("��������� �������� ��� ��������� '%s'"), pParam->name().c_str()));
			}
			LPRDOFUNFunctionListElement pListElement = *elem_it;
			ASSERT(pListElement);
			if (pListElement->isEquivalence())
			{
				if ((param_it + 1) == m_paramList.end())
				{
					Converter::s_converter()->error().error(pListElement->src_info(), rdo::format(_T("����� ������ ��������� ��������� �������� ��� ���������: %s"),pParam->name().c_str()));
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
					Converter::s_converter()->error().error(pListElement->src_info(), rdo::format(_T("����� ������ ��������� ��������� �������� ��� ����������: %s"), str.c_str()));
				}
			}
			rdo::runtime::LPRDOCalcFuncParam pFuncParam   = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParamNumber, pParam->src_info());
			ASSERT(pFuncParam);
			rdo::runtime::LPRDOCalcIsEqual   pCompareCalc = pListElement->createIsEqualCalc(pParam->getType(), pFuncParam, pListElement->src_info());
			ASSERT(pCompareCalc);
			rdo::runtime::LPRDOCalc          pAndCalc     = rdo::Factory<rdo::runtime::RDOCalcAnd>::create(pCaseCalc, pCompareCalc);
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
			Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("����� �������� ��� ��������� '%s' ��������� ���� ���������"), (*param_it)->name().c_str()));
		}
		if (!(*elem_it)->isEquivalence())
		{
			Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("������� ����� ��������, ������ '%s' ��������� ���� ���������"), (*elem_it)->src_text().c_str()));
		}
		++elem_it;
		if (elem_it == m_elementList.end())
		{
			--elem_it;
			Converter::s_converter()->error().error((*elem_it)->src_info(), rdo::format(_T("����� ����� ��������� ��������� �������� ������� '%s'"), name().c_str()));
		}
		rdo::runtime::LPRDOCalcConst pResultCalc = (*elem_it)->createResultCalc(m_pReturn->getType(), (*elem_it)->src_info());
		pFunListCalc->addCase(pCaseCalc, pResultCalc);
		++elem_it;
	}
	setFunctionCalc(pFunListCalc);
}

void RDOFUNFunction::createTableCalc(CREF(YYLTYPE) elements_pos)
{
	ElementList::const_iterator it = m_elementList.begin();
	while (it != m_elementList.end())
	{
		if ((*it)->isEquivalence())
		{
			Converter::s_converter()->error().error((*it)->src_info(), _T("������ '=' ���������� � ��������� �������"));
		}
		++it;
	}
	ruint param_cnt = m_paramList.size();
	ruint range     = 1;
	rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcConst>::create(0);
	ASSERT(pCalc);
	rdo::runtime::RDOSrcInfo srcInfo(src_info());
	srcInfo.setSrcText(_T("0"));
	pCalc->setSrcInfo(srcInfo);
	for (ruint currParam = 0; currParam < param_cnt; currParam++)
	{
		LPRDOParam pFunctionParam = m_paramList.at(currParam);
		ASSERT(pFunctionParam);
		rdo::runtime::LPRDOCalcFuncParam pFuncParam = rdo::Factory<rdo::runtime::RDOCalcFuncParam>::create(currParam, pFunctionParam->src_info());
		ASSERT(pFuncParam);
		rdo::runtime::LPRDOCalc pValue2 = pFuncParam;
		if (pFunctionParam->getType()->type()->typeID() != rdo::runtime::RDOType::t_enum)
		{
			rdo::runtime::LPRDOCalcConst pCalcConst1 = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
			pCalcConst1->setSrcInfo(pFunctionParam->src_info());
			pValue2 = rdo::Factory<rdo::runtime::RDOCalcMinus>::create(pValue2, pCalcConst1);
		}
		rdo::runtime::LPRDOCalcConst pCalcConst2 = rdo::Factory<rdo::runtime::RDOCalcConst>::create(range);
		ASSERT(pCalcConst2);
		pCalcConst2->setSrcInfo(pFunctionParam->src_info());
		rdo::runtime::LPRDOCalcMult pCalcMult = rdo::Factory<rdo::runtime::RDOCalcMultEnumSafe>::create(pCalcConst2, pValue2);
		rdo::runtime::LPRDOCalcPlus pCalcAdd  = rdo::Factory<rdo::runtime::RDOCalcPlusEnumSafe>::create(pCalcMult, pCalc);

		switch (pFunctionParam->getType()->type()->typeID())
		{
		case rdo::runtime::RDOType::t_int:
			if (dynamic_cast<PTR(RDOTypeIntRange)>(pFunctionParam->getType()->type().get()))
			{
				LPRDOTypeIntRange pRange = pFunctionParam->getType()->type().object_static_cast<RDOTypeIntRange>();
				if (pRange->range()->getMin()->value().getInt() != 1)
				{
					Converter::s_converter()->error().error(pRange->range()->src_info(), rdo::format(_T("����������� �������� ��������� ������ ���� 1, ������� �������� [%d..%d]"), pRange->range()->getMin()->value().getInt(), pRange->range()->getMax()->value().getInt()));
				}
				range *= pRange->range()->getMax()->value().getInt() - pRange->range()->getMin()->value().getInt() + 1;
			}
			else
			{
				Converter::s_converter()->error().error(pFunctionParam->getType()->src_info(), _T("��� ��������� ��������� ������� ������ ���� ����� ���������� ��������"));
			}
			break;
		case rdo::runtime::RDOType::t_enum:
			range *= pFunctionParam->getType()->type().object_static_cast<RDOEnumType>()->getEnums()->getValues().size();
			break;
		default:
			Converter::s_converter()->error().error(pFunctionParam->getType()->src_info(), _T("�������� ��������� ������� ����� ���� ������ ��� ������������� ����"));
		}
		pCalc = pCalcAdd;
	}

	if (m_elementList.size() != range)
	{
		Converter::s_converter()->error().error(elements_pos, rdo::format(_T("�������� ���������� ��������� ��������� ������� '%s': %d, ��������� %d"), name().c_str(), m_elementList.size(), range));
	}

	rdo::runtime::LPRDOFuncTableCalc pFuncTableCalc = rdo::Factory<rdo::runtime::RDOFuncTableCalc>::create(pCalc);
	ASSERT(pFuncTableCalc);
	pFuncTableCalc->setSrcInfo(src_info());
	for (ruint currElem = 0; currElem < range; currElem++)
	{
		LPRDOFUNFunctionListElement pListElement = m_elementList.at(currElem);
		ASSERT(pListElement);
		rdo::runtime::LPRDOCalcConst  pResultCalc  = pListElement->createResultCalc(m_pReturn->getType(), pListElement->src_info());
		ASSERT(pResultCalc);
		pFuncTableCalc->addResultCalc(pResultCalc);
	}
	setFunctionCalc(pFuncTableCalc);
}

void RDOFUNFunction::createAlgorithmicCalc(CREF(RDOParserSrcInfo) /* body_src_info */)
{
	rdo::runtime::LPRDOFunAlgorithmicCalc pFunAlgorithmicCalc = rdo::Factory<rdo::runtime::RDOFunAlgorithmicCalc>::create();
	ASSERT(pFunAlgorithmicCalc);

	pFunAlgorithmicCalc->setSrcInfo(src_info());
	rbool defaultFlag = false;
	rbool trueConst   = false;
	rdo::runtime::LPRDOCalcConst pCondition;
	int size = m_calculateIfList.size();
	int cnt  = 0;
	for (int i = 0; i < size; i++)
	{
		rdo::runtime::LPRDOCalc pLogicCalc = m_calculateIfList[i]->getCondition()->getCalc(m_pReturn->getType()->type()->typeID());
		ASSERT(pLogicCalc);
		rdo::runtime::LPRDOCalcConst pConditionLast = pLogicCalc.object_dynamic_cast<rdo::runtime::RDOCalcConst>();
		if (trueConst)
		{
			Converter::s_converter()->error().warning(m_calculateIfList[i]->getCondition()->src_info(), rdo::format(_T("������� �� ������������: %s"), m_calculateIfList[i]->getCondition()->src_text().c_str()));
			Converter::s_converter()->error().warning(pCondition->srcInfo(), rdo::format(_T("��������� ������� ������� �������: %s"), pCondition->srcInfo().src_text().c_str()));
		}
		else if (!pConditionLast || pConditionLast->calcValue(Converter::s_converter()->runtime()).getAsBool())
		{
			//! ���������� ������ false-������� ���������� ���������
			pFunAlgorithmicCalc->addCalcIf(pLogicCalc, m_calculateIfList[i]->getAction()->createCalc(m_pReturn->getType()));
			cnt++;
		}
		if (!defaultFlag && pConditionLast && pConditionLast->calcValue(Converter::s_converter()->runtime()).getAsBool())
		{
			trueConst   = true;
			defaultFlag = true;
			pCondition  = pConditionLast;
		}
	}
	if (!cnt)
	{
		Converter::s_converter()->error().warning(src_info(), rdo::format(_T("����������� ���� ������� '%s'"), name().c_str()));
	}
	if (!trueConst)
	{
		if (m_pReturn->getDefault()->defined())
		{
			rdo::runtime::LPRDOCalcConst pCalcCondition = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
			rdo::runtime::LPRDOCalcConst pCalcAction    = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getType()->value_cast(m_pReturn->getDefault())->value());
			ASSERT(pCalcCondition);
			ASSERT(pCalcAction   );
			pCalcCondition->setSrcInfo(m_pReturn->getType()->src_info());
			pCalcAction->setSrcInfo(m_pReturn->getType()->src_info());
			pFunAlgorithmicCalc->addCalcIf(pCalcCondition, pCalcAction);
			defaultFlag = true;
		}
	}
	if (!defaultFlag)
	{
		//! ��������� �������������� �������� �� ���������, ���� ��� �� ������ � ����� ����
		rdo::runtime::LPRDOCalcConst pCalcCondition = rdo::Factory<rdo::runtime::RDOCalcConst>::create(1);
		rdo::runtime::LPRDOCalcConst pCalcAction    = rdo::Factory<rdo::runtime::RDOCalcConst>::create(m_pReturn->getType()->type()->get_default()->value());
		ASSERT(pCalcCondition);
		ASSERT(pCalcAction   );
		pCalcCondition->setSrcInfo(m_pReturn->getType()->src_info());
		pCalcAction->setSrcInfo(m_pReturn->getType()->src_info());
		pFunAlgorithmicCalc->addCalcIf(pCalcCondition, pCalcAction);
		Converter::s_converter()->error().warning(src_info(), rdo::format(_T("��� ������� '%s' ������������ �������� �� ���������"), name().c_str()));
	}
	setFunctionCalc(pFunAlgorithmicCalc);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroup
// --------------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup(CREF(RDOParserSrcInfo) res_info)
	: RDOParserSrcInfo(res_info)
{
	init(res_info);
}

RDOFUNGroup::~RDOFUNGroup()
{}

void RDOFUNGroup::init(CREF(RDOParserSrcInfo) res_info)
{
	m_pResType = Converter::s_converter()->findRTPResType(res_info.src_text());
	if (!m_pResType)
	{
		Converter::s_converter()->error().error(res_info, rdo::format(_T("����������� ��� �������: %s"), res_info.src_text().c_str()));
	}
	Converter::s_converter()->insertFUNGroup(this);
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNGroupLogic
// --------------------------------------------------------------------------------
RDOFUNGroupLogic::RDOFUNGroupLogic(FunGroupType funType, CREF(RDOParserSrcInfo) res_info)
	: RDOFUNGroup(res_info)
	, m_funType  (funType )
{}

LPRDOFUNLogic RDOFUNGroupLogic::createFunLogic(REF(LPRDOFUNLogic) pCondition)
{
	rdo::runtime::LPRDOFunCalcGroup calc;
	switch (m_funType)
	{
	case fgt_exist    : setSrcText(_T("Exist(")     + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcExist    >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notexist : setSrcText(_T("NotExist(")  + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcNotExist >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_forall   : setSrcText(_T("ForAll(")    + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcForAll   >::create(getResType()->getNumber(), pCondition->getCalc()); break;
	case fgt_notforall: setSrcText(_T("NotForAll(") + getResType()->name() + _T(": ") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcNotForAll>::create(getResType()->getNumber(), pCondition->getCalc()); break;
	default: Converter::s_converter()->error().error(src_info(), _T("���������� ������: �������������� ��� �������"));
	}
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
	pLogic->setSrcInfo(src_info());
	return pLogic;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFUNSelect
// --------------------------------------------------------------------------------
RDOFUNSelect::RDOFUNSelect(CREF(RDOParserSrcInfo) res_info)
	: RDOFUNGroup(RDOParserSrcInfo(res_info.src_text()))
{}

//! ��� Select ��� ������� �� ���� � �������
void RDOFUNSelect::initSelect(LPRDOFUNLogic pCondition)
{
	m_pCalcSelect = rdo::Factory<rdo::runtime::RDOFunCalcSelect>::create(getResType()->getNumber(), pCondition->getCalc());
	m_pCalcSelect->setSrcInfo(pCondition->src_info());
}

//! ��������� ������� ��� �������� Select'�
LPRDOFUNLogic RDOFUNSelect::createFunSelectGroup(RDOFUNGroupLogic::FunGroupType funType, REF(LPRDOFUNLogic) pCondition)
{
	ASSERT(pCondition);
	rdo::runtime::LPRDOFunCalcSelectBase calc;
	switch (funType)
	{
	case RDOFUNGroupLogic::fgt_exist    : setSrcText(src_text() + _T(".Exist(")     + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectExist    >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notexist : setSrcText(src_text() + _T(".NotExist(")  + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotExist >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_forall   : setSrcText(src_text() + _T(".ForAll(")    + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectForAll   >::create(m_pCalcSelect, pCondition->getCalc()); break;
	case RDOFUNGroupLogic::fgt_notforall: setSrcText(src_text() + _T(".NotForAll(") + pCondition->src_text() + _T(")")); calc = rdo::Factory<rdo::runtime::RDOFunCalcSelectNotForAll>::create(m_pCalcSelect, pCondition->getCalc()); break;
	default: Converter::s_converter()->error().error(pCondition->src_info(), _T("���������� ������: ����������� ����� ��� ������ ��������"));
	}
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(calc, false);
	pLogic->setSrcInfo(src_info());
	return pLogic;
}

LPRDOFUNLogic RDOFUNSelect::createFunSelectEmpty(CREF(RDOParserSrcInfo) empty_info)
{
	setSrcText(src_text() + _T(".") + empty_info.src_text());
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdo::runtime::RDOFunCalcSelectEmpty>::create(m_pCalcSelect), false);
	pLogic->setSrcInfo(empty_info);
	return pLogic;
}

LPRDOFUNArithm RDOFUNSelect::createFunSelectSize(CREF(RDOParserSrcInfo) size_info)
{
	setSrcText(src_text() + _T(".") + size_info.src_text());
	Converter::s_converter()->getFUNGroupStack().pop_back();
	LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(
		rdo::Factory<RDOValue>::create(rdo::Factory<RDOType__int>::create().object_parent_cast<RDOType>(), size_info),
		rdo::Factory<rdo::runtime::RDOFunCalcSelectSize>::create(m_pCalcSelect).object_parent_cast<rdo::runtime::RDOCalc>()
	);
	pArithm->setSrcInfo(size_info);
	return pArithm;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
