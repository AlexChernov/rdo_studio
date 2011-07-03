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
ProcGUICalc::ProcGUICalc(CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(LPRPShapeDataBlock) pParams)
	: m_pRuntime(pRuntime)
	, m_pParams (pParams )
{
	ASSERT(m_pRuntime);
	ASSERT(m_pParams );
	switch(pParams->getZakon())
	{
		case RPShapeDataBlock::Const: // ��������� 
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getConstCalc(pParams->getExp()));
			break;
		}	
		case RPShapeDataBlock::Normal: // ����������
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getNormalCalc(pParams->getBase(), pParams->getExp(), pParams->getDisp()));
			break;
		}
		case RPShapeDataBlock::Uniform: // ����������� �����
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getUniformCalc(pParams->getBase(), pParams->getExp(), pParams->getDisp()));
			break;
		}
		case RPShapeDataBlock::Exp: // ����������������
		{
			m_pCalc = static_cast<rdoRuntime::LPRDOCalc>(getExpCalc(pParams->getBase(), pParams->getExp()));
			break;
		}
	}
}

ProcGUICalc::~ProcGUICalc()
{}

rdoRuntime::LPRDOCalcConst ProcGUICalc::getConstCalc(double arg)
{
	rdoRuntime::LPRDOCalcConst pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(arg);
	ASSERT(pCalc);
	return pCalc;
}

