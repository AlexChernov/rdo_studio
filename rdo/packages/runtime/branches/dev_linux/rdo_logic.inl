/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_logic.inl
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      03.10.2009
  \brief     ������ - ��������� ��
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_simulator.h"
#include "simulator/runtime/rdo_priority.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

#pragma warning(disable : 4786)

/// @todo �� ����� �� ����� ������� �� ��������?

/*!
  \def   LOGIC_FOR_ALL()
  \brief �������� �� ������ ��
*/
#define LOGIC_FOR_ALL() STL_FOR_ALL(m_childList, it)

// --------------------------------------------------------------------------------
// -------------------- RDOOrderSimple
// --------------------------------------------------------------------------------
inline LPIBaseOperation RDOOrderSimple::sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container)
{
	STL_FOR_ALL(container, it)
	{
		if ((*it)->onCheckCondition(pRuntime))
		{
			return *it;
		}
	}
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOOrderMeta
// --------------------------------------------------------------------------------
inline LPIBaseOperation RDOOrderMeta::sort(CREF(LPRDORuntime) pRuntime, REF(BaseOperationList) container)
{
	if (container.empty())
		return NULL;

	STL_FOR_ALL_CONST(container, it)
	{
		LPIPriority pattern = *it;
		if (pattern)
		{
			LPRDOCalc prior = pattern->getPrior();
			if (prior)
			{
				RDOValue value = prior->calcValue(pRuntime);
				if (value < 0 || value > 1)
				{
					pRuntime->error(rdo::format(_T("��������� ���������� ����� �� ������� ��������� [0..1]: %s"), value.getAsString().c_str()), prior);
				}
			}
		}
	}
	std::sort(container.begin(), container.end(), RDODPTActivityCompare(pRuntime));
	STL_FOR_ALL(container, it)
	{
		if ((*it)->onCheckCondition(pRuntime))
		{
			return *it;
		}
	}
	return NULL;
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogic
// --------------------------------------------------------------------------------
template <class Order>
inline RDOLogic<Order>::RDOLogic(CREF(LPRDORuntime) pRuntime, LPIBaseOperationContainer pParent)
	: m_pCondition   (NULL )
	, m_lastCondition(false)
	, m_first        (NULL )
	, m_parent       (pParent ? pParent : (pRuntime ? pRuntime->m_pMetaLogic : LPIBaseOperationContainer(NULL)))
{}

template <class Order>
inline RDOLogic<Order>::~RDOLogic()
{}

template <class Order>
inline void RDOLogic<Order>::init(CREF(LPRDORuntime) pRuntime)
{
	if (pRuntime)
	{
		pRuntime->appendLogic(rdo::UnknownPointer(this).query_cast<IBaseOperation>(), this->m_parent);
	}
}

template <class Order>
inline void RDOLogic<Order>::setCondition(CREF(LPRDOCalc) pCondition)
{
	m_pCondition = pCondition;
}

template <class Order>
inline void RDOLogic<Order>::onStart(CREF(LPRDORuntime) pRuntime)
{
	m_lastCondition = checkSelfCondition(pRuntime);
	if (m_lastCondition)
	{
		start(pRuntime);
	}
}

template <class Order>
inline void RDOLogic<Order>::onStop(CREF(LPRDORuntime) pRuntime)
{
	m_lastCondition = false;
	stop(pRuntime);
}

template <class Order>
inline rbool RDOLogic<Order>::onCheckCondition(CREF(LPRDORuntime) pRuntime)
{
	rbool condition = checkSelfCondition(pRuntime);
	if (condition != m_lastCondition)
	{
		m_lastCondition = condition;
		if (condition)
		{
			start(pRuntime);
		}
		else
		{
			stop(pRuntime);
		}
	}

	if (!condition)
		return false;

	m_first = Order::sort(pRuntime, m_childList);
	return m_first ? true : false;
}

template <class Order>
inline IBaseOperation::BOResult RDOLogic<Order>::onDoOperation(CREF(LPRDORuntime) pRuntime)
{
	if (m_lastCondition)
	{
		if (!m_first)
			return IBaseOperation::BOR_cant_run;

		IBaseOperation::BOResult result = m_first->onDoOperation(pRuntime);
		if (result == IBaseOperation::BOR_must_continue)
			pRuntime->setMustContinueOpr(m_first);

		return result;
	}
	else
	{
		return IBaseOperation::BOR_cant_run;
	}
}

template <class Order>
inline void RDOLogic<Order>::onMakePlaned(CREF(LPRDORuntime) pRuntime, PTR(void) pParam)
{
	LOGIC_FOR_ALL()
	{
		(*it)->onMakePlaned(pRuntime, pParam);
	}
}

template <class Order>
inline IBaseOperation::BOResult RDOLogic<Order>::onContinue(CREF(LPRDORuntime) pRuntime)
{
	LOGIC_FOR_ALL()
	{
		if ((*it)->onContinue(pRuntime) == IBaseOperation::BOR_must_continue)
			return IBaseOperation::BOR_must_continue;
	}
	return IBaseOperation::BOR_cant_run;
}

template <class Order>
inline rbool RDOLogic<Order>::checkSelfCondition(CREF(LPRDORuntime) pRuntime)
{
	return m_pCondition ? m_pCondition->calcValue(pRuntime).getAsBool() : true;
}

template <class Order>
inline void RDOLogic<Order>::start(CREF(LPRDORuntime) pRuntime)
{
	LOGIC_FOR_ALL()
	{
		(*it)->onStart(pRuntime);
	}
}

template <class Order>
inline void RDOLogic<Order>::stop(CREF(LPRDORuntime) pRuntime)
{
	LOGIC_FOR_ALL()
	{
		(*it)->onStop(pRuntime);
	}
}

template <class Order>
inline rbool RDOLogic<Order>::empty() const
{
	return m_childList.empty();
}

template <class Order>
inline typename RDOLogic<Order>::Iterator RDOLogic<Order>::begin()
{
	return m_childList.begin();
}

template <class Order>
inline typename RDOLogic<Order>::Iterator RDOLogic<Order>::end()
{
	return m_childList.end();
}

template <class Order>
inline typename RDOLogic<Order>::CIterator RDOLogic<Order>::begin() const
{
	return m_childList.begin();
}

template <class Order>
inline typename RDOLogic<Order>::CIterator RDOLogic<Order>::end() const
{
	return m_childList.end();
}

template <class Order>
inline REF(LPIBaseOperation) RDOLogic<Order>::back()
{
	return m_childList.back();
}

template <class Order>
inline void RDOLogic<Order>::append(CREF(Item) item)
{
	m_childList.push_back(item);
}

template <class Order>
inline void RDOLogic<Order>::clear()
{
	m_childList.clear();
}

CLOSE_RDO_RUNTIME_NAMESPACE
