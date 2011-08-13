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

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getTriangularCalc(int m_pBase,double m_pArg1,double m_pArg2,double m_pArg3)
{
	PTR(rdoRuntime::RandGeneratorTriangular) pGenerator = new rdoRuntime::RandGeneratorTriangular();
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_pBase, pGenerator);
	ASSERT(m_pInitCalc);
	m_pRuntime->addInitCalc(m_pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextTriangular>::create(pGenerator);
	ASSERT(m_pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	rdoRuntime::LPRDOCalcConst pArg2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg2);
	rdoRuntime::LPRDOCalcConst pArg3 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg3);
	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	pFuctionCall->addParameter(pArg3);
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
ProcGUIBlock::ProcGUIBlock(CREF(LPProcGUIProcess) pProcess, CREF(tstring) name)
	: m_name(name)
{
	ASSERT(pProcess);
	pProcess->insertBlock(this);
}

ProcGUIBlock::~ProcGUIBlock()
{}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockGenerate
// ----------------------------------------------------------------------------
ProcGUIBlockGenerate::ProcGUIBlockGenerate(CREF(LPProcGUIProcess) pProcess, CREF(rdoRuntime::LPRDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockCreate) pParams)
	: ProcGUIBlock(pProcess, pParams->getName())
	, ProcGUICalc (pRuntime, pParams           )
	, m_pParams   (pParams                     )
{
	ASSERT(pProcess );
	ASSERT(pRuntime );
	ASSERT(pParser  );
	ASSERT(m_pParams);

	tstring rtpName      = _T("���������");
	tstring rtpParamName = _T("�����_��������");

	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(pParser);
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
			pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("������ �������� ���� �������: %s"), rtpName.c_str()));
		}
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtpName];
	}

	//! GENERATE
	m_pBlock = F(rdoRuntime::RDOPROCGenerate)::create(pProcess->getProcess(), getCalc(), m_pParams->getAmount());
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
	ASSERT(pProcess );
	ASSERT(m_pParams);

	m_pBlock = F(rdoRuntime::RDOPROCTerminate)::create(pProcess->getProcess(), static_cast<ruint>(m_pParams->getTermInc()));
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
	ASSERT(pProcess );
	ASSERT(pRuntime );
	ASSERT(pParser  );
	ASSERT(m_pParams);

	CREF(RPShapeDataBlockProcess::ActionList) actionList = m_pParams->getActionList();
	STL_FOR_ALL_CONST(actionList, it)
	{
		switch (*it)
		{
			case RPShapeDataBlockProcess::A_SEIZE:
			{
				LPProcGUIQueue pQueue = rdo::Factory<ProcGUIQueue>::create(pProcess, pParser, m_pParams->getName());
				ASSERT(pQueue);
				pQueue->createRuntime(pProcess, pParser);

				LPProcGUISeize pSeize = rdo::Factory<ProcGUISeize>::create(pProcess, pParser, m_pParams);
				ASSERT(pSeize);
				pSeize->createRuntime(pProcess, pParser);

				LPProcGUIDepart pDepart = rdo::Factory<ProcGUIDepart>::create(pProcess, pParser, m_pParams->getName());
				ASSERT(pDepart);
				pDepart->createRuntime(pProcess, pParser);

				break;
			}
			case RPShapeDataBlockProcess::A_ADVANCE:
			{
				LPProcGUIAdvance pAdvance = rdo::Factory<ProcGUIAdvance>::create(pProcess, pRuntime, m_pParams);
				ASSERT(pAdvance);
				break;
			}
			case RPShapeDataBlockProcess::A_RELEASE:
			{
				LPProcGUIRelease pRelease = rdo::Factory<ProcGUIRelease>::create(pProcess, pParser, m_pParams);
				ASSERT(pRelease);
				pRelease->createRuntime(pProcess, pParser);
				break;
			}
		}
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
	, m_pParams   (pParams)
{
	ASSERT(pProcess );
	ASSERT(pRuntime );
	ASSERT(m_pParams);

	m_pBlock = F(rdoRuntime::RDOPROCAdvance)::create(pProcess->getProcess(), getCalc());
	ASSERT(m_pBlock);
}

ProcGUIAdvance::~ProcGUIAdvance()
{}

// ----------------------------------------------------------------------------
// ---------- ProcGUISeize
// ----------------------------------------------------------------------------
ProcGUISeize::ProcGUISeize(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(LPRPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, rdo::format(_T("%s Seize"), pParams->getName().c_str()))
	, m_pParams   (pParams)
{
	ASSERT(pProcess );
	ASSERT(pParser  );
	ASSERT(m_pParams);

	CREF(RPShapeDataBlockProcess::ResNameList) resNameList = m_pParams->getResNameList();
	STL_FOR_ALL_CONST(resNameList, it)
	{
		addResourceName(*it);
		pProcess->addResNameToBlock(*it);
		tstring resName = *it;
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(pParser);
		rdoMBuilder::RDOResType      rtp;
		//! ���� ������ ����������, ����� ��� ��� � ���������
		if (rssList[resName].exist())
		{
			rtp = rssList[resName].getType();
			if (rdoMBuilder::BlockForSeize::checkType(rtp, rdoParse::RDOParserSrcInfo()))
			{
				if (!rssList[resName].checkParserResourceType<rdoParse::RDOPROCResource>(pParser))
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
			rdoMBuilder::RDOResTypeList rtpList(pParser);
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
	}
}

ProcGUISeize::~ProcGUISeize()
{}

void ProcGUISeize::createRuntime(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser)
{
	ASSERT(pProcess);
	ASSERT(pParser );

	STL_FOR_ALL_CONST(m_resNameList, it)
	{
		rdoParse::LPRDORSSResource pResource = pParser->findRSSResource(*it);
		if (pResource)
		{
			tstring resName = pResource->name();
			//! �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(pParser);
			//! �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[resName].getType();
			//! "���������"
			tstring rtpParamName = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtpParamName].exist())
			{
				pParser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtpParamName.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtpParamName].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUISeize: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}
	if (!m_parserForRuntime.empty())
	{
		m_pBlock = F(rdoRuntime::RDOPROCSeize)::create(pProcess->getProcess(), m_parserForRuntime);
		ASSERT(m_pBlock);
	}
	else
	{
		pParser->error().error(rdoParse::RDOParserSrcInfo(), _T("���������� ������: ���� Seize �� �������� ��������"));
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
	, m_pParams   (pParams)
{
	ASSERT(pProcess );
	ASSERT(pParser  );
	ASSERT(m_pParams);

	CREF(RPShapeDataBlockProcess::ResNameList) resNameList = m_pParams->getResNameList();
	STL_FOR_ALL_CONST(resNameList, it)
	{
		addResourceName(*it);
		pProcess->addResNameToBlock(*it);
		tstring resName = *it;
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(pParser);
		rdoMBuilder::RDOResType      rtp;
		//! ���� ������ ����������, ����� ��� ��� � ���������
		if (rssList[resName].exist())
		{
			rtp = rssList[resName].getType();
			if (rdoMBuilder::BlockForSeize::checkType(rtp, rdoParse::RDOParserSrcInfo()))
			{
				if (!rssList[resName].checkParserResourceType<rdoParse::RDOPROCResource>(pParser))
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
			rdoMBuilder::RDOResTypeList rtpList(pParser);
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
	}
}

ProcGUIRelease::~ProcGUIRelease()
{}

void ProcGUIRelease::createRuntime(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser)
{
	ASSERT(pProcess);
	ASSERT(pParser );

	STL_FOR_ALL_CONST(m_resNameList, it)
	{
		rdoParse::LPRDORSSResource pResource = pParser->findRSSResource(*it);
		if (pResource)
		{
			tstring resName = pResource->name();
			//! �������� ������ ���� ��������
			rdoMBuilder::RDOResourceList rssList(pParser);
			//! �������� ��� �������
			rdoMBuilder::RDOResType rtp = rssList[resName].getType();
			//! "���������"
			tstring rtpParamName = rdoRuntime::RDOPROCBlockForSeize::getStateParamName();
			//! �������� ��� �� ������� ������������� ���������
			if (!rtp.m_params[rtpParamName].exist())
			{
				pParser->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ��������� ������������� ���� '%s'"), rtp.name().c_str(), rtpParamName.c_str()));
			}
			rdoRuntime::parser_for_Seize bbb;
			bbb.Id_res   = pResource->getID();
			bbb.Id_param = rtp.m_params[rtpParamName].id(); 
			m_parserForRuntime.push_back(bbb);
		}
		else
		{
			pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIRelease: �� ����� parser-������ '%s'"), it->c_str()));
		}
	}
	if (!m_parserForRuntime.empty())
	{
		m_pBlock = F(rdoRuntime::RDOPROCRelease)::create(pProcess->getProcess(), m_parserForRuntime);
		ASSERT(m_pBlock);
	}
	else
	{
		pParser->error().error(rdoParse::RDOParserSrcInfo(), _T("���������� ������: ���� Release �� �������� ��������"));
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
	, m_resourceName(name)
{
	ASSERT(pProcess);
	ASSERT(pParser );

	pProcess->addResNameToBlock(m_resourceName);
	// �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList(pParser);
	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(pParser);
	rdoMBuilder::RDOResType     rtp;
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

void ProcGUIQueue::createRuntime(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser)
{
	ASSERT(pProcess);
	ASSERT(pParser );

	rdoParse::LPRDORSSResource pResource = pParser->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring resName = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(pParser);
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[resName].getType();
		//! "�����_�������"
		tstring rtpParamName        = rdoRuntime::RDOPROCQueue::getQueueParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtpParamName].id();
	}
	else
	{
		pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIQueue: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pBlock = F(rdoRuntime::RDOPROCQueue)::create(pProcess->getProcess(), m_parserForRuntime);
	ASSERT(m_pBlock);
}


// ----------------------------------------------------------------------------
// ---------- ProcGUIDepart
// ----------------------------------------------------------------------------
ProcGUIDepart::ProcGUIDepart(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser, CREF(tstring) name)
	: ProcGUIBlock  (pProcess, rdo::format(_T("%s Depart"), name.c_str()))
	, m_resourceName(name)
{
	ASSERT(pProcess);
	ASSERT(pParser );

	pProcess->addResNameToBlock(m_resourceName);
	//! �������� ������ ���� ��������
	rdoMBuilder::RDOResourceList rssList(pParser);
	//! �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(pParser);
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

void ProcGUIDepart::createRuntime(CREF(LPProcGUIProcess) pProcess, CREF(rdoParse::LPRDOParser) pParser)
{
	ASSERT(pProcess);
	ASSERT(pParser );

	rdoParse::LPRDORSSResource pResource = pParser->findRSSResource(m_resourceName);
	if (pResource)
	{
		tstring resName = pResource->name();
		//! �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(pParser);
		//! �������� ��� �������
		rdoMBuilder::RDOResType rtp = rssList[resName].getType();
		//! "�����_�������"
		tstring rtpParamName        = rdoRuntime::RDOPROCDepart::getDepartParamName();
		m_parserForRuntime.Id_res   = pResource->getID();
		m_parserForRuntime.Id_param = rtp.m_params[rtpParamName].id();
	}	
	else
	{
		pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("���������� ������ ProcGUIDepart: �� ����� parser-������ '%s'"), m_resourceName.c_str()));
	}
	m_pBlock = F(rdoRuntime::RDOPROCDepart)::create(pProcess->getProcess(), m_parserForRuntime);
	ASSERT(m_pBlock);
}

CLOSE_RDO_SIMULATOR_NAMESPACE
