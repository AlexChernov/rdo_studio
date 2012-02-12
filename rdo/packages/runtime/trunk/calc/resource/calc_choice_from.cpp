/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_choice_from.cpp
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     RDOCalc ��� ������� ����������� �������� � �������� ��������
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#ifdef COMPILER_GCC
	#include <float.h>
#endif // COMPILER_GCC
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/resource/calc_choice_from.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdoprocess.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceCalc
// --------------------------------------------------------------------------------
RDOSelectResourceCalc::RDOSelectResourceCalc(ResourceID relResID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType)
	: m_relResID       (relResID       )
	, m_pCalcChoiceFrom(pCalcChoiceFrom)
	, m_pCalcOrder     (pCalcOrder     )
	, m_orderType      (orderType      )
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceNonExistCalc
// --------------------------------------------------------------------------------
RDOSelectResourceNonExistCalc::RDOSelectResourceNonExistCalc(ResourceID relResID)
	: RDOSelectResourceCalc(relResID, NULL, NULL)
{
	m_value = 1;
}

REF(RDOValue) RDOSelectResourceNonExistCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getCurrentActivity()->setRelRes(m_relResID, ruint(~0));
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCalc
// --------------------------------------------------------------------------------
RDOSelectResourceDirectCalc::RDOSelectResourceDirectCalc(ResourceID relResID, ResourceID resID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType)
	: RDOSelectResourceCalc(relResID, pCalcChoiceFrom, pCalcOrder, orderType)
	, m_resID              (resID                                           )
{}

rbool RDOSelectResourceDirectCalc::compare(CREF(LPRDOCalc) pCalc) const
{
	LPRDOSelectResourceDirectCalc pDirectCalc = pCalc.object_dynamic_cast<RDOSelectResourceDirectCalc>();
	if (!pDirectCalc)
	{
		return false;
	}
	return m_relResID == pDirectCalc->m_relResID && m_resID == pDirectCalc->m_resID;
}

REF(RDOValue) RDOSelectResourceDirectCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	pRuntime->getCurrentActivity()->setRelRes(m_relResID, m_resID);
	if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
	{
		pRuntime->getCurrentActivity()->setRelRes(m_relResID, ruint(~0));
		m_value = 0;
		return m_value;
	}
	m_value = 1;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCalc
// --------------------------------------------------------------------------------
RDOSelectResourceByTypeCalc::RDOSelectResourceByTypeCalc(ResourceID relResID, ResourceID resTypeID, CREF(LPRDOCalc) pChoiceCalc, CREF(LPRDOCalc) pOrderCalc, Type orderType)
	: RDOSelectResourceCalc(relResID, pChoiceCalc, pOrderCalc, orderType)
	, m_resTypeID          (resTypeID                                   )
{}

REF(RDOValue) RDOSelectResourceByTypeCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	RDOValue   maxVal      = -DBL_MAX;
	RDOValue   minVal      = DBL_MAX;
	ResourceID resMinMaxID = ResourceID(~0);

	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it && (*it)->checkType(m_resTypeID))
		{
			ResourceID resID = (*it)->getTraceID();

			switch (m_orderType)
			{
			case order_empty:
			case order_first:
				{
					pRuntime->getCurrentActivity()->setRelRes(m_relResID, resID);
					if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(m_relResID, ruint(~0));
						continue;
					}
					m_value = 1;
					return m_value;
				}
			case order_with_min:
				{
					pRuntime->getCurrentActivity()->setRelRes(m_relResID, resID);
					if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(m_relResID, ruint(~0));
						continue;
					}
					RDOValue tmp = m_pCalcOrder->calcValue(pRuntime);
					if (tmp < minVal)
					{
						minVal      = tmp;
						resMinMaxID = resID;
					}
					break;
				}
			case order_with_max:
				{
					pRuntime->getCurrentActivity()->setRelRes(m_relResID, resID);
					if (m_pCalcChoiceFrom && !m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool())
					{
						pRuntime->getCurrentActivity()->setRelRes(m_relResID, ruint(~0));
						continue;
					}
					RDOValue tmp = m_pCalcOrder->calcValue(pRuntime);
					if (tmp > maxVal)
					{
						maxVal      = tmp;
						resMinMaxID = resID;
					}
					break;
				}
			}
		}
	}

	if (resMinMaxID != ~0)
	{
		pRuntime->getCurrentActivity()->setRelRes(m_relResID, resMinMaxID);
		m_value = 1;
		return m_value;
	}
	m_value = 0;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- IRDOSelectResourceCommon
