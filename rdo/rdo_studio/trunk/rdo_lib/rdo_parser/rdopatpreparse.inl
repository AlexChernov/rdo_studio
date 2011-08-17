/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdopatpreparse.inl
 * author   : ����� �������
 * date     : 22.05.10
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOEvent
// ----------------------------------------------------------------------------
inline RDOEvent::RDOEvent(CREF(tstring) name, rbool regular)
	: m_name    (name   )
	, m_regullar(regular)
{}

inline RDOEvent::~RDOEvent()
{}

inline CREF(tstring) RDOEvent::name() const
{
	return m_name;
}

inline void RDOEvent::attachCalc(CREF(rdoRuntime::LPRDOCalcEventStop) pCalc)
{
	ASSERT(pCalc);
	m_calcList.push_back(pCalc);
}

inline void RDOEvent::attachCalc(CREF(rdoRuntime::LPRDOCalcEventPlan) pCalc)
{
	ASSERT(pCalc);
	m_calcList.push_back(pCalc);
}

inline void RDOEvent::setRuntimeEvent(LPIBaseOperation pRuntimeEvent)
{
	ASSERT(pRuntimeEvent);
	m_runtimeEvent = pRuntimeEvent;
}

inline LPIBaseOperation RDOEvent::getRuntimeEvent() const
{
	return m_runtimeEvent;
}

inline rbool RDOEvent::getRegular() const
{
	return m_regullar;
}

inline REF(RDOEvent::CalcList) RDOEvent::getCalcList()
{
	return m_calcList;
}

inline rdoRuntime::LPRDOCalc RDOEvent::getInitCalc() const
{
	return m_pInitCalc;
}

inline void RDOEvent::setInitCalc(CREF(rdoRuntime::LPRDOCalc) pCalc)
{
	ASSERT(pCalc);
	m_pInitCalc = pCalc;
}

CLOSE_RDO_PARSER_NAMESPACE
