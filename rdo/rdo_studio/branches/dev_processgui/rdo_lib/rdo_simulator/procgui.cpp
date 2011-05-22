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
#include "rdo_lib/rdo_mbuilder/process.h"
// ===============================================================================

OPEN_RDO_SIMULATOR_NAMESPACE

// --------------------------------------------------------------------
// ---------- ProcGUICalc
// --------------------------------------------------------------------
ProcGUICalc::ProcGUICalc(PTR(rdoRuntime::RDORuntime) pRuntime, CREF(LPRPShapeDataBlock) pParams)
	: m_pRuntime(pRuntime)
	, m_pParams(pParams)
{
	ASSERT(m_pRuntime);
	ASSERT(m_pParams);
	switch(pParams->getZakon())
	{
		case RPShapeDataBlock::Const: // ��������� 
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getConstCalc(pParams->getExp()));
			break;
		}	
		case RPShapeDataBlock::Normal: // ����������
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getNormalCalc(pParams->getBase(),pParams->getExp(),pParams->getDisp()));
			break;
		}
		case RPShapeDataBlock::Uniform: // ����������� �����
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getUniformCalc(pParams->getBase(),pParams->getExp(),pParams->getDisp()));
			break;
		}
		case RPShapeDataBlock::Exp: // ����������������
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getExpCalc(pParams->getBase(),pParams->getExp()));
			break;
		}
	}
}

ProcGUICalc::~ProcGUICalc()
{}

rdoRuntime::LPRDOCalcConst ProcGUICalc::getConstCalc(double m_pArg1)
{
	rdoRuntime::LPRDOCalcConst pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	ASSERT(pCalc);
	return pCalc;
}