// --------------------------------------------------------------------------------
IRDOSelectResourceCommon::IRDOSelectResourceCommon()
{}

IRDOSelectResourceCommon::~IRDOSelectResourceCommon()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceCommonCalc
// --------------------------------------------------------------------------------
RDOSelectResourceCommonCalc::RDOSelectResourceCommonCalc(CREF(SelectResourceCommonList) resSelectorList, rbool useCommonWithMax, CREF(LPRDOCalc) pCalcChoiceFrom)
	: m_resSelectorList (resSelectorList )
	, m_useCommonWithMax(useCommonWithMax)
	, m_pCalcChoiceFrom (pCalcChoiceFrom )
{
	if (m_pCalcChoiceFrom)
	{
		setSrcInfo(m_pCalcChoiceFrom->srcInfo());
	}
}

void RDOSelectResourceCommonCalc::getBest(REF(ResourceIDTable) allNumbs, ruint level, REF(ResourceIDList) res, REF(RDOValue) bestVal, CREF(LPRDORuntime) pRuntime, REF(rbool) hasBest) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < m_resSelectorList.size(); i++)
		{
			if (!m_resSelectorList[i]->callChoice(pRuntime))
			{
				return; // state not valid
			}
		}
		RDOValue newVal = const_cast<PTR(RDOSelectResourceCommonCalc)>(this)->m_pCalcChoiceFrom->calcValue(pRuntime);
		if (!hasBest || (m_useCommonWithMax && (newVal > bestVal)) ||
		   (!m_useCommonWithMax && (newVal < bestVal))) // found better value
		{
			ASSERT(res.size() == m_resSelectorList.size());
			for (ruint i = 0; i < m_resSelectorList.size(); i++)
			{
				res[i] = pRuntime->getCurrentActivity()->getResByRelRes(i);
			}
			bestVal = newVal;
			hasBest = true;
		}
		return;
	}
	ASSERT(level < allNumbs.size());
	REF(ResourceIDList) ourLevel = allNumbs[level];
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		pRuntime->getCurrentActivity()->setRelRes(level, ourLevel[i]);
		getBest(allNumbs, level+1, res, bestVal, pRuntime, hasBest);
	}
}

rbool RDOSelectResourceCommonCalc::getFirst(REF(ResourceIDTable) allNumbs, ruint level, CREF(LPRDORuntime) pRuntime) const
{
	if (level >= allNumbs.size())
	{
		for (ruint i = 0; i < m_resSelectorList.size(); i++)
		{
			if (!m_resSelectorList[i]->callChoice(pRuntime))
			{
				return false;
			}
		}
		return true;
	}
	ASSERT(level < allNumbs.size());
	REF(ResourceIDList) ourLevel = allNumbs[level];
	for (ruint i = 0; i < ourLevel.size(); i++)
	{
		pRuntime->getCurrentActivity()->setRelRes(level, ourLevel[i]);
		if (getFirst(allNumbs, level+1, pRuntime)) return true;
	}
	return false;
}

//rbool RDOSelectResourceCommonCalc::getFirst(REF(ResourceIDTable) allNumbs, int level,CREF(LPRDORuntime) pRuntime) const
//{
//	if (level <= 0) {
//		for (ruint i = 0; i < m_resSelectorList.size(); i++) {
//			if (!m_resSelectorList[i]->callChoice(pRuntime)) {
//				return false;
//			}
//		}
//		return true;
//	} else {
//		level--;
//		ASSERT(level < allNumbs.size());
//		REF(ResourceIDList) ourLevel = allNumbs[level];
//		for (ruint i = 0; i < ourLevel.size(); i++) {
//			pRuntime->setRelRes(level, ourLevel[i]);
//			if (getFirst(allNumbs, level, pRuntime)) return true;
//		}
//	}
//	return false;
//}