rdoRuntime::LPRDOCalc ProcGUICalc::getCalc()
{
	return m_pCalc;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getNormalCalc(ruint base, double arg1, double arg2)
{
	PTR(rdoRuntime::RandGeneratorNormal) pGenerator = new rdoRuntime::RandGeneratorNormal();
	ASSERT(pGenerator);
	rdoRuntime::LPRDOCalcSeqInit pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, pGenerator);
	ASSERT(pInitCalc);
	m_pRuntime->addInitCalc(pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextNormal>::create(pGenerator);
	ASSERT(pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(arg1);
	rdoRuntime::LPRDOCalcConst pArg2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(arg2);
	ASSERT(pArg1);
	ASSERT(pArg2);
	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	return pFuctionCall;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getUniformCalc(ruint base, double arg1, double arg2)
{
	PTR(rdoRuntime::RandGeneratorUniform) pGenerator = new rdoRuntime::RandGeneratorUniform();
	ASSERT(pGenerator);
	rdoRuntime::LPRDOCalcSeqInit pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, pGenerator);
	ASSERT(pInitCalc);
	m_pRuntime->addInitCalc(pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextUniform>::create(pGenerator);
	ASSERT(pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(arg1);
	rdoRuntime::LPRDOCalcConst pArg2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(arg2);
	ASSERT(pArg1);
	ASSERT(pArg2);
	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	return pFuctionCall;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getExpCalc(ruint base, double arg)
{
	PTR(rdoRuntime::RandGeneratorExponential) pGenerator = new rdoRuntime::RandGeneratorExponential();
	ASSERT(pGenerator);
	rdoRuntime::LPRDOCalcSeqInit pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, pGenerator);
	ASSERT(pInitCalc);
	m_pRuntime->addInitCalc(pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextExponential>::create(pGenerator);
	ASSERT(pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg = rdo::Factory<rdoRuntime::RDOCalcConst>::create(arg);
	ASSERT(pArg);
	pFuctionCall->addParameter(pArg);
	return pFuctionCall;
}

// --------------------------------------------------------------------
// ---------- ProcGUIProcess
// --------------------------------------------------------------------
tstring ProcGUIProcess::s_namePrefix = _T("");
tstring ProcGUIProcess::s_nameSufix  = _T("s");

ProcGUIProcess::ProcGUIProcess(CREF(rdoRuntime::LPRDORuntime) pRuntime)
	: m_pRuntime(pRuntime)
{
	ASSERT(m_pRuntime);
	m_pProcess = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_pRuntime);
	ASSERT(m_pProcess);
	m_pProcess.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIProcess::~ProcGUIProcess()
{}

void ProcGUIProcess::clear()
{
	m_pProcess = NULL;
	m_blockList.clear();
}

void ProcGUIProcess::insertBlock(CREF(LPProcGUIBlock) pBlock)
{
	ASSERT(pBlock);
	m_blockList.push_back(pBlock);
}

void ProcGUIProcess::initResources(CREF(rdoParse::LPRDOParser) pParser)
{
	STL_FOR_ALL_CONST(m_resNameList, it)
	{
		rdoParse::LPRDORSSResource pResource = pParser->findRSSResource(*it);
		ASSERT(pResource);
		rdoRuntime::LPRDOCalc pCalc = pResource->createCalc();
		ASSERT(pCalc);
		pParser->runtime()->addInitCalc(pCalc);
	}
}

void ProcGUIProcess::addResNameToBlock(CREF(tstring) name)
{
	ASSERT(!name.empty());

	ResNameList::const_iterator it = std::find(m_resNameList.begin(), m_resNameList.end(), name);
	if (it == m_resNameList.end())
	{
		m_resNameList.push_back(name);
	}
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
ProcGUIBlockGenerate::ProcGUIBlockGenerate(CREF(LPProcGUIProcess) pProcess, CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockCreate) pParams/* CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc*/)
	: ProcGUIBlock(pProcess, pParams->getName())
	, ProcGUICalc (pRuntime, pParams)
	, m_pParser   (pParser )
	, m_pParams   (pParams )
{
	tstring rtpName      = _T("���������");
	tstring rtpParamName = _T("�����_��������");

	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(m_pParser);
	//! ����� ��� �������, ���� ��� ���, �� �������
	if (!rtpList[rtpName].exist())
	{
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp(rtpName);
		//! ������� �������� �����_��������
		rtp.m_params.append(rdoMBuilder::RDOResType::Param(rtpParamName, rdo::Factory<rdoParse::RDOType__real>::create()));
		//! ������� ��� �������
		if (!rtpList.append<rdoRuntime::RDOResourceTypeTransact>(rtp))
		{
			m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("������ �������� ���� �������: %s"), rtpName.c_str()));
		}
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtpName];
	}

	//! GENERATE
	m_pBlock = F(rdoRuntime::RDOPROCGenerate)::create(getProcess()->getProcess(), getCalc(), m_pParams->getAmount());
	ASSERT(m_pBlock);
}

ProcGUIBlockGenerate::~ProcGUIBlockGenerate()
{}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockTerminate
// ----------------------------------------------------------------------------
ProcGUIBlockTerminate::ProcGUIBlockTerminate(CREF(LPProcGUIProcess) pProcess, CREF(LPRPShapeDataBlockTerminate) pParams)
	: ProcGUIBlock(pProcess, pParams->getName())
	, m_pParams   (pParams                     )
{
	m_pBlock = F(rdoRuntime::RDOPROCTerminate)::create(getProcess()->getProcess(), static_cast<ruint>(m_pParams->getTermInc()));
	ASSERT(m_pBlock);
}

ProcGUIBlockTerminate::~ProcGUIBlockTerminate()
{}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockProcess
// ----------------------------------------------------------------------------
ProcGUIBlockProcess::ProcGUIBlockProcess(CREF(LPProcGUIProcess) pProcess, CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, pParams->getName())
	, m_pParams   (pParams                     )
{
	std::list <RPShapeDataBlockProcess::resAction> action = m_pParams->getAction();
	std::list <RPShapeDataBlockProcess::resAction>::iterator it = action.begin();
	while(it != action.end())
	{
		switch(*it)
		{
			case RPShapeDataBlockProcess::Seize:
			{
				LPProcGUIQueue pQueue = rdo::Factory<ProcGUIQueue>::create(getProcess(), pParser, m_pParams->getName());
				ASSERT(pQueue);
				pQueue->createRuntime();
				LPProcGUISeize pSeize = rdo::Factory<ProcGUISeize>::create(getProcess(), pParser, m_pParams);
				ASSERT(pSeize);
				pSeize->createRuntime();
				LPProcGUIDepart pDepart = rdo::Factory<ProcGUIDepart>::create(getProcess(), pParser, m_pParams->getName());
				ASSERT(pDepart);
				pDepart->createRuntime();
				break;
			}
			case RPShapeDataBlockProcess::Advance:
			{
				LPProcGUIAdvance pAdvance = rdo::Factory<ProcGUIAdvance>::create(getProcess(), pRuntime, m_pParams);
				ASSERT(pAdvance);
				break;
			}
			case RPShapeDataBlockProcess::Release:
			{
				LPProcGUIRelease pRelease = rdo::Factory<ProcGUIRelease>::create(getProcess(), pParser, m_pParams);
				ASSERT(pRelease);
				pRelease->createRuntime();
				break;
			}
		}
		it++;
	}
	pProcess->initResources(pParser);
}

ProcGUIBlockProcess::~ProcGUIBlockProcess()
{}

// ----------------------------------------------------------------------------
// ---------- ProcGUIAdvance
// ----------------------------------------------------------------------------
ProcGUIAdvance::ProcGUIAdvance(CREF(LPProcGUIProcess) pProcess, CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(LPRPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, rdo::format(_T("%s Advance"), pParams->getName().c_str()))
	, ProcGUICalc (pRuntime, pParams)
	, m_pParams   (pParams )
{
	m_pBlock = F(rdoRuntime::RDOPROCAdvance)::create(getProcess()->getProcess(), getCalc());
	ASSERT(m_pBlock);
}

ProcGUIAdvance::~ProcGUIAdvance()
{}

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
		addResourceName(*it);
		pProcess->addResNameToBlock(*it);
		tstring resName = *it;
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		rdoMBuilder::RDOResType rtp;
		//! ���� ������ ����������, ����� ��� ��� � ���������
		if (rssList[resName].exist())
		{
			rtp = rssList[resName].getType();
			if (rdoMBuilder::BlockForSeize::checkType(rtp, rdoParse::RDOParserSrcInfo()))
			{
				if (!rssList[resName].checkParserResourceType<rdoParse::RDOPROCResource>(m_pParser))
				{
					rdoMBuilder::BlockForSeize::reobjectRes(rtp, resName);
				}
			}
		}
		else
		{
			//! ������ �� ������, ������������ ��� ���� �� ����� �������
			//! ������������ ��� ���� �� ����� �������
			tstring rtpName(ProcGUIProcess::s_namePrefix + resName + ProcGUIProcess::s_nameSufix);
			//! �������� ������ ���� ����� ��������
			rdoMBuilder::RDOResTypeList rtpList(m_pParser);
			//! ����� ��� �������
			if (rtpList[rtpName].exist())
			{
				rdoMBuilder::RDOResType rtp_ = rtpList[rtpName];
				if (rdoMBuilder::BlockForSeize::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
				{
					rdoMBuilder::BlockForSeize::createRes(rtp_, resName);
				}
			}
			else
			{
				rtp = rdoMBuilder::BlockForSeize::createType(rtpName, rdoParse::RDOParserSrcInfo());
				rdoMBuilder::BlockForSeize::createRes(rtp, resName);
			}
		}
		
		it++;
	}
}

ProcGUISeize::~ProcGUISeize()
{}

void ProcGUISeize::createRuntime()
{
	STL_FOR_ALL_CONST(m_resNameList, it)
	{
		rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource(*it);
		if (pResource)
		{
			tstring resName = pResource->name();
			//! �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(m_pParser);
			//! �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[resName].getType();
			//! "���������"
			tstring rtpParamName = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtpParamName].exist())
			{
				m_pParser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtpParamName.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtpParamName].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUISeize: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}
	if (!m_parserForRuntime.empty())
	{
		m_pBlock = F(rdoRuntime::RDOPROCSeize)::create(getProcess()->getProcess(), m_parserForRuntime);
		ASSERT(m_pBlock);
	}
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), _T("���������� ������: ���� Seize �� �������� ��������"));
	}
}

