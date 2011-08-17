/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdodpt.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdodpt.h"
#include "rdo_lib/rdo_converter/rdoopr.h"
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_lexer.h"
#include "rdo_lib/rdo_converter/rdorss.h"
#include "rdo_lib/rdo_converter/rdo_type.h"
#include "rdo_lib/rdo_runtime/rdo_rule.h"
#include "rdo_lib/rdo_runtime/rdo_operation.h"
#include "rdo_lib/rdo_runtime/rdo_keyboard.h"
#include "rdo_lib/rdo_runtime/rdoprocess.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptfree.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptsome.h"
#include "rdo_lib/rdo_runtime/rdo_logic_dptprior.h"
#include "rdo_lib/rdo_runtime/rdodptrtime.h"
#include "rdo_lib/rdo_runtime/rdo_dptsearch_activity.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

int cnv_dptlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_dpterror(PTR(char) mes)
{}

int cnv_proc_rtp_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_proc_rtp_error(PTR(char) mes)
{}

int cnv_proc_rss_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_proc_rss_error(PTR(char) mes)
{}

int cnv_proc_opr_lex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void cnv_proc_opr_error(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivity
// ----------------------------------------------------------------------------
RDODPTActivity::RDODPTActivity(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDOParserSrcInfo(src_info)
	, m_currParam     (0       )
{
	Converter::s_converter()->checkActivityName(src_info);
	m_pPattern = Converter::s_converter()->findPATPattern(pattern_src_info.src_text());
	if (!m_pPattern)
	{
		Converter::s_converter()->error().error(pattern_src_info, rdo::format(_T("�� ������ �������: %s"), pattern_src_info.src_text().c_str()));
	}
}

RDODPTActivity::~RDODPTActivity()
{}

void RDODPTActivity::addParam(CREF(RDOValue) param)
{
	if (m_pPattern->m_paramList.size() <= m_currParam)
	{
		if (param.src_pos().m_first_line == src_pos().m_first_line)
		{
			if (dynamic_cast<PTR(RDOOPROperation)>(this))
			{
				Converter::s_converter()->error().push_only(param, rdo::format(_T("������� ����� ���������� ��� ������� '%s' ��� �������� �������� '%s'"), m_pPattern->name().c_str(), name().c_str()));
			}
			else
			{
				Converter::s_converter()->error().push_only(param, rdo::format(_T("������� ����� ���������� ��� ������� '%s' ��� �������� ���������� '%s'"), m_pPattern->name().c_str(), name().c_str()));
			}
			Converter::s_converter()->error().push_only(m_pPattern->src_info(), _T("��. �������"));
			Converter::s_converter()->error().push_done();
		}
		else
		{
			if (dynamic_cast<PTR(RDOOPROperation)>(this))
			{
				Converter::s_converter()->error().error(param, _T("��� �������� ������ ������������� ����������"));
			}
			else
			{
				Converter::s_converter()->error().error(param, _T("��� ���������� ������ ������������� ����������"));
			}
		}
	}
	rdoRuntime::RDOValue val;
	LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
	if (param->getAsString() == _T("*"))
	{
		if (!pPatternParam->getDefault().defined())
		{
			Converter::s_converter()->error().push_only(param, rdo::format(_T("��� �������� ��-��������� ��� ��������� '%s'"), pPatternParam->src_text().c_str()));
			Converter::s_converter()->error().push_only(pPatternParam->src_info(), rdo::format(_T("��. �������� '%s', ��� '%s'"), pPatternParam->src_text().c_str(), pPatternParam->getType()->src_text().c_str()));
			Converter::s_converter()->error().push_done();
		}
		val = pPatternParam->getDefault().value();
	}
	else
	{
		val = pPatternParam->getType()->value_cast(param).value();
	}
	rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOSetPatternParamCalc>::create(m_currParam, val);
	ASSERT(pCalc);
	pCalc->setSrcInfo(RDOParserSrcInfo(param.getPosAsYY(), rdo::format(_T("�������� ������� %s.%s = %s"), m_pPattern->name().c_str(), pPatternParam->name().c_str(), param->getAsString().c_str())));
	m_currParam++;
}

void RDODPTActivity::endParam(CREF(YYLTYPE) param_pos)
{
	if (m_pPattern->m_paramList.size() > m_currParam)
	{
		LPRDOParam pPatternParam = m_pPattern->m_paramList.at(m_currParam);
		Converter::s_converter()->error().push_only(param_pos, rdo::format(_T("������� �� ��� ��������� ������� '%s':"), m_pPattern->src_text().c_str()));
		for (ruint i = m_currParam; i < m_pPattern->m_paramList.size(); i++)
		{
			pPatternParam = m_pPattern->m_paramList.at(i);
			Converter::s_converter()->error().push_only(pPatternParam->src_info(), rdo::format(_T("��������� �������� '%s' ����� ��� '%s'"), pPatternParam->name().c_str(), pPatternParam->getType()->src_text().c_str()));
		}
		Converter::s_converter()->error().push_done();
	}
	if (m_pPattern->getType() == RDOPATPattern::PT_Keyboard)
	{
		if (!static_cast<PTR(RDODPTActivityHotKey)>(this)->hasHotKey())
		{
			if (dynamic_cast<PTR(RDOOPROperation)>(this))
			{
				Converter::s_converter()->error().push_only(param_pos, _T("��� ������������ �������� ������ ���� ������� �������"));
			}
			else
			{
				Converter::s_converter()->error().push_only(param_pos, _T("��� ���������� ������ ���� ������� �������"));
			}
			Converter::s_converter()->error().push_only(m_pPattern->src_info(), _T("��. �������"));
			Converter::s_converter()->error().push_done();
		}
	}
}

rbool RDODPTActivity::setPrior(REF(LPRDOFUNArithm) pPrior)
{
	return false;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityHotKey
// ----------------------------------------------------------------------------
RDODPTActivityHotKey::RDODPTActivityHotKey(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivity(src_info, pattern_src_info)
{
	switch (pattern()->getType())
	{
	case RDOPATPattern::PT_IE       :
	case RDOPATPattern::PT_Rule     :
	case RDOPATPattern::PT_Operation:
	case RDOPATPattern::PT_Keyboard :
		break;

	default:
		Converter::s_converter()->error().push_only(this->src_info(), _T("����������� ��� �������"));
		Converter::s_converter()->error().push_only(pattern()->src_info(), _T("��. �������"));
		Converter::s_converter()->error().push_done();
	}
}

IKeyboard::AddHotKeyResult RDODPTActivityHotKey::addHotKey(CREF(tstring) hotKey)
{
	rdoRuntime::RDORuntime::RDOHotKeyToolkit::KeyCode scan_code = Converter::s_converter()->runtime()->rdoHotKeyToolkit.codeFromString( hotKey );
	if (scan_code == rdoRuntime::RDORuntime::RDOHotKeyToolkit::UNDEFINED_KEY)
	{
		return IKeyboard::addhk_notfound;
	}
	if (std::find(m_scanCodeList.begin(), m_scanCodeList.end(), scan_code) != m_scanCodeList.end())
	{
		return IKeyboard::addhk_already;
	}
	m_scanCodeList.push_back(scan_code);
	return IKeyboard::addhk_ok;
}

rbool RDODPTActivityHotKey::hasHotKey() const
{
	STL_FOR_ALL_CONST(m_scanCodeList, it)
	{
		if (*it != VK_SHIFT && *it != VK_CONTROL)
		{
			return true;
		}
	}
	return false;
}

void RDODPTActivityHotKey::addHotKey(CREF(tstring) hotKey, CREF(YYLTYPE) hotkey_pos)
{
	if (pattern()->getType() != RDOPATPattern::PT_Keyboard)
	{
		Converter::s_converter()->error().push_only(hotkey_pos, _T("������� ������� ������������ ������ � ������������ ���������"));
		Converter::s_converter()->error().push_only(pattern()->src_info(), _T("��. �������"));
		Converter::s_converter()->error().push_done();
	}

	switch (addHotKey(hotKey))
	{
	case rdoRuntime::RDOKeyboard::addhk_ok:
		break;

	case rdoRuntime::RDOKeyboard::addhk_already:
		if (dynamic_cast<PTR(RDOOPROperation)>(this))
		{
			Converter::s_converter()->error().error(hotkey_pos, rdo::format(_T("��� �������� '%s' ������� ��� ���������"), src_text().c_str()));
		}
		else
		{
			Converter::s_converter()->error().error(hotkey_pos, rdo::format(_T("��� ���������� '%s' ������� ��� ���������"), src_text().c_str()));
		}
		break;

	case rdoRuntime::RDOKeyboard::addhk_notfound:
		Converter::s_converter()->error().error(hotkey_pos, rdo::format(_T("����������� �������: %s"), hotKey.c_str()));
		break;

	case rdoRuntime::RDOKeyboard::addhk_dont:
		Converter::s_converter()->error().push_only(src_info(), rdo::format(_T("�������� '%s' �� �������� ������������"), src_text().c_str()));
		Converter::s_converter()->error().push_only(pattern()->src_info(), _T("��. �������"));
		Converter::s_converter()->error().push_done();
		break;

	default:
		Converter::s_converter()->error().error(src_info(), _T("���������� ������: RDODPTActivityHotKey::addHotKey"));
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFreeActivity
// ----------------------------------------------------------------------------
RDODPTFreeActivity::RDODPTFreeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{
	Converter::s_converter()->insertDPTFreeActivity(this);
}

// ----------------------------------------------------------------------------
// ---------- RDODPTFree
// ----------------------------------------------------------------------------
RDODPTFree::RDODPTFree(CREF(RDOParserSrcInfo) src_info)
	: RDOLogicActivity<rdoRuntime::RDODPTFree, RDODPTFreeActivity>(src_info)
{
	Converter::s_converter()->checkDPTName(this->src_info());
	m_pRuntimeLogic = F(rdoRuntime::RDODPTFree)::create(Converter::s_converter()->runtime());
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());
	Converter::s_converter()->insertDPTFree(this);
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSomeActivity
// ----------------------------------------------------------------------------
RDODPTSomeActivity::RDODPTSomeActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

// ----------------------------------------------------------------------------
// ---------- RDODPTPriorActivity
// ----------------------------------------------------------------------------
RDODPTPriorActivity::RDODPTPriorActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivityHotKey(pDPT, src_info, pattern_src_info)
{}

// ----------------------------------------------------------------------------
// ---------- RDODPTSome
// ----------------------------------------------------------------------------
RDODPTSome::RDODPTSome(CREF(RDOParserSrcInfo) src_info, LPILogic pParent)
	: RDOLogicActivity<rdoRuntime::RDODPTSome, RDODPTSomeActivity>(src_info)
{
	Converter::s_converter()->checkDPTName(this->src_info());
	m_pRuntimeLogic = F(rdoRuntime::RDODPTSome)::create(Converter::s_converter()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());
	Converter::s_converter()->insertDPTSome(this);
}

void RDODPTSome::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTPrior
// ----------------------------------------------------------------------------
RDODPTPrior::RDODPTPrior(CREF(RDOParserSrcInfo) src_info, LPILogic pParent)
	: RDOLogicActivity<rdoRuntime::RDODPTPrior, RDODPTPriorActivity>(src_info)
{
	Converter::s_converter()->checkDPTName(this->src_info());
	m_pRuntimeLogic = F(rdoRuntime::RDODPTPrior)::create(Converter::s_converter()->runtime(), pParent);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());
	Converter::s_converter()->insertDPTPrior(this);
}

void RDODPTPrior::end()
{
	if (getConditon())
	{
		m_pRuntimeLogic->setCondition(getConditon()->getCalc());
	}
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearchActivity
// ----------------------------------------------------------------------------
RDODPTSearchActivity::RDODPTSearchActivity(LPIBaseOperationContainer pDPT, CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) pattern_src_info)
	: RDODPTActivity(src_info, pattern_src_info   )
	, m_value       (IDPTSearchActivity::vt_before)
{
	if (pattern()->getType() != RDOPATPattern::PT_Rule)
	{
		Converter::s_converter()->error().push_only(this->src_info(), _T("������ ������������� ������� ����� ���� ������������ � ����� �������� ������� ���� search"));
		Converter::s_converter()->error().push_only(pattern()->src_info(), _T("��. �������"));
		Converter::s_converter()->error().push_done();
	}
	for (RDOPATPattern::RelResList::const_iterator it = pattern()->rel_res_begin(); it != pattern()->rel_res_end(); ++it)
	{
		if (((*it)->m_statusBegin == rdoRuntime::RDOResource::CS_Create) || ((*it)->m_statusBegin == rdoRuntime::RDOResource::CS_Erase))
		{
			Converter::s_converter()->error().push_only(this->src_info(), rdo::format(_T("� ������������� ������� '%s' ������ ��������� ��� ������� �������, �.�. ��� ������������ � ����� ���� search"), src_text().c_str()));
			Converter::s_converter()->error().push_only(pattern()->src_info(), _T("��. �������"));
			Converter::s_converter()->error().push_only((*it)->src_info(), _T("��. ����������� ������"));
			Converter::s_converter()->error().push_done();
		}
	}
}

void RDODPTSearchActivity::setValue(IDPTSearchActivity::ValueTime value, CREF(LPRDOFUNArithm) pRuleCost, CREF(YYLTYPE) param_pos)
{
	endParam(param_pos);
	m_value     = value;
	m_pRuleCost = pRuleCost;
}

// ----------------------------------------------------------------------------
// ---------- RDODPTSearch
// ----------------------------------------------------------------------------
RDODPTSearch::RDODPTSearch(CREF(RDOParserSrcInfo) src_info, rdoRuntime::RDODPTSearchTrace::DPT_TraceFlag trace, LPILogic pParent)
	: RDOLogicActivity<rdoRuntime::RDODPTSearchRuntime, RDODPTSearchActivity>(src_info)
	, m_trace  (trace  )
	, m_closed (false  )
	, m_pParent(pParent)
{
	Converter::s_converter()->checkDPTName   (this->src_info());
	Converter::s_converter()->insertDPTSearch(this);
}

void RDODPTSearch::end()
{
	rdoRuntime::LPRDOCalc pCalcCondition = m_pConditon     ? m_pConditon->getCalc()     : rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
	rdoRuntime::LPRDOCalc pCalcTerminate = m_pTermConditon ? m_pTermConditon->getCalc() : rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);

	m_pRuntimeLogic = F(rdoRuntime::RDODPTSearchRuntime)::create(Converter::s_converter()->runtime(),
		m_pParent,
		pCalcCondition,
		pCalcTerminate,
		m_pEvalBy->createCalc(),
		m_compTops,
		m_trace);
	ASSERT(m_pRuntimeLogic);
	m_pRuntimeLogic->init(Converter::s_converter()->runtime());

	int size = getActivities().size();
	for (int i = 0; i < size; i++)
	{
		LPRDODPTSearchActivity pSearchActivity = getActivities().at(i);
		ASSERT(pSearchActivity);
		LPIDPTSearchLogic pSearchLogic = m_pRuntimeLogic;
		ASSERT(pSearchLogic);
	}
	m_closed = true;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
tstring RDOPROCProcess::s_name_prefix = _T("");
tstring RDOPROCProcess::s_name_sufix  = _T("s");

RDOPROCProcess::RDOPROCProcess(CREF(RDOParserSrcInfo) info)
	: RDOParserSrcInfo(info )
	, m_closed        (false)
{
	Converter::s_converter()->insertPROCProcess(this);
	m_pRuntime = F(rdoRuntime::RDOPROCProcess)::create(info.src_text(), Converter::s_converter()->runtime());
	ASSERT(m_pRuntime);
	m_pRuntime.query_cast<ILogic>()->init(Converter::s_converter()->runtime());
}

rbool RDOPROCProcess::setPrior(REF(LPRDOFUNArithm) pPrior)
{
	LPILogic pRuntimeLogic = getRunTime();
	LPIPriority pPriority = pRuntimeLogic.query_cast<IPriority>();
	if (pPriority)
	{
		return pPriority->setPrior(pPrior->createCalc());
	}
	return false;
}

void RDOPROCProcess::end()
{
	m_closed = true;
	if (getConditon())
	{
		getRunTime()->setCondition(getConditon()->getCalc());
	}
}

void RDOPROCProcess::insertBlock(CREF(LPRDOPROCOperator) pBlock)
{
	ASSERT(pBlock);
	m_blockList.push_back(pBlock);
}

void RDOPROCProcess::insertChild(REF(LPRDOPROCProcess) pProcess)
{
	ASSERT(pProcess);
	m_childProcessList.push_back(pProcess);
	pProcess->m_pParentProcess = this;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCOperator
// ----------------------------------------------------------------------------
RDOPROCOperator::RDOPROCOperator(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: m_name    (name    )
	, m_pProcess(pProcess)
{
	ASSERT(pProcess);
	m_pProcess->insertBlock(this);
}

RDOPROCOperator::~RDOPROCOperator()
{}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
RDOPROCGenerate::RDOPROCGenerate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCGenerate)::create(Converter::s_converter()->getLastPROCProcess()->getRunTime(), pTimeCalc);
	ASSERT(m_pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForQueue
// ----------------------------------------------------------------------------
RDOPROCBlockForQueue::RDOPROCBlockForQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCOperator(pProcess, name)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
RDOPROCQueue::RDOPROCQueue(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCQueue::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCQueue::setResource(CREF(tstring) name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCDepart
// ----------------------------------------------------------------------------
RDOPROCDepart::RDOPROCDepart(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForQueue(pProcess, name)
{}

void RDOPROCDepart::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCDepart::setResource(CREF(tstring) name)
{
	ASSERT(!name.empty()         );
	ASSERT(m_resourceName.empty());

	m_resourceName = name;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCOperator(pProcess, name)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
RDOPROCSeize::RDOPROCSeize(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForSeize(pProcess, name)
{}

void RDOPROCSeize::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCSeize::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
RDOPROCRelease::RDOPROCRelease(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name)
	: RDOPROCBlockForSeize(pProcess, name)
{}

void RDOPROCRelease::createRuntime()
{
   NEVER_REACH_HERE;
}

void RDOPROCRelease::addResource(CREF(tstring) name)
{
	ASSERT(!name.empty());
	m_resourceList.push_back(name);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
RDOPROCAdvance::RDOPROCAdvance(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCAdvance)::create(Converter::s_converter()->getLastPROCProcess()->getRunTime(), pTimeCalc);
	ASSERT(m_pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
RDOPROCTerminate::RDOPROCTerminate(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(ruint) term)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCTerminate)::create(Converter::s_converter()->getLastPROCProcess()->getRunTime(), term);
	ASSERT(m_pRuntime);
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAssign
// ----------------------------------------------------------------------------
RDOPROCAssign::RDOPROCAssign(CREF(LPRDOPROCProcess) pProcess, CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pValue, int resID, int paramID)
	: RDOPROCOperator(pProcess, name)
{
	m_pRuntime = F(rdoRuntime::RDOPROCAssign)::create(Converter::s_converter()->getLastPROCProcess()->getRunTime(), pValue, resID, paramID);
	ASSERT(m_pRuntime);
}

CLOSE_RDO_CONVERTER_NAMESPACE