REF(RDOValue) RDOSelectResourceCommonCalc::doCalc(CREF(LPRDORuntime) pRuntime)
{
	ResourceIDTable allNumbs;
	ResourceIDList res;
	for (ruint i = 0; i < m_resSelectorList.size(); i++)
	{
		ResourceIDList resourceIDList;
		m_resSelectorList[i]->getPossibleNumbers(pRuntime, resourceIDList);
		allNumbs.push_back(resourceIDList);
		res.push_back(pRuntime->getCurrentActivity()->getResByRelRes(i));
	}
	if (!m_pCalcChoiceFrom)
	{
		// first
//		if (getFirst(allNumbs, allNumbs.size(), pRuntime)) {
//			return true;
//		}
		if (getFirst(allNumbs, 0, pRuntime))
		{
			m_value = 1;
			return m_value;
		}
	}
	else
	{
		// with_min / with_max
		RDOValue bestVal = 0;
		rbool found = false;
		getBest(allNumbs, 0, res, bestVal, pRuntime, found);
		if (found)
		{
			for (ruint i = 0; i < res.size(); i++)
			{
				pRuntime->getCurrentActivity()->setRelRes(i, res[i]);
			}
			m_value = 1;
			return m_value;
		}
	}
	m_value = 0;
	return m_value;
}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceDirectCommonCalc
// --------------------------------------------------------------------------------
RDOSelectResourceDirectCommonCalc::RDOSelectResourceDirectCommonCalc(ResourceID relResID, ResourceID resID, CREF(LPRDOCalc) pCalcChoiceFrom, CREF(LPRDOCalc) pCalcOrder, Type orderType)
	: RDOSelectResourceDirectCalc(relResID, resID, pCalcChoiceFrom, pCalcOrder, orderType)
{}

void RDOSelectResourceDirectCommonCalc::getPossibleNumbers(CREF(LPRDORuntime) pRuntime, REF(ResourceIDList) resourceIDList) const
{
	UNUSED(pRuntime);
	resourceIDList.push_back(m_resID);
}

rbool RDOSelectResourceDirectCommonCalc::callChoice(CREF(LPRDORuntime) pRuntime) const
{
	return (m_pCalcChoiceFrom && !const_cast<PTR(RDOSelectResourceDirectCommonCalc)>(this)->m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool()) ? false : true;
}

RDOSelectResourceDirectCommonCalc::~RDOSelectResourceDirectCommonCalc()
{}

// --------------------------------------------------------------------------------
// -------------------- RDOSelectResourceByTypeCommonCalc
// --------------------------------------------------------------------------------
RDOSelectResourceByTypeCommonCalc::RDOSelectResourceByTypeCommonCalc(ResourceID relResID, ResourceID resTypeID, CREF(LPRDOCalc) pChoiceCalc, CREF(LPRDOCalc) pOrderCalc, Type orderType)
	: RDOSelectResourceByTypeCalc(relResID, resTypeID, pChoiceCalc, pOrderCalc, orderType)
{}

void RDOSelectResourceByTypeCommonCalc::getPossibleNumbers(CREF(LPRDORuntime) pRuntime, REF(ResourceIDList) resourceIDList) const
{
	RDORuntime::ResCIterator end = pRuntime->res_end();
	for (RDORuntime::ResCIterator it = pRuntime->res_begin(); it != end; it++)
	{
		if (*it == LPRDOResource(NULL))
			continue;

		if (!(*it)->checkType(m_resTypeID))
			continue;

		resourceIDList.push_back((*it)->getTraceID());
	}
}

rbool RDOSelectResourceByTypeCommonCalc::callChoice(CREF(LPRDORuntime) pRuntime) const
{
	return (m_pCalcChoiceFrom && !const_cast<PTR(RDOSelectResourceByTypeCommonCalc)>(this)->m_pCalcChoiceFrom->calcValue(pRuntime).getAsBool()) ? false : true;
}

RDOSelectResourceByTypeCommonCalc::~RDOSelectResourceByTypeCommonCalc()
{}

CLOSE_RDO_RUNTIME_NAMESPACE