void ProcGUISeize::addResourceName(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resNameList.push_back(name);
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
		addResourceName(*it);
		pProcess->addResNameToBlock(*it);
		tstring resName = *it;
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		rdoMBuilder::RDOResType rtp;
		//! ���� ������ ����������, ����� ��� ��� � ���������
		if (rssList[resName].exist())
		{
			rtp = rssList[resName].getType();
			if (rdoMBuilder::BlockForSeize::checkType(rtp, rdoParse::RDOParserSrcInfo()))
			{
				if (!rssList[resName].checkParserResourceType<rdoParse::RDOPROCResource>(m_pParser))
				{
					rdoMBuilder::BlockForSeize::reobjectRes(rtp, resName);
				}
			}
		}
		else
		{
			//! ������ �� ������, ������������ ��� ���� �� ����� �������
			//! ������������ ��� ���� �� ����� �������
			tstring rtpName(ProcGUIProcess::s_namePrefix + resName + ProcGUIProcess::s_nameSufix);
			//! �������� ������ ���� ����� ��������
			rdoMBuilder::RDOResTypeList rtpList(m_pParser);
			//! ����� ��� �������
			if (rtpList[rtpName].exist())
			{
				rdoMBuilder::RDOResType rtp_ = rtpList[rtpName];
				if (rdoMBuilder::BlockForSeize::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
				{
					rdoMBuilder::BlockForSeize::createRes(rtp_, resName);
				}
			}
			else
			{
				rtp = rdoMBuilder::BlockForSeize::createType(rtpName, rdoParse::RDOParserSrcInfo());
				rdoMBuilder::BlockForSeize::createRes(rtp, resName);
			}
		}
		it++;
	}
}

ProcGUIRelease::~ProcGUIRelease()
{}

void ProcGUIRelease::createRuntime()
{
	STL_FOR_ALL_CONST(m_resNameList, it){
		rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource((*it));
		if (pResource)
		{
			tstring resName = pResource->name();
			//! �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(m_pParser);
			//! �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[resName].getType();
			//! "���������"
			tstring rtpParamName = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtpParamName].exist())
			{
				m_pParser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtpParamName.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtpParamName].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIRelease: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}
	if (!m_parserForRuntime.empty())
	{
		m_pBlock = F(rdoRuntime::RDOPROCRelease)::create(getProcess()->getProcess(), m_parserForRuntime);
		ASSERT(m_pBlock);
	}
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), _T("���������� ������: ���� Release �� �������� ��������"));
	}
}

