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
#include "rdo_lib/rdo_simulator/ProcGUI.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
// ===============================================================================

// --------------------------------------------------------------------
// ---------- ProcGUIBlock
// --------------------------------------------------------------------
ProcGUIBlock::ProcGUIBlock(PTR(rdoParse::RDOParser) pParser, PTR(rdoRuntime::RDORuntime) pRuntime)	
	: m_pParser (pParser )
	, m_pRuntime(pRuntime)
{
	ASSERT(m_pParser );
	ASSERT(m_pRuntime);

	//! ������� �������
	m_pProcess = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_pRuntime);
	ASSERT(m_pProcess);
	m_pProcess.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIBlock::~ProcGUIBlock()
{}

void ProcGUIBlock::Create()
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
		rdoRuntime::RDOPROCTransact::typeID = rtp.id();
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
		rdoRuntime::RDOPROCTransact::typeID = rtp.id();
	}

	//! GENERATE
	rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(4);
	LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, pCalc);
	ASSERT(pBlock);

	//! TERMINATE
	pBlock = F(rdoRuntime::RDOPROCTerminate)::create(m_pProcess, 1);
	ASSERT(pBlock);
}
