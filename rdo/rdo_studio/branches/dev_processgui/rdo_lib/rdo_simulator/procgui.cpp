/*
 * copyright: (c) RDO-Team, 2011
 * filename : procgui.cpp
 * author   : ������� ������
 * date     : 22.04.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_simulator/procgui.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
// ===============================================================================

OPEN_RDO_SIMULATOR_NAMESPACE

// --------------------------------------------------------------------
// ---------- ProcGUIBlock
// --------------------------------------------------------------------
ProcGUIBlock::ProcGUIBlock(CREF(rdoParse::LPRDOParser) pParser, PTR(rdoRuntime::RDORuntime) pRuntime)
	: m_pParser (pParser )
	, m_pRuntime(pRuntime)
{
	ASSERT(m_pParser );
	ASSERT(m_pRuntime);

	//! ������ �������
	m_pProcess = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_pRuntime);
	ASSERT(m_pProcess);
	m_pProcess.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIBlock::~ProcGUIBlock()
{}

void ProcGUIBlock::Create(REF(RPShapeDataBlockCreate) pParams)
{
	tstring rtp_name       = _T("���������");
	tstring rtp_param_name = _T("�����_��������");

	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(m_pParser);
	//! ����� ��� �������, ���� ��� ���, �� �������
	if (!rtpList[rtp_name].exist())
	{
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp(rtp_name);
		//! ������� �������� �����_��������
		rtp.m_params.append(rdoMBuilder::RDOResType::Param(rtp_param_name, rdo::Factory<rdoParse::RDOType__real>::create()));
		//! ������� ��� �������
		if (!rtpList.append(rtp))
		{
			m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("������ �������� ���� �������: %s"), rtp_name.c_str()));
		}
		rdoRuntime::RDOPROCTransact::s_typeID = rtp.id();
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
		rdoRuntime::RDOPROCTransact::s_typeID = rtp.id();
	}

	//! GENERATE
	switch(pParams.getZakon()) // ���������� �������� ���� ������ �� ������
	{
	case RPShapeDataBlock::Const: // ��������� 
		{
				
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getConstCalc(pParams.getExp()), pParams.getAmount());
				ASSERT(pBlock);
				break;
		}	
	case RPShapeDataBlock::Normal: // ����������
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getNormalCalc(pParams.getBase(), pParams.getExp(), pParams.getDisp()), pParams.getAmount());
				ASSERT(pBlock);
				break;
		}
	case RPShapeDataBlock::Uniform: // ����������� �����
		{
				LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getUniformCalc(pParams.getBase(), pParams.getExp(), pParams.getDisp()), pParams.getAmount());
				ASSERT(pBlock);
				break;
		}
	case RPShapeDataBlock::Exp: // ����������������
		{
				LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getExpCalc(pParams.getBase(), pParams.getExp()), pParams.getAmount());
				ASSERT(pBlock);
				break;
		}		
	}
}

void ProcGUIBlock::Process(REF(RPShapeDataBlockProcess) pParams)
{
	std::list <RPShapeDataBlockProcess::resAction> action = pParams.getAction();
	std::list <RPShapeDataBlockProcess::resAction>::iterator it = action.begin();
	while(it != action.end())
	{
		if(*it == RPShapeDataBlockProcess::Seize)
		{
			//���������
		}
		else if(*it == RPShapeDataBlockProcess::Advance)
		{
			//���������
			Advance(pParams);
		}
		else if(*it == RPShapeDataBlockProcess::Release)
		{
			//����������
		}
		it++;
	}
}

void ProcGUIBlock::Advance(REF(RPShapeDataBlockProcess) pParams)
{
	switch(pParams.getZakon())
	{
		case RPShapeDataBlock::Const:
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess, getConstCalc(pParams.getExp()));
			ASSERT(pBlock);
			break;
		}
		case RPShapeDataBlock::Normal: 
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess, getNormalCalc(pParams.getBase(), pParams.getExp(), pParams.getDisp()));
			ASSERT(pBlock);
			break;
		}
		case RPShapeDataBlock::Uniform:
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess, getUniformCalc(pParams.getBase(), pParams.getExp(), pParams.getDisp()));
			ASSERT(pBlock);
			break;
		}
		case RPShapeDataBlock::Exp:
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess,  getExpCalc(pParams.getBase(), pParams.getExp()));
			ASSERT(pBlock);
			break;
		}
	}
}

void ProcGUIBlock::Terminate(REF(RPShapeDataBlockTerminate) pParams)
{
	LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCTerminate)::create(m_pProcess, static_cast<int>(pParams.getTermInc()));
	ASSERT(pBlock);
}

rdoRuntime::LPRDOCalcConst ProcGUIBlock::getConstCalc(double m_pArg1)
{
	rdoRuntime::LPRDOCalcConst pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	ASSERT(pCalc);
	return pCalc;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getNormalCalc(int m_pBase,double m_pArg1,double m_pArg2)
{
	PTR(rdoRuntime::RandGeneratorNormal) pGenerator = new rdoRuntime::RandGeneratorNormal();
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_pBase, pGenerator);
	ASSERT(m_pInitCalc);
	m_pRuntime->addInitCalc(m_pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextNormal>::create(pGenerator);
	ASSERT(m_pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	rdoRuntime::LPRDOCalcConst pArg2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg2);
	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	return pFuctionCall;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getUniformCalc(int m_pBase,double m_pArg1,double m_pArg2)
{
	PTR(rdoRuntime::RandGeneratorUniform) pGenerator = new rdoRuntime::RandGeneratorUniform();
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_pBase, pGenerator);
	ASSERT(m_pInitCalc);
	m_pRuntime->addInitCalc(m_pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextUniform>::create(pGenerator);
	ASSERT(m_pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	rdoRuntime::LPRDOCalcConst pArg2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg2);
	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	return pFuctionCall;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getExpCalc(int m_pBase,double m_pArg1)
{
	PTR(rdoRuntime::RandGeneratorExponential) pGenerator = new rdoRuntime::RandGeneratorExponential();
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_pBase, pGenerator);
	ASSERT(m_pInitCalc);
	m_pRuntime->addInitCalc(m_pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextExponential>::create(pGenerator);
	ASSERT(m_pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	pFuctionCall->addParameter(pArg1);
	return pFuctionCall;
}

CLOSE_RDO_SIMULATOR_NAMESPACE
