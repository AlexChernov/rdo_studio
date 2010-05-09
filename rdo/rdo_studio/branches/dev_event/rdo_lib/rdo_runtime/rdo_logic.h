/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_H_
#define _RDO_LOGIC_H_

#pragma warning(disable : 4786)

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_lib/rdo_runtime/rdo_logic_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

class RDOCalc;

// ----------------------------------------------------------------------------
// ---------- RDOLogicBase
// ----------------------------------------------------------------------------
template <class Order>
class RDOLogic: public IBaseOperation, public IBaseOperationContainer, public ILogic, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IBaseOperationContainer)
	QUERY_INTERFACE(ILogic)
QUERY_INTERFACE_END

public:
	typedef  BaseOperationList                  ChildList;
	typedef  BaseOperationList::iterator        Iterator;
	typedef  BaseOperationList::const_iterator  CIterator;

protected:
	RDOLogic(PTR(RDOSimulator) sim = NULL, LPIBaseOperationContainer parent = NULL);
	virtual ~RDOLogic();

	DECLARE_IBaseOperationContainer;

	PTR(RDOCalc)               m_condition;
	rbool                      m_lastCondition;
	ChildList                  m_childList;
	LPIBaseOperation           m_first;
	LPIBaseOperationContainer  m_parent;

private:
	rbool checkSelfCondition(PTR(RDOSimulator) sim);
	void  start             (PTR(RDOSimulator) sim);
	void  stop              (PTR(RDOSimulator) sim);

	DECLARE_IBaseOperation;
	DECLARE_ILogic;
};

// ----------------------------------------------------------------------------
// ---------- RDOOrderSimple
// ----------------------------------------------------------------------------
class RDOOrderSimple
{
public:
	static LPIBaseOperation sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container);
};

// ----------------------------------------------------------------------------
// ---------- RDOOrderMeta
// ----------------------------------------------------------------------------
class RDOOrderMeta
{
public:
	static LPIBaseOperation sort(PTR(RDOSimulator) sim, REF(BaseOperationList) container);
};

// ----------------------------------------------------------------------------
// ---------- RDOLogicSimple
// ----------------------------------------------------------------------------
class RDOLogicSimple: public RDOLogic<RDOOrderSimple>
{
protected:
	DEFINE_IFACTORY(RDOLogicSimple);

	RDOLogicSimple(PTR(RDOSimulator) sim, LPIBaseOperationContainer parent = NULL)
		: RDOLogic<RDOOrderSimple>(sim, parent)
	{}
	virtual ~RDOLogicSimple()
	{}
};

// ----------------------------------------------------------------------------
// ---------- RDOLogicMeta
// ----------------------------------------------------------------------------
class RDOLogicMeta: public RDOLogic<RDOOrderMeta>
{
protected:
	DEFINE_IFACTORY(RDOLogicMeta);

	RDOLogicMeta()
		: RDOLogic<RDOOrderMeta>()
	{}
	virtual ~RDOLogicMeta()
	{}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_logic.inl"

#endif //! _RDO_LOGIC_H_
