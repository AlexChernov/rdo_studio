// ====================================================================== INCLUDES

// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_simulator/ProcGUI.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"

// ===============================================================================
// --------------------------------------------------------------------
// ---------- 
// --------------------------------------------------------------------
ProcGUIBlock::ProcGUIBlock(PTR(rdoParse::RDOParser) m_pParser, PTR(rdoRuntime::RDORuntime) m_pRuntime)	
	: m_Parser  ( m_pParser  )
	, m_Runtime ( m_pRuntime )
{
	//������� �������
	l_runtime = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_Runtime);
	ASSERT(l_runtime);
	l_runtime.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIBlock::~ProcGUIBlock()
{}

void ProcGUIBlock::Create()
{
	tstring rtp_name       = _T("���������");
	tstring rtp_param_name = _T("�����_��������");

	// �������� ������ ���� ����� ��������
	rdoMBuilder::RDOResTypeList rtpList(m_Parser);
	// ����� ��� �������, ���� ��� ���, �� �������
	if (!rtpList[rtp_name].exist())
	{
		// �������� ��� �������
		rdoMBuilder::RDOResType rtp(rtp_name);
		// ������� �������� �����_��������
		rtp.m_params.append(rdoMBuilder::RDOResType::Param(rtp_param_name, rdo::Factory<rdoParse::RDOType__real>::create()));
		// ������� ��� �������
		rdoRuntime::RDOPROCTransact::typeID = rtp.id();
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
		rdoRuntime::RDOPROCTransact::typeID = rtp.id();
	}
	//generate
	rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(4);

	b_runtime = F(rdoRuntime::RDOPROCGenerate)::create(l_runtime,pCalc);
	ASSERT(b_runtime);

	//terminate
	b_runtime = F(rdoRuntime::RDOPROCTerminate)::create(l_runtime, 1);
	ASSERT(b_runtime);
}