rdoRuntime::LPRDOCalc ProcGUICalc::getCalc()
{
	return m_pCalc;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getNormalCalc(int m_pBase,double m_pArg1,double m_pArg2)
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

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getUniformCalc(int m_pBase,double m_pArg1,double m_pArg2)
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

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getExpCalc(int m_pBase,double m_pArg1)
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

// --------------------------------------------------------------------
// ---------- ProcGUIProcess
// --------------------------------------------------------------------
tstring ProcGUIProcess::s_name_prefix = _T("");
tstring ProcGUIProcess::s_name_sufix  = _T("s");

ProcGUIProcess::ProcGUIProcess(PTR(rdoRuntime::RDORuntime) pRuntime)
	: m_pRuntime(pRuntime)
{
	ASSERT(m_pRuntime);
	m_pProcess = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_pRuntime);
	ASSERT(m_pProcess);
	m_pProcess.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIProcess::~ProcGUIProcess()
{}

void ProcGUIProcess::insertBlock(CREF(LPProcGUIBlock) pBlock)
{
	ASSERT(pBlock);
	m_blockList.push_back(pBlock);
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlock
// ----------------------------------------------------------------------------
ProcGUIBlock::ProcGUIBlock(CREF(LPProcGUIProcess) pProcess, CREF(tstring) pName)
	: m_name    (pName   )
	, m_pProcess(pProcess)
{
	ASSERT(m_pProcess);
	m_pProcess->insertBlock(this);
}

ProcGUIBlock::~ProcGUIBlock()
{}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockGenerate
// ----------------------------------------------------------------------------
ProcGUIBlockGenerate::ProcGUIBlockGenerate(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockCreate) pParams/* CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc*/)
	: ProcGUIBlock(pProcess, pParams->getName())
	, ProcGUICalc (pRuntime, pParams)
	, m_pParser   (pParser )
	, m_pParams   (pParams )
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
	m_pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess->getProcess(), getCalc(), m_pParams->getAmount());
	ASSERT(m_pBlock);
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockTerminate
// ----------------------------------------------------------------------------
ProcGUIBlockTerminate::ProcGUIBlockTerminate(CREF(LPProcGUIProcess) pProcess, CREF(LPRPShapeDataBlockTerminate) pParams)
	: ProcGUIBlock(pProcess, pParams->getName())
	, m_pParams   (pParams                     )
{
	m_pBlock = F(rdoRuntime::RDOPROCTerminate)::create(m_pProcess->getProcess(), static_cast<ruint>(m_pParams->getTermInc()));
	ASSERT(m_pBlock);
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockProcess
// ----------------------------------------------------------------------------
ProcGUIBlockProcess::ProcGUIBlockProcess(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, pParams->getName())
	, m_pParams   (pParams                     )
{
	std::list <RPShapeDataBlockProcess::resAction> action = m_pParams->getAction();
	std::list <RPShapeDataBlockProcess::resAction>::iterator it = action.begin();

	LPProcGUIQueue pQueue = rdo::Factory<ProcGUIQueue>::create(pProcess, pParser, m_pParams->getName());
	ASSERT(pQueue);
	pQueue->createRuntime();

	while(it != action.end())
	{
		switch(*it)
		{
			case RPShapeDataBlockProcess::Seize:
			{
				LPProcGUISeize pSeize = rdo::Factory<ProcGUISeize>::create(m_pProcess, pParser, m_pParams);
				ASSERT(pSeize);
				pSeize->createRuntime();
				break;
			}
			case RPShapeDataBlockProcess::Advance:
			{
				LPProcGUIAdvance pAdvance = rdo::Factory<ProcGUIAdvance>::create(m_pProcess, pRuntime, m_pParams);
				ASSERT(pAdvance);
				break;
			}
			case RPShapeDataBlockProcess::Release:
			{
				LPProcGUIRelease pRelease = rdo::Factory<ProcGUIRelease>::create(m_pProcess, pParser, m_pParams);
				ASSERT(pRelease);
				pRelease->createRuntime();
				break;
			}
		}
		it++;
	}

	LPProcGUIDepart pDepart = rdo::Factory<ProcGUIDepart>::create(pProcess, pParser, m_pParams->getName());
	ASSERT(pDepart);
	pDepart->createRuntime();

	STL_FOR_ALL_CONST(pParser->getRSSResources(), rss_it)
	{
		rdoRuntime::LPRDOCalc calc = (*rss_it)->createCalc();
		pParser->runtime()->addInitCalc(calc);
	}
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIAdvance
// ----------------------------------------------------------------------------
ProcGUIAdvance::ProcGUIAdvance(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(LPRPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, rdo::format(_T("%s Advance"), pParams->getName().c_str()))
	, ProcGUICalc (pRuntime, pParams)
	, m_pParams   (pParams )
{
	m_pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess->getProcess(), getCalc());
	ASSERT(m_pBlock);
}

// ----------------------------------------------------------------------------
// ---------- ProcGUISeize
// ----------------------------------------------------------------------------
ProcGUISeize::ProcGUISeize(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, rdo::format(_T("%s Seize"), pParams->getName().c_str()))
	, m_pParams   (pParams                             )
	, m_pParser   (pParser                             )
{
	std::list <tstring> resources = m_pParams->getRes();
	std::list <tstring>::iterator it = resources.begin();
	while(it != resources.end()) 
	{
		addResource(*it);
		tstring res_name = *it;
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		rdoMBuilder::RDOResType rtp;
		//! ���� ������ ����������, ����� ��� ��� � ���������
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			if (rdoMBuilder::BlockForSeize::checkType(rtp, rdoParse::RDOParserSrcInfo()))
			{
				if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(m_pParser))
				{
					rdoMBuilder::BlockForSeize::reobjectRes(rtp, res_name);
				}
			}
		}
		else
		{
			//! ������ �� ������, ������������ ��� ���� �� ����� �������
			//! ������������ ��� ���� �� ����� �������
			tstring rtp_name(ProcGUIProcess::s_name_prefix + res_name + ProcGUIProcess::s_name_sufix);
			//! �������� ������ ���� ����� ��������
			rdoMBuilder::RDOResTypeList rtpList(m_pParser);
			//! ����� ��� �������
			if (rtpList[rtp_name].exist())
			{
				rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdoMBuilder::BlockForSeize::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
				{
					rdoMBuilder::BlockForSeize::createRes(rtp_, res_name);
				}
			}
			else
			{
				rtp = rdoMBuilder::BlockForSeize::createType(rtp_name, rdoParse::RDOParserSrcInfo());
				rdoMBuilder::BlockForSeize::createRes(rtp, res_name);
			}
		}
		
		it++;
	}
}

