/*!
  \copyright (c) RDO-Team, 2008
  \file      rdo_activity.inl
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      13.04.2008
  \brief     �������� �������� ������ ��� ������� � ����������� -- RDOActivity
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOActivity
// --------------------------------------------------------------------------------
inline RDOActivity::RDOActivity(rbool trace, CREF(tstring) name)
	: RDOTraceableObject(trace)
	, m_oprName         (name )
{}

inline RDOActivity::~RDOActivity()
{}

inline void RDOActivity::updateRelRes(CREF(LPRDORuntime) pRuntime)
{
	getRelevantResources(pRuntime, m_relevantResources);
}

// --------------------------------------------------------------------------------
// -------------------- RDOActivityPattern
// --------------------------------------------------------------------------------
template<class T>
inline RDOActivityPattern<T>::RDOActivityPattern(CREF(rdo::intrusive_ptr<T>) pPattern, rbool trace, CREF(tstring) name)
	: RDOActivity(trace, name)
	, m_pPattern (pPattern   )
{}

template<class T>
inline RDOActivityPattern<T>::~RDOActivityPattern()
{}

template<class T>
inline void RDOActivityPattern<T>::writeModelStructure(REF(std::ostream) stream) const
{
	stream << m_oprName << " " << tracePatternId() << std::endl;
}

template<class T>
inline CREF(tstring) RDOActivityPattern<T>::tracePatternId() const
{
	return m_pPattern->traceId();
}

CLOSE_RDO_RUNTIME_NAMESPACE
