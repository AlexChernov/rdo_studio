/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_activity.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      13.04.2008
  \brief     �������� �������� ������ ��� ������� � ����������� -- RDOActivity
  \indent    4T
*/

#ifndef _LIB_RUNTIME_ACTIVITY_H_
#define _LIB_RUNTIME_ACTIVITY_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_resource.h"
#include "simulator/runtime/rdo_model_i.h"
#include "simulator/runtime/rdo_activity_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   RDOActivity
  \brief   ���������� ��
*/
class RDOActivity: public RDOTraceableObject, public IActivity, public IActivityTrace, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IActivity)
	QUERY_INTERFACE(IActivityTrace)
QUERY_INTERFACE_END

protected:
	RDOActivity(rbool trace, CREF(tstring) name);
	virtual ~RDOActivity();

	tstring                   m_oprName;
	std::list<LPRDOResource>  m_relevantResources; // ������ ����������� ��������
	std::vector<ruint>        m_relResID;          // �������� ������ id ��������, ������� ����� ������������ �������
	std::vector<LPRDOCalc>    m_paramsCalcs;

	void setPatternParameters              (CREF(LPRDORuntime) pRuntime);
	void getRelevantResources              (CREF(LPRDORuntime) pRuntime, REF(std::list<LPRDOResource>) rel_res_list);
	void incrementRelevantResourceReference(CREF(LPRDORuntime) pRuntime);
	void decrementRelevantResourceReference(CREF(LPRDORuntime) pRuntime);
	void updateRelRes                      (CREF(LPRDORuntime) pRuntime);
	void updateConvertStatus               (CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOResource::ConvertStatus>) status_list);

private:
	DECLARE_IActivity;
	DECLARE_IActivityTrace;
};

/*!
  \class   RDOActivityPattern
  \brief   ������� ���������� ��
*/
template<class T>
class RDOActivityPattern: public RDOActivity, public IModelStructure, public IActivityPatternTrace
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOActivity)
	QUERY_INTERFACE(IModelStructure)
	QUERY_INTERFACE(IActivityPatternTrace)
QUERY_INTERFACE_END

protected:
	RDOActivityPattern(CREF(rdo::intrusive_ptr<T>) pPattern, rbool trace, CREF(tstring) name);
	virtual ~RDOActivityPattern();

	rdo::intrusive_ptr<T> m_pPattern;

private:
	void writeModelStructure(REF(std::ostream) stream) const;
	CREF(tstring) tracePatternId() const;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_activity.inl"

#endif // _LIB_RUNTIME_ACTIVITY_H_