void ProcGUISeize::createRuntime()
{
	STL_FOR_ALL_CONST(m_resList, it){
		rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource((*it));
		if (pResource)
		{
			tstring res_name = pResource->name();
			//! �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(m_pParser);
			//! �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			//! "���������"
			tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtp_param_name].exist())
			{
				m_pParser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUISeize: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}
	if (!m_parserForRuntime.empty())
	{
		m_pBlock = F(rdoRuntime::RDOPROCSeize)::create(m_pProcess->getProcess(), m_parserForRuntime);
		ASSERT(m_pBlock);
	}
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), _T("���������� ������: ���� Seize �� �������� ��������"));
	}
}

void ProcGUISeize::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resList.push_back(name);
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIRelease
// ----------------------------------------------------------------------------
ProcGUIRelease::ProcGUIRelease(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, rdo::format(_T("%s Release"), pParams->getName().c_str()))
	, m_pParams   (pParams                             )
	, m_pParser   (pParser                             )
{
	std::list <tstring> resources = m_pParams->getRes();
	std::list <tstring>::iterator it = resources.begin();
	while(it != resources.end()) 
	{
		addResource(*it);
		tstring res_name = *it;
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		rdoMBuilder::RDOResType rtp;
		//! ���� ������ ����������, ����� ��� ��� � ���������
		if (rssList[res_name].exist())
		{
			rtp = rssList[res_name].getType();
			if (rdoMBuilder::BlockForSeize::checkType(rtp, rdoParse::RDOParserSrcInfo()))
			{
				if (!rssList[res_name].checkParserResourceType<rdoParse::RDOPROCResource>(m_pParser))
				{
					rdoMBuilder::BlockForSeize::reobjectRes(rtp, res_name);
				}
			}
		}
		else
		{
			//! ������ �� ������, ������������ ��� ���� �� ����� �������
			//! ������������ ��� ���� �� ����� �������
			tstring rtp_name(ProcGUIProcess::s_name_prefix + res_name + ProcGUIProcess::s_name_sufix);
			//! �������� ������ ���� ����� ��������
			rdoMBuilder::RDOResTypeList rtpList(m_pParser);
			//! ����� ��� �������
			if (rtpList[rtp_name].exist())
			{
				rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
				if (rdoMBuilder::BlockForSeize::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
				{
					rdoMBuilder::BlockForSeize::createRes(rtp_, res_name);
				}
			}
			else
			{
				rtp = rdoMBuilder::BlockForSeize::createType(rtp_name, rdoParse::RDOParserSrcInfo());
				rdoMBuilder::BlockForSeize::createRes(rtp, res_name);
			}
		}
		
		it++;
	}
}

void ProcGUIRelease::createRuntime()
{
	STL_FOR_ALL_CONST(m_resList, it){
		rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource((*it));
		if (pResource)
		{
			tstring res_name = pResource->name();
			//! �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(m_pParser);
			//! �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
			//! "���������"
			tstring rtp_param_name = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtp_param_name].exist())
			{
				m_pParser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtp_param_name].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIRelease: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}
	if (!m_parserForRuntime.empty())
	{
		m_pBlock = F(rdoRuntime::RDOPROCRelease)::create(m_pProcess->getProcess(), m_parserForRuntime);
		ASSERT(m_pBlock);
	}
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), _T("���������� ������: ���� Release �� �������� ��������"));
	}
}

