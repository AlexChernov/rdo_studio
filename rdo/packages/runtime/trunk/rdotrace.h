/*!
  \copyright (c) RDO-Team, 2011
  \file      rdotrace.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     ������������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_TRACE_H_
#define _LIB_RUNTIME_TRACE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <fstream>
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace_i.h"
#include "simulator/runtime/rdo_runtime_interface_registrator.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class TreeNodeTrace;
class RDOEvent;
class RDORule;
class RDOOperation;
class RDOTraceableObject;
class TreeRootTrace;
class RDOPokazTrace;
class TreeNode;
class TreeRoot;
PREDECLARE_POINTER(RDOResource);
PREDECLARE_POINTER(RDORuntime);

/*!
  \class     RDOEndL
  \brief     ��������� ����������� ������ �����������
*/
class RDOEndL
{
public:
	virtual void onEndl();
};

inline std::ostream &operator << (std::ostream &stream, RDOEndL& rdoEndL);

/*!
  \class     RDOTrace
  \brief     ��������� ������ �����������
*/
class RDOTrace
{
friend class RDOSimulatorTrace;
friend class RDOResource;

public:
	RDOTrace();
	virtual ~RDOTrace();

	rbool canTrace() const;

	void  startWriting();
	void  stopWriting();
	rbool canWrite() const;
	rbool isNull() const;

	// Search in tree
	virtual void writeSearchBegin(double currentTime, tstring decisionPointId);
	virtual void writeSearchDecisionHeader();
	virtual void writeSearchDecision(CREF(LPRDORuntime) pRuntime, TreeNode *node);
	virtual void writeString(tstring);
	virtual void writeSearchOpenNode(int nodeCount, int parentCount, double pathCost, double restCost);
	virtual void writeSearchNodeInfo(char sign, TreeNodeTrace *node);
	virtual void writeSearchResult(char letter, CREF(LPRDORuntime) pRuntime, TreeRoot *treeRoot);

	virtual void writeEvent              (CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);
	virtual void writeRule               (CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);
	virtual void writeAfterOperationBegin(CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);
	virtual void writeAfterOperationEnd  (CREF(LPIBaseOperation) opr, CREF(LPRDORuntime) pRuntime);

	virtual void writeTraceBegin(CREF(LPRDORuntime) pRuntime);
	virtual void writeModelBegin(CREF(LPRDORuntime) pRuntime);
	virtual void writeTraceEnd  (CREF(LPRDORuntime) pRuntime);
	virtual void writeStatus    (CREF(LPRDORuntime) pRuntime, char *status);

	virtual void writePermanentResources(CREF(LPRDORuntime) pRuntime, CREF(std::list<LPRDOResource>) res_perm);

	tstring traceResourcesList( char prefix, CREF(LPRDORuntime) pRuntime, CREF(std::list<LPRDOResource>) rel_res_list);

	virtual void writePokaz(CREF(LPRDORuntime) pRuntime, PTR(RDOPokazTrace) pok);

public:
	virtual REF(std::ostream) getOStream();
	virtual REF(RDOEndL)      getEOL();

protected:
	rbool         m_isNullTracer;

private:
	rbool         m_canWriteToStream;
	std::ofstream m_emptyOut;
	RDOEndL       m_emptyEndL;
};

/*!
  \class     RDOTraceableObject
  \brief     ������������ ������
*/
class RDOTraceableObject: public ITrace
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(ITrace)
QUERY_INTERFACE_END

public:
	enum { NONE = 0xFFFFFFFF };

	rbool traceable() const;
	void  setTrace(rbool trace);

	ruint getTraceID() const;
	void  setTraceID(ruint id);
	void  setTraceID(ruint id, ruint str_id);

	REF(tstring) traceId() const;

protected:
	RDOTraceableObject(rbool trace, ruint id = NONE, tstring str = _T(""));
	virtual ~RDOTraceableObject();

private:
	rbool           m_trace;
	ruint           m_id;
	mutable tstring m_str_id;
};

/*!
  \class     RDOPokazTrace
  \brief     ������������ ����������
*/
class RDOPokazTrace: public RDOTraceableObject, public IPokazTrace, public IPokazTraceValue
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOTraceableObject)
	QUERY_INTERFACE(IPokazTrace)
QUERY_INTERFACE_END

public:
	RDOPokazTrace(CREF(LPRDORuntime) pRuntime, rbool trace);

protected:
	LPRDORuntime m_pRuntime;
	rbool        m_wasChanged;

	DECLARE_IPokazTrace;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdotrace.inl"

#endif // _LIB_RUNTIME_TRACE_H_