void ProcGUIRelease::addResourceName(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resNameList.push_back(name);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
ProcGUIQueue::ProcGUIQueue(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(tstring) name)
	: ProcGUIBlock  (pProcess, rdo::format(_T("%s Queue"), name.c_str()))
	, m_resourceName(name   )
	, m_pParser     (pParser)
{
	pProcess->addResNameToBlock(m_resourceName);
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList(m_pParser);
	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(m_pParser);
	rdoMBuilder::RDOResType rtp;
	tstring rtpName   = _T("QDEPART");
	tstring queueName = _T("�������_") + m_resourceName;
	//! ���� ������ ����������, ����� ��� ��� � ���������
	if (rssList[m_resourceName].exist())
	{
		rtp = rssList[m_resourceName].getType();
		rdoMBuilder::BlockForQueue::checkType(rtp, rdoParse::RDOParserSrcInfo());
		rdoParse::LPRDOPMDWatchPar pPokaz = rdo::Factory<rdoParse::RDOPMDWatchPar>::create(rdoParse::RDOParserSrcInfo(queueName));
		ASSERT(pPokaz);
		pPokaz->init(false, rdoParse::RDOParserSrcInfo(m_resourceName), rdoParse::RDOParserSrcInfo(tstring(_T("�����_�������"))));
	}
	else
	{
		//! ���� ��� "QDEPART" ����������
		if (rtpList[rtpName].exist())
		{
			rdoMBuilder::RDOResType rtp_ = rtpList[rtpName];
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
				rdoParse::LPRDOPMDWatchPar pPokaz = rdo::Factory<rdoParse::RDOPMDWatchPar>::create(rdoParse::RDOParserSrcInfo(queueName));
				ASSERT(pPokaz);
				pPokaz->init(false, rdoParse::RDOParserSrcInfo(m_resourceName), rdoParse::RDOParserSrcInfo(tstring(_T("�����_�������"))));
			}
		}
		else
		{
			rdoMBuilder::RDOResType rtp_ = rdoMBuilder::BlockForQueue::createType(rtpName, rdoParse::RDOParserSrcInfo());
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
				rdoParse::LPRDOPMDWatchPar pPokaz = rdo::Factory<rdoParse::RDOPMDWatchPar>::create(rdoParse::RDOParserSrcInfo(queueName));
				ASSERT(pPokaz);
				pPokaz->init(false, rdoParse::RDOParserSrcInfo(m_resourceName), rdoParse::RDOParserSrcInfo(tstring(_T("�����_�������"))));
			}
		}
	}
}

ProcGUIQueue::~ProcGUIQueue()
{}

void ProcGUIQueue::createRuntime()
{
	rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring resName = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[resName].getType();
		//! "�����_�������"
		tstring rtpParamName        = rdoRuntime::RDOPROCQueue::getQueueParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtpParamName].id();
	}
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIQueue: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pBlock = F(rdoRuntime::RDOPROCQueue)::create(getProcess()->getProcess(), m_parserForRuntime);
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
	pProcess->addResNameToBlock(m_resourceName);
	//! �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList(m_pParser);
	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(m_pParser);
	rdoMBuilder::RDOResType rtp;
	tstring rtpName = _T("QDEPART");
	//! ���� ������ ����������, ����� ��� ��� � ���������
	if (rssList[m_resourceName].exist())
	{
		rtp = rssList[m_resourceName].getType();
		rdoMBuilder::BlockForQueue::checkType(rtp, rdoParse::RDOParserSrcInfo());
	}
	else
	{
		if (rtpList[rtpName].exist())
		{
			rdoMBuilder::RDOResType rtp_ = rtpList[rtpName];
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
			}
		}
		else
		{
			rdoMBuilder::RDOResType rtp_ = rdoMBuilder::BlockForQueue::createType(rtpName, rdoParse::RDOParserSrcInfo());
			if (rdoMBuilder::BlockForQueue::checkType(rtp_, rdoParse::RDOParserSrcInfo()))
			{
				rdoMBuilder::BlockForQueue::createRes(rtp_, m_resourceName);
			}
		}
	}
}

ProcGUIDepart::~ProcGUIDepart()
{}

void ProcGUIDepart::createRuntime()
{
	rdoParse::LPRDORSSResource pResource = m_pParser->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring resName = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(m_pParser);
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[resName].getType();
		//! "�����_�������"
		tstring rtpParamName        = rdoRuntime::RDOPROCDepart::getDepartParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtpParamName].id();
	}	
	else
	{
		m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIDepart: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pBlock = F(rdoRuntime::RDOPROCDepart)::create(getProcess()->getProcess(), m_parserForRuntime);
	ASSERT(m_pBlock);
}

CLOSE_RDO_SIMULATOR_NAMESPACE