void ProcGUIRelease::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resList.push_back(name);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
ProcGUIQueue::ProcGUIQueue(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(tstring) name)
	: ProcGUIBlock  (pProcess, rdo::format(_T("%s Queue"), name.c_str()))
	, m_resourceName(name                                )
	, m_pParser     (pParser                             )
{
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList(m_pParser);
	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(m_pParser);
	rdoMBuilder::RDOResType rtp;
	tstring rtp_name = _T("QDEPART");
	tstring q_name   = _T("�������_") + m_resourceName;
	//! ���� ������ ����������, ����� ��� ��� � ���������
	if (rssList[m_resourceName].exist())
	{
		rtp = rssList[m_resourceName].getType();
		rdoMBuilder::BlockForQueue::checkType(rtp, rdoParse::RDOParserSrcInfo());
		rdoParse::LPRDOPMDWatchPar pPokaz = rdo::Factory<rdoParse::RDOPMDWatchPar>::create(rdoParse::RDOParserSrcInfo(q_name));
		ASSERT(pPokaz);
		pPokaz->init(false, rdoParse::RDOParserSrcInfo(m_resourceName), rdoParse::RDOParserSrcInfo(tstring(_T("�����_�������"))));
	}
	else
	{
		//! ���� ��� "QDEPART" ����������
		if (rtpList[rtp_name].exist())
		{
			rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
				rdoParse::LPRDOPMDWatchPar pPokaz = rdo::Factory<rdoParse::RDOPMDWatchPar>::create(rdoParse::RDOParserSrcInfo(q_name));
				ASSERT(pPokaz);
				pPokaz->init(false, rdoParse::RDOParserSrcInfo(m_resourceName), rdoParse::RDOParserSrcInfo(tstring(_T("�����_�������"))));
			}
		}
		else
		{
			rdoMBuilder::RDOResType rtp_ = rdoMBuilder::BlockForQueue::createType(rtp_name, rdoParse::RDOParserSrcInfo());
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
				rdoParse::LPRDOPMDWatchPar pPokaz = rdo::Factory<rdoParse::RDOPMDWatchPar>::create(rdoParse::RDOParserSrcInfo(q_name));
				ASSERT(pPokaz);
				pPokaz->init(false, rdoParse::RDOParserSrcInfo(m_resourceName), rdoParse::RDOParserSrcInfo(tstring(_T("�����_�������"))));
			}
		}
	}
}

void ProcGUIQueue::createRuntime()
{
	rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring res_name = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		//! "�����_�������"
		tstring rtp_param_name      = rdoRuntime::RDOPROCQueue::getQueueParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id();
	}
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIQueue: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pBlock = F(rdoRuntime::RDOPROCQueue)::create(m_pProcess->getProcess(), m_parserForRuntime);
	ASSERT(m_pBlock);
}


// ----------------------------------------------------------------------------
// ---------- ProcGUIDepart
// ----------------------------------------------------------------------------
ProcGUIDepart::ProcGUIDepart(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(tstring) name)
	: ProcGUIBlock  (pProcess, rdo::format(_T("%s Depart"), name.c_str()))
	, m_resourceName(name                                )
	, m_pParser     (pParser                             )
{
	//! �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList(m_pParser);
	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(m_pParser);
	rdoMBuilder::RDOResType rtp;
	tstring rtp_name = _T("QDEPART");
	//! ���� ������ ����������, ����� ��� ��� � ���������
	if (rssList[m_resourceName].exist())
	{
		rtp = rssList[m_resourceName].getType();
		rdoMBuilder::BlockForQueue::checkType(rtp, rdoParse::RDOParserSrcInfo());
	}
	else
	{
		if (rtpList[rtp_name].exist())
		{
			rdoMBuilder::RDOResType rtp_ = rtpList[rtp_name];
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
			}
		}
		else
		{
			rdoMBuilder::RDOResType rtp_ = rdoMBuilder::BlockForQueue::createType(rtp_name, rdoParse::RDOParserSrcInfo());
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
			}
		}
	}
}

void ProcGUIDepart::createRuntime()
{
	rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring res_name = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[res_name].getType();
		//! "�����_�������"
		tstring rtp_param_name      = rdoRuntime::RDOPROCDepart::getDepartParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtp_param_name].id(); 
	}	
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIDepart: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pBlock = F(rdoRuntime::RDOPROCDepart)::create(m_pProcess->getProcess(), m_parserForRuntime);
	ASSERT(m_pBlock);
}

CLOSE_RDO_SIMULATOR_NAMESPACE
