/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_activity.h
 * author   : ������ ������, ����� �������
 * date     : 13.04.2008
 * bref     : �������� �������� ������ ��� ������� � ����������� -- RDOActivity
 * indent   : 4T
 */

#ifndef _RDO_ACTIVITY_H_
#define _RDO_ACTIVITY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdostream.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_resource.h"
#include "rdo_lib/rdo_runtime/rdo_model_i.h"
#include "rdo_lib/rdo_runtime/rdo_activity_i.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOActivity
// ----------------------------------------------------------------------------
class RDOActivity: public RDOTraceableObject, public IActivity, public IActivityTrace, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IActivity)
	QUERY_INTERFACE(IActivityTrace)
QUERY_INTERFACE_END

protected:
	RDOActivity(rbool trace, CREF(tstring) name ):
		RDOTraceableObject( trace ),
		m_oprName( name )
	{}
	virtual ~RDOActivity() {}

	tstring                   m_oprName;
	std::list<LPRDOResource>  m_relevantResources; // ������ ����������� ��������
	std::vector< int >        m_relResID;          // �������� ������ id ��������, ������� ����� ������������ �������
	std::vector<LPRDOCalc>    m_paramsCalcs;

	void setPatternParameters( CREF(LPRDORuntime) pRuntime );
	void getRelevantResources( CREF(LPRDORuntime) pRuntime, std::list<LPRDOResource>& rel_res_list );
	void incrementRelevantResourceReference( CREF(LPRDORuntime) pRuntime );
	void decrementRelevantResourceReference( CREF(LPRDORuntime) pRuntime );

	void updateRelRes(CREF(LPRDORuntime) pRuntime)
	{
		getRelevantResources(pRuntime, m_relevantResources);
	}
	void updateConvertStatus( CREF(LPRDORuntime) pRuntime, const std::vector< RDOResource::ConvertStatus >& status_list );

private:
	DECLARE_IActivity;
	DECLARE_IActivityTrace;
};

// ----------------------------------------------------------------------------
// ---------- RDOActivityPattern
// ----------------------------------------------------------------------------
template< class T >
class RDOActivityPattern: public RDOActivity, public IModelStructure, public IActivityPatternTrace
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOActivity)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IActivityPatternTrace)
QUERY_INTERFACE_END

protected:
	RDOActivityPattern(CREF(rdo::intrusive_ptr<T>) pPattern, rbool trace, CREF(tstring) name )
		: RDOActivity(trace, name )
		, m_pPattern (pPattern    )
	{}
	virtual ~RDOActivityPattern()
	{}

	rdo::intrusive_ptr<T> m_pPattern;

private:
	void writeModelStructure(REF(std::ostream) stream) const
	{
		stream << m_oprName << " " << tracePatternId() << std::endl;
	}
	CREF(tstring) tracePatternId() const
	{
		return m_pPattern->traceId();
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_ACTIVITY_H_
